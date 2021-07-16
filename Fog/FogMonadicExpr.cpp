#include <Fog/FogIncludeAll.h>















TYPEINFO_SINGLE(FogMonadicExpr, Super)

FogMonadicExpr::FogMonadicExpr(const This& anExpr)
:
    Super(anExpr)
{}

bool FogMonadicExpr::emit(FogEmitContext& emitContext) const
{
    emitContext.emit_space_and_text(prefix());
    expr().emit(emitContext);
    emitContext.emit_space_and_text(suffix());
    return true;
}

size_t FogMonadicExpr::executable_tokens() const { return 1 + expr().executable_tokens(); }

bool FogMonadicExpr::get_object(FogTokenRef& tokenValue, FogScopeContext& inScope) const
{
    FogTokenRef tempValue;
    return expr().get_object(tempValue, inScope) && tempValue->get_monadic(tokenValue, inScope, monadic_token());
}

char FogMonadicExpr::print_named(std::ostream& s, const PrimId *fullId, char tailChar) const
{
    tailChar = FogStream::space_and_emit(s, tailChar, prefix());
    tailChar = expr().print_named(s, fullId, tailChar);
    tailChar = FogStream::space_and_emit(s, tailChar, suffix());
    return tailChar;
}

bool FogMonadicExpr::resolve_semantics(FogSemanticsContext& theSemantics) const
{
    FogChildSemanticsContext exprSemantics(theSemantics);
    if (!expr().resolve_semantics(exprSemantics))
        return false;
    theSemantics.reset();
    if (exprSemantics.is_basic_expression())
        theSemantics.set(FogSemantics::IS_BASIC_EXPRESSION);
    return true;
}
 
#define FOGMONADICDECL_IMPLEMENTATION(T, P, S, MT) \
TYPEINFO_SINGLE(T, Super) \
FOGTOKEN_LEAF_IMPL(T) \
FogTokenType::TokenType T::monadic_token() const { return FogTokenType::TokenType(MT); } \
const char *T::prefix() const { return P; } \
const char *T::suffix() const { return S; }

FOGMONADICDECL_IMPLEMENTATION(FogAddrExpr, "&", "", '&')
FOGMONADICDECL_IMPLEMENTATION(FogComplExpr, "~", "", '~')
FOGMONADICDECL_IMPLEMENTATION(FogLogNotExpr, "!", "", '!')
FOGMONADICDECL_IMPLEMENTATION(FogNegExpr, "-", "", '-')
FOGMONADICDECL_IMPLEMENTATION(FogParenExpr, "(", ")", '=')
FOGMONADICDECL_IMPLEMENTATION(FogPostDecExpr, "", "--", FogTokenType::DEC)
FOGMONADICDECL_IMPLEMENTATION(FogPostIncExpr, "", "++", FogTokenType::INC)
FOGMONADICDECL_IMPLEMENTATION(FogPreDecExpr, "--", "", FogTokenType::DEC)
FOGMONADICDECL_IMPLEMENTATION(FogPreIncExpr, "++", "", FogTokenType::INC)
FOGMONADICDECL_IMPLEMENTATION(FogSizeofExpr, "sizeof ", "", FogTokenType::SIZEOF)
FOGMONADICDECL_IMPLEMENTATION(FogStarExpr, "*", "", '*')
FOGMONADICDECL_IMPLEMENTATION(FogTypeidExpr, "typeid(", ")", FogTokenType::TYPEID)
 
//  void FogAddrExpr::emit(FogEmitContext& emitContext) const
//  {
//    FogEmitContext::ChangedForUseAs changedContext(emitContext, FogEmitContext::GLOBAL_SIGNATURE);
//    Super::emit(emitContext);
//  }

bool FogAddrExpr::make_specifier(FogMakeSpecifierContext& makeSpecifierContext)
{
    if (!expr().make_specifier(makeSpecifierContext))
        return false;
    FogObjectSpecifier *objectSpecifier = makeSpecifierContext.get_object_specifier(*this);
    if (objectSpecifier)
        objectSpecifier->add(FogReferenceModifier::make());
    return true;
}

bool FogAddrExpr::make_typed_expression(FogExprRef& theExpr, FogName& theType)
{
    theExpr.adopt(new FogAndExpr(theType, expr()));
    return true;
}

