#define YY_CcpParser_h_included

/*   A Bison++ parser, made from CcpParser.y  */

 /*  with Bison++ version bison++ Version 1.21-8(long), adapted from GNU bison by coetmeur@icdc.fr
  */



/*  -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
/*  Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Bob Corbett and Richard Stallman

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 1, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/*  HEADER SECTION */
#if defined( _MSDOS ) || defined(MSDOS) || defined(__MSDOS__) 
#define __MSDOS_AND_ALIKE
#endif
#if defined(_MSC_VER)
#define __HAVE_NO_ALLOCA
#define __MSDOS_AND_ALIKE
#endif

#ifndef alloca
#if defined( __GNUC__)
#define alloca __builtin_alloca

#elif (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc)  || defined (__sgi)
#include <alloca.h>

#elif defined (__MSDOS_AND_ALIKE)
#include <malloc.h>
#ifndef __TURBOC__
/*  MS C runtime lib */
#define alloca _alloca
#endif

#elif defined(_AIX)
#include <malloc.h>
#pragma alloca

#elif defined(__hpux)
#ifdef __cplusplus
extern "C" {
void *alloca (unsigned int);
};
#else /*  not __cplusplus */
void *alloca ();
#endif /*  not __cplusplus */

#endif /*  not _AIX  not MSDOS, or __TURBOC__ or _AIX, not sparc.  */
#endif /*  alloca not defined.  */
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
#ifndef __STDC__
#define const
#endif
#endif
#include <stdio.h>
#define YYBISON 1  

/*  #line 73 "../bison_pp/bison.cc" */



#include <Ccp/CcpIncludeAll.h>

#define YY_CcpParser_DEBUG  1
#define YY_CcpParser_ENUM_TOKEN  CcpParserEnums
#define YY_CcpParser_STYPE  CcpParserValue
#define YY_CcpParser_INHERIT  : public CcpParserContext
#define YY_CcpParser_MEMBERS  TYPEDECL_SINGLE(CcpParser, CcpParserContext)
#define YY_CcpParser_CONSTRUCTOR_PARAM  std::istream *inputStream, const PrimString& fileName, std::ostream *outputStream
#define YY_CcpParser_CONSTRUCTOR_INIT  : CcpParserContext(inputStream, fileName, outputStream)
#define YY_CcpParser_CONSTRUCTOR_CODE  yydebug = CcpReader::debug_yacc();


#include <cstdio>


/*  %{ and %header{ and %union, during decl */
#define YY_CcpParser_BISON 1
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
#endif
#endif
#ifdef YYSTYPE
#ifndef YY_CcpParser_STYPE 
#define YY_CcpParser_STYPE YYSTYPE
#endif
#endif
#ifdef YYDEBUG
#ifndef YY_CcpParser_DEBUG
#define  YY_CcpParser_DEBUG YYDEBUG
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

/*  #line 117 "../bison_pp/bison.cc" */



/*   YY_CcpParser_PURE */
#endif

/*  section apres lecture def, avant lecture grammaire S2 */

/*  #line 121 "../bison_pp/bison.cc" */



/*  prefix */
#ifndef YY_CcpParser_DEBUG

/*  #line 123 "../bison_pp/bison.cc" */

#define YY_CcpParser_DEBUG 1


/*  YY_CcpParser_DEBUG */
#endif


#ifndef YY_CcpParser_LSP_NEEDED

/*  #line 128 "../bison_pp/bison.cc" */



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
      /*  We used to use `unsigned long' as YY_CcpParser_STYPE on MSDOS,
	 but it seems better to be consistent.
	 Most programs should declare their own type anyway.  */

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
#if YY_CcpParser_COMPATIBILITY != 0
/*  backward compatibility */
#ifdef YY_CcpParser_LTYPE
#ifndef YYLTYPE
#define YYLTYPE YY_CcpParser_LTYPE
#else
/*  WARNING obsolete !!! user defined YYLTYPE not reported into generated header */
#endif
#endif
#ifndef YYSTYPE
#define YYSTYPE YY_CcpParser_STYPE
#else
/*  WARNING obsolete !!! user defined YYSTYPE not reported into generated header */
#endif
#ifdef YY_CcpParser_PURE
#ifndef YYPURE
#define YYPURE YY_CcpParser_PURE
#endif
#endif
#ifdef YY_CcpParser_DEBUG
#ifndef YYDEBUG
#define YYDEBUG YY_CcpParser_DEBUG 
#endif
#endif
#ifndef YY_CcpParser_ERROR_VERBOSE
#ifdef YYERROR_VERBOSE
#define YY_CcpParser_ERROR_VERBOSE YYERROR_VERBOSE
#endif
#endif
#ifndef YY_CcpParser_LSP_NEEDED
#ifdef YYLSP_NEEDED
#define YY_CcpParser_LSP_NEEDED YYLSP_NEEDED
#endif
#endif
#endif
#ifndef YY_USE_CLASS
/*  TOKEN C */

