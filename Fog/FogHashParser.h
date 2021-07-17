#ifndef YY_FogHashParser_h_included
#define YY_FogHashParser_h_included
#define YY_USE_CLASS
/* before anything */
#ifdef c_plusplus
	#ifndef __cplusplus
		#define __cplusplus
	#endif
#endif

#define YY_FogHashParser_DEBUG  1
#define YY_FogHashParser_ENUM_TOKEN  FogHashParserEnums
#define YY_FogHashParser_STYPE  FogParserValue
#define YY_FogHashParser_INHERIT  : public PrimPrint
#define YY_FogHashParser_MEMBERS  \
	FogLexerMacro& _lexer; \
	bool _truth; \
	bool _primed; \
	FogTokenContext _context; \
	PrimAdopted<PrimOstrstream> _s; \
	PrimAdopted<PrimIdList> _formals; \
	PrimIdHandle _id; \
	size_t _line; \
	FogStringRefToConst _string; \
	FogTokensRef _toks; \
	const FogLexerState& _lexer_state; \
	private: \
	void invoke(const FogKeyword& anId, bool canBeDEFINED); \
	inline bool is_defined(const FogKeyword& anId); \
	const FogLexerState& set_state(FogTokenType::TokenType tokenType); \
	void start_define(const FogKeyword *anId); \
	public: \
	virtual ~FogHashParser(); \
	const PrimIdList *formals() const { return _formals.get(); } \
	const PrimId& id() const { return *_id; } \
	size_t line_number() const { return _line; } \
	PrimOstrstream *stream() { return _s.get(); } \
	const PrimId& string_id() const { return _string->bin_id(); } \
	FogTokens& tokens() const { return *_toks; } \
	bool truth() const { return _truth; }
#define YY_FogHashParser_CONSTRUCTOR_PARAM  FogLexerMacro& aLexer, const FogTokenContext& startContext
#define YY_FogHashParser_CONSTRUCTOR_INIT  : \
	_lexer(aLexer), \
	_truth(true), \
	_primed(true), \
	_context(startContext), \
	_line(0), \
	_lexer_state(set_state(startContext.token().pp_token_type_enum()))
#define YY_FogHashParser_CONSTRUCTOR_CODE  \
	YY_FogHashParser_DEBUG_FLAG = Fog::debug_yacc(); \
	CONDMSG(YY_FogHashParser_DEBUG_FLAG || Fog::debug_lex2hash(), \
	        "\nStarting # directive parser.");
/* %{ and %header{ and %union, during decl */
#ifndef YY_FogHashParser_COMPATIBILITY
	#ifndef YY_USE_CLASS
		#define  YY_FogHashParser_COMPATIBILITY 1
	#else
		#define  YY_FogHashParser_COMPATIBILITY 0
	#endif
#endif

#if YY_FogHashParser_COMPATIBILITY != 0
	/* backward compatibility */
	#ifdef YYLTYPE
		#ifndef YY_FogHashParser_LTYPE
			#define YY_FogHashParser_LTYPE YYLTYPE
			/* WARNING obsolete !!! user defined YYLTYPE not reported into generated header */
			/* use %define LTYPE */
		#endif
	#endif
	/*#ifdef YYSTYPE*/
	#ifndef YY_FogHashParser_STYPE
		#define YY_FogHashParser_STYPE YYSTYPE
		/* WARNING obsolete !!! user defined YYSTYPE not reported into generated header */
		/* use %define STYPE */
	#endif
	/*#endif*/
	#ifdef YYDEBUG
		#ifndef YY_FogHashParser_DEBUG
			#define  YY_FogHashParser_DEBUG YYDEBUG
			/* WARNING obsolete !!! user defined YYDEBUG not reported into generated header */
			/* use %define DEBUG */
		#endif
	#endif
	/* use goto to be compatible */
	#ifndef YY_FogHashParser_USE_GOTO
		#define YY_FogHashParser_USE_GOTO 1
	#endif
#endif

/* use no goto to be clean in C++ */
#ifndef YY_FogHashParser_USE_GOTO
	#define YY_FogHashParser_USE_GOTO 0
#endif

#ifndef YY_FogHashParser_PURE
	/* YY_FogHashParser_PURE */
