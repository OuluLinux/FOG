//**************************** RACAL Confidential ****************************
//
//	Title:			Bison++ parser for Flexible Object Generator
//
//	File Name:		FogHashParser.y
//
//	Author:			E.D.Willink
//
//	SCCS:			%W% %G%
//END
//


//
// 	FogHashParser is a secondary parser invoked from the lexer to resolve
//		# directives
//
//	The parser support multiple gramars, corresponding to each # directive
//
//	Warning: This parser operates prior to the garbage collector so transiently owned PrimId and PrimString
//	must be used before any shift occurs - use after a reduction only sequence is ok.
//
%name FogHashParser
%define DEBUG 1
%define ENUM_TOKEN FogHashParserEnums
%define STYPE FogParserValue
%define INHERIT : public PrimPrint

%define MEMBERS \
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

%define CONSTRUCTOR_PARAM FogLexerMacro& aLexer, const FogTokenContext& startContext
%define CONSTRUCTOR_INIT : \
	_lexer(aLexer), \
	_truth(true), \
	_primed(true), \
	_context(startContext), \
	_line(0), \
	_lexer_state(set_state(startContext.token().pp_token_type_enum()))
%define CONSTRUCTOR_CODE \
	YY_FogHashParser_DEBUG_FLAG = Fog::debug_yacc(); \
	CONDMSG(YY_FogHashParser_DEBUG_FLAG || Fog::debug_lex2hash(), \
		"\nStarting # directive parser.");
	

%{
#include <Fog/FogIncludeAll.h>
%}

%type	<_bool>			is_defined_id
%type	<_long>  		expr expr_add expr_and expr_cond expr_eq expr_log_and expr_log_or expr_mul
%type	<_long>			expr_or expr_rel expr_shift expr_val expr_xor
%type	<keyword>		punct

