
#ifndef PRIMMANIP_HXX
#define PRIMMANIP_HXX


#ifndef NO_PRIM_STREAMS




//  
//  	An instance PrimIosSave may be constructed to save the state of an iostream prior to
//  	changing stream formatting. Destruction of the instance automatically restores the stream state.
//  
class PrimIosSave
{
private:
 std::ios& _ios;       //   ios to be restored by destructor.
 std::ios_base::fmtflags _flags;       //   Flags to be restored.
 int _precision;       //   Precision to be restored.
 char _fill;        //   Fill character to be restored.
 int _width;        //   Field width to be restored.

public:
//  
//  	The constructor saves the state of anIos for restoration by the destructor.
//  
 PrimIosSave(std::ios& anIos)
 :
  _ios(anIos),
  _flags(anIos.flags()),
  _precision(anIos.precision()),
  _fill(anIos.fill()),
  _width(anIos.width())
 {}
//  
//  	The destructor restores the ios state saved by the constructor.
//  
 ~PrimIosSave()
 {
  _ios.flags(_flags);
  _ios.precision(_precision);
  _ios.fill(_fill);
  _ios.width(_width);
 }
};

//  
//  	A PrimIndent is an ostream manipulator class used to generate indentation.
//  .note tutorial
//  	A PrimIndent may be used in the following way to generate five levels (ten spaces)
//  	of indentation:
//  
//  	s << indent(5) << "Some indented text." << endl;
//  
class PrimIndent
{
private:
 int _indentation;       //   Required indentation level.

private:
//  
//  	Construct an indentation descriptor for anIndent
//  
 PrimIndent(int anIndent) : _indentation(anIndent) {}

public:
//  
//  	Return the number of spaces per indentation level.
//  
 static unsigned int spaces_per_indent() { return 2; }
 friend inline PrimIndent indent(int anIndent);
 friend std::ostream& operator<<(std::ostream& s, const PrimIndent& aPrimIndent);
};

//  
//  	Create a spacing integer, so that when printed multiple spaces appear in the stream output.
//  
inline PrimIndent indent(int anIndent) { return PrimIndent(anIndent); }

std::istream& endl(std::istream& s);

#if defined(NEEDS_STD) && !defined(NO_FSTREAM_SUPPORT)
int ofstream_open_mode();
#endif

//  
//  	A PrimCString is an ostream manipulator class used to output a counted text buffer in C format,
//  	translating any binary characters to printable representation.
//  .note tutorial
//  	A PrimCString may be used in the following way
//  
//  	s << c_string(aBuffer, aCount) << endl;		-- a counted string in ""
//  	s << c_string(aString, "'") << endl;		-- null-terminated string in ''
//  	s << c_string(aChar) << endl;				-- single character in ''
//  	s << c_string(aChar, "") << endl;			-- single character without surrounding text
//  
class PrimCString
{
public:
 enum PrimCStringFlags
 {
  ECHO_WHITESPACE = 0x0001,      //   Whitespace as is.
  OCTAL_WHITESPACE = 0x0002,      //   Whitespace as \ooo.
  ECHO_DOUBLE_QUOTE = 0x0004,      //   " as "
  OCTAL_DOUBLE_QUOTE = 0x0008,     //   " as \42
  ECHO_SINGLE_QUOTE = 0x0010,      //   ' as '
  OCTAL_SINGLE_QUOTE = 0x0020,     //   ' as \47
  ECHO_BACKSLASH = 0x0040,      //   \ as \ alone
  OCTAL_BACKSLASH = 0x0080,      //   \ as \144
  STRINGIZE_BACKSLASH = 0x0100,     //   Synthetic \ as \\.
  STRINGIZE_DOUBLE_QUOTE = 0x0200,    //   " as \" even if STRINGIZE_BACKSLASH
  ECHO = ECHO_WHITESPACE | ECHO_SINGLE_QUOTE | ECHO_DOUBLE_QUOTE | ECHO_BACKSLASH,
  STRINGIZE = ECHO_WHITESPACE | ECHO_SINGLE_QUOTE | STRINGIZE_DOUBLE_QUOTE | STRINGIZE_BACKSLASH,
  AS_STRING = ECHO_SINGLE_QUOTE,
  AS_CHARACTER = ECHO_DOUBLE_QUOTE
 };
private:
 const unsigned char *_string;     //   The text characters.
 size_t _count;         //   The character count.
 const char *_pre_text;       //   Prefix text.
 const char *_post_text;       //   Postfix text.
 int _flags;

private:
//  
//  	Construct a c string formatter.
//  
 PrimCString(const unsigned char *aString, size_t aCount, const char *preText, const char *postText,
  PrimCStringFlags controlFlags)
  : _string(aString), _count(aCount), _pre_text(preText), _post_text(postText), _flags(controlFlags) {}

public:
	friend PrimCString c_string(const char *aString, size_t aCount, const char *preText,
			const char *postText, PrimCStringFlags controlFlags);
	friend PrimCString c_string(const signed char *aString, size_t aCount, const char *preText,
			const char *postText, PrimCStringFlags controlFlags);
	friend PrimCString c_string(const unsigned char *aString, size_t aCount, const char *preText,
			const char *postText, PrimCStringFlags controlFlags);
	friend PrimCString c_string(const char *aString, const char *preText,
			const char *postText, PrimCStringFlags controlFlags);
	friend PrimCString c_string(const signed char *aString, const char *preText,
			const char *postText, PrimCStringFlags controlFlags);
	friend PrimCString c_string(const unsigned char *aString, const char *preText,
			const char *postText, PrimCStringFlags controlFlags);
	friend PrimCString c_string(const char aChar, const char *preText,
			const char *postText, PrimCStringFlags controlFlags);
	friend PrimCString c_string(const signed char aChar, const char *preText,
			const char *postText, PrimCStringFlags controlFlags);
	friend PrimCString c_string(const unsigned char aChar, const char *preText,
			const char *postText, PrimCStringFlags controlFlags);
	friend std::ostream& operator<<(std::ostream& s, const PrimCString& cString);

};

