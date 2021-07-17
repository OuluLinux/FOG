#include <Prim/PrimIncludeAll.h>






#ifndef NO_PRIM_STREAMS
//
//  	Insert two spaces into the output stream for each level of indentation requested in anIndent
//
std::ostream& operator<<(std::ostream& s, const PrimIndent& anIndent) {
	for (int i = 0; i < anIndent._indentation; i++)
		s << "  ";
		
	return s;
}

//
//  	Optionally strip non-new-line whitespace from the input stream according to skipws, then demand a
//  	new-line, setting the stream bad if not found.
//
std::istream& endl(std::istream& s) {
	if ((s.flags() & std::ios::skipws) != 0) {
		int ch;
		
		while (ch = s.peek(), isspace(ch))
			if (s.get() == '\n')
				return s;
	}
	
	char c;
	s >> c;
	
	if (s && (c != '\n'))
		s.clear(std::ios::badbit);
		
	return s;
}

//
//  	Create a c_string formatter for a counted string.
//
PrimCString c_string(const char* aString, size_t aCount,  const char* preText, const char* postText,
                     PrimCString::PrimCStringFlags controlFlags) {
	return PrimCString((const unsigned char*)aString, aCount, preText, postText, controlFlags);
}

//
//  	Create a c_string formatter for a null-terminated string.
//
PrimCString c_string(const char* aString,  const char* preText, const char* postText,
                     PrimCString::PrimCStringFlags controlFlags) {
	return PrimCString((const unsigned char*)aString, aString ? strlen(aString) : 0,
	                   preText, postText, controlFlags);
}

//
//  	Generate a C string formatted representation.
//
//  	The most compact form is used by default, using single character escapoes such as \v, octal
//  	escapes for other non-printables, and escaped quotes to guarantee a lack of ambiguity. These
//  	rules mya be changed by the control flags.
//
std::ostream& operator<<(std::ostream& s, const PrimCString& cString) {
	PrimIosSave iosSave(s);
	s << std::oct;
	
	if (cString._pre_text)
		s << cString._pre_text;
		
	const unsigned char* p = cString._string;
	bool prevCharOctal = false;
	const char* backSlash = (cString._flags & cString.STRINGIZE_BACKSLASH) ? "\\\\" : "\\";
	
	for (size_t i = cString._count; i-- > 0; p++) {
		bool thisCharOctal = false;
		
		if (*p == '\\') {
			if (cString._flags & cString.ECHO_BACKSLASH)
				s << *p;
			else if (cString._flags & cString.OCTAL_BACKSLASH) {
				s << backSlash << int(*p);
				thisCharOctal = true;
			}
			else
				s << "\\\\";
		}
		else if ( *p == '\'' ) {
			if ( cString._flags & cString.ECHO_SINGLE_QUOTE )
				s << *p;
			else if ( cString._flags & cString.OCTAL_SINGLE_QUOTE ) {
				s << backSlash << int ( *p );
				thisCharOctal = true;
			}
			else
				s << backSlash << "'";
		}
		else if ( *p == '\"' ) {
			if ( cString._flags & cString.ECHO_DOUBLE_QUOTE )
				s << *p;
			else if ( cString._flags & cString.OCTAL_DOUBLE_QUOTE ) {
				s << backSlash << int ( *p );
				thisCharOctal = true;
			}
			else if ( cString._flags & cString.STRINGIZE_DOUBLE_QUOTE )
				s << "\\\"";
			else
				s << backSlash << "\"";
		}
		else if (isspace(*p)) {
			if ((cString._flags & cString.ECHO_WHITESPACE) || (*p == ' '))
				s << *p;
			else if (cString._flags & cString.OCTAL_WHITESPACE) {
				s << backSlash << int(*p);
				thisCharOctal = true;
			}
			else if (*p == '')
				s << backSlash << "f";
			else if (*p == '\n')
				s << backSlash << "n";
			else if (*p == '\r')
				s << backSlash << "r";
			else if (*p == '\t')
				s << backSlash << "t";
			else if (*p == '')
				s << backSlash << "v";
			else {      //   Never happens - earlier cases are complete.
				s << backSlash << int(*p);
				thisCharOctal = true;
			}
		}
		else if (prevCharOctal && ('0' <= *p ) && (*p <= '7')) {
			s << backSlash << int(*p);
			thisCharOctal = true;
		}
		else if (*p == '')
			s << backSlash << "a";
		else if (*p == '')
			s << backSlash << "b";
		else if ((*p < ' ') || ('~' < *p)) {
			s << backSlash << int(*p);
			thisCharOctal = true;
		}
		else
			s << *p;
			
		prevCharOctal = thisCharOctal;
	}
	
	if (cString._post_text)
		s << cString._post_text;
		
	return s;
}

//
//  	Format an error number and its explanation.
//
std::ostream& operator<<(std::ostream& s, const PrimError& errNo) {
	return s << "errno=" << errNo._err_no << "[" << c_string(strerror(errNo._err_no), "", "") << "]";
}
#endif

#if defined(NEEDS_STD) && !defined(NO_FSTREAM_SUPPORT)
#include <sysent.h>      //   umask()
#include <stat.h>     //   S_xxx

//
//  	Return the default file access mode as identified by umask().
//
int ofstream_open_mode() {
	int oldMode = (int)umask(~0);      //   Get current mode
	umask(oldMode);          //   Restore mode
#if defined(S_IRUSR)
	return ~oldMode & (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
#elif defined(_S_IREAD)
	return ~oldMode & (_S_IWRITE | _S_IREAD);
#else
	return ~oldMode;
#endif
}
#endif
