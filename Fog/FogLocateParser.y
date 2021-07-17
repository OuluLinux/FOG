//**************************** RACAL Confidential ****************************
//
//	Title:			Bison++ parser
//
//	File Name:		FogLocateParser.y
//
//	Author:			E.D.Willink
//
//	SCCS:			%W% %G%
//END
//



//
// 	FogLocateParser is a secondary parser invoked from the lexer to locate the arguments of a function-like
//		macro invocation from the main source stream. Similar functionality for nested invocations occurs
//		in FogReplaceParser.
//
% name FogLocateParser
% define DEBUG 1
% define ENUM_TOKEN FogLocateParserEnums
% define STYPE FogParserValue
% define INHERIT : public PrimPrint

% define MEMBERS \
FogLexerMacro& _lexer; \
FogMacroListOfRefToConst& _args; \
const FogMacro& _macro; \
FogTokensRef _toks; \
FogTokenContext _current; \
private: \
void add_arg(); \
public: \
virtual ~FogLocateParser();

% define CONSTRUCTOR_PARAM \
FogLexerMacro& aLexer, \
FogMacroListOfRefToConst& locatedArguments, \
const FogMacro& aMacro
% define CONSTRUCTOR_INIT : \ _lexer(aLexer), \
_args(locatedArguments), \
_macro(aMacro)
% define CONSTRUCTOR_CODE \
YY_FogLocateParser_DEBUG_FLAG = Fog::debug_yacc(); \
CONDMSG(YY_FogLocateParser_DEBUG_FLAG || Fog::debug_lex2locate(), \
        "\nStarting macro argument location parser for " << _macro.id().str());

% {
#include <Fog/FogIncludeAll.h>

	%
}

% type	<keyword>	punct



% term ASM 258
% term AUTO 259
% term BREAK 260
% term CASE 261
% term CATCH 262
% term CLASS 263
% term CONST 264
% term CONST_CAST 265
% term CONTINUE 266
% term DEFAULT 267
% term DELETE 268
% term DO 269
% term DYNAMIC_CAST 270
% term ELSE 271
% term ENUM 272
% term EXPLICIT 273
% term EXPORT 274
% term EXTERN 275
% term FALSE 276
% term FOR 277
% term FRIEND 278
% term GOTO 279
% term IF 280
% term INLINE 281
% term MUTABLE 282
% term NAMESPACE 283
% term NEW 284
% term OPERATOR 285
% term PRIVATE 286
% term PROTECTED 287
% term PUBLIC 288
% term REGISTER 289
% term REINTERPRET_CAST 290
% term RETURN 291
% term SIZEOF 292
% term STATIC 293
% term STATIC_CAST 294
% term STRUCT 295
% term SWITCH 296
% term TEMPLATE 297
% term THIS 298
% term THROW 299
% term TRUE 300
% term TRY 301
% term TYPEDEF 302
% term TYPEID 303
% term TYPENAME 304
% term UNION 305
% term USING 306
% term VIRTUAL 307
% term VOLATILE 308
% term WHILE 309
% term ELLIPSIS 310
% term SCOPE 311
% term SHL 312
% term SHR 313
% term EQ 314
% term NE 315
% term LE 316
% term GE 317
% term LOG_AND 318
% term LOG_OR 319
% term INC 320
% term DEC 321
% term ARROW 322
% term ARROW_STAR 323
% term DOT_STAR 324
% term ASS_ADD 325
% term ASS_AND 326
% term ASS_DIV 327
% term ASS_MOD 328
% term ASS_MUL 329
% term ASS_OR 330
% term ASS_SHL 331
% term ASS_SHR 332
% term ASS_SUB 333
% term ASS_XOR 334
% term BuiltInTypeSpecifier 335
% term CharacterLiteral 336
% term Identifier 337
% term MetaType 338
% term NewLine 339
% term NumberLiteral 340
% term StringLiteral 341
% term TreeLiteral 342
% term ERROR 343
% term NIL 344
% term RAMMARG 345
% term BODY 346
% term DERIVED 347
% term EMIT 348
% term ENTRY 349
% term EXIT 350
% term FILE 351
% term FROZEN 352
% term GUARD 353
% term IMPLEMENTATION 354
% term INCLUDE 355
% term INTERFACE 356
% term NOGUARD 357
% term NOIMPLEMENTATION 358
% term OVERLOAD 359
% term PATH 360
% term POOL 361
% term POST 362
% term PRE 363
% term PREFIX 364
% term PURE 365
% term SUFFIX 366
% term UTILITY 367
% term ALL 368
% term PLACEHOLDER 369
% term PLACEMENT 370
% term NOAUTO 371
% term HASH_BLANK 372
% term HASH_DEFINE 373
% term HASH_ELIF 374
% term HASH_ELSE 375
% term HASH_ENDIF 376
% term HASH_ERROR 377
% term HASH_IF 378
% term HASH_IFDEF 379
% term HASH_IFNDEF 380
% term HASH_INCLUDE 381
% term HASH_LINE 382
% term HASH_PRAGMA 383
% term HASH_UNDEF 384
% term HASH_HASH 385
% term DI_HASH_HASH 386
% term DI_HASH 387
% term DI_SQUARE 388
% term DI_ERAUQS 389
% term DI_ECARB 390
% term DEFINED 391
% term DO_FUNCTION 392
% term MacroId 393
% term Punct 394
% term Text 395
% term SHIFT_THERE 396
% term REDUCE_HERE_MOSTLY 397
% term REDUCE_HERE 398
% term BOOL 399
% term CHAR 400
% term DOUBLE 401
% term FLOAT 402
% term INT 403
% term LONG 404
% term SHORT 405
% term SIGNED 406
% term UNSIGNED 407
% term VOID 408
% term WCHAR_T 409
% term IntegerLiteral 410
% term FloatingLiteral 411
% token	<brace>				'{'
% token	<character_literal>	CharacterLiteral
% token	<identifier>		Identifier
% token	<keyword>			ELLIPSIS SCOPE
% token	<keyword>			SHL SHR EQ NE LE GE
% token	<keyword>			LOG_AND LOG_OR INC DEC ARROW ARROW_STAR DOT_STAR
% token	<keyword>			ASS_ADD ASS_AND ASS_DIV ASS_MOD ASS_MUL
% token	<keyword>			ASS_OR ASS_SHL ASS_SHR ASS_SUB ASS_XOR
% token	<keyword>			HASH_HASH
% token	<keyword>			'}' '[' ']' '#' '(' ')' ';' ':' '?' '.' '+' '-' '*' '/' '%'
% token	<keyword>			'^' '&' '|' '~' '!' '=' '<' '>' ','
% token	<keyword>			'@' '$' '\'' '"' '\\'
% token	<keyword>			MacroId Punct
% token	<line>				NewLine
% token	<number_literal>	NumberLiteral
% token	<spacing>			' '
% token	<string_literal>	StringLiteral


