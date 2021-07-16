#include <Fog/FogIncludeAll.h>


























TYPEINFO_SINGLE(FogParserBase, Super)

FogParserBase::FogParserBase(FogLexerTower& aLexer, FogToken& firstToken, const char *aTitle, int aDepth)
:
 _title(aTitle),
 _lexer(aLexer),
 _saved_scope(aLexer.reader().scope()),
 _saved_utility_mode(aLexer.reader().utility_mode()),
 _depth(aDepth),
 _ws_prefix(false),
 _nest(0),
 _result(0),
 _target_file(0)
{
 _primed[0].assign(firstToken);
 _lexer.reader().set_diagnostic_depth(_depth);
}

FogParserBase::~FogParserBase()
{
 _lexer.reader().set_scope(_saved_scope);
 _lexer.reader().set_utility_mode(_saved_utility_mode);
 _lexer.reader().set_diagnostic_depth(_depth-1);
}

void FogParserBase::check_lookahead(int yyChar)
{
 if ((yyChar > 0) && (yyChar != '#') && (_lookahead.value()->token_type_enum() != yyChar))
  raw_error("BUG -- lost track of parser look ahead.");
}

FogRaw *FogParserBase::compile_declaration(FogUtility *utilityMode, FogRaw *aDeclaration)
{
 if (!aDeclaration || _nest)
  return aDeclaration;
 FogStaticParseContext parseContext(scope(), *aDeclaration, *_access,
  utilityMode ? *utilityMode : FogUtility::emit_utility());
 return aDeclaration->compile_declaration(parseContext) ? aDeclaration : 0;
}

FogRaw *FogParserBase::compile_statement(FogRaw *aDeclaration) { return aDeclaration; }
void FogParserBase::diagnostic(const char *msg) { DIAMSGZ(msg); }
void FogParserBase::error(const char *msg) { ERRMSGZ(msg); }
void FogParserBase::garbage_add(const FogToken *aToken) { if (aToken) _lexer.add_garbage(*aToken); }
void FogParserBase::garbage_adopt(const FogToken *aToken) { if (aToken) _lexer.adopt_garbage(*aToken); }

//  
//  	Get the next token for phase 6 processing.
//  
//  	$ and @ tokens are intercepted directly and resolved by a nested parsed so that the perception
//  	by the invoking parser is of a single token indicating the return
//  	type of the dollar expression. This satisfies the requirements of single token lookahead
//  	and avoids any shift reduce conflicts that could otherwise ensue.
//  
const char *FogParserBase::get(FogTokenContext& tokenContext)
{
 const char *getState = "<PRIMED>";
 if (!_primed[0].value()->is_null())
 {
  tokenContext = _primed[0];
  _primed[0] = _primed[1];
  _primed[1].reset();
  if (Fog::debug_lex2main())
   _lexer.print_token(getState, &tokenContext.token());
  return getState;
 }
 getState = _lexer.get(tokenContext);
 if (Fog::debug_lex2main())
  _lexer.print_token(getState, &tokenContext.token());
 if (!getState)
  return 0;
 if (!tokenContext.line().is_null())
  _line = tokenContext.line();
 size_t dollarCount = 0;
 switch (tokenContext.token().token_type_enum())
 {
  case '$':
  {
   while (true)
   {
    dollarCount++;
    FogTokenRef peekedValue;
    const char *peekState = _lexer.peek(peekedValue, false); //   Adjacent peek
    if (Fog::debug_lex2main())
     _lexer.print_token(peekState, &tokenContext.token());
    if (!peekState)
     break;
    if (peekedValue->token_type_enum() != '$')
     break;
    getState = _lexer.get(tokenContext);
    if (Fog::debug_lex2main())
     _lexer.print_token(getState, &tokenContext.token());
   }
  } /*   drop-through */
  case '@':
  {
   static FogToken& formalPrimer = FogTokenType::get_character('$');
   FogParser nestedParser(_lexer, formalPrimer, "$", _depth+1);
   int parseError = nestedParser.yyparse();
   FogToken *aToken = nestedParser.result();
   FogExpr *anExpr = aToken ? aToken->is_expr() : 0;
   if (!parseError && anExpr)
            {
    tokenContext.value().adopt(new FogTreeLiteral(*anExpr, dollarCount));
       getState = "<$RETURN>";
            }
   else
            {                                  //   ??? If meta-function returning void
                WRNMSG("INVESTIGATE -- parser failure/0-expression return.");
    tokenContext.assign(FogTokenType::nil());
    getState = 0;
            }
   if (Fog::debug_lex2main())
    _lexer.print_token(getState, &tokenContext.token());
   break;
  }
  default:
   break;
 }
 return getState;
}

