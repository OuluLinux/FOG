#include <Fog/FogIncludeAll.h>












TYPEINFO_SINGLE(FogDyadicExpr, Super)

FogDyadicExpr::FogDyadicExpr(const This& anExpr)
:
 Super(anExpr),
 _expr1(anExpr._expr1)
{}

void FogDyadicExpr::create_usage(FogUsageContext& aContext) const
{
 Super::create_usage(aContext);
 _expr1->create_usage(aContext);
}

bool FogDyadicExpr::emit(FogEmitContext& emitContext) const
{
 expr().emit(emitContext);
 emitContext.emit_space_and_text(separator());
 _expr1->emit(emitContext);
 return true;
}

size_t FogDyadicExpr::executable_tokens() const { return 1 + expr().executable_tokens() + expr1().executable_tokens(); }

bool FogDyadicExpr::get_object(FogTokenRef& tokenValue, FogScopeContext& inScope) const
{
 FogTokenRef leftValue;
 FogTokenRef rightValue;
 if (!expr().get_object(leftValue, inScope))
  return false;
 if (!_expr1->get_object(rightValue, inScope))
  return false;
 return leftValue->get_dyadic(tokenValue, inScope, dyadic_token(), *rightValue);
}

void FogDyadicExpr::install(FogInstallContext& installContext) const
{
 Super::install(installContext);
 _expr1->install(installContext);
}

bool FogDyadicExpr::is_actual(const FogScopeContext& scopeContext) const
{
 return Super::is_actual(scopeContext) && _expr1->is_actual(scopeContext);
}

void FogDyadicExpr::make_actual_from(FogMakeActualContext& makeActualContext)
{
 Super::make_actual_from(makeActualContext);
 _expr1->make_actual(_expr1.to_const(), makeActualContext);
}

void FogDyadicExpr::merge_from(FogMergeContext& mergeContext, const This& anExpr)
{
 Super::merge_from(mergeContext, anExpr);
 anExpr._expr1->merge_into(mergeContext, _expr1.to_const());
}

const FogMerge& FogDyadicExpr::needs_merge_from(FogMergeContext& mergeContext, const This& anExpr) const
{
 FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, anExpr));
 needsMerge |= _expr1->needs_merge(mergeContext, *anExpr._expr1);
 return needsMerge;
}

std::ostream& FogDyadicExpr::print_depth(std::ostream& s, int aDepth) const
{
 Super::print_depth(s, aDepth);
 _expr1->print_deep(s, aDepth);
 return s;
}

std::ostream& FogDyadicExpr::print_members(std::ostream& s, int aDepth) const
{
 Super::print_members(s, aDepth);
 _expr1->print_on(s, aDepth);
 return s;
}

char FogDyadicExpr::print_named(std::ostream& s, const PrimId *fullId, char tailChar) const
{
 tailChar = Super::print_named(s, 0, tailChar);
 tailChar = FogStream::space_and_emit(s, tailChar, separator());
 if (_expr1)
  tailChar = _expr1->print_named(s, 0, tailChar);
 return tailChar;
}

bool FogDyadicExpr::resolve_semantics(FogSemanticsContext& theSemantics) const
{
 FogChildSemanticsContext leftSemantics(theSemantics);
 FogChildSemanticsContext rightSemantics(theSemantics);
 if (!expr().resolve_semantics(leftSemantics) || !expr1().resolve_semantics(rightSemantics))
  return false;
 theSemantics.reset();
 if (leftSemantics.is_basic_expression() && rightSemantics.is_basic_expression())
  theSemantics.set(FogSemantics::IS_BASIC_EXPRESSION);
 return true;
}
 
#define FOGDYADICEXPR_IMPLEMENTATION(T, N, DT) \
TYPEINFO_SINGLE(T, Super) \
FOGTOKEN_LEAF_IMPL(T) \
FogTokenType::TokenType T::dyadic_token() const { return FogTokenType::TokenType(DT); } \
const char *T::separator() const { return N; }