#endif

/* prefix */

#ifndef YY_FogHashParser_DEBUG
	#define YY_FogHashParser_DEBUG 1
	/* YY_FogHashParser_DEBUG */
#endif

#ifndef YY_FogHashParser_LSP_NEEDED
	/* YY_FogHashParser_LSP_NEEDED*/
#endif

/* DEFAULT LTYPE*/
#ifdef YY_FogHashParser_LSP_NEEDED
#ifndef YY_FogHashParser_LTYPE
#ifndef BISON_YYLTYPE_ISDECLARED
#define BISON_YYLTYPE_ISDECLARED
typedef
struct yyltype {
	int timestamp;
	int first_line;
	int first_column;
	int last_line;
	int last_column;
	char* text;
}
yyltype;
#endif

#define YY_FogHashParser_LTYPE yyltype
#endif
#endif

/* DEFAULT STYPE*/
#ifndef YY_FogHashParser_STYPE
	#define YY_FogHashParser_STYPE int
#endif

/* DEFAULT MISCELANEOUS */
#ifndef YY_FogHashParser_PARSE
	#define YY_FogHashParser_PARSE yyparse
#endif

#ifndef YY_FogHashParser_LEX
	#define YY_FogHashParser_LEX yylex
#endif

#ifndef YY_FogHashParser_LVAL
	#define YY_FogHashParser_LVAL yylval
#endif

#ifndef YY_FogHashParser_LLOC
	#define YY_FogHashParser_LLOC yylloc
#endif

#ifndef YY_FogHashParser_CHAR
	#define YY_FogHashParser_CHAR yychar
#endif

#ifndef YY_FogHashParser_NERRS
	#define YY_FogHashParser_NERRS yynerrs
#endif

#ifndef YY_FogHashParser_DEBUG_FLAG
	#define YY_FogHashParser_DEBUG_FLAG yydebug
#endif

#ifndef YY_FogHashParser_ERROR
	#define YY_FogHashParser_ERROR yyerror
#endif

#ifndef YY_FogHashParser_PARSE_PARAM
	#ifndef __STDC__
		#ifndef __cplusplus
			#ifndef YY_USE_CLASS
				#define YY_FogHashParser_PARSE_PARAM
				#ifndef YY_FogHashParser_PARSE_PARAM_DEF
					#define YY_FogHashParser_PARSE_PARAM_DEF
				#endif
			#endif
		#endif
	#endif
	#ifndef YY_FogHashParser_PARSE_PARAM
		#define YY_FogHashParser_PARSE_PARAM void
	#endif
#endif

/* TOKEN C */
#ifndef YY_USE_CLASS

#ifndef YY_FogHashParser_PURE
	#ifndef yylval
		extern YY_FogHashParser_STYPE YY_FogHashParser_LVAL;
	#else
		#if yylval != YY_FogHashParser_LVAL
			extern YY_FogHashParser_STYPE YY_FogHashParser_LVAL;
		#else
			#warning "Namespace conflict, disabling some functionality (bison++ only)"
		#endif
	#endif
#endif

