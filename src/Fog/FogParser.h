#ifndef YY_FogParser_h_included
#define YY_FogParser_h_included
#define YY_USE_CLASS
/* before anything */
#ifdef c_plusplus
 #ifndef __cplusplus
  #define __cplusplus
 #endif
#endif

#define YY_FogParser_DEBUG  1
#define YY_FogParser_ENUM_TOKEN  FogParserEnums
#define YY_FogParser_STYPE  FogParserValue
#define YY_FogParser_INHERIT  : public FogParserMaker
#define YY_FogParser_MEMBERS  \
    typedef FogParserMaker Super; \
    typedef FogParser This; \
    TYPEDECL_SINGLE(This, Super) \
    YACC_BANG_TYPE _bang; \
private: \
    void advance_search(); \
    void end_search(FogToken *aToken); \
    void make_result(FogToken *aResult); \
    YACC_MARK_TYPE mark(); \
    YACC_MARK_TYPE mark_type1(); \
    void pop_bang(YACC_BANG_TYPE bangValue); \
    YACC_BANG_TYPE push_bang(); \
    void remark(YACC_MARK_TYPE anIndex); \
    void remark_type1(YACC_MARK_TYPE anIndex); \
    void rewind_colon(YACC_MARK_TYPE aMark, const FogToken *aToken = 0); \
    void start_search(bool enableType1 = false); \
    void template_test(); \
    void unmark(const FogToken *aToken = 0); \
public: \
    virtual ~FogParser(); \
    virtual int yy_debug() const { return yydebug; } \
    void yydiag(const char *msg) { Super::diagnostic(msg); } \
    void yywarn(const char *msg); \
    static void terminate();
#define YY_FogParser_CONSTRUCTOR_PARAM  FogLexerTower& aLexer, FogToken& firstToken, const char *aTitle, int aDepth
#define YY_FogParser_CONSTRUCTOR_INIT  : Super(aLexer, firstToken, aTitle, aDepth), _bang(0)
#define YY_FogParser_CONSTRUCTOR_CODE  \
    YY_FogParser_DEBUG_FLAG = Fog::debug_yacc(); \
    CONDMSG(YY_FogParser_DEBUG_FLAG, "\n\nStarting " << title() << " parser.");
 /* %{ and %header{ and %union, during decl */
#ifndef YY_FogParser_COMPATIBILITY
 #ifndef YY_USE_CLASS
  #define  YY_FogParser_COMPATIBILITY 1
 #else
  #define  YY_FogParser_COMPATIBILITY 0
 #endif
#endif

#if YY_FogParser_COMPATIBILITY != 0
/* backward compatibility */
 #ifdef YYLTYPE
  #ifndef YY_FogParser_LTYPE
   #define YY_FogParser_LTYPE YYLTYPE
/* WARNING obsolete !!! user defined YYLTYPE not reported into generated header */
/* use %define LTYPE */
  #endif
 #endif
/*#ifdef YYSTYPE*/
  #ifndef YY_FogParser_STYPE
   #define YY_FogParser_STYPE YYSTYPE
  /* WARNING obsolete !!! user defined YYSTYPE not reported into generated header */
   /* use %define STYPE */
  #endif
/*#endif*/
 #ifdef YYDEBUG
  #ifndef YY_FogParser_DEBUG
   #define  YY_FogParser_DEBUG YYDEBUG
   /* WARNING obsolete !!! user defined YYDEBUG not reported into generated header */
   /* use %define DEBUG */
  #endif
 #endif 
 /* use goto to be compatible */
 #ifndef YY_FogParser_USE_GOTO
  #define YY_FogParser_USE_GOTO 1
 #endif
#endif

/* use no goto to be clean in C++ */
#ifndef YY_FogParser_USE_GOTO
 #define YY_FogParser_USE_GOTO 0
#endif

#ifndef YY_FogParser_PURE
/* YY_FogParser_PURE */
#endif

/* prefix */

#ifndef YY_FogParser_DEBUG
#define YY_FogParser_DEBUG 1
/* YY_FogParser_DEBUG */
#endif

#ifndef YY_FogParser_LSP_NEEDED
 /* YY_FogParser_LSP_NEEDED*/
#endif

/* DEFAULT LTYPE*/
#ifdef YY_FogParser_LSP_NEEDED
 #ifndef YY_FogParser_LTYPE
  #ifndef BISON_YYLTYPE_ISDECLARED
   #define BISON_YYLTYPE_ISDECLARED
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
  #endif

  #define YY_FogParser_LTYPE yyltype
 #endif
#endif

