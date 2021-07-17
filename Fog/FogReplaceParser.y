//**************************** RACAL Confidential ****************************
//
//	Title:			Bison++ macro replacement parser
//
//	File Name:		FogReplaceParser.y
//
//	Author:			E.D.Willink
//
//	SCCS:			%W% %G%
//END
//


//
// 	FogReplaceParser is a secondary parser invoked from the lexer to perform the replacement of macro
//	invocations within a macro replacement.
//
//	Function and Object-like macros are expanded recusively.
//	The problem of self-referential identifiers is resolved by the distinct MacroId token.
//	Constructor provides a token buffer that may be used by the FogSubstituteParser to build an input.
//	Usage invokes one of parse_function or parse_object to load the the topkens, perform
//	## concatenation and then perform replacement.
//	## concatenation is resolved first, rather than convolved into the parser to
//	avoid nested generation of macro-invocations and a whitespace shift-reduce difficulty.
//
% name FogReplaceParser
% define DEBUG 1
% define ENUM_TOKEN FogReplaceParserEnums
% define STYPE FogParserValue
% define INHERIT : public PrimPrint

% define MEMBERS \
FogLexerMacro& _lexer; \
FogTokenRef _primed; \
FogTokenListOfRef& _toks; \
const PrimId& _macro_id; \
FogTokensRef _input_tokens; \
size_t _index; \
const FogMacro* _invoke; \
FogMacroListOfRefToConst _args; \
FogTokensRef _arg_toks; \
private: \
void add_arg(); \
void invoke_function(); \
void invoke_test(FogKeyword& anId); \
static FogToken& join(FogToken& firstToken, FogToken& secondToken); \
public: \
virtual ~FogReplaceParser(); \
FogTokens& input_tokens() { return *_input_tokens; } \
int parse_function(const FogMacro& aMacro, const FogMacroListOfRefToConst& locatedArguments); \
int parse_object(const FogTokenListOfRef& inputTokens);

% define CONSTRUCTOR_PARAM \
FogLexerMacro& aLexer, \
FogTokenListOfRef& outputTokens, \
const PrimId& macroId
% define CONSTRUCTOR_INIT : \ _lexer(aLexer), \
_toks(outputTokens), \
_macro_id(macroId), \
_input_tokens(new FogTokens, FogTokensRef::ADOPT), \
_index(0), \
_invoke(0)
% define CONSTRUCTOR_CODE \
YY_FogReplaceParser_DEBUG_FLAG = Fog::debug_yacc(); \
CONDMSG(YY_FogReplaceParser_DEBUG_FLAG || Fog::debug_lex2replace(), \
        "\nStarting macro replacement parser for " << _macro_id.str());


% {
#include <Fog/FogIncludeAll.h>
	%
}

% type	<keyword>		id punct

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
gram0:	/* empty */
|	gram punct						{ if ($2) _toks.add(*$2); }
|	gram ','						{ if ($2) _toks.add(*$2); }
	|	gram ')'						{ if ($2) _toks.add(*$2); }
		|	gram '{'						{ if ($2) _toks.add(*$2); }
			|	gram CharacterLiteral			{ if ($2) _toks.add(*$2); }
				|	gram MacroId					{ if ($2) _toks.add(*$2); }
					|	gram NumberLiteral				{ if ($2) _toks.add(*$2); }
						|	gram StringLiteral				{ if ($2) _toks.add(*$2); }
							|	gram id							{}
							|	gram id DO_FUNCTION ws.opt args	{ invoke_function(); _args.vacate(); }
						gram3:	gram0
							|	gram3 '('						{ if ($2) _toks.add(*$2); }
								|	gram3 ' '						{ if ($2) _toks.add(*$2); }
									|	gram3 NewLine					{ if ($2) _toks.add(*$2); }
									gram:	gram id DO_FUNCTION ws.opt		{ if ($2) _toks.add(*$2); }
											|	gram3
											
										id:		Identifier						{ $$ = $1; if ($1) invoke_test(*$1); }
										
											args1:	'('								{ _args.vacate(); _arg_toks.adopt(new FogTokens); }
|	args1 tokens
|	args1 ','						{ add_arg(); _arg_toks.adopt(new FogTokens); }
args:	args1 ')'						{ add_arg(); }

tokens:	punct							{ if ($1) _arg_toks->add(*$1); }
|	'{'								{ if ($1) _arg_toks->add(*$1); }
	|	' '								{ if ($1) _arg_toks->add(*$1); }
		|	CharacterLiteral				{ if ($1) _arg_toks->add(*$1); }
			|	Identifier						{ if ($1) _arg_toks->add(*$1); }
				|	MacroId							{ if ($1) _arg_toks->add(*$1); }
					|	NewLine							{ if ($1) _arg_toks->add(*$1); }
						|	NumberLiteral					{ if ($1) _arg_toks->add(*$1); }
							|	StringLiteral					{ if ($1) _arg_toks->add(*$1); }
								|	toks0 ')'						{ if ($2) _arg_toks->add(*$2); }
								
								toks0:	'('								{ if ($1) _arg_toks->add(*$1); }
|	toks0 tokens
|	toks0 ','						{ if ($2) _arg_toks->add(*$2); }

punct:	Punct | HASH_HASH | SHL | SHR | EQ | NE | LE | GE | LOG_AND | LOG_OR
|	'#' | '}' | '[' | ']' | ';' | ':' | '?' | '.'
|	'+' | '-' | '*' | '/' | '%' | '^' | '&' | '|' | '~' | '!' | '=' | '<' | '>'
|	'@' | '$' | '\'' | '"' | '\\'