#define	ASM	258
#define	AUTO	259
#define	BREAK	260
#define	CASE	261
#define	CATCH	262
#define	CLASS	263
#define	CONST	264
#define	CONST_CAST	265
#define	CONTINUE	266
#define	DEFAULT	267
#define	DELETE	268
#define	DO	269
#define	DYNAMIC_CAST	270
#define	ELSE	271
#define	ENUM	272
#define	EXPLICIT	273
#define	EXPORT	274
#define	EXTERN	275
#define	FALSE	276
#define	FOR	277
#define	FRIEND	278
#define	GOTO	279
#define	IF	280
#define	INLINE	281
#define	MUTABLE	282
#define	NAMESPACE	283
#define	NEW	284
#define	OPERATOR	285
#define	PRIVATE	286
#define	PROTECTED	287
#define	PUBLIC	288
#define	REGISTER	289
#define	REINTERPRET_CAST	290
#define	RETURN	291
#define	SIZEOF	292
#define	STATIC	293
#define	STATIC_CAST	294
#define	STRUCT	295
#define	SWITCH	296
#define	TEMPLATE	297
#define	THIS	298
#define	THROW	299
#define	TRUE	300
#define	TRY	301
#define	TYPEDEF	302
#define	TYPEID	303
#define	TYPENAME	304
#define	UNION	305
#define	USING	306
#define	VIRTUAL	307
#define	VOLATILE	308
#define	WHILE	309
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
#define	BuiltInTypeSpecifier	335
#define	CharacterLiteral	336
#define	Identifier	337
#define	MetaType	338
#define	NewLine	339
#define	NumberLiteral	340
#define	StringLiteral	341
#define	TreeLiteral	342
#define	ERROR	343
#define	NIL	344
#define	RAMMARG	345
#define	BODY	346
#define	DERIVED	347
#define	EMIT	348
#define	ENTRY	349
#define	EXIT	350
#define	FILE	351
#define	FROZEN	352
#define	GUARD	353
#define	IMPLEMENTATION	354
#define	INCLUDE	355
#define	INTERFACE	356
#define	NOGUARD	357
#define	NOIMPLEMENTATION	358
#define	OVERLOAD	359
#define	PATH	360
#define	POOL	361
#define	POST	362
#define	PRE	363
#define	PREFIX	364
#define	PURE	365
#define	SUFFIX	366
#define	UTILITY	367
#define	ALL	368
#define	PLACEHOLDER	369
#define	PLACEMENT	370
#define	NOAUTO	371
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
#define	IntegerLiteral	410
#define	FloatingLiteral	411

/* #defines token */
/* after #define tokens, before const tokens S5*/
#else
#ifndef YY_FogHashParser_CLASS
	#define YY_FogHashParser_CLASS FogHashParser
#endif

#ifndef YY_FogHashParser_INHERIT
	#define YY_FogHashParser_INHERIT
#endif

#ifndef YY_FogHashParser_MEMBERS
	#define YY_FogHashParser_MEMBERS
#endif

#ifndef YY_FogHashParser_LEX_BODY
	#define YY_FogHashParser_LEX_BODY
#endif

#ifndef YY_FogHashParser_ERROR_BODY
	#define YY_FogHashParser_ERROR_BODY
#endif

#ifndef YY_FogHashParser_CONSTRUCTOR_PARAM
	#define YY_FogHashParser_CONSTRUCTOR_PARAM
#endif
/* choose between enum and const */
#ifndef YY_FogHashParser_USE_CONST_TOKEN
	#define YY_FogHashParser_USE_CONST_TOKEN 0
	/* yes enum is more compatible with flex,  */
	/* so by default we use it */
#endif
#if YY_FogHashParser_USE_CONST_TOKEN != 0
	#ifndef YY_FogHashParser_ENUM_TOKEN
		#define YY_FogHashParser_ENUM_TOKEN yy_FogHashParser_enum_token
	#endif
#endif