% start gram
% %
gram:	ws.opt '(' args ')'				{ add_arg(); YYACCEPT; }
|	error							{ _lexer.raw_unget(_current); YYABORT; }
args:	/* empty */						{ _args.vacate(); _toks.adopt(new FogTokens); }
|	args tokens
|	args ','						{ add_arg(); _toks.adopt(new FogTokens); }

toks0:	'('								{ if ($1) _toks->add(*$1); }
|	toks0 tokens
|	toks0 ','						{ if ($2) _toks->add(*$2); }
tokens:	punct							{ if ($1) _toks->add(*$1); }
	|	'{'								{ if ($1) _toks->add(*$1); }
		|	' '								{ if ($1) _toks->add(*$1); }
			|	CharacterLiteral				{ if ($1) _toks->add(*$1); }
				|	Identifier						{ if ($1) _toks->add(*$1); }
					|	MacroId							{ if ($1) _toks->add(*$1); }
						|	NewLine							{ if ($1) _toks->add(*$1); }
							|	NumberLiteral					{ if ($1) _toks->add(*$1); }
								|	StringLiteral					{ if ($1) _toks->add(*$1); }
									|	toks0 ')'						{ if ($2) _toks->add(*$2); }
									
									punct:	Punct | HASH_HASH | SHL | SHR | EQ | NE | LE | GE | LOG_AND | LOG_OR
										|	'#' | '}' | '[' | ']' | ';' | ':' | '?' | '.'
										|	'+' | '-' | '*' | '/' | '%' | '^' | '&' | '|' | '~' | '!' | '=' | '<' | '>'
										|	'@' | '$' | '\'' | '"' | '\\'
										
									ws.opt:	/* empty */
										|	ws.opt ' '
										|	ws.opt NewLine
										
										% %
#include <Fog/FogIncludeAll.h>
										
										FogLocateParser::~FogLocateParser() {
											CONDMSG(YY_FogLocateParser_DEBUG_FLAG || Fog::debug_lex2locate(),
											        "Exiting macro argument location parser for " << _macro.id().str() << "\n");
										}

void FogLocateParser::add_arg() {
	if (_toks && _toks->tally()) {
		const PrimId& anId = _macro.formal(_args.tally());
		_args.adopt(new FogMacro(anId, 0, *_toks));
	}
}

//
//	Pass msg to the lexer to get a sensible error message.
//
inline void FogLocateParser::yyerror(char* msg) { ERRMSGZ(msg); }

//
//	Instruct the lexer to produce another token.
//
int FogLocateParser::yylex() {
	_lexer.raw_get(_current);
	
	if (Fog::debug_lex2locate()) {
		PrimOstrstream s;
		
		if (yydebug)
			s << "\t\t\t\t";
			
		s << "<LOCATE> >> ";
		_current.value()->print_diagnostic(s, 0);
		DIAMSGZ(s.str());
	}
	
	yylval.token = &_current.token();
	return yylval.token->pp_token_type_enum();
}
