//**************************** RACAL Confidential ****************************
//
//	  Title:			Flex++ parser for C++ preprocessor
//
//	  File Name:		CcpLexer.l
//
//	  Author:			E.D.Willink
//
//	  SCCS:				%W% %G%
//
//	  Latest Modification:
// EDW	1.1		Date:	 8-Oct-1996		Original
// EDW	1.2		Date:	 4-Sep-1997		Add TEST
// EDW	1.3		Date:	28-Apr-1998		Adjust to PrimStringHandle
//END
//
%{
#include <CcpParser.hxx>
#ifndef CCPREADER_HXX
#include <CcpReader.hxx>
#endif
#ifndef PRIMADOPTED_H
#include <PrimAdopted.H>
#endif
#ifndef PRIMNOTICE_HXX
#include <PrimNotice.hxx>
#endif
#include <cstring>

#define KEYWORD_TOKEN(name, CODE) \
	{ got_keyword(yylval); CODE return CcpParser::name; }

%}

%header{
#ifndef CCPDEFINE_HXX
#include <CcpDefine.hxx>
#endif
#ifndef CCPLEXERCONTEXT_HXX
#include <CcpLexerContext.hxx>
#endif
#ifndef CCPPARSERVALUE_HXX
#include <CcpParserValue.hxx>
#endif
#ifndef PRIMMAPOFREF_H
#include <PrimMapOfRef.H>
#endif
%}

%name CcpLexer
//
//	Post define state awaiting a definition line
//
%s LEX_DEFINE_BODY 
//
//	Post define state awaiting a definition name
//
%s LEX_DEFINE_ID 
//
//	Post inclusion state awaiting a closing , or ;
//
%s LEX_EXCLUDE 
//
//	Post "library" state awaiting include file name
//
%s LEX_INCLUDE 
//
//	Normal state, first line has been read.
//
%s LEX_NORMAL 
//
//	Lexing state processing tokens after a %keyword.
//
%s LEX_PERCENT 

%define INHERIT : public PrimPrint
%define CONSTRUCTOR_PARAM std::istream *inputStream, const PrimString& fileName, std::ostream *outputStream
%define CONSTRUCTOR_INIT :\
	_string_buffer(0), \
	_string_buffer_capacity(0), \
	_context(fileName)
%define CONSTRUCTOR_CODE \
	yyin = inputStream; \
	yyout = outputStream
%define WRAP_NOCODE
%define LEX_PARAM CcpParserValue& yylval
%define IOSTREAM
%define MEMBERS \
	private: \
		TYPEDECL_SINGLE(CcpLexer, PrimPrint) \
		PrimAdoptedArray<unsigned char> _string_buffer; \
		unsigned int _string_buffer_capacity; \
		CcpLexerContext _context; \
		PrimMapOfRefToConst<CcpDefine> _defines; \
	private: \
		void advance(int numCols) { _context.advance(numCols); } \
		bool expand_line(); \
		void got_include(); \
		void got_keyword(CcpParserValue& yylval); \
		void got_line(CcpParserValue& yylval); \
		void got_token(); \
		std::ostream& message(std::ostream& s, const char *msg) \
			{ return _context.message(s, msg, yytext, yyleng); } \
		bool set_string_buffer_capacity(unsigned int aCapacity); \
		const char *text() const { return (const char *)yytext; } \
	public: \
		void add(const CcpDefine& aDefine) { _defines.add(aDefine); } \
		void diagnostic(const char *msg); \
		void error(const char *msg); \
		const char *file() const { return _context.file(); } \
		void warning(const char *msg);

identifier						[a-zA-Z0-9]+
letters							[\x21-\x7e]+

%%
<INITIAL>\%.*					{				// Read, buffer and reject non-blank line 1.
									_context.buffer_line(yytext, yyleng, false);
									if (!expand_line())
										yyless(1);
									BEGIN(LEX_PERCENT);
								}
<INITIAL>.*						{				// Read, buffer and reject non-blank line 1.
									_context.buffer_line(yytext, yyleng, false);
									if (!expand_line())
										yyless(0);
									BEGIN(LEX_NORMAL);
									if (!_context.text() || !*_context.text())
									{
										yylval.string() = 0;
										CONDMSG(CcpReader::debug_lex(), _context.line() << " BLANK LINE");
										return CcpParser::Line;
									}											}
<INITIAL>\n%.*					{				// Read blank line 1 and then buffer and reject line 2.
									_context.buffer_line(&yytext[1], yyleng-1, true);
									if (!expand_line())
										yyless(2);
									BEGIN(LEX_PERCENT);
								}