//  
//  	Update tokenContext to reflect a non-string concatenation of adjacent tokens.
//  
void FogParserBase::get_nonstring_concatenation(FogTokenContext& tokenContext)
{
 if (!peek_for_nonstring_concatenation())
  return;
 FogToken& firstToken = tokenContext.token();
 const FogTokenType::TokenType ppTokenType = firstToken.pp_token_type_enum();
    const FogElementType *elementType = &FogElementType::identifier();
 switch (ppTokenType)
 {
  case FogTokenType::CharacterLiteral:
   elementType = &FogElementType::character(firstToken.is_wide());
   break;
  case FogTokenType::NumberLiteral:
   elementType = &FogElementType::number();
   break;
  default:
   break;
 }
 FogMultipleId *multipleId = new FogMultipleId(*elementType);
 if (!multipleId)
  return;
 garbage_adopt(multipleId);
 multipleId->add(firstToken);
 do
 {
  while (get(tokenContext) && tokenContext.token().is_white())
   ;
  multipleId->add(tokenContext.token());
 } while (peek_for_nonstring_concatenation());
 if (multipleId->is_resolved())
 {
  FogToken *netToken = 0;
  FogStaticScopeContext inScope(scope());
  switch (ppTokenType)
  {
   case FogTokenType::CharacterLiteral:
   {
    PrimIdHandle characterId;
    multipleId->get_character(characterId, inScope);
    netToken = firstToken.is_wide()
     ? &FogTokenType::make_wide_character(characterId->str(), characterId->length())
     : &FogTokenType::make_narrow_character(characterId->str(), characterId->length());
    break;
   }
   case FogTokenType::TreeLiteral:
   case FogTokenType::Identifier:
   case FogTokenType::MacroId:
   {
    PrimIdHandle identifierId;
    multipleId->get_identifier(identifierId, inScope);
    netToken = &FogTokenType::make_identifier(identifierId->str(), identifierId->length());
    break;
   }
   case FogTokenType::NumberLiteral:
   {
    PrimIdHandle numberId;
    multipleId->get_identifier(numberId, inScope);
    netToken = &FogTokenType::make_number(numberId->str(), numberId->length());
    break;
   }
   default:
    ERRMSG("BUG - unexpected conclusion " << viz(firstToken)
       << " for FogParserBase::get_nonstring_concatenation.");
    break;
  }
  tokenContext.assign(netToken ? *netToken : FogTokenType::nil());
 }
 else
  tokenContext.assign(*multipleId);
}

//  
//  	Update tokenContext to reflect a string concatenation of space separated strings or adjacent non-strings.
//  
void FogParserBase::get_string_concatenation(FogTokenContext& tokenContext)
{
 if (!peek_for_string_concatenation())
  return;
 FogToken& firstToken = tokenContext.token();
    const FogElementType& elementType = FogElementType::string(firstToken.is_wide());
 FogMultipleId *multipleId = new FogMultipleId(elementType);
 if (!multipleId)
  return;
 garbage_adopt(multipleId);
 multipleId->add(firstToken);
 do
 {
  while (get(tokenContext) && tokenContext.token().is_white())
   ;
  multipleId->add(tokenContext.token());
 } while (peek_for_string_concatenation());
 if (multipleId->is_resolved())
 {
  FogStaticScopeContext inScope(scope());
  PrimIdHandle stringId;
  multipleId->get_string(stringId, inScope);
  FogToken& netToken = firstToken.is_wide()
   ? FogTokenType::make_wide_string(stringId->str(), stringId->length())
   : FogTokenType::make_narrow_string(stringId->str(), stringId->length());
  tokenContext.assign(netToken);
 }
 else
  tokenContext.assign(*multipleId);
}

FogScope& FogParserBase::global_scope() { return scope().global_scope(); }