/*  #line 236 "../bison_pp/bison.cc" */

#define DEFINE 258
#define ENDDEFINE 259
#define IMPLEMENTATION 260
#define IMPORT 261
#define INTERFACE 262
#define TEST 263
#define Line 264
#define String 265



 /*  #defines tokens */
#else
/*  CLASS */
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
#ifndef YY_CcpParser_CONSTRUCTOR_CODE
#define YY_CcpParser_CONSTRUCTOR_CODE
#endif
#ifndef YY_CcpParser_CONSTRUCTOR_INIT
#define YY_CcpParser_CONSTRUCTOR_INIT
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

/*  #line 280 "../bison_pp/bison.cc" */

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

/*  #line 283 "../bison_pp/bison.cc" */

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
 int YY_CcpParser_PARSE (YY_CcpParser_PARSE_PARAM);
 virtual void YY_CcpParser_ERROR(const char *msg) YY_CcpParser_ERROR_BODY;
#ifdef YY_CcpParser_PURE
#ifdef YY_CcpParser_LSP_NEEDED
 virtual int  YY_CcpParser_LEX (YY_CcpParser_STYPE *YY_CcpParser_LVAL,YY_CcpParser_LTYPE *YY_CcpParser_LLOC) YY_CcpParser_LEX_BODY;
#else
 virtual int  YY_CcpParser_LEX (YY_CcpParser_STYPE *YY_CcpParser_LVAL) YY_CcpParser_LEX_BODY;
#endif
#else
 virtual int YY_CcpParser_LEX() YY_CcpParser_LEX_BODY;
 YY_CcpParser_STYPE YY_CcpParser_LVAL;
#ifdef YY_CcpParser_LSP_NEEDED
 YY_CcpParser_LTYPE YY_CcpParser_LLOC;
#endif
 int   YY_CcpParser_NERRS;
 int    YY_CcpParser_CHAR;
#endif
#if YY_CcpParser_DEBUG != 0
 int YY_CcpParser_DEBUG_FLAG;   /*   nonzero means print parse trace     */
#endif
public:
 YY_CcpParser_CLASS(YY_CcpParser_CONSTRUCTOR_PARAM);
public:
 YY_CcpParser_MEMBERS 
};
/*  other declare folow */
#if YY_CcpParser_USE_CONST_TOKEN != 0

/*  #line 314 "../bison_pp/bison.cc" */

const int YY_CcpParser_CLASS::DEFINE=258;
const int YY_CcpParser_CLASS::ENDDEFINE=259;
const int YY_CcpParser_CLASS::IMPLEMENTATION=260;
const int YY_CcpParser_CLASS::IMPORT=261;
const int YY_CcpParser_CLASS::INTERFACE=262;
const int YY_CcpParser_CLASS::TEST=263;
const int YY_CcpParser_CLASS::Line=264;
const int YY_CcpParser_CLASS::String=265;



 /*  const YY_CcpParser_CLASS::token */
#endif
/* apres const  */
YY_CcpParser_CLASS::YY_CcpParser_CLASS(YY_CcpParser_CONSTRUCTOR_PARAM) YY_CcpParser_CONSTRUCTOR_INIT
{
#if YY_CcpParser_DEBUG != 0
YY_CcpParser_DEBUG_FLAG=0;
#endif
YY_CcpParser_CONSTRUCTOR_CODE;
};
#endif

/*  #line 325 "../bison_pp/bison.cc" */



#define YYFINAL  17
#define YYFLAG  -99999
#define YYNTBASE 11

#define YYTRANSLATE(x) ((unsigned)(x) <= 265 ? yytranslate[x] : 15)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    10
};