/* DEFAULT STYPE*/
#ifndef YY_FogParser_STYPE
 #define YY_FogParser_STYPE int
#endif

/* DEFAULT MISCELANEOUS */
#ifndef YY_FogParser_PARSE
 #define YY_FogParser_PARSE yyparse
#endif

#ifndef YY_FogParser_LEX
 #define YY_FogParser_LEX yylex
#endif

#ifndef YY_FogParser_LVAL
 #define YY_FogParser_LVAL yylval
#endif

#ifndef YY_FogParser_LLOC
 #define YY_FogParser_LLOC yylloc
#endif

#ifndef YY_FogParser_CHAR
 #define YY_FogParser_CHAR yychar
#endif

#ifndef YY_FogParser_NERRS
 #define YY_FogParser_NERRS yynerrs
#endif

#ifndef YY_FogParser_DEBUG_FLAG
 #define YY_FogParser_DEBUG_FLAG yydebug
#endif

#ifndef YY_FogParser_ERROR
 #define YY_FogParser_ERROR yyerror
#endif

#ifndef YY_FogParser_PARSE_PARAM
 #ifndef __STDC__
  #ifndef __cplusplus
   #ifndef YY_USE_CLASS
    #define YY_FogParser_PARSE_PARAM
    #ifndef YY_FogParser_PARSE_PARAM_DEF
     #define YY_FogParser_PARSE_PARAM_DEF
    #endif
   #endif
  #endif
 #endif
 #ifndef YY_FogParser_PARSE_PARAM
  #define YY_FogParser_PARSE_PARAM void
 #endif
#endif

/* TOKEN C */
#ifndef YY_USE_CLASS

 #ifndef YY_FogParser_PURE
  #ifndef yylval
   extern YY_FogParser_STYPE YY_FogParser_LVAL;
  #else
   #if yylval != YY_FogParser_LVAL
    extern YY_FogParser_STYPE YY_FogParser_LVAL;
   #else
    #warning "Namespace conflict, disabling some functionality (bison++ only)"
   #endif
  #endif
 #endif

#define	PRIVATE	286
#define	PROTECTED	287
#define	PUBLIC	288
#define	BOOL	399
#define	CHAR	400
#define	DOUBLE	401
#define	FLOAT	402
#define	INT	403
#define	LONG	404
#define	SHORT	405
#define	SIGNED	406
#define	UNSIGNED	407
#define	VOID	408
#define	WCHAR_T	409
#define	CLASS	263
#define	ENUM	272
#define	NAMESPACE	283
#define	STRUCT	295
#define	TYPENAME	304
#define	UNION	305
#define	CONST	264
#define	VOLATILE	308
#define	AUTO	259
#define	EXPLICIT	273
#define	EXPORT	274
#define	EXTERN	275
#define	FRIEND	278
#define	INLINE	281
#define	MUTABLE	282
#define	REGISTER	289
#define	STATIC	293
#define	TEMPLATE	297
#define	TYPEDEF	302
#define	USING	306
#define	VIRTUAL	307
#define	ASM	258
#define	BREAK	260
#define	CASE	261
#define	CATCH	262
#define	CONST_CAST	265
#define	CONTINUE	266
#define	DEFAULT	267
#define	DELETE	268
#define	DO	269
#define	DYNAMIC_CAST	270
#define	ELSE	271
#define	FALSE	276
#define	FOR	277
#define	GOTO	279
#define	IF	280
#define	NEW	284
#define	OPERATOR	285
#define	REINTERPRET_CAST	290
#define	RETURN	291
#define	SIZEOF	292
#define	STATIC_CAST	294
#define	SWITCH	296
#define	THIS	298
#define	THROW	299
#define	TRUE	300
#define	TRY	301
#define	TYPEID	303
#define	WHILE	309
#define	FloatingLiteral	411
#define	IntegerLiteral	410
#define	BuiltInTypeSpecifier	335
#define	MetaType	338
#define	TreeLiteral	342
#define	DERIVED	347
#define	FILE	351
#define	GUARD	353
#define	IMPLEMENTATION	354
#define	INCLUDE	355
#define	INTERFACE	356
#define	ALL	368
#define	PLACEHOLDER	369
#define	PLACEMENT	370
#define	NOAUTO	371
#define	NOGUARD	357
#define	NOIMPLEMENTATION	358
#define	OVERLOAD	359
#define	PATH	360
#define	PREFIX	364
#define	PURE	365
#define	SUFFIX	366
#define	BODY	346
#define	ENTRY	349
#define	EXIT	350
#define	POST	362
#define	PRE	363
#define	EMIT	348
#define	FROZEN	352
#define	POOL	361
#define	UTILITY	367
#define	ELLIPSIS	310
#define	SCOPE	311
#define	SHL	312
#define	SHR	313
#define	EQ	314
#define	NE	315
#define	LE	316
#define	GE	317
#define	LOG_AND	318
#define	LOG_OR	319
#define	INC	320
#define	DEC	321
#define	ARROW	322
#define	ARROW_STAR	323
#define	DOT_STAR	324
#define	ASS_ADD	325
#define	ASS_AND	326
#define	ASS_DIV	327
#define	ASS_MOD	328
#define	ASS_MUL	329
#define	ASS_OR	330
#define	ASS_SHL	331
#define	ASS_SHR	332
#define	ASS_SUB	333
#define	ASS_XOR	334
#define	CharacterLiteral	336
#define	Identifier	337
#define	NewLine	339
#define	NumberLiteral	340
#define	StringLiteral	341
#define	ERROR	343
#define	NIL	344
#define	RAMMARG	345
#define	HASH_BLANK	372
#define	HASH_DEFINE	373
#define	HASH_ELIF	374
#define	HASH_ELSE	375
#define	HASH_ENDIF	376
#define	HASH_ERROR	377
#define	HASH_IF	378
#define	HASH_IFDEF	379
#define	HASH_IFNDEF	380
#define	HASH_INCLUDE	381
#define	HASH_LINE	382
#define	HASH_PRAGMA	383
#define	HASH_UNDEF	384
#define	HASH_HASH	385
#define	DI_HASH_HASH	386
#define	DI_HASH	387
#define	DI_SQUARE	388
#define	DI_ERAUQS	389
#define	DI_ECARB	390
#define	DEFINED	391
#define	DO_FUNCTION	392
#define	MacroId	393
#define	Punct	394
#define	Text	395
#define	SHIFT_THERE	396
#define	REDUCE_HERE_MOSTLY	397
#define	REDUCE_HERE	398

 /* #defines token */
