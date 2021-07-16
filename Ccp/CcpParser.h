#ifndef YY_CcpParser_h_included
#define YY_CcpParser_h_included


/*  before anything */
#ifdef c_plusplus
#ifndef __cplusplus
#define __cplusplus
#endif
#endif
#ifdef __cplusplus
#ifndef YY_USE_CLASS
#define YY_USE_CLASS
#endif
#else
#endif
#include <stdio.h>

/*  #line 14 "../bison_pp/bison.h" */



#include <Ccp/CcpIncludeAll.h>

#define YY_CcpParser_DEBUG  1
#define YY_CcpParser_ENUM_TOKEN  CcpParserEnums
#define YY_CcpParser_STYPE  CcpParserValue
#define YY_CcpParser_INHERIT  : public CcpParserContext
#define YY_CcpParser_MEMBERS  TYPEDECL_SINGLE(CcpParser, CcpParserContext)
#define YY_CcpParser_CONSTRUCTOR_PARAM  std::istream *inputStream, const PrimString& fileName, std::ostream *outputStream
#define YY_CcpParser_CONSTRUCTOR_INIT  : CcpParserContext(inputStream, fileName, outputStream)
#define YY_CcpParser_CONSTRUCTOR_CODE  yydebug = CcpReader::debug_yacc();


 /*  %{ and %header{ and %union, during decl */
#ifndef YY_CcpParser_COMPATIBILITY
#ifndef YY_USE_CLASS
#define  YY_CcpParser_COMPATIBILITY 1
#else
#define  YY_CcpParser_COMPATIBILITY 0
#endif
#endif

#if YY_CcpParser_COMPATIBILITY != 0
/*  backward compatibility */
#ifdef YYLTYPE
#ifndef YY_CcpParser_LTYPE
#define YY_CcpParser_LTYPE YYLTYPE
/*  WARNING obsolete !!! user defined YYLTYPE not reported into generated header */
/*  use %define LTYPE */
#endif
#endif
#ifdef YYSTYPE
#ifndef YY_CcpParser_STYPE 
#define YY_CcpParser_STYPE YYSTYPE
/*  WARNING obsolete !!! user defined YYSTYPE not reported into generated header */
/*  use %define STYPE */
#endif
#endif
#ifdef YYDEBUG
#ifndef YY_CcpParser_DEBUG
#define  YY_CcpParser_DEBUG YYDEBUG
/*  WARNING obsolete !!! user defined YYDEBUG not reported into generated header */
/*  use %define DEBUG */
#endif
#endif
#ifdef YY_CcpParser_STYPE
#ifndef yystype
#define yystype YY_CcpParser_STYPE
#endif
#endif
/*  use goto to be compatible */
#ifndef YY_CcpParser_USE_GOTO
#define YY_CcpParser_USE_GOTO 1
#endif
#endif

/*  use no goto to be clean in C++ */
#ifndef YY_CcpParser_USE_GOTO
#define YY_CcpParser_USE_GOTO 0
#endif

#ifndef YY_CcpParser_PURE

/*  #line 63 "../bison_pp/bison.h" */



/*  YY_CcpParser_PURE */
#endif

/*  #line 65 "../bison_pp/bison.h" */



/*  prefix */
#ifndef YY_CcpParser_DEBUG

/*  #line 67 "../bison_pp/bison.h" */

#define YY_CcpParser_DEBUG 1


/*  YY_CcpParser_DEBUG */
#endif
#ifndef YY_CcpParser_LSP_NEEDED

/*  #line 70 "../bison_pp/bison.h" */



 /*  YY_CcpParser_LSP_NEEDED*/
#endif
/*  DEFAULT LTYPE*/
#ifdef YY_CcpParser_LSP_NEEDED
#ifndef YY_CcpParser_LTYPE
typedef
  struct yyltype
    {
      int timestamp;
      int first_line;
      int first_column;
      int last_line;
      int last_column;
      char *text;
   }
  yyltype;

#define YY_CcpParser_LTYPE yyltype
#endif
#endif
/*  DEFAULT STYPE*/
#ifndef YY_CcpParser_STYPE
#define YY_CcpParser_STYPE int
#endif
/*  DEFAULT MISCELANEOUS */
#ifndef YY_CcpParser_PARSE
#define YY_CcpParser_PARSE yyparse
#endif
#ifndef YY_CcpParser_LEX
#define YY_CcpParser_LEX yylex
#endif
#ifndef YY_CcpParser_LVAL
#define YY_CcpParser_LVAL yylval
#endif
#ifndef YY_CcpParser_LLOC
#define YY_CcpParser_LLOC yylloc
#endif
#ifndef YY_CcpParser_CHAR
#define YY_CcpParser_CHAR yychar
#endif
#ifndef YY_CcpParser_NERRS
#define YY_CcpParser_NERRS yynerrs
#endif
#ifndef YY_CcpParser_DEBUG_FLAG
#define YY_CcpParser_DEBUG_FLAG yydebug
#endif
#ifndef YY_CcpParser_ERROR
#define YY_CcpParser_ERROR yyerror
#endif