<INITIAL>\n.*					{				// Read blank line 1 and then buffer and reject line 2.
									_context.buffer_line(&yytext[1], yyleng-1, true);
									if (!expand_line())
										yyless(1);
									BEGIN(LEX_NORMAL);
									if (!_context.text() || !*_context.text())
									{
										yylval.string() = 0;
										CONDMSG(CcpReader::debug_lex(), _context.line() << " BLANK LINE");
										return CcpParser::Line;
									}											}
\n\%.*							{				// Read and buffer any subsequent line.
									_context.buffer_line(&yytext[1], yyleng-1, false);
									if (!expand_line())
										yyless(2);
									BEGIN(LEX_PERCENT);
								}
\n.*							{				// Read and buffer any subsequent line.
									_context.buffer_line(&yytext[1], yyleng-1, false);
									if (!expand_line())
										yyless(1);
									BEGIN(LEX_NORMAL);
									if (!_context.text() || !*_context.text())
									{
										yylval.string() = 0;
										CONDMSG(CcpReader::debug_lex(), _context.line() << " BLANK LINE");
										return CcpParser::Line;
									}									
								}
<LEX_PERCENT>\%.* 				{ CONDMSG(CcpReader::debug_lex(),
									_context.line() << " %COMMENT \" " << text() << "\""); }

<LEX_PERCENT>define				KEYWORD_TOKEN(DEFINE,;)
<LEX_PERCENT>enddefine			KEYWORD_TOKEN(ENDDEFINE,;)
<LEX_PERCENT>implementation		KEYWORD_TOKEN(IMPLEMENTATION,;)
<LEX_PERCENT>import				KEYWORD_TOKEN(IMPORT,;)
<LEX_PERCENT>test				KEYWORD_TOKEN(TEST,;)

<LEX_PERCENT>include			{ got_keyword(yylval); BEGIN(LEX_INCLUDE); }
<LEX_INCLUDE>[ \t]+				{ advance(yyleng); CONDMSG(CcpReader::debug_lex(),
									_context.line() << " SPACING \"" << text() << "\""); }
<LEX_INCLUDE>{letters}			{ got_include(); }
<LEX_EXCLUDE>.*					{}

<LEX_PERCENT>interface			KEYWORD_TOKEN(INTERFACE,;)
<LEX_PERCENT>{letters}			{ got_line(yylval); return CcpParser::String; }
<LEX_PERCENT>[ \t]+				{ advance(yyleng); CONDMSG(CcpReader::debug_lex(),
									_context.line() << " %SPACING \"" << text() << "\""); }

<LEX_PERCENT>.					{ advance(yyleng); CONDMSG(CcpReader::debug_lex(),
									_context.line() << " %. \"" << text() << "\"");}
<LEX_NORMAL>.*					{ got_line(yylval); return CcpParser::Line; }
.								{ advance(yyleng); CONDMSG(CcpReader::debug_lex(),
									_context.line() << " . \"" << text() << "\"");}
%%
#ifdef ECHO
#undef ECHO
#endif

TYPEINFO_SINGLE(CcpLexer, PrimPrint)

#ifndef PRIMMANIP_HXX
#include <PrimManip.hxx>
#endif
#include <cctype>
#include <climits>

//
//	Generate a diagnostic using msg to explain a problem with respect to the source context.
//
void CcpLexer::diagnostic(const char *msg)
{
	std::ostrstream s;
	message(s, msg);
	s << std::ends;
	DIAMSG(s.str());
	delete s.str();
}

//
//	Generate an error diagnostic using msg to explain a problem with respect to the source context.
//
void CcpLexer::error(const char *msg)
{
	std::ostrstream s;
	message(s, msg);
	s << std::ends;
	ERRMSG(s.str());
	delete s.str();
}

//
//	Return true if new input line got expanded and so is available for reread.
//
bool CcpLexer::expand_line()
{
	const unsigned char *pBegin = 0;
	const unsigned char *pEnd = 0;
	for (const unsigned char *p = _context.text(); *p; p++)
	{
		if ((*p == '$') && (*(p+1) == '{'))
			pBegin = p;
		else if (pBegin && (*p == '}'))
		{
			pEnd = p;
			break;
		}
	}
	if (!pBegin)
		return false;
	if (!pEnd)
	{
		error("unclosed macro instantiation");
		return false;
	}
	const PrimIdHandle defineName((const char *)(pBegin+2), pEnd - (pBegin+2));
	const CcpDefine *aDefine = _defines.find(*defineName);
	if (!aDefine)
	{
		error("undefined macro instantiation");
		return false;
	}
	std::strstream *s = new std::strstream;
	s->write((const char *)_context.text(), pBegin - _context.text());
	bool firstOutput = true; 
	for (PrimTextIterator q(aDefine->text()); q; ++q, firstOutput = false)
	{
		if (!firstOutput)
			*s << "\n";
		*s << *q;
	}
	*s << (const char *)(pEnd+1) << std::ends;
	_context.push(*PrimStringHandle(aDefine->id().str()), YY_CURRENT_BUFFER);
	yy_switch_to_buffer(yy_create_buffer(s, YY_BUF_SIZE));
	BEGIN(INITIAL);
	return true;
}