/* after #define tokens, before const tokens S5*/
#else
 #ifndef YY_FogParser_CLASS
  #define YY_FogParser_CLASS FogParser
 #endif

 #ifndef YY_FogParser_INHERIT
  #define YY_FogParser_INHERIT
 #endif

 #ifndef YY_FogParser_MEMBERS
  #define YY_FogParser_MEMBERS 
 #endif

 #ifndef YY_FogParser_LEX_BODY
  #define YY_FogParser_LEX_BODY  
 #endif

 #ifndef YY_FogParser_ERROR_BODY
  #define YY_FogParser_ERROR_BODY  
 #endif

 #ifndef YY_FogParser_CONSTRUCTOR_PARAM
  #define YY_FogParser_CONSTRUCTOR_PARAM
 #endif
 /* choose between enum and const */
 #ifndef YY_FogParser_USE_CONST_TOKEN
  #define YY_FogParser_USE_CONST_TOKEN 0
  /* yes enum is more compatible with flex,  */
  /* so by default we use it */ 
 #endif
 #if YY_FogParser_USE_CONST_TOKEN != 0
  #ifndef YY_FogParser_ENUM_TOKEN
   #define YY_FogParser_ENUM_TOKEN yy_FogParser_enum_token
  #endif
 #endif

class YY_FogParser_CLASS YY_FogParser_INHERIT
{
public: 
 #if YY_FogParser_USE_CONST_TOKEN != 0
  /* static const int token ... */
  static const int PRIVATE;
static const int PROTECTED;
static const int PUBLIC;
static const int BOOL;
static const int CHAR;
static const int DOUBLE;
static const int FLOAT;
static const int INT;
static const int LONG;
static const int SHORT;
static const int SIGNED;
static const int UNSIGNED;
static const int VOID;
static const int WCHAR_T;
static const int CLASS;
static const int ENUM;
static const int NAMESPACE;
static const int STRUCT;
static const int TYPENAME;
static const int UNION;
static const int CONST;
static const int VOLATILE;
static const int AUTO;
static const int EXPLICIT;
static const int EXPORT;
static const int EXTERN;
static const int FRIEND;
static const int INLINE;
static const int MUTABLE;
static const int REGISTER;
static const int STATIC;
static const int TEMPLATE;
static const int TYPEDEF;
static const int USING;
static const int VIRTUAL;
static const int ASM;
static const int BREAK;
static const int CASE;
static const int CATCH;
static const int CONST_CAST;
static const int CONTINUE;
static const int DEFAULT;
static const int DELETE;
static const int DO;
static const int DYNAMIC_CAST;
static const int ELSE;
static const int FALSE;
static const int FOR;
static const int GOTO;
static const int IF;
static const int NEW;
static const int OPERATOR;
static const int REINTERPRET_CAST;
static const int RETURN;
static const int SIZEOF;
static const int STATIC_CAST;
static const int SWITCH;
static const int THIS;
static const int THROW;
static const int TRUE;
static const int TRY;
static const int TYPEID;
static const int WHILE;
static const int FloatingLiteral;
static const int IntegerLiteral;
static const int BuiltInTypeSpecifier;
static const int MetaType;
static const int TreeLiteral;
static const int DERIVED;
static const int FILE;
static const int GUARD;
static const int IMPLEMENTATION;
static const int INCLUDE;
static const int INTERFACE;
static const int ALL;
static const int PLACEHOLDER;
static const int PLACEMENT;
static const int NOAUTO;
static const int NOGUARD;
static const int NOIMPLEMENTATION;
static const int OVERLOAD;
static const int PATH;
static const int PREFIX;
static const int PURE;
static const int SUFFIX;
static const int BODY;
static const int ENTRY;
static const int EXIT;
static const int POST;
static const int PRE;
static const int EMIT;
static const int FROZEN;
static const int POOL;
static const int UTILITY;
static const int ELLIPSIS;
static const int SCOPE;
static const int SHL;
static const int SHR;
static const int EQ;
static const int NE;
static const int LE;
static const int GE;
static const int LOG_AND;
static const int LOG_OR;
static const int INC;
static const int DEC;
static const int ARROW;
static const int ARROW_STAR;
static const int DOT_STAR;
static const int ASS_ADD;
static const int ASS_AND;
static const int ASS_DIV;
static const int ASS_MOD;
static const int ASS_MUL;
static const int ASS_OR;
static const int ASS_SHL;
static const int ASS_SHR;
static const int ASS_SUB;
static const int ASS_XOR;
static const int CharacterLiteral;
static const int Identifier;
static const int NewLine;
static const int NumberLiteral;
static const int StringLiteral;
static const int ERROR;
static const int NIL;
static const int RAMMARG;
static const int HASH_BLANK;
static const int HASH_DEFINE;
static const int HASH_ELIF;
static const int HASH_ELSE;
static const int HASH_ENDIF;
static const int HASH_ERROR;
static const int HASH_IF;
static const int HASH_IFDEF;
static const int HASH_IFNDEF;
static const int HASH_INCLUDE;
static const int HASH_LINE;
static const int HASH_PRAGMA;
static const int HASH_UNDEF;
static const int HASH_HASH;
static const int DI_HASH_HASH;
static const int DI_HASH;
static const int DI_SQUARE;
static const int DI_ERAUQS;
static const int DI_ECARB;
static const int DEFINED;
static const int DO_FUNCTION;
static const int MacroId;
static const int Punct;
static const int Text;
static const int SHIFT_THERE;
static const int REDUCE_HERE_MOSTLY;
static const int REDUCE_HERE;

