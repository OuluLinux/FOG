//**************************** RACAL Confidential ****************************
//
//	Title:			Bison++ parser for C++ preprocessor
//
//	File Name:		CppParser.y
//
//	Author:			E.D.Willink
//
//	SCCS:				%W% %G%
//
//	Latest Modification:
// EDW	1.1		Date:	20-Sep-1996		Original
// EDW	1.2		Date:	 4-Sep-1997		Add TEST
// EDW	1.3		Date:	28-Apr-1998		Adjust to PrimStringHandle, use std streams
//END
//

%header{
#ifndef CCPPARSERCONTEXT_HXX
#include <CcpParserContext.hxx>
#endif
#ifndef CCPPARSERVALUE_HXX
#include <CcpParserValue.hxx>
#endif
#ifndef CCPREADER_HXX
#include <CcpReader.hxx>
#endif
%}

%name CcpParser

%define DEBUG 1
%define ENUM_TOKEN CcpParserEnums
%define STYPE CcpParserValue
%define INHERIT : public CcpParserContext
%define MEMBERS TYPEDECL_SINGLE(CcpParser, CcpParserContext)
%define CONSTRUCTOR_PARAM std::istream *inputStream, const PrimString& fileName, std::ostream *outputStream
%define CONSTRUCTOR_INIT : CcpParserContext(inputStream, fileName, outputStream)
%define CONSTRUCTOR_CODE yydebug = CcpReader::debug_yacc();

%{
#include <cstdio>
%}
%token DEFINE ENDDEFINE
%token<number()> IMPLEMENTATION IMPORT INTERFACE TEST
%token<string()> Line String
%%
start:					file										{}

file:					/* empty */
	|					file line

line:					IMPLEMENTATION								{ set_implementation($1); }
	|					INTERFACE									{ set_interface($1); }
	|					TEST										{ set_test($1); }
	|					IMPORT String								{ add_import($1, *$2); }
	|					IMPORT String String						{ add_import($1, *$2, *$3); }
	|					Line										{ add_line(*$1); }
	|					define ENDDEFINE							{ install_define(); }

define:					DEFINE String								{ create_define(*$2); }
	|					define Line									{ add_line(*$2); }
%%
TYPEINFO_SINGLE(CcpParser, CcpParserContext)

//
//	Pass msg to the lexer to get a sensible error message.
//
void CcpParser::yyerror(char *msg) { CcpParserContext::yyerror(msg); }

//
//	Instruct the lexer to produce another token.
//
int CcpParser::yylex() { return (CcpParserContext::yylex(yylval)); }