class YY_FogHashParser_CLASS YY_FogHashParser_INHERIT {
public:
#if YY_FogHashParser_USE_CONST_TOKEN != 0
	/* static const int token ... */
	static const int ASM;
	static const int AUTO;
	static const int BREAK;
	static const int CASE;
	static const int CATCH;
	static const int CLASS;
	static const int CONST;
	static const int CONST_CAST;
	static const int CONTINUE;
	static const int DEFAULT;
	static const int DELETE;
	static const int DO;
	static const int DYNAMIC_CAST;
	static const int ELSE;
	static const int ENUM;
	static const int EXPLICIT;
	static const int EXPORT;
	static const int EXTERN;
	static const int FALSE;
	static const int FOR;
	static const int FRIEND;
	static const int GOTO;
	static const int IF;
	static const int INLINE;
	static const int MUTABLE;
	static const int NAMESPACE;
	static const int NEW;
	static const int OPERATOR;
	static const int PRIVATE;
	static const int PROTECTED;
	static const int PUBLIC;
	static const int REGISTER;
	static const int REINTERPRET_CAST;
	static const int RETURN;
	static const int SIZEOF;
	static const int STATIC;
	static const int STATIC_CAST;
	static const int STRUCT;
	static const int SWITCH;
	static const int TEMPLATE;
	static const int THIS;
	static const int THROW;
	static const int TRUE;
	static const int TRY;
	static const int TYPEDEF;
	static const int TYPEID;
	static const int TYPENAME;
	static const int UNION;
	static const int USING;
	static const int VIRTUAL;
	static const int VOLATILE;
	static const int WHILE;
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
	static const int BuiltInTypeSpecifier;
	static const int CharacterLiteral;
	static const int Identifier;
	static const int MetaType;
	static const int NewLine;
	static const int NumberLiteral;
	static const int StringLiteral;
	static const int TreeLiteral;
	static const int ERROR;
	static const int NIL;
	static const int RAMMARG;
	static const int BODY;
	static const int DERIVED;
	static const int EMIT;
	static const int ENTRY;
	static const int EXIT;
	static const int FILE;
	static const int FROZEN;
	static const int GUARD;
	static const int IMPLEMENTATION;
	static const int INCLUDE;
	static const int INTERFACE;
	static const int NOGUARD;
	static const int NOIMPLEMENTATION;
	static const int OVERLOAD;
	static const int PATH;
	static const int POOL;
	static const int POST;
	static const int PRE;
	static const int PREFIX;
	static const int PURE;
	static const int SUFFIX;
	static const int UTILITY;
	static const int ALL;
	static const int PLACEHOLDER;
	static const int PLACEMENT;
	static const int NOAUTO;
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
	static const int IntegerLiteral;
	static const int FloatingLiteral;
	
	/* decl const */
#else
	enum YY_FogHashParser_ENUM_TOKEN { YY_FogHashParser_NULL_TOKEN = 0
	, ASM = 258
	, AUTO = 259
	, BREAK = 260
	, CASE = 261
	, CATCH = 262
	, CLASS = 263
	, CONST = 264
	, CONST_CAST = 265
	, CONTINUE = 266
	, DEFAULT = 267
	, DELETE = 268
	, DO = 269
	, DYNAMIC_CAST = 270
	, ELSE = 271
	, ENUM = 272
	, EXPLICIT = 273
	, EXPORT = 274
	, EXTERN = 275
	, FALSE = 276
	, FOR = 277
	, FRIEND = 278
	, GOTO = 279
	, IF = 280
	, INLINE = 281
	, MUTABLE = 282
	, NAMESPACE = 283
	, NEW = 284
	, OPERATOR = 285
	, PRIVATE = 286
	, PROTECTED = 287
	, PUBLIC = 288
	, REGISTER = 289
	, REINTERPRET_CAST = 290
	, RETURN = 291
	, SIZEOF = 292
	, STATIC = 293
	, STATIC_CAST = 294
	, STRUCT = 295
	, SWITCH = 296
	, TEMPLATE = 297
	, THIS = 298
	, THROW = 299
	, TRUE = 300
	, TRY = 301
	, TYPEDEF = 302
	, TYPEID = 303
	, TYPENAME = 304
	, UNION = 305
	, USING = 306
	, VIRTUAL = 307
	, VOLATILE = 308
	, WHILE = 309
	, ELLIPSIS = 310
	, SCOPE = 311
	, SHL = 312
	, SHR = 313
	, EQ = 314
	, NE = 315
	, LE = 316
	, GE = 317
	, LOG_AND = 318
	, LOG_OR = 319
	, INC = 320
	, DEC = 321
	, ARROW = 322
	, ARROW_STAR = 323
	, DOT_STAR = 324
	, ASS_ADD = 325
	, ASS_AND = 326
	, ASS_DIV = 327
	, ASS_MOD = 328
	, ASS_MUL = 329
	, ASS_OR = 330
	, ASS_SHL = 331
	, ASS_SHR = 332
	, ASS_SUB = 333
	, ASS_XOR = 334
	, BuiltInTypeSpecifier = 335
	, CharacterLiteral = 336
	, Identifier = 337
	, MetaType = 338
	, NewLine = 339
	, NumberLiteral = 340
	, StringLiteral = 341
	, TreeLiteral = 342
	, ERROR = 343
	, NIL = 344
	, RAMMARG = 345
	, BODY = 346
	, DERIVED = 347
	, EMIT = 348
	, ENTRY = 349
	, EXIT = 350
	, FILE = 351
	, FROZEN = 352
	, GUARD = 353
	, IMPLEMENTATION = 354
	, INCLUDE = 355
	, INTERFACE = 356
	, NOGUARD = 357
	, NOIMPLEMENTATION = 358
	, OVERLOAD = 359
	, PATH = 360
	, POOL = 361
	, POST = 362
	, PRE = 363
	, PREFIX = 364
	, PURE = 365
	, SUFFIX = 366
	, UTILITY = 367
	, ALL = 368
	, PLACEHOLDER = 369
	, PLACEMENT = 370
	, NOAUTO = 371
	, HASH_BLANK = 372
	, HASH_DEFINE = 373
	, HASH_ELIF = 374
	, HASH_ELSE = 375
	, HASH_ENDIF = 376
	, HASH_ERROR = 377
	, HASH_IF = 378
	, HASH_IFDEF = 379
	, HASH_IFNDEF = 380
	, HASH_INCLUDE = 381
	, HASH_LINE = 382
	, HASH_PRAGMA = 383
	, HASH_UNDEF = 384
	, HASH_HASH = 385
	, DI_HASH_HASH = 386
	, DI_HASH = 387
	, DI_SQUARE = 388
	, DI_ERAUQS = 389
	, DI_ECARB = 390
	, DEFINED = 391
	, DO_FUNCTION = 392
	, MacroId = 393
	, Punct = 394
	, Text = 395
	, SHIFT_THERE = 396
	, REDUCE_HERE_MOSTLY = 397
	, REDUCE_HERE = 398
	, BOOL = 399
	, CHAR = 400
	, DOUBLE = 401
	, FLOAT = 402
	, INT = 403
	, LONG = 404
	, SHORT = 405
	, SIGNED = 406
	, UNSIGNED = 407
	, VOID = 408
	, WCHAR_T = 409
	, IntegerLiteral = 410
	, FloatingLiteral = 411
	