 /* decl const */
 #else
  enum YY_FogParser_ENUM_TOKEN { YY_FogParser_NULL_TOKEN=0
  	,PRIVATE=286
	,PROTECTED=287
	,PUBLIC=288
	,BOOL=399
	,CHAR=400
	,DOUBLE=401
	,FLOAT=402
	,INT=403
	,LONG=404
	,SHORT=405
	,SIGNED=406
	,UNSIGNED=407
	,VOID=408
	,WCHAR_T=409
	,CLASS=263
	,ENUM=272
	,NAMESPACE=283
	,STRUCT=295
	,TYPENAME=304
	,UNION=305
	,CONST=264
	,VOLATILE=308
	,AUTO=259
	,EXPLICIT=273
	,EXPORT=274
	,EXTERN=275
	,FRIEND=278
	,INLINE=281
	,MUTABLE=282
	,REGISTER=289
	,STATIC=293
	,TEMPLATE=297
	,TYPEDEF=302
	,USING=306
	,VIRTUAL=307
	,ASM=258
	,BREAK=260
	,CASE=261
	,CATCH=262
	,CONST_CAST=265
	,CONTINUE=266
	,DEFAULT=267
	,DELETE=268
	,DO=269
	,DYNAMIC_CAST=270
	,ELSE=271
	,FALSE=276
	,FOR=277
	,GOTO=279
	,IF=280
	,NEW=284
	,OPERATOR=285
	,REINTERPRET_CAST=290
	,RETURN=291
	,SIZEOF=292
	,STATIC_CAST=294
	,SWITCH=296
	,THIS=298
	,THROW=299
	,TRUE=300
	,TRY=301
	,TYPEID=303
	,WHILE=309
	,FloatingLiteral=411
	,IntegerLiteral=410
	,BuiltInTypeSpecifier=335
	,MetaType=338
	,TreeLiteral=342
	,DERIVED=347
	,FILE=351
	,GUARD=353
	,IMPLEMENTATION=354
	,INCLUDE=355
	,INTERFACE=356
	,ALL=368
	,PLACEHOLDER=369
	,PLACEMENT=370
	,NOAUTO=371
	,NOGUARD=357
	,NOIMPLEMENTATION=358
	,OVERLOAD=359
	,PATH=360
	,PREFIX=364
	,PURE=365
	,SUFFIX=366
	,BODY=346
	,ENTRY=349
	,EXIT=350
	,POST=362
	,PRE=363
	,EMIT=348
	,FROZEN=352
	,POOL=361
	,UTILITY=367
	,ELLIPSIS=310
	,SCOPE=311
	,SHL=312
	,SHR=313
	,EQ=314
	,NE=315
	,LE=316
	,GE=317
	,LOG_AND=318
	,LOG_OR=319
	,INC=320
	,DEC=321
	,ARROW=322
	,ARROW_STAR=323
	,DOT_STAR=324
	,ASS_ADD=325
	,ASS_AND=326
	,ASS_DIV=327
	,ASS_MOD=328
	,ASS_MUL=329
	,ASS_OR=330
	,ASS_SHL=331
	,ASS_SHR=332
	,ASS_SUB=333
	,ASS_XOR=334
	,CharacterLiteral=336
	,Identifier=337
	,NewLine=339
	,NumberLiteral=340
	,StringLiteral=341
	,ERROR=343
	,NIL=344
	,RAMMARG=345
	,HASH_BLANK=372
	,HASH_DEFINE=373
	,HASH_ELIF=374
	,HASH_ELSE=375
	,HASH_ENDIF=376
	,HASH_ERROR=377
	,HASH_IF=378
	,HASH_IFDEF=379
	,HASH_IFNDEF=380
	,HASH_INCLUDE=381
	,HASH_LINE=382
	,HASH_PRAGMA=383
	,HASH_UNDEF=384
	,HASH_HASH=385
	,DI_HASH_HASH=386
	,DI_HASH=387
	,DI_SQUARE=388
	,DI_ERAUQS=389
	,DI_ECARB=390
	,DEFINED=391
	,DO_FUNCTION=392
	,MacroId=393
	,Punct=394
	,Text=395
	,SHIFT_THERE=396
	,REDUCE_HERE_MOSTLY=397
	,REDUCE_HERE=398