%term ASM 258
%term AUTO 259
%term BREAK 260
%term CASE 261
%term CATCH 262
%term CLASS 263
%term CONST 264
%term CONST_CAST 265
%term CONTINUE 266
%term DEFAULT 267
%term DELETE 268
%term DO 269
%term DYNAMIC_CAST 270
%term ELSE 271
%term ENUM 272
%term EXPLICIT 273
%term EXPORT 274
%term EXTERN 275
%term FALSE 276
%term FOR 277
%term FRIEND 278
%term GOTO 279
%term IF 280
%term INLINE 281
%term MUTABLE 282
%term NAMESPACE 283
%term NEW 284
%term OPERATOR 285
%term PRIVATE 286
%term PROTECTED 287
%term PUBLIC 288
%term REGISTER 289
%term REINTERPRET_CAST 290
%term RETURN 291
%term SIZEOF 292
%term STATIC 293
%term STATIC_CAST 294
%term STRUCT 295
%term SWITCH 296
%term TEMPLATE 297
%term THIS 298
%term THROW 299
%term TRUE 300
%term TRY 301
%term TYPEDEF 302
%term TYPEID 303
%term TYPENAME 304
%term UNION 305
%term USING 306
%term VIRTUAL 307
%term VOLATILE 308
%term WHILE 309
%term ELLIPSIS 310
%term SCOPE 311
%term SHL 312
%term SHR 313
%term EQ 314
%term NE 315
%term LE 316
%term GE 317
%term LOG_AND 318
%term LOG_OR 319
%term INC 320
%term DEC 321
%term ARROW 322
%term ARROW_STAR 323
%term DOT_STAR 324
%term ASS_ADD 325
%term ASS_AND 326
%term ASS_DIV 327
%term ASS_MOD 328
%term ASS_MUL 329
%term ASS_OR 330
%term ASS_SHL 331
%term ASS_SHR 332
%term ASS_SUB 333
%term ASS_XOR 334
%term BuiltInTypeSpecifier 335
%term CharacterLiteral 336
%term Identifier 337
%term MetaType 338
%term NewLine 339
%term NumberLiteral 340
%term StringLiteral 341
%term TreeLiteral 342
%term ERROR 343
%term NIL 344
%term RAMMARG 345
%term BODY 346
%term DERIVED 347
%term EMIT 348
%term ENTRY 349
%term EXIT 350
%term FILE 351
%term FROZEN 352
%term GUARD 353
%term IMPLEMENTATION 354
%term INCLUDE 355
%term INTERFACE 356
%term NOGUARD 357
%term NOIMPLEMENTATION 358
%term OVERLOAD 359
%term PATH 360
%term POOL 361
%term POST 362
%term PRE 363
%term PREFIX 364
%term PURE 365
%term SUFFIX 366
%term UTILITY 367
%term ALL 368
%term PLACEHOLDER 369
%term PLACEMENT 370
%term NOAUTO 371
%term HASH_BLANK 372
%term HASH_DEFINE 373
%term HASH_ELIF 374
%term HASH_ELSE 375
%term HASH_ENDIF 376
%term HASH_ERROR 377
%term HASH_IF 378
%term HASH_IFDEF 379
%term HASH_IFNDEF 380
%term HASH_INCLUDE 381
%term HASH_LINE 382
%term HASH_PRAGMA 383
%term HASH_UNDEF 384
%term HASH_HASH 385
%term DI_HASH_HASH 386
%term DI_HASH 387
%term DI_SQUARE 388
%term DI_ERAUQS 389
%term DI_ECARB 390
%term DEFINED 391
%term DO_FUNCTION 392
%term MacroId 393
%term Punct 394
%term Text 395
%term SHIFT_THERE 396
%term REDUCE_HERE_MOSTLY 397
%term REDUCE_HERE 398
%term BOOL 399
%term CHAR 400
%term DOUBLE 401
%term FLOAT 402
%term INT 403
%term LONG 404
%term SHORT 405
%term SIGNED 406
%term UNSIGNED 407
%term VOID 408
%term WCHAR_T 409
%term IntegerLiteral 410
%term FloatingLiteral 411
%token	<brace>				'{'
%token	<character_literal>	CharacterLiteral
%token	<identifier>		Identifier
%token	<keyword>			ELLIPSIS SCOPE
%token	<keyword>			SHL SHR EQ NE LE GE
%token	<keyword>			LOG_AND LOG_OR INC DEC ARROW ARROW_STAR DOT_STAR
%token	<keyword>			ASS_ADD ASS_AND ASS_DIV ASS_MOD ASS_MUL
%token	<keyword>			ASS_OR ASS_SHL ASS_SHR ASS_SUB ASS_XOR
%token	<keyword>			HASH_HASH
%token	<keyword>			'}' '[' ']' '#' '(' ')' ';' ':' '?' '.' '+' '-' '*' '/' '%'
%token	<keyword>			'^' '&' '|' '~' '!' '=' '<' '>' ','
%token	<keyword>			'@' '$' '\'' '"' '\\'
%token	<keyword>			MacroId Punct
%token	<line>				NewLine
%token	<number_literal>	NumberLiteral
%token	<spacing>			' '
%token	<string_literal>	StringLiteral


%start grammar
%%
grammar:		hash_stmt line							{ YYACCEPT; }
	|			error line								{ YYABORT; }
hash_stmt:		hash_blank
	|			hash_define
	|			HASH_ELIF expr							{ _truth = $2 != 0; }
	|			HASH_ELSE ws.opt
	|			HASH_ENDIF ws.opt
	|			hash_error
	|			HASH_IF expr							{ _truth = $2 != 0; }
	|			HASH_IFDEF ws.opt is_defined_id ws.opt	{ _truth = $3; }
	|			HASH_IFNDEF ws.opt is_defined_id ws.opt	{ _truth = !$3; }
	|			hash_include ws.opt
	|			hash_line
	|			hash_pragma
	|			hash_undef ws.opt

hash_blank1:	HASH_BLANK ws.opt
hash_blank2:	hash_blank1 StringLiteral				{ WRNMSG("Unexpected trailing token(s) ignored"); }
	|			hash_blank2 StringLiteral
	|			hash_blank2 ' '
hash_blank:		hash_blank1
	|			hash_blank2

hash_define1:	HASH_DEFINE ws.opt Identifier			{ start_define($3); }
hash_define_ob:	hash_define1
hash_define_fn:	hash_define1 '(' ws.opt					{ _formals.reset(new PrimIdList); }
hash_define2:	hash_define_fn Identifier				{ if ($2) _formals->add($2->id()); }
	|			hash_define2 ws.opt ',' ws.opt Identifier	{ if ($5) _formals->add($5->id()); }
hash_define2a:	hash_define_ob
	|			hash_define_fn ')'
	|			hash_define2 ws.opt ')'
