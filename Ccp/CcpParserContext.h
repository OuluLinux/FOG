// **************************** RACAL Confidential ****************************
// 
// 	  Title:			Parser context class interface
// 
// 	  File Name:		CcpParserContext.hxx
// 
// 	  Author:			E.D.Willink
// 
// 	  SCCS:				%W% %G%
// 
// 	  Latest Modification:
//  EDW	1.1		Date:	 8-Oct-1996		Original
//  EDW	1.2		Date:	 4-Sep-1997		Add set_test
//  EDW	1.3		Date:	28-Apr-1998		Adjust to PrimStringHandle, use std streams
// END
// 

#ifndef CCPPARSERCONTEXT_HXX
#define CCPPARSERCONTEXT_HXX


// 
// 	CcpParserContext is the base class for CcpParser and implements the current context of the parser.
// 	This class only exists as part of CcpParser, and could be combined therein save for the need to define
// 	everything as BISON++ macros (and exceeding permitted length limits).
// 
class CcpParserContext : public PrimPrint
{
 TYPEDECL_SINGLE(CcpParserContext, PrimPrint)
private:
 CcpLexer _lexer;       //  The lexer tokenising the input stream.
 bool _in_implementation;     //  True if writing to implementation.
 bool _in_interface;       //  True if writing to interface.
 bool _in_test;        //  True if writing to test harness.
 bool _has_implementation;     //  True if an implementation declared.
 bool _has_interface;      //  True if an interface declared.
 bool _has_test;        //  True if a test harness declared.
 PrimText _implementation;     //  The implementation lines.
 PrimText _interface;      //  The interface lines.
 PrimText _test;        //  The test lines.
 PrimRef<CcpDefine> _define;     //  Non-null during define parsing.

private:
 void add_line_number(int lineNumber);

protected:
 CcpParserContext(std::istream *inputStream, const PrimString& fileName, std::ostream *outputStream);
 void add_import(int lineNumber, const PrimString& includeFile,
  const PrimString& guardName = PrimString::null());
 void add_line(const PrimString& aLine);
 void add_line(const char *aLine) { add_line(*PrimStringHandle(aLine)); }
 void create_define(const PrimString& aName);
 void install_define();
 void set_implementation(int lineNumber);
 void set_interface(int lineNumber);
 void set_test(int lineNumber);
// 
// 	Instruct the lexer to produce another token.
// 
 int yylex(CcpParserValue& parserValue) { return _lexer.yylex(parserValue); }

public:
 bool has_implementation() const { return _has_implementation; }
 bool has_interface() const { return _has_interface; }
 bool has_test() const { return _has_test; }
// 
// 	Return the implementation.
// 
 const PrimText& implementation() const { return _implementation; }
// 
// 	Return the interface.
// 
 const PrimText& interface() const { return _interface; }
// 
// 	Return the test.
// 
 const PrimText& test() const { return _test; }
// 
// 	Pass msg to the lexer to get a sensible diagnostic message.
// 
 void yydiag(const char *msg) { _lexer.diagnostic(msg); }
// 
// 	Pass msg to the lexer to get a sensible error message.
// 
 void yyerror(const char *msg) { _lexer.error(msg); }
// 
// 	Pass msg to the lexer to get a sensible warning message.
// 
 void yywarn(const char *msg) { _lexer.warning(msg); }
};

#endif