bool FogAddrExpr::resolve_semantics(FogSemanticsContext& theSemantics) const
{
    FogChildSemanticsContext exprSemantics(theSemantics);
    if (!expr().resolve_semantics(exprSemantics))
        return false;
    theSemantics.reset();
    if (exprSemantics.is_basic_expression())
        theSemantics.set(FogSemantics::IS_BASIC_EXPRESSION);
    if (exprSemantics.is_conversion_declarator_opt())
        theSemantics.set(FogSemantics::IS_CONVERSION_DECLARATOR);
    if (exprSemantics.is_function_declarator())
        theSemantics.set(FogSemantics::IS_FUNCTION_DECLARATOR);
    if (exprSemantics.is_object_declarator_id())
        theSemantics.set(FogSemantics::IS_OBJECT_DECLARATOR_ID);
    if (exprSemantics.is_parameter_declarator_opt())
        theSemantics.set(FogSemantics::IS_PARAMETER_DECLARATOR_OPT);
    if (exprSemantics.is_type_id_declarator_opt())
        theSemantics.set(FogSemantics::IS_TYPE_ID_DECLARATOR_OPT);
    return true;
}

void FogAddrExpr::set_decl_specifier_expression(FogExprRef& anExpr, FogDeclSpecifier& declSpecifiers)
{
    anExpr.adopt(new FogAndExpr(declSpecifiers, expr()));
}
 
bool FogComplExpr::make_specifier(FogMakeSpecifierContext& makeSpecifierContext)
{
    set_destructor_expression();
    return expr().make_specifier(makeSpecifierContext);
}

bool FogComplExpr::make_typed_expression(FogExprRef& theExpr, FogName& theType)
{
    ERRMSG("INVESTIGATE -- make_typed_expression for " << viz(*this));
    set_destructor_expression();
    return expr().make_typed_expression(theExpr, theType);
//      FogName *dtorName = expr().is_name();
//      if (!dtorName)
//      {
//          ERRMSG("BUG -- expected name in " << viz(*this));
//          return false;
//      }
//      FogNameRef theName(new FogDestructorId(*dtorName), FogNameRef::ADOPT);
//      theExpr.adopt(new FogTypedName(theType, *theName));
//      return true;
}

bool FogComplExpr::resolve_semantics(FogSemanticsContext& theSemantics) const
{
    FogChildSemanticsContext exprSemantics(theSemantics);
    if (!expr().resolve_semantics(exprSemantics))
        return false;
    theSemantics.reset();
    if (exprSemantics.is_basic_expression())
        theSemantics.set(FogSemantics::IS_BASIC_EXPRESSION);
    if (exprSemantics.is_class_name())
        theSemantics.set(FogSemantics::IS_DESTRUCTOR);
    if (exprSemantics.is_destructor_declarator())
        theSemantics.set(FogSemantics::IS_DESTRUCTOR_TYPE_ID);
    if (exprSemantics.is_type_name())
        theSemantics.set(FogSemantics::IS_PSEUDO_DESTRUCTOR_ID);
    if (exprSemantics.is_meta_type_name())
        theSemantics.set(FogSemantics::IS_META_DTOR_NAME);
    return true;
}

void FogComplExpr::set_decl_specifier_expression(FogExprRef& anExpr, FogDeclSpecifier& declSpecifiers)
{
    set_decorated_decl_specifier_expression(anExpr, declSpecifiers);
    set_destructor_expression();
    anExpr = expr();
}
 
bool FogPostDecExpr::get_object(FogTokenRef& tokenValue, FogScopeContext& inScope) const
{
    if (!expr().get_object(tokenValue, inScope))
        return false;
    FogTokenRef theValue; 
    if (!tokenValue->get_monadic(theValue, inScope, monadic_token()))
        return false;
    return expr().put_value(inScope, *theValue, inScope);
}

bool FogPostIncExpr::get_object(FogTokenRef& tokenValue, FogScopeContext& inScope) const
{
    if (!expr().get_object(tokenValue, inScope))
        return false;
    FogTokenRef theValue; 
    if (!tokenValue->get_monadic(theValue, inScope, monadic_token()))
        return false;
    return expr().put_value(inScope, *theValue, inScope);
}

bool FogPreDecExpr::get_object(FogTokenRef& tokenValue, FogScopeContext& inScope) const
{
    FogTokenRefToConst tempValue(*this); 
    return Super::get_object(tokenValue, inScope)
        && expr().put_value(inScope, *tokenValue, inScope);
}

bool FogPreIncExpr::get_object(FogTokenRef& tokenValue, FogScopeContext& inScope) const
{
    FogTokenRefToConst tempValue(*this); 
    return Super::get_object(tokenValue, inScope)
        && expr().put_value(inScope, *tokenValue, inScope);
}
 