//
//	Establish aName as the new source file.
//
void CcpLexer::got_include()
{
	CONDMSG(CcpReader::debug_lex(), _context.line() << " INCLUDE \"" << text() << "\"");
	std::ifstream *aStream = 0;
	for (PrimTextIterator p1(CcpReader::include_paths()); p1; ++p1)
	{
		unsigned int len = strlen(p1->str()) + 1 + yyleng;
		if (!set_string_buffer_capacity(len + 1))
			return;
		strcpy((char *)_string_buffer.get(), p1->str());
		strcat((char *)_string_buffer.get(), "/");
		strcat((char *)_string_buffer.get(), text());
		aStream = new std::ifstream((char *)_string_buffer.get());
		if (!aStream)
			;
		else if (!*aStream)
		{
			delete aStream;
			aStream = 0;
		}
		else
			break;
	}
	if (!aStream || !*aStream)
	{
		ERRMSG(*this << " failed to open any file for " << c_string(text()));
		BEGIN(LEX_EXCLUDE);
	}
	else
	{
		_context.push(*PrimStringHandle((char *)_string_buffer.get()), YY_CURRENT_BUFFER);
		yy_switch_to_buffer(yy_create_buffer(aStream, YY_BUF_SIZE));
		BEGIN(INITIAL);
	}
}

//
//	Configure the lexer to reflect successful parsing of a keyword.
//
void CcpLexer::got_keyword(CcpParserValue& yylval)
{
	advance(yyleng);
	yylval.number() = _context.line();
	CONDMSG(CcpReader::debug_lex(), _context.line() << " KEYWORD \"" << text() << "\"");
}

//
//	Configure the lexer to reflect successful parsing of a line.
//
void CcpLexer::got_line(CcpParserValue& yylval)
{
	advance(yyleng);
	yylval.string() = text();
	CONDMSG(CcpReader::debug_lex(), _context.line() << " LINE " << yylval.string());
}

//
//	Configure the lexer to reflect successful parsing of a character token.
//
void CcpLexer::got_token()
{
	advance(yyleng);
	CONDMSG(CcpReader::debug_lex(), _context.line() << " TOKEN \"" << char(yytext[0]) << "\"");
}

//
//	Set the capacity of the string buffer to at least aCapacity, returning true if successful.
//
bool CcpLexer::set_string_buffer_capacity(unsigned int aCapacity)
{
	if (_string_buffer_capacity < aCapacity)
	{
		if (aCapacity <= 256)
			aCapacity = 257;
		else if (aCapacity <= 1024)
			aCapacity = 1025;
		else if (aCapacity <= 4096)
			aCapacity = 4097;
		_string_buffer.reset(new unsigned char[aCapacity]);
		if (_string_buffer)
			_string_buffer_capacity = aCapacity;
		else
		{
			_string_buffer_capacity = 0;
			ERRMSG(*this << "Failed to allocate mermory for " << aCapacity << " character string buffer.");
		}
	}
	return _string_buffer != 0;
}

//
//	Generate a warning diagnostic using msg to explain a problem with respect to the source context.
//
void CcpLexer::warning(const char *msg)
{
	std::ostrstream s;
	message(s, msg);
	s << std::ends;
	WRNMSG(s.str());
	delete s.str();
}

//
//	Restore the previous source file reading context from the stack.
//
int CcpLexer::yywrap()
{
	CONDMSG(CcpReader::debug_lex(), _context.line() << " End of file " << c_string(file()));
	std::istream *deadStream = yyin;
	struct yy_buffer_state *deadBuffer = YY_CURRENT_BUFFER;
	if (!_context.pop())
		return 1;
	yy_switch_to_buffer(_context.buffer());
	delete deadStream;
	yy_delete_buffer(deadBuffer);
	BEGIN(LEX_EXCLUDE);
	return 0;
}