#if YY_CcpParser_DEBUG != 0
static const long yyprhs[] = {     0,
     0,     2,     3,     6,     8,    10,    12,    15,    19,    21,
    24,    27
};

static const long yyrhs[] = {    12,
     0,     0,    12,    13,     0,     5,     0,     7,     0,     8,
     0,     6,    10,     0,     6,    10,    10,     0,     9,     0,
    14,     4,     0,     3,    10,     0,    14,     9,     0
};

#endif

#if YY_CcpParser_DEBUG != 0
static const long yyrline[] = { 0,
    48,    50,    51,    53,    54,    55,    56,    57,    58,    59,
    61,    62
};

static const char * const yytname[] = {   "$","error","$illegal.","DEFINE","ENDDEFINE",
"IMPLEMENTATION","IMPORT","INTERFACE","TEST","Line","String","start","file",
"line","define",""
};
#endif

static const long yyr1[] = {     0,
    11,    12,    12,    13,    13,    13,    13,    13,    13,    13,
    14,    14
};

static const long yyr2[] = {     0,
     1,     0,     2,     1,     1,     1,     2,     3,     1,     2,
     2,     2
};

static const long yydefact[] = {     2,
     1,     0,     4,     0,     5,     6,     9,     3,     0,    11,
     7,    10,    12,     8,     0,     0,     0
};

static const long yydefgoto[] = {    15,
     1,     8,     9
};

static const long yypact[] = {-99999,
    -3,    -9,-99999,    -2,-99999,-99999,-99999,-99999,     3,-99999,
    -1,-99999,-99999,-99999,    10,    11,-99999
};

static const long yypgoto[] = {-99999,
-99999,-99999,-99999
};


#define YYLAST  12


static const long yytable[] = {     2,
    10,     3,     4,     5,     6,     7,    12,    11,    14,    16,
    17,    13
};

static const long yycheck[] = {     3,
    10,     5,     6,     7,     8,     9,     4,    10,    10,     0,
     0,     9
};


 /*  fattrs + tables */

/*  parser code folow  */


/*  This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/*  Note: dollar marks section change
   the next  is replaced by the list of actions, each action
   as one case of the switch.  */ 