FogStarExpr::FogStarExpr(const This& anExpr)
:
    Super(anExpr),
    _ptr_op(anExpr._ptr_op)
{}

FogStarExpr::FogStarExpr(FogModifier& ptrOp, FogExpr& anExpr)
:
    Super(anExpr),
    _ptr_op(ptrOp)
{}

void FogStarExpr::create_usage(FogUsageContext& aContext) const
{
 Super::create_usage(aContext);
 _ptr_op->create_usage(aContext);
}

bool FogStarExpr::emit(FogEmitContext& emitContext) const
{
    _ptr_op->emit(emitContext);
    expr().emit(emitContext);
    return true;
}

void FogStarExpr::install(FogInstallContext& installContext) const
{
 Super::install(installContext);
 _ptr_op->install(installContext);
}

bool FogStarExpr::is_actual(const FogScopeContext& scopeContext) const
{
 return Super::is_actual(scopeContext) && _ptr_op->is_actual(scopeContext);
}

void FogStarExpr::make_actual_from(FogMakeActualContext& makeActualContext)
{
 Super::make_actual_from(makeActualContext);
 _ptr_op->make_actual(_ptr_op.to_const(), makeActualContext);
}

void FogStarExpr::merge_from(FogMergeContext& mergeContext, const This& thatExpr)
{
 Super::merge_from(mergeContext, thatExpr);
 thatExpr._ptr_op->merge_into(mergeContext, _ptr_op.to_const());
}

const FogMerge& FogStarExpr::needs_merge_from(FogMergeContext& mergeContext, const This& anExpr) const
{
 FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, anExpr));
 needsMerge |= _ptr_op->needs_merge(mergeContext, *anExpr._ptr_op);
 return needsMerge;
}

bool FogStarExpr::make_specifier(FogMakeSpecifierContext& makeSpecifierContext)
{
    if (!expr().make_specifier(makeSpecifierContext))
        return false;
    FogObjectSpecifier *objectSpecifier = makeSpecifierContext.get_object_specifier(*this);
    if (objectSpecifier)
        objectSpecifier->add(*_ptr_op);
    return true;
}

bool FogStarExpr::make_typed_expression(FogExprRef& theExpr, FogName& theType)
{
    theExpr.adopt(new FogMulExpr(theType, *_ptr_op, expr()));
    return true;
}

char FogStarExpr::print_named(std::ostream& s, const PrimId *fullId, char tailChar) const
{
 tailChar = Super::print_named(s, 0, tailChar);
 return tailChar;
}

bool FogStarExpr::resolve_semantics(FogSemanticsContext& theSemantics) const
{
    FogChildSemanticsContext exprSemantics(theSemantics);
    if (!expr().resolve_semantics(exprSemantics))
        return false;
    theSemantics.reset();
    if (exprSemantics.is_basic_expression() && _ptr_op->is_star())
        theSemantics.set(FogSemantics::IS_BASIC_EXPRESSION);
    if (exprSemantics.is_conversion_declarator_opt())
        theSemantics.set(FogSemantics::IS_CONVERSION_DECLARATOR);
    if (exprSemantics.is_function_declarator())
        theSemantics.set(FogSemantics::IS_FUNCTION_DECLARATOR);
    if (exprSemantics.is_object_declarator_id())
        theSemantics.set(FogSemantics::IS_OBJECT_DECLARATOR_ID);
    if (exprSemantics.is_parameter_declarator_opt())
        theSemantics.set(FogSemantics::IS_PARAMETER_DECLARATOR_OPT);
    if (exprSemantics.is_type_id_declarator_opt())
        theSemantics.set(FogSemantics::IS_TYPE_ID_DECLARATOR_OPT);
    return true;
}

void FogStarExpr::set_decl_specifier_expression(FogExprRef& anExpr, FogDeclSpecifier& declSpecifiers)
{
    anExpr.adopt(new FogMulExpr(declSpecifiers, *_ptr_op, expr()));
}
 
void FogSizeofExpr::create_usage(FogUsageContext& usageContext) const
{
    Super::create_usage(usageContext);      //  .bugbug use resolve_semantics toi distinguish type/value
}

void FogTypeidExpr::create_usage(FogUsageContext& usageContext) const
{
    Super::create_usage(usageContext);      //  .bugbug use resolve_semantics toi distinguish type/value
}