FOGDYADICEXPR_IMPLEMENTATION(FogAddExpr, " + ", '+')
FOGDYADICEXPR_IMPLEMENTATION(FogAndExpr, " & ", '&')
FOGDYADICEXPR_IMPLEMENTATION(FogArrowStarExpr, " ->* ", FogTokenType::ARROW_STAR)
FOGDYADICEXPR_IMPLEMENTATION(FogDivExpr, " / ", '/')
FOGDYADICEXPR_IMPLEMENTATION(FogDotStarExpr, " .* ", FogTokenType::DOT_STAR)
FOGDYADICEXPR_IMPLEMENTATION(FogEqExpr, " == ", FogTokenType::EQ)
FOGDYADICEXPR_IMPLEMENTATION(FogGeExpr, " >= ", FogTokenType::GE)
FOGDYADICEXPR_IMPLEMENTATION(FogGtExpr, " > ", '>')
FOGDYADICEXPR_IMPLEMENTATION(FogLeExpr, " <= ", FogTokenType::LE)
FOGDYADICEXPR_IMPLEMENTATION(FogLogAndExpr, " && ", FogTokenType::LOG_AND)
FOGDYADICEXPR_IMPLEMENTATION(FogLogOrExpr, " || ", FogTokenType::LOG_OR)
FOGDYADICEXPR_IMPLEMENTATION(FogLtExpr, " < ", '<')
FOGDYADICEXPR_IMPLEMENTATION(FogModExpr, " % ", '%')
FOGDYADICEXPR_IMPLEMENTATION(FogMulExpr, " * ", '*')
FOGDYADICEXPR_IMPLEMENTATION(FogNeExpr, " != ", FogTokenType::NE)
FOGDYADICEXPR_IMPLEMENTATION(FogOrExpr, " | ", '|')
FOGDYADICEXPR_IMPLEMENTATION(FogShlExpr, " << ", FogTokenType::SHL)
FOGDYADICEXPR_IMPLEMENTATION(FogShrExpr, " >> ", FogTokenType::SHR)
FOGDYADICEXPR_IMPLEMENTATION(FogSubExpr, " - ", '-')
FOGDYADICEXPR_IMPLEMENTATION(FogXorExpr, " ^ ", '^')
 
void FogAddExpr::make_actual_from(FogMakeActualContext& makeActualContext)
{
    Super::make_actual_from(makeActualContext);         //  .bugbug need to convert (type)expr to cast
}

void FogAndExpr::make_actual_from(FogMakeActualContext& makeActualContext)
{
    Super::make_actual_from(makeActualContext);         //  .bugbug need to convert (type)expr to cast
}

bool FogAndExpr::make_specifier(FogMakeSpecifierContext& makeSpecifierContext)
{
 if (!expr1().make_specifier(makeSpecifierContext))
  return false;
 FogObjectSpecifier *objectSpecifier = makeSpecifierContext.get_object_specifier(*this);
 FogName *typeName = expr().is_name();
 if (!objectSpecifier)
  ;
 else if (!typeName)
  ERRMSG("BUG -- expected name as left of " << viz(*this));
 else
 {
  typeName->make_type_specifier(makeSpecifierContext);
  objectSpecifier->add(FogReferenceModifier::make());
 }
 return true;
}

bool FogAndExpr::resolve_semantics(FogSemanticsContext& theSemantics) const
{
 FogChildSemanticsContext leftSemantics(theSemantics);
 FogChildSemanticsContext rightSemantics(theSemantics);
 if (!expr().resolve_semantics(leftSemantics) || !expr1().resolve_semantics(rightSemantics))
  return false;
 if (rightSemantics.is_basic_expression())
 {
  if (leftSemantics.is_basic_expression())
   theSemantics.set(FogSemantics::IS_BASIC_EXPRESSION);
  if (leftSemantics.is_type_cast())
  {
   theSemantics.set(FogSemantics::IS_BASIC_EXPRESSION);
   theSemantics.set(FogSemantics::IS_MISPARSED_EXPRESSION);
  }
 }
 if (leftSemantics.is_typed_decl_specifier_seq())
 {
  if (rightSemantics.is_function_declarator())
   theSemantics.set(FogSemantics::IS_FUNCTION_TYPE_ID);
  if (rightSemantics.is_object_declarator_id())
   theSemantics.set(FogSemantics::IS_OBJECT_TYPE_ID);
  if (rightSemantics.is_parameter_declarator_opt())
   theSemantics.set(FogSemantics::IS_PARAMETER_TYPE_ID);
 }
 if (leftSemantics.is_type_specifier_seq())
 {
  if (rightSemantics.is_conversion_declarator_opt())
   theSemantics.set(FogSemantics::IS_CONVERSION_TYPE_ID);
  if (rightSemantics.is_type_id_declarator_opt())
   theSemantics.set(FogSemantics::IS_TYPE_ID);
 }
 return true;
}