void FogParserBase::include(const PrimId& fileStem, const FogUtility *aUtility)
{
 if (aUtility)
  _lexer.push_include(fileStem, *aUtility);
}

//  
//  	Return the line associated with the next token. (If we've already processed a ';' then the current line
//  	is the one following).
//  
FogLine& FogParserBase::make_current_line()
{
 FogTokenRef peekedValue;
 const char *peekState = peek(peekedValue);
 FogLine *aLine = peekedValue->is_line();
 if (!aLine)
  aLine = _line.pointer();
 garbage_add(aLine);
 return *aLine;
}

void *FogParserBase::mark(bool hasLookAhead)
{
 unget(hasLookAhead);
 _lexer.mark();
 return 0;
}

//  
//  	Peek the next token for phase 6 processing.
//  
const char *FogParserBase::peek(FogTokenRef& peekedValue, bool skipWhite)
{
 const char *peekState = 0;
 if (!_primed[0].value()->is_null())
 {
  peekedValue = _primed[0].value();
  if (!skipWhite || !peekedValue->is_white())
   peekState = "<PEEK-PRIMED>";
 }
 if (!peekState && !_primed[1].value()->is_null())
 {
  peekedValue = _primed[1].value();
  if (!skipWhite || !peekedValue->is_white())
   peekState = "<PEEK-PRIMED>";
 }
 if (!peekState)
  peekState = _lexer.peek(peekedValue, skipWhite);
 if (Fog::debug_lex2main())
  _lexer.print_token(peekState, peekedValue.pointer());
 return peekState;
}

//  
//  	Peek at the following tokens and return true if the next token can concatenate with the current token.
//  	Whitespace suppresses concatenation.
//  
bool FogParserBase::peek_for_nonstring_concatenation()
{
 FogTokenRef peekedValue;
 const char *peekState = peek(peekedValue, false);
 if (!peekState)
  return false;
 switch (peekedValue->pp_token_type_enum())
 {
  case '0':
  case FogTokenType::MacroId:
   ERRMSG("BUG - unexpected pp_token_type_enum for " << *peekedValue
      << " in FogParserBase::peek_for_nonstring_concatenation");
  case '$':
  case '@': case FogTokenType::TreeLiteral:
  case FogTokenType::CharacterLiteral: //  case FogTokenType::CharacterExpression:
  case FogTokenType::Identifier: //  case FogTokenType::IdentifierExpression:
  case FogTokenType::NumberLiteral: //  case FogTokenType::NumberExpression:
  case FogTokenType::StringLiteral: //  case FogTokenType::StringExpression: case FogTokenType::Strings:
   return true;
  default:
   return false;
 }
}

//  
//  	Peek at the following tokens and return true if the next non-whitespace token can concatenate with the
//  	current token.
//  
bool FogParserBase::peek_for_string_concatenation()
{
 FogTokenRef peekedValue;
 const char *peekState = peek(peekedValue, false);
 if (!peekState)
  return false;
 switch (peekedValue->pp_token_type_enum())
 {
  case '0':
  case FogTokenType::MacroId:
   ERRMSG("BUG - unexpected pp_token_type_enum for " << *peekedValue
      << " in FogParserBase::peek_for_string_concatenation");
  case '$':
  case '@': case FogTokenType::TreeLiteral:
  case FogTokenType::CharacterLiteral: //  case FogTokenType::CharacterExpression:
  case FogTokenType::Identifier: //  case FogTokenType::IdentifierExpression:
  case FogTokenType::NumberLiteral: //  case FogTokenType::NumberExpression:
  case FogTokenType::StringLiteral: //  case FogTokenType::StringExpression: case FogTokenType::Strings:
   return true;
  case ' ':
  case FogTokenType::NewLine:
   peekState = peek(peekedValue, true);    //   Skip white peek for strings only
   if (!peekState)
    return false;
   switch (peekedValue->pp_token_type_enum())
   {
    case FogTokenType::StringLiteral: //  case FogTokenType::StringExpression: case FogTokenType::Strings:
     return true;
    default:
     return false;
   }
  default:
   return false;
 }
}

std::ostream& FogParserBase::print_this(std::ostream& s) const
{
 return s << (_title ? _title : "?");
}

void FogParserBase::queue(FogToken& firstToken)
{
 if (!_primed[1].value()->is_null())
  raw_error("BUG - should not queue a token while two tokens queued.");
 _primed[1] = _primed[0];
 _primed[0].assign(firstToken);
}