	/* enum token */
	                                 }; /* end of enum declaration */
#endif
public:
	int YY_FogHashParser_PARSE(YY_FogHashParser_PARSE_PARAM);
	virtual void YY_FogHashParser_ERROR(const char* msg) YY_FogHashParser_ERROR_BODY;
#ifdef YY_FogHashParser_PURE
#ifdef YY_FogHashParser_LSP_NEEDED
	virtual int  YY_FogHashParser_LEX(YY_FogHashParser_STYPE* YY_FogHashParser_LVAL, YY_FogHashParser_LTYPE* YY_FogHashParser_LLOC) YY_FogHashParser_LEX_BODY;
#else
	virtual int  YY_FogHashParser_LEX(YY_FogHashParser_STYPE* YY_FogHashParser_LVAL) YY_FogHashParser_LEX_BODY;
#endif
#else
	virtual int YY_FogHashParser_LEX() YY_FogHashParser_LEX_BODY;
	YY_FogHashParser_STYPE YY_FogHashParser_LVAL;
#ifdef YY_FogHashParser_LSP_NEEDED
	YY_FogHashParser_LTYPE YY_FogHashParser_LLOC;
#endif
	int YY_FogHashParser_NERRS;
	int YY_FogHashParser_CHAR;
#endif
#if YY_FogHashParser_DEBUG != 0
public:
	int YY_FogHashParser_DEBUG_FLAG;	/*  nonzero means print parse trace	*/
#endif
public:
	YY_FogHashParser_CLASS(YY_FogHashParser_CONSTRUCTOR_PARAM);
public:
	YY_FogHashParser_MEMBERS
};
/* other declare folow */
#endif


#if YY_FogHashParser_COMPATIBILITY != 0
	/* backward compatibility */
	/*  Removed due to bison problems
	/#ifndef YYSTYPE
	/ #define YYSTYPE YY_FogHashParser_STYPE
	/#endif*/
	
	#ifndef YYLTYPE
		#define YYLTYPE YY_FogHashParser_LTYPE
	#endif
	#ifndef YYDEBUG
		#ifdef YY_FogHashParser_DEBUG
			#define YYDEBUG YY_FogHashParser_DEBUG
		#endif
	#endif
	
#endif
/* END */
#endif