PrimCString c_string(const char *aString, size_t aCount, const char *preText = "\"",
   const char *postText = "\"", PrimCString::PrimCStringFlags controlFlags = PrimCString::AS_STRING);
   
PrimCString c_string(const char *aString, const char *preText = "\"",
   const char *postText = "\"", PrimCString::PrimCStringFlags controlFlags = PrimCString::AS_STRING);
  
inline PrimCString c_string(const signed char *aString, size_t aCount, const char *preText = "\"",
		const char *postText = "\"", PrimCString::PrimCStringFlags controlFlags = PrimCString::AS_STRING) {
	return c_string((const char *)aString, aCount, preText, postText, controlFlags);
}

inline PrimCString c_string(const unsigned char *aString, size_t aCount, const char *preText = "\"",
		const char *postText = "\"", PrimCString::PrimCStringFlags controlFlags = PrimCString::AS_STRING) {
	return c_string((const char *)aString, aCount, preText, postText, controlFlags);
}

inline PrimCString c_string(const signed char *aString, const char *preText = "\"",
		const char *postText = "\"", PrimCString::PrimCStringFlags controlFlags = PrimCString::AS_STRING) {
	return c_string((const char *)aString, preText, postText, controlFlags);
}

inline PrimCString c_string(const unsigned char *aString, const char *preText = "\"",
		const char *postText = "\"", PrimCString::PrimCStringFlags controlFlags = PrimCString::AS_STRING) {
	return c_string((const char *)aString, preText, postText, controlFlags);
}

inline PrimCString c_string(const char aChar, const char *preText = "'",
		const char *postText = "'", PrimCString::PrimCStringFlags controlFlags = PrimCString::AS_CHARACTER) {
	return c_string(&aChar, 1, preText, postText, controlFlags);
}

inline PrimCString c_string(const signed char aChar, const char *preText = "'",
		const char *postText = "'", PrimCString::PrimCStringFlags controlFlags = PrimCString::AS_CHARACTER) {
	return c_string((const char *)&aChar, 1, preText, postText, controlFlags);
}

inline PrimCString c_string(const unsigned char aChar, const char *preText = "'",
		const char *postText = "'", PrimCString::PrimCStringFlags controlFlags = PrimCString::AS_CHARACTER) {
	return c_string((const char *)&aChar, 1, preText, postText, controlFlags);
}

//  
//  	A PrimError is an ostream manipulator class used to output an error number nad its corresponding
//  	explanation..
//  .note tutorial
//  	A PrimError may be used in the following way
//  
//  	s << error << endl;		-- most recent error message
//  	s << error(error_number) << endl;	-- explicit error message
//  
//  	however the risk od corruption of errno by other output may merit:
//  
//  	PrimError savedError;
//  	...
//  	s << savedError << endl;
//  
//  
class PrimError
{
private:
 int _err_no;     //   The error number.

public:
//  
//  	Construct a c string formatter.
//  
 PrimError(int errNo = errno) : _err_no(errNo) {}
//  
//  	Construct an error number formatter for the an explicit error.
//  
 friend PrimError error(int errNo = errno) { return PrimError(errNo); }
 friend std::ostream& operator<<(std::ostream& s, const PrimError& errNo);
};
#endif



#endif
 