//  
//  	Emit an erro message directly, bypassing any marks.
//  
void FogParserBase::raw_error(const char *msg)
{
 FogNullMonitor rawMonitor;
 error(msg);
}

void FogParserBase::remark(YACC_MARK_TYPE)
{
 if (!_primed[0].value()->is_null())
  raw_error("INVESTIGATE -- primed tokens in FogParserBase::remark");
 _primed[1].reset();
 _primed[0].reset();
 _lookahead.reset();
 _lexer.remark();
}

FogScope& FogParserBase::scope() { return _lexer.reader().scope(); }

void FogParserBase::set_nested_access(const FogAccess *anAccess)
{
 if (!anAccess)
  ;
 else if (!scope().is_name_space())
  _access = *anAccess;
 else if (!Fog::no_namespace())
  ERRMSG("Access specifier ignored for name space " << viz(scope()));
}

void FogParserBase::set_result(FogToken *aResult, bool hasLookAhead)
{
 _result = aResult;
 unget(hasLookAhead);
}

void FogParserBase::set_utility_mode(const FogUtility& utilityMode) { _lexer.reader().set_utility_mode(utilityMode); }

#if 0
FogTargetFile *FogParserBase::target_file()
{
 const FogLine& sourceLine = _lookahead.line();
 const FogSourceFile& sourceFile = sourceLine.file();
 if (_target_file && (_target_file->source_file() != &sourceFile))
  _target_file = 0;
 if (!_target_file && !sourceFile.source_type().is_top())
 {
  FogFileManager& fileManager = _lexer.reader().scope().global_scope().file_manager();
  _target_file = fileManager.make_target_file(sourceFile);
 }
 return _target_file;
}
#endif

void FogParserBase::unget(bool hasLookAhead)
{
 if (!_primed[1].value()->is_null())
 {
  _lexer.unget(_primed[1], true);
  _primed[1].reset();
 }
 if (!_primed[0].value()->is_null())
 {
  raw_error("INVESTIGATE -- unget() of primed tokens");
  _lexer.unget(_primed[0], true);
  _primed[0].reset();
 }
 if (hasLookAhead && !_lookahead.value()->is_null())
  _lexer.unget(_lookahead, _ws_prefix);
 _lookahead.reset();
}

//  
//  	Remove the mark, while preserving aToken, creating a share before garbage collecting and adopting
//  	into the less nested garbage collection.
//  
void FogParserBase::unmark(const FogToken *aToken, bool hasLookAhead)
{
 FogTokenRefToConst lifeBelt(aToken);
 if (_primed[0].value()->is_null())
  ;
 else if (_primed[0].value()->pp_token_type_enum() != '#')    //   -- happens in end_search()
 {
  raw_error("INVESTIGATE -- primed tokens in FogParserBase::unmark");
  _primed[1].reset();
  _primed[0].reset();
 }
 if (!hasLookAhead)
  _lookahead.reset();
 _lexer.unmark();
 if (aToken)
  _lexer.add_garbage(*aToken);
}

const FogUtility& FogParserBase::utility_mode() const { return _lexer.reader().utility_mode(); }
void FogParserBase::warning(const char *msg) { WRNMSGZ(msg); }

//  
//  	Instruct the lexer to produce another token.
//  
FogToken *FogParserBase::yylex()
{
 _ws_prefix = false;
 while (get(_lookahead))       //   0 if meta-function returned void.
 {
  const FogTokenType::TokenType ppTokenType = _lookahead.token().pp_token_type_enum();
  switch (ppTokenType)
  {
   case FogTokenType::TreeLiteral:
   case FogTokenType::CharacterLiteral:
   case FogTokenType::Identifier:
   case FogTokenType::MacroId:
   case FogTokenType::NumberLiteral:
    get_nonstring_concatenation(_lookahead);
    break;   
   case FogTokenType::StringLiteral:
    get_string_concatenation(_lookahead);
    break;
   default:
    break;
  }
  if (!_lookahead.token().is_white())
   break;
  if (!_lookahead.token().is_line())
   _ws_prefix = true;
 }
 return _lookahead.token().is_null() ? 0 : &_lookahead.token();
}