ws.opt:	/* empty */
|	ws.opt ' '
|	ws.opt NewLine

% %
#include <Fog/FogIncludeAll.h>

FogReplaceParser::~FogReplaceParser() {
	CONDMSG(YY_FogReplaceParser_DEBUG_FLAG || Fog::debug_lex2replace(),
	        "Exiting macro replacement parser for " << _macro_id.str() << "\n");
}

void FogReplaceParser::add_arg() {
	if (_invoke) {
		const PrimId& anId = _invoke->formal(_args.tally());
		_args.adopt(new FogMacro(anId, 0, *_arg_toks));
		_arg_toks.reset();
	}
}

void FogReplaceParser::invoke_function() {
	if (_invoke)
		_lexer.invoke_function(_toks, *_invoke, _args);
}

void FogReplaceParser::invoke_test(FogKeyword& anId) {
	if (anId.id() == _macro_id)
		_toks.add(FogTokenType::make_macro_identifier(anId.id()));
	else if ((_invoke = _lexer.is_defined(anId.id())) == 0)
		_toks.add(anId);
	else if (!_invoke->is_function_like())
		_lexer.invoke_object(_toks, *_invoke);
	else
		_primed = FogTokenType::get(FogTokenType::DO_FUNCTION);
}

FogToken& FogReplaceParser::join(FogToken& firstToken, FogToken& secondToken) {
	PrimOstrstream s;
	firstToken.print_source(s, 0);				//.bugbug maybe print_stringize/print_named ?
	secondToken.print_source(s, 0);
	size_t aSize = s.pcount();
	
	switch (firstToken.pp_token_type_enum()) {
	case FogTokenType::CharacterLiteral:
		if (firstToken.is_wide())
			return FogTokenType::make_wide_character(s.str(), aSize);
		else
			return FogTokenType::make_narrow_character(s.str(), aSize);
			
	case FogTokenType::Identifier:
	case FogTokenType::MacroId:
		return FogTokenType::make_identifier(s.str(), aSize);
		
	case FogTokenType::NumberLiteral:
		return FogTokenType::make_number(s.str(), aSize);
		
	case FogTokenType::StringLiteral:
		if (firstToken.is_wide())
			return FogTokenType::make_wide_string(s.str(), aSize);
		else
			return FogTokenType::make_narrow_string(s.str(), aSize);
			
	default:
		ERRMSG("BUG - don't know how to concatenate " << firstToken << " and " << secondToken);
		return firstToken;
	}
}

int FogReplaceParser::parse_function(const FogMacro& aMacro, const FogMacroListOfRefToConst& locatedArguments) {
	{
		FogSubstituteParser substituteParser(_lexer, *_input_tokens, aMacro, locatedArguments);
		int substituteStatus = substituteParser.yyparse();
		
		if ((substituteStatus != 0) && !aMacro.is_bad()) {
			ERRMSG("Bad token sequence in " << viz(aMacro));
			aMacro.set_bad();
		}
	}
	return parse_object(_input_tokens->tokens());
}

//
//	Perform replacement after copying inputTokens to _input_tokens replacing any ## combinations.
//	Note that inputTokens may alias _input_tokens in the case of a function-like macro.
//
int FogReplaceParser::parse_object(const FogTokenListOfRef& inputTokens) {
	CONDMSG(YY_FogReplaceParser_DEBUG_FLAG || Fog::debug_lex2replace(),
	        "\nStarting macro replacement parser for " << _macro_id.str());
	        
	if (inputTokens.tally()) {
		const FogTokenRef* p = &inputTokens[0];
		size_t q = 0;
		const FogTokenRef* pEnd = &inputTokens[inputTokens.tally() - 1];
		
		while (p <= pEnd) {
			if ((*p)->pp_token_type_enum() != FogTokenType::HASH_HASH) {
				if (q >= _input_tokens->tally())
					_input_tokens->add(**p);
				else
					_input_tokens->tokens()[q].assign(**p);
					
				p++;
				q++;
			}
			else {
				for (++p; (p <= pEnd) && (*p)->is_white(); p++)
					;
					
				while ((q > 0) && _input_tokens->tokens()[--q]->is_white())
					;
					
				if (p > pEnd)
					ERRMSG("## should not be last token");
				else if ((q <= 0) && _input_tokens->tokens()[q]->is_white())
					ERRMSG("## should not be first token");
				else {
					_input_tokens->tokens()[q].assign(join(*_input_tokens->tokens()[q], **p));
					q++;
					p++;
				}
			}
		}
		
		_input_tokens->tokens().set_tally(q);
	}
	
	return yyparse();
}

//
//	Pass msg to the lexer to get a sensible error message.
//
inline void FogReplaceParser::yyerror(char* msg) { ERRMSGZ(msg); }

int FogReplaceParser::yylex() {
	FogTokenRef theToken;
	
	if (_primed->pp_token_type_enum()) {
		theToken = _primed;
		_primed.reset();
	}
	else {
		if (_index >= _input_tokens->tally())
			return 0;
			
		theToken = *_input_tokens->tokens()[_index++];
	}
	
	if (Fog::debug_lex2replace()) {
		PrimOstrstream s;
		
		if (yydebug)
			s << "\t\t\t\t";
			
		s << "<REPLACE> >> ";
		theToken->print_diagnostic(s, 0);
		DIAMSGZ(s.str());
	}
	
	yylval.token = theToken.pointer();
	return theToken->pp_token_type_enum();
}