void FogAndExpr::set_decl_specifier_expression(FogExprRef& anExpr, FogDeclSpecifier& declSpecifiers)
{
 set_decorated_decl_specifier_expression(anExpr, declSpecifiers);
}
 
FogMulExpr::FogMulExpr(const This& anExpr)
:
 Super(anExpr),
 _ptr_op(anExpr._ptr_op)
{}

FogMulExpr::FogMulExpr(FogExpr& firstExpr, FogModifier& ptrOp, FogExpr& secondExpr)
:
 Super(firstExpr, secondExpr),
 _ptr_op(ptrOp)
{}

bool FogMulExpr::emit(FogEmitContext& emitContext) const
{
 expr().emit(emitContext);
 emitContext.emit_space_and_text(" ");
 _ptr_op->emit(emitContext);
 emitContext.emit_space_and_text(" ");
 expr1().emit(emitContext);
 return true;
}

void FogMulExpr::make_actual_from(FogMakeActualContext& makeActualContext)
{
    Super::make_actual_from(makeActualContext);         //  .bugbug need to convert (type)expr to cast
}

bool FogMulExpr::make_specifier(FogMakeSpecifierContext& makeSpecifierContext)
{
 if (!expr1().make_specifier(makeSpecifierContext))
  return false;
 FogObjectSpecifier *objectSpecifier = makeSpecifierContext.get_object_specifier(*this);
 FogName *typeName = expr().is_name();
 if (!objectSpecifier)
  ;
 else if (!typeName)
  ERRMSG("BUG -- expected name as left of " << viz(*this));
 else
 {
  typeName->make_type_specifier(makeSpecifierContext);
  objectSpecifier->add(*_ptr_op);
 }
 return true;
}

char FogMulExpr::print_named(std::ostream& s, const PrimId *fullId, char tailChar) const
{
 tailChar = expr().print_named(s, 0, tailChar);
 tailChar = _ptr_op->print_named(s, 0, tailChar);
 tailChar = expr1().print_named(s, fullId, tailChar);
 return tailChar;
}

bool FogMulExpr::resolve_semantics(FogSemanticsContext& theSemantics) const
{
 FogChildSemanticsContext leftSemantics(theSemantics);
 FogChildSemanticsContext rightSemantics(theSemantics);
 if (!expr().resolve_semantics(leftSemantics) || !expr1().resolve_semantics(rightSemantics))
  return false;
 if (rightSemantics.is_basic_expression() && _ptr_op->is_star())
 {
  if (leftSemantics.is_basic_expression())
   theSemantics.set(FogSemantics::IS_BASIC_EXPRESSION);
  if (leftSemantics.is_type_cast())
  {
   theSemantics.set(FogSemantics::IS_BASIC_EXPRESSION);
   theSemantics.set(FogSemantics::IS_MISPARSED_EXPRESSION);
  }
 }
 if (leftSemantics.is_typed_decl_specifier_seq())
 {
  if (rightSemantics.is_function_declarator())
   theSemantics.set(FogSemantics::IS_FUNCTION_TYPE_ID);
  if (rightSemantics.is_object_declarator_id())
   theSemantics.set(FogSemantics::IS_OBJECT_TYPE_ID);
  if (rightSemantics.is_parameter_declarator_opt())
   theSemantics.set(FogSemantics::IS_PARAMETER_TYPE_ID);
 }
 if (leftSemantics.is_type_specifier_seq())
 {
  if (rightSemantics.is_conversion_declarator_opt())
   theSemantics.set(FogSemantics::IS_CONVERSION_TYPE_ID);
  if (rightSemantics.is_type_id_declarator_opt())
   theSemantics.set(FogSemantics::IS_TYPE_ID);
 }
 return true;
}

void FogMulExpr::set_decl_specifier_expression(FogExprRef& anExpr, FogDeclSpecifier& declSpecifiers)
{
 set_decorated_decl_specifier_expression(anExpr, declSpecifiers);
}

void FogSubExpr::make_actual_from(FogMakeActualContext& makeActualContext)
{
    Super::make_actual_from(makeActualContext);         //  .bugbug need to convert (type)expr to cast
}