hash_define3:	hash_define2a ws.seq
	|			hash_define4
	|			hash_define4 ws.seq						{ _toks->add(FogTokenType::get_character(' ')); }
hash_define4:	hash_define3 punct						{ if ($2) _toks->add(*$2); }
	|			hash_define3 '{'						{ if ($2) _toks->add(*$2); }
	|			hash_define3 CharacterLiteral			{ if ($2) _toks->add(*$2); }
	|			hash_define3 Identifier					{ if ($2) _toks->add(*$2); }
//	|			hash_define3 MacroId					{ if ($2) _toks->add(*$2); }
	|			hash_define3 NumberLiteral				{ if ($2) _toks->add(*$2); }
	|			hash_define3 StringLiteral				{ if ($2) _toks->add(*$2); }
hash_define:	hash_define2a
	|			hash_define2a ws.seq
	|			hash_define4
	|			hash_define4 ws.seq

hash_error:		HASH_ERROR								{ _s.reset(new PrimOstrstream); if (_s) *_s << "#error "; } 
	|			hash_error StringLiteral				{ if (_s && $2) *_s << *$2; }
	|			hash_error ' '							{ if (_s) *_s << ' '; }

hash_include:	HASH_INCLUDE hash_include1
hash_include1:	StringLiteral							{ if ($1) _string = *$1; }
	|			' ' hash_include1						{}
	|			macro.include hash_include1

hash_line1:		HASH_LINE ws.opt NumberLiteral ws.opt	{ _line = $3 ? $3->as_int() : 0; }
hash_line2:		hash_line1 StringLiteral				{ if ($2) _string = *$2; }
hash_line:		hash_line1
	|			hash_line2 ws.opt

hash_pragma:	HASH_PRAGMA								{ _s.reset(new PrimOstrstream); }
	|			hash_pragma StringLiteral				{ if (_s && $2) *_s << *$2; }
	|			hash_pragma ' '							{ if (_s) *_s << ' '; }

hash_undef:		HASH_UNDEF ws.opt Identifier			{ if ($3) _id = $3->id(); }

expr_val:		' ' expr_val							{ $$ = $2; }
	|			macro.expr expr_val						{ $$ = $2; }
	|			macro.expr DEFINED ws.opt '(' ws.opt is_defined_id ws.opt ')'	{ $$ = $6; }
	|			macro.expr DEFINED ws.opt is_defined_id	{ $$ = $4; }
	|			'+' expr_val							{ $$ = $2; }
	|			'-' expr_val							{ $$ = -$2; }
	|			'!' expr_val							{ $$ = !$2; }
	|			'~' expr_val							{ $$ = ~$2; }
	|			'(' expr ')'							{ $$ = $2; }
	|			NumberLiteral							{ $$ = $1 ? $1->as_int() : 0; }
	|			CharacterLiteral						{ $$ = $1 ? $1->value() : 0; }
	|			line									{ ERRMSG("Missing term in preprocessor constant expression.");
														  _truth = false; YYACCEPT; }

expr_mul:		expr_val
	|			expr_mul ' '
	|			expr_mul macro.expr
	|			expr_mul '*' expr_val					{ $$ = $1 * $3; }
	|			expr_mul '/' expr_val					{ if ($3 == 0) { $$ = 0; WRNMSG("Divide by zero in preprocessor."); }
														  else { $$ = $1 / $3; } }
	|			expr_mul '%' expr_val					{ if ($3 == 0) { $$ = 0; WRNMSG("Modulus zero in preprocessor."); }
														  else { $$ = $1 % $3; } }
expr_add:		expr_mul
	|			expr_add '+' expr_mul					{ $$ = $1 + $3; }
	|			expr_add '-' expr_mul					{ $$ = $1 - $3; }
expr_shift:		expr_add
	|			expr_shift SHL expr_add					{ if ($3 < 0) { $$ = 0; WRNMSG("-ve left shift in preprocessor."); }
														  else { $$ = $1 << $3; } }
	|			expr_shift SHR expr_add					{ if ($3 < 0) { $$ = 0; WRNMSG("-ve right shift in preprocessor."); }
														  else { $$ = $1 >> $3;
															if ($1 < 0) WRNMSG("right shift of -ve in preprocessor."); } }
expr_rel:		expr_shift
	|			expr_rel '<' expr_shift					{ $$ = $1 < $3; }
	|			expr_rel LE expr_shift					{ $$ = $1 <= $3; }
	|			expr_rel GE expr_shift					{ $$ = $1 >= $3; }
	|			expr_rel '>' expr_shift					{ $$ = $1 > $3; }