 /* enum token */
     }; /* end of enum declaration */
 #endif
public:
 int YY_FogParser_PARSE(YY_FogParser_PARSE_PARAM);
 virtual void YY_FogParser_ERROR(char *msg) YY_FogParser_ERROR_BODY;
 #ifdef YY_FogParser_PURE
  #ifdef YY_FogParser_LSP_NEEDED
   virtual int  YY_FogParser_LEX(YY_FogParser_STYPE *YY_FogParser_LVAL,YY_FogParser_LTYPE *YY_FogParser_LLOC) YY_FogParser_LEX_BODY;
  #else
   virtual int  YY_FogParser_LEX(YY_FogParser_STYPE *YY_FogParser_LVAL) YY_FogParser_LEX_BODY;
  #endif
 #else
  virtual int YY_FogParser_LEX() YY_FogParser_LEX_BODY;
  YY_FogParser_STYPE YY_FogParser_LVAL;
  #ifdef YY_FogParser_LSP_NEEDED
   YY_FogParser_LTYPE YY_FogParser_LLOC;
  #endif
  int YY_FogParser_NERRS;
  int YY_FogParser_CHAR;
 #endif
 #if YY_FogParser_DEBUG != 0
  public:
   int YY_FogParser_DEBUG_FLAG;	/*  nonzero means print parse trace	*/
 #endif
public:
 YY_FogParser_CLASS(YY_FogParser_CONSTRUCTOR_PARAM);
public:
 YY_FogParser_MEMBERS 
};
/* other declare folow */
#endif


#if YY_FogParser_COMPATIBILITY != 0
 /* backward compatibility */
 /* Removed due to bison problems
 /#ifndef YYSTYPE
 / #define YYSTYPE YY_FogParser_STYPE
 /#endif*/

 #ifndef YYLTYPE
  #define YYLTYPE YY_FogParser_LTYPE
 #endif
 #ifndef YYDEBUG
  #ifdef YY_FogParser_DEBUG 
   #define YYDEBUG YY_FogParser_DEBUG
  #endif
 #endif

#endif
/* END */
#endif