#ifndef YY_CcpParser_PARSE_PARAM
#ifndef __STDC__
#ifndef __cplusplus
#ifndef YY_USE_CLASS
#define YY_CcpParser_PARSE_PARAM
#ifndef YY_CcpParser_PARSE_PARAM_DEF
#define YY_CcpParser_PARSE_PARAM_DEF
#endif
#endif
#endif
#endif
#ifndef YY_CcpParser_PARSE_PARAM
#define YY_CcpParser_PARSE_PARAM void
#endif
#endif

/*  TOKEN C */
#ifndef YY_USE_CLASS

#ifndef YY_CcpParser_PURE
extern YY_CcpParser_STYPE YY_CcpParser_LVAL;
#endif


/*  #line 143 "../bison_pp/bison.h" */

#define DEFINE 258
#define ENDDEFINE 259
#define IMPLEMENTATION 260
#define IMPORT 261
#define INTERFACE 262
#define TEST 263
#define Line 264
#define String 265



 /*  #defines token */
/*  after #define tokens, before const tokens S5*/
#else
#ifndef YY_CcpParser_CLASS
#define YY_CcpParser_CLASS CcpParser
#endif

#ifndef YY_CcpParser_INHERIT
#define YY_CcpParser_INHERIT
#endif
#ifndef YY_CcpParser_MEMBERS
#define YY_CcpParser_MEMBERS 
#endif
#ifndef YY_CcpParser_LEX_BODY
#define YY_CcpParser_LEX_BODY  
#endif
#ifndef YY_CcpParser_ERROR_BODY
#define YY_CcpParser_ERROR_BODY  
#endif
#ifndef YY_CcpParser_CONSTRUCTOR_PARAM
#define YY_CcpParser_CONSTRUCTOR_PARAM
#endif
/*  choose between enum and const */
#ifndef YY_CcpParser_USE_CONST_TOKEN
#define YY_CcpParser_USE_CONST_TOKEN 0
/*  yes enum is more compatible with flex,  */
/*  so by default we use it */ 
#endif
#if YY_CcpParser_USE_CONST_TOKEN != 0
#ifndef YY_CcpParser_ENUM_TOKEN
#define YY_CcpParser_ENUM_TOKEN yy_CcpParser_enum_token
#endif
#endif

class YY_CcpParser_CLASS YY_CcpParser_INHERIT
{
public: 
#if YY_CcpParser_USE_CONST_TOKEN != 0
/*  static const int token ... */

/*  #line 182 "../bison_pp/bison.h" */

static const int DEFINE;
static const int ENDDEFINE;
static const int IMPLEMENTATION;
static const int IMPORT;
static const int INTERFACE;
static const int TEST;
static const int Line;
static const int String;



 /*  decl const */
#else
enum YY_CcpParser_ENUM_TOKEN { YY_CcpParser_NULL_TOKEN=0

/*  #line 185 "../bison_pp/bison.h" */

 ,DEFINE=258
 ,ENDDEFINE=259
 ,IMPLEMENTATION=260
 ,IMPORT=261
 ,INTERFACE=262
 ,TEST=263
 ,Line=264
 ,String=265



 /*  enum token */
     }; /*  end of enum declaration */
#endif
public:
 int YY_CcpParser_PARSE(YY_CcpParser_PARSE_PARAM);
 virtual void YY_CcpParser_ERROR(const char *msg) YY_CcpParser_ERROR_BODY;
#ifdef YY_CcpParser_PURE
#ifdef YY_CcpParser_LSP_NEEDED
 virtual int  YY_CcpParser_LEX(YY_CcpParser_STYPE *YY_CcpParser_LVAL,YY_CcpParser_LTYPE *YY_CcpParser_LLOC) YY_CcpParser_LEX_BODY;
#else
 virtual int  YY_CcpParser_LEX(YY_CcpParser_STYPE *YY_CcpParser_LVAL) YY_CcpParser_LEX_BODY;
#endif
#else
 virtual int YY_CcpParser_LEX() YY_CcpParser_LEX_BODY;
 YY_CcpParser_STYPE YY_CcpParser_LVAL;
#ifdef YY_CcpParser_LSP_NEEDED
 YY_CcpParser_LTYPE YY_CcpParser_LLOC;
#endif
 int YY_CcpParser_NERRS;
 int YY_CcpParser_CHAR;
#endif
#if YY_CcpParser_DEBUG != 0
public:
 int YY_CcpParser_DEBUG_FLAG; /*   nonzero means print parse trace	*/
#endif
public:
 YY_CcpParser_CLASS(YY_CcpParser_CONSTRUCTOR_PARAM);
public:
 YY_CcpParser_MEMBERS 
};
/*  other declare folow */
#endif


#if YY_CcpParser_COMPATIBILITY != 0
/*  backward compatibility */
#ifndef YYSTYPE
#define YYSTYPE YY_CcpParser_STYPE
#endif

#ifndef YYLTYPE
#define YYLTYPE YY_CcpParser_LTYPE
#endif
#ifndef YYDEBUG
#ifdef YY_CcpParser_DEBUG 
#define YYDEBUG YY_CcpParser_DEBUG
#endif
#endif

#endif
/*  END */

/*  #line 236 "../bison_pp/bison.h" */

#endif