#if YY_CcpParser_USE_GOTO != 0
/*  
 SUPRESSION OF GOTO : on some C++ compiler (sun c++)
  the goto is strictly forbidden if any constructor/destructor
  is used in the whole function (very stupid isn't it ?)
 so goto are to be replaced with a 'while/switch/case construct'
 here are the macro to keep some apparent compatibility
*/
#define YYGOTO(lb) {yy_gotostate=lb;continue;}
#define YYBEGINGOTO  enum yy_labels yy_gotostate=yygotostart; \
                     for(;;) switch(yy_gotostate) { case yygotostart: {
#define YYLABEL(lb) } case lb: {
#define YYENDGOTO } } 
#define YYBEGINDECLARELABEL enum yy_labels {yygotostart
#define YYDECLARELABEL(lb) ,lb
#define YYENDDECLARELABEL  };
#else
/*  macro to keep goto */
#define YYGOTO(lb) goto lb
#define YYBEGINGOTO 
#define YYLABEL(lb) lb:
#define YYENDGOTO
#define YYBEGINDECLARELABEL 
#define YYDECLARELABEL(lb)
#define YYENDDECLARELABEL 
#endif
/*  LABEL DECLARATION */
YYBEGINDECLARELABEL
  YYDECLARELABEL(yynewstate)
  YYDECLARELABEL(yybackup)
/*  YYDECLARELABEL(yyresume) */
  YYDECLARELABEL(yydefault)
  YYDECLARELABEL(yyreduce)
  YYDECLARELABEL(yyerrlab)   /*  here on detecting error */
  YYDECLARELABEL(yyerrlab1)   /*  here on error raised explicitly by an action */
  YYDECLARELABEL(yyerrdefault)  /*  current state does not do anything special for the error token. */
  YYDECLARELABEL(yyerrpop)   /*  pop the current state because it cannot handle the error token */
  YYDECLARELABEL(yyerrhandle)  
YYENDDECLARELABEL
/*  ALLOCA SIMULATION */
/*  __HAVE_NO_ALLOCA */
#ifdef __HAVE_NO_ALLOCA
#ifdef __cplusplus
inline
#endif
int __alloca_free_ptr(char *ptr,char *ref)
{if(ptr!=ref) free(ptr);
 return 0;}

#define __ALLOCA_alloca(size) malloc(size)
#define __ALLOCA_free(ptr,ref) __alloca_free_ptr((char *)ptr,(char *)ref)

#ifdef YY_CcpParser_LSP_NEEDED
#define __ALLOCA_return(num) \
            return( __ALLOCA_free(yyss,yyssa)+\
      __ALLOCA_free(yyvs,yyvsa)+\
      __ALLOCA_free(yyls,yylsa)+\
     (num))
#else
#define __ALLOCA_return(num) \
            return( __ALLOCA_free(yyss,yyssa)+\
      __ALLOCA_free(yyvs,yyvsa)+\
     (num))
#endif
#else
#define __ALLOCA_return(num) return(num)
#define __ALLOCA_alloca(size) alloca(size)
#define __ALLOCA_free(ptr,ref) 
#endif

/*  ENDALLOCA SIMULATION */

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (YY_CcpParser_CHAR = YYEMPTY)
#define YYEMPTY         -2
#define YYEOF           0
#define YYACCEPT        __ALLOCA_return(0)
#define YYABORT         __ALLOCA_return(1)
#define YYERROR         YYGOTO(yyerrlab1)
/*  Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL          YYGOTO(yyerrlab)
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do                                                              \
  if (YY_CcpParser_CHAR == YYEMPTY && yylen == 1)                               \
    { YY_CcpParser_CHAR = (token), YY_CcpParser_LVAL = (value);                 \
      yychar1 = YYTRANSLATE (YY_CcpParser_CHAR);                                \
      YYPOPSTACK;                                               \
      YYGOTO(yybackup);                                            \
    }                                                           \
  else                                                          \
    { YY_CcpParser_ERROR ("syntax error: cannot back up"); YYERROR; }   \
while (0)

#define YYTERROR        1
#define YYERRCODE       256

#ifndef YY_CcpParser_PURE
/*  UNPURE */
#define YYLEX           YY_CcpParser_LEX()
#ifndef YY_USE_CLASS
/*  If nonreentrant, and not class , generate the variables here */
int     YY_CcpParser_CHAR;                      /*   the lookahead symbol        */
YY_CcpParser_STYPE      YY_CcpParser_LVAL;              /*   the semantic value of the */
    /*   lookahead symbol    */
int YY_CcpParser_NERRS;                 /*   number of parse errors so far */
#ifdef YY_CcpParser_LSP_NEEDED
YY_CcpParser_LTYPE YY_CcpParser_LLOC;   /*   location data for the lookahead     */
   /*   symbol                              */
#endif
#endif


#else
/*  PURE */
#ifdef YY_CcpParser_LSP_NEEDED
#define YYLEX           YY_CcpParser_LEX(&YY_CcpParser_LVAL, &YY_CcpParser_LLOC)
#else
#define YYLEX           YY_CcpParser_LEX(&YY_CcpParser_LVAL)
#endif
#endif
#ifndef YY_USE_CLASS
#if YY_CcpParser_DEBUG != 0
int YY_CcpParser_DEBUG_FLAG;                    /*   nonzero means print parse trace     */
/*  Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif
#endif



/*   YYINITDEPTH indicates the initial size of the parser's stacks       */

#ifndef YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*   YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif


#if __GNUC__ > 1                /*  GNU C and GNU C++ define this.  */
#define __yy_bcopy(FROM,TO,COUNT)       __builtin_memcpy(TO,FROM,COUNT)
#else                           /*  not GNU C or C++ */

/*  This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */

#ifdef __cplusplus
static void __yy_bcopy (char *from, char *to, int count)
#else
#ifdef __STDC__
static void __yy_bcopy (char *from, char *to, int count)
#else
static void __yy_bcopy (from, to, count)
     char *from;
     char *to;
     int count;
#endif
#endif
{
  char *f = from;
  char *t = to;
  int i = count;

  while (i-- > 0)
    *t++ = *f++;
}
#endif

int
#ifdef YY_USE_CLASS
 YY_CcpParser_CLASS::
#endif
     YY_CcpParser_PARSE(YY_CcpParser_PARSE_PARAM)
#ifndef __STDC__
#ifndef __cplusplus
#ifndef YY_USE_CLASS
/*  parameter definition without protypes */
YY_CcpParser_PARSE_PARAM_DEF
#endif
#endif
#endif
{
  int yystate;
  int yyn;
  short *yyssp;
  YY_CcpParser_STYPE *yyvsp;
  int yyerrstatus;      /*   number of tokens to shift before error messages enabled */
  int yychar1=0;          /*   lookahead token as an internal (translated) token number */

  short yyssa[YYINITDEPTH];     /*   the state stack                     */
  YY_CcpParser_STYPE yyvsa[YYINITDEPTH];        /*   the semantic value stack            */

  short *yyss = yyssa;          /*   refer to the stacks thru separate pointers */
  YY_CcpParser_STYPE *yyvs = yyvsa;     /*   to allow yyoverflow to reallocate them elsewhere */

#ifdef YY_CcpParser_LSP_NEEDED
  YY_CcpParser_LTYPE yylsa[YYINITDEPTH];        /*   the location stack                  */
  YY_CcpParser_LTYPE *yyls = yylsa;
  YY_CcpParser_LTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YY_CcpParser_PURE
  int YY_CcpParser_CHAR;
  YY_CcpParser_STYPE YY_CcpParser_LVAL;
  int YY_CcpParser_NERRS;
#ifdef YY_CcpParser_LSP_NEEDED
  YY_CcpParser_LTYPE YY_CcpParser_LLOC;
#endif
#endif

  YY_CcpParser_STYPE yyval;             /*   the variable used to return         */
    /*   semantic values from the action     */
    /*   routines                            */

  int yylen;
/*  start loop, in which YYGOTO may be used. */
YYBEGINGOTO

#if YY_CcpParser_DEBUG != 0
  if (YY_CcpParser_DEBUG_FLAG)
    fprintf(stderr, "Starting parse\n");
#endif
  yystate = 0;
  yyerrstatus = 0;
  YY_CcpParser_NERRS = 0;
  YY_CcpParser_CHAR = YYEMPTY;          /*  Cause a token to be read.  */

  /*  Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YY_CcpParser_LSP_NEEDED
  yylsp = yyls;
#endif

/*  Push a new state, which is found in  yystate  .  */
/*  In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
YYLABEL(yynewstate)

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /*  Give user a chance to reallocate the stack */
      /*  Use copies of these so that the &'s don't force the real ones into memory. */
      YY_CcpParser_STYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YY_CcpParser_LSP_NEEDED
      YY_CcpParser_LTYPE *yyls1 = yyls;
#endif

      /*  Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /*  Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YY_CcpParser_LSP_NEEDED
      /*  This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
   &yyss1, size * sizeof (*yyssp),
   &yyvs1, size * sizeof (*yyvsp),
   &yyls1, size * sizeof (*yylsp),
   &yystacksize);
#else
      yyoverflow("parser stack overflow",
   &yyss1, size * sizeof (*yyssp),
   &yyvs1, size * sizeof (*yyvsp),
   &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YY_CcpParser_LSP_NEEDED
      yyls = yyls1;
#endif
#else /*  no yyoverflow */
      /*  Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
 {
   YY_CcpParser_ERROR("parser stack overflow");
   __ALLOCA_return(2);
 }
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
 yystacksize = YYMAXDEPTH;
      yyss = (short *) __ALLOCA_alloca (yystacksize * sizeof (*yyssp));
      __yy_bcopy ((char *)yyss1, (char *)yyss, size * sizeof (*yyssp));
      __ALLOCA_free(yyss1,yyssa);
      yyvs = (YY_CcpParser_STYPE *) __ALLOCA_alloca (yystacksize * sizeof (*yyvsp));
      __yy_bcopy ((char *)yyvs1, (char *)yyvs, size * sizeof (*yyvsp));
      __ALLOCA_free(yyvs1,yyvsa);
#ifdef YY_CcpParser_LSP_NEEDED
      yyls = (YY_CcpParser_LTYPE *) __ALLOCA_alloca (yystacksize * sizeof (*yylsp));
      __yy_bcopy ((char *)yyls1, (char *)yyls, size * sizeof (*yylsp));
      __ALLOCA_free(yyls1,yylsa);
#endif
#endif /*  no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YY_CcpParser_LSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YY_CcpParser_DEBUG != 0
      if (YY_CcpParser_DEBUG_FLAG)
 fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
 YYABORT;
    }

#if YY_CcpParser_DEBUG != 0
  if (YY_CcpParser_DEBUG_FLAG)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  YYGOTO(yybackup);
YYLABEL(yybackup)

/*  Do appropriate processing given the current state.  */
/*  Read a lookahead token if we need one and don't already have one.  */
/*  YYLABEL(yyresume) */

  /*  First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    YYGOTO(yydefault);

  /*  Not known => get a lookahead token if don't already have one.  */

  /*  yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (YY_CcpParser_CHAR == YYEMPTY)
    {
#if YY_CcpParser_DEBUG != 0
      if (YY_CcpParser_DEBUG_FLAG)
 fprintf(stderr, "Reading a token: ");
#endif
      YY_CcpParser_CHAR = YYLEX;
    }

  /*  Convert token to internal form (in yychar1) for indexing tables with */

  if (YY_CcpParser_CHAR <= 0)           /*  This means end of input. */
    {
      yychar1 = 0;
      YY_CcpParser_CHAR = YYEOF;                /*  Don't call YYLEX any more */

#if YY_CcpParser_DEBUG != 0
      if (YY_CcpParser_DEBUG_FLAG)
 fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(YY_CcpParser_CHAR);

#if YY_CcpParser_DEBUG != 0
      if (YY_CcpParser_DEBUG_FLAG)
 {
   fprintf (stderr, "Next token is %d (%s", YY_CcpParser_CHAR, yytname[yychar1]);
   /*  Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
   YYPRINT (stderr, YY_CcpParser_CHAR, YY_CcpParser_LVAL);
#endif
   fprintf (stderr, ")\n");
 }
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    YYGOTO(yydefault);

  yyn = yytable[yyn];

  /*  yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
 YYGOTO(yyerrlab);
      yyn = -yyn;
      YYGOTO(yyreduce);
    }
  else if (yyn == 0)
    YYGOTO(yyerrlab);

  if (yyn == YYFINAL)
    YYACCEPT;

  /*  Shift the lookahead token.  */

#if YY_CcpParser_DEBUG != 0
  if (YY_CcpParser_DEBUG_FLAG)
    fprintf(stderr, "Shifting token %d (%s), ", YY_CcpParser_CHAR, yytname[yychar1]);
#endif

  /*  Discard the token being shifted unless it is eof.  */
  if (YY_CcpParser_CHAR != YYEOF)
    YY_CcpParser_CHAR = YYEMPTY;

  *++yyvsp = YY_CcpParser_LVAL;
#ifdef YY_CcpParser_LSP_NEEDED
  *++yylsp = YY_CcpParser_LLOC;
#endif

  /*  count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  YYGOTO(yynewstate);

/*  Do the default action for the current state.  */
YYLABEL(yydefault)

  yyn = yydefact[yystate];
  if (yyn == 0)
    YYGOTO(yyerrlab);

/*  Do a reduction.  yyn is the number of a rule to reduce with.  */
YYLABEL(yyreduce)
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /*  implement default value of the action */

#if YY_CcpParser_DEBUG != 0
  if (YY_CcpParser_DEBUG_FLAG)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
        yyn, yyrline[yyn]);

      /*  Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
 fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


/*  #line 814 "../bison_pp/bison.cc" */


  switch (yyn) {

case 1:

{;
    break;}
case 4:

{ set_implementation(yyvsp[0].number()); ;
    break;}
case 5:

{ set_interface(yyvsp[0].number()); ;
    break;}
case 6:

{ set_test(yyvsp[0].number()); ;
    break;}
case 7:

{ add_import(yyvsp[-1].number(), *yyvsp[0].string()); ;
    break;}
case 8:

{ add_import(yyvsp[-2].number(), *yyvsp[-1].string(), *yyvsp[0].string()); ;
    break;}
case 9:

{ add_line(*yyvsp[0].string()); ;
    break;}
case 10:

{ install_define(); ;
    break;}
case 11:

{ create_define(*yyvsp[0].string()); ;
    break;}
case 12:

{ add_line(*yyvsp[0].string()); ;
    break;}
}


   /*  the action file gets copied in in place of this dollarsign  */
  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YY_CcpParser_LSP_NEEDED
  yylsp -= yylen;
#endif

#if YY_CcpParser_DEBUG != 0
  if (YY_CcpParser_DEBUG_FLAG)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
 fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YY_CcpParser_LSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = YY_CcpParser_LLOC.first_line;
      yylsp->first_column = YY_CcpParser_LLOC.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /*  Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  YYGOTO(yynewstate);

YYLABEL(yyerrlab)   /*  here on detecting error */

  if (! yyerrstatus)
    /*  If not already recovering from an error, report this error.  */
    {
      ++YY_CcpParser_NERRS;

#ifdef YY_CcpParser_ERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
 {
   int size = 0;
   char *msg;
   int x, count;

   count = 0;
   /*  Start X at -yyn if nec to avoid negative indexes in yycheck.  */
   for (x = (yyn < 0 ? -yyn : 0);
        x < (sizeof(yytname) / sizeof(char *)); x++)
     if (yycheck[x + yyn] == x)
       size += strlen(yytname[x]) + 15, count++;
   msg = (char *) malloc(size + 15);
   if (msg != 0)
     {
       strcpy(msg, "parse error");

       if (count < 5)
  {
    count = 0;
    for (x = (yyn < 0 ? -yyn : 0);
         x < (sizeof(yytname) / sizeof(char *)); x++)
      if (yycheck[x + yyn] == x)
        {
   strcat(msg, count == 0 ? ", expecting `" : " or `");
   strcat(msg, yytname[x]);
   strcat(msg, "'");
   count++;
        }
  }
       YY_CcpParser_ERROR(msg);
       free(msg);
     }
   else
     YY_CcpParser_ERROR ("parse error; also virtual memory exceeded");
 }
      else
#endif /*  YY_CcpParser_ERROR_VERBOSE */
 YY_CcpParser_ERROR("parse error");
    }

  YYGOTO(yyerrlab1);
YYLABEL(yyerrlab1)   /*  here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /*  if just tried and failed to reuse lookahead token after an error, discard it.  */

      /*  return failure if at end of input */
      if (YY_CcpParser_CHAR == YYEOF)
 YYABORT;

#if YY_CcpParser_DEBUG != 0
      if (YY_CcpParser_DEBUG_FLAG)
 fprintf(stderr, "Discarding token %d (%s).\n", YY_CcpParser_CHAR, yytname[yychar1]);
#endif

      YY_CcpParser_CHAR = YYEMPTY;
    }

  /*  Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;              /*  Each real token shifted decrements this */

  YYGOTO(yyerrhandle);

YYLABEL(yyerrdefault)  /*  current state does not do anything special for the error token. */

#if 0
  /*  This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /*  If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) YYGOTO(yydefault);
#endif

YYLABEL(yyerrpop)   /*  pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YY_CcpParser_LSP_NEEDED
  yylsp--;
#endif

#if YY_CcpParser_DEBUG != 0
  if (YY_CcpParser_DEBUG_FLAG)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
 fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

YYLABEL(yyerrhandle)

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    YYGOTO(yyerrdefault);

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    YYGOTO(yyerrdefault);

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
 YYGOTO(yyerrpop);
      yyn = -yyn;
      YYGOTO(yyreduce);
    }
  else if (yyn == 0)
    YYGOTO(yyerrpop);

  if (yyn == YYFINAL)
    YYACCEPT;

#if YY_CcpParser_DEBUG != 0
  if (YY_CcpParser_DEBUG_FLAG)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = YY_CcpParser_LVAL;
#ifdef YY_CcpParser_LSP_NEEDED
  *++yylsp = YY_CcpParser_LLOC;
#endif

  yystate = yyn;
  YYGOTO(yynewstate);
/*  end loop, in which YYGOTO may be used. */
  YYENDGOTO
}

/*  END */

/*  #line 1013 "../bison_pp/bison.cc" */



TYPEINFO_SINGLE(CcpParser, CcpParserContext)

// 
// 	Pass msg to the lexer to get a sensible error message.
// 
void CcpParser::yyerror(const char *msg) { CcpParserContext::yyerror(msg); }

// 
// 	Instruct the lexer to produce another token.
// 
int CcpParser::yylex() { return (CcpParserContext::yylex(yylval)); }