expr_eq:		expr_rel
	|			expr_eq EQ expr_rel						{ $$ = $1 == $3; }
	|			expr_eq NE expr_rel						{ $$ = $1 != $3; }
expr_and:		expr_eq	
	|			expr_and '&' expr_eq					{ $$ = $1 & $3; }
expr_xor:		expr_and
	|			expr_xor '^' expr_and					{ $$ = $1 ^ $3; }
expr_or:		expr_xor
	|			expr_or '|' expr_xor					{ $$ = $1 | $3; }
expr_log_and:	expr_or
	|			expr_log_and LOG_AND expr_or			{ $$ = $1 && $3; }
expr_log_or:	expr_log_and
	|			expr_log_or LOG_OR expr_log_and 		{ $$ = $1 || $3; }
expr_cond:		expr_log_or
	|			expr_log_or '?' expr ':' expr			{ $$ = $1 ? $3 : $5; }
expr:			expr_cond

is_defined_id:	Identifier								{ $$ = $1 && is_defined(*$1); }
	|			MacroId									{ $$ = $1 && is_defined(*$1); }
line:			NewLine									{ _lexer.raw_unget(_context); }

//
//	An Id that could be replaced by a macro is treated as a free prefix. If macro invocation
//	succeeds the replacement tokens follow on. If it fails the BANG token follows.
//
macro.include:	Identifier								{ if ($1) invoke(*$1, false); }
macro.expr:		Identifier								{ if ($1) invoke(*$1, true); }

punct:			Punct | HASH_HASH | SHL | SHR | EQ | NE | LE | GE | LOG_AND | LOG_OR
	|			'#' | '(' | ',' | ')' | '}' | '[' | ']' | ';' | ':' | '?' | '.'
	|			'+' | '-' | '*' | '/' | '%' | '^' | '&' | '|' | '~' | '!' | '=' | '<' | '>'		
	|			'@' | '$' | '\'' | '"' | '\\'

ws.seq:			' '										{}
	|			ws.seq ' '								{}
ws.opt:			/* empty */								{}
	|			ws.opt ' '								{}

%%
#include <Fog/FogIncludeAll.h>

FogHashParser::~FogHashParser()
{
	_lexer.begin(_lexer_state);
	CONDMSG(YY_FogHashParser_DEBUG_FLAG || Fog::debug_lex2hash(), "Exiting # directive parser.\n");
}

void FogHashParser::invoke(const FogKeyword& anId, bool canBeDEFINED)
{
	static const PrimIdHandle definedId("defined");
	if (canBeDEFINED && (anId.id() == *definedId))
	{
		_context.assign(FogTokenType::DEFINED);
		_primed = true;
	}
	else if (!_lexer.hash_push(anId.id()))
	{
		_context.assign(FogTokenType::zero());
		_primed = true;
	}
}

inline bool FogHashParser::is_defined(const FogKeyword& anId) { return _lexer.is_defined(anId.id()) != 0; }

const FogLexerState& FogHashParser::set_state(FogTokenType::TokenType tokenType)
{
	switch (tokenType)
	{
		case HASH_INCLUDE:
			return _lexer.begin_include();
		case HASH_BLANK:
		case HASH_ERROR:
		case HASH_PRAGMA:
			return _lexer.begin_soak();
		default:
			return _lexer.state();
	}
}

void FogHashParser::start_define(const FogKeyword *anId) 
{
	_id = anId ? anId->id() : PrimId::null();
 	_toks.adopt(new FogTokens);
	_formals.reset();
}

//
//	Pass msg to the lexer to get a sensible error message.
//
inline void FogHashParser::yyerror(char *msg) { ERRMSGZ(msg); }

//
//	Instruct the lexer to produce another token, using any token cached in _context as first choice.
//
int FogHashParser::yylex()
{
	if (!_primed)
		_lexer.raw_get(_context);
	_primed = false;
	if (Fog::debug_lex2hash())
	{
		PrimOstrstream s;
		if (yydebug)
			s << "\t\t\t\t";
		s << "<HASH> >> ";
		_context.value()->print_diagnostic(s, 0);
		DIAMSGZ(s.str());
	}
	yylval.token = &_context.token();
	return yylval.token->pp_token_type_enum();
}
