#include <Fog/FogIncludeAll.h>









TYPEINFO_SINGLE(FogCondExpr, Super)
FOGTOKEN_LEAF_IMPL(FogCondExpr)

FogCondExpr::FogCondExpr(const This& anExpr)
:
    Super(anExpr),
    _true(anExpr._true),
    _false(anExpr._false)
{}

void FogCondExpr::create_usage(FogUsageContext& aContext) const
{
    Super::create_usage(aContext);
    _true->create_usage(aContext);
    _false->create_usage(aContext);
}

bool FogCondExpr::emit(FogEmitContext& emitContext) const
{
    FogResolutionScopeContext weakContext(emitContext, FogResolutionScopeContext::RESOLVE_LAZY);
    FogTokenRef tokenValue;
    if (!expr().get_number_token(tokenValue, weakContext))
    {
        expr().emit(emitContext);
        emitContext.emit_space_and_text(" ? ");
        _true->emit(emitContext);
        emitContext.emit_space_and_text(" : ");
        _false->emit(emitContext);
        return true;
    }
    else
    {
        const PrimNumber& aNumber = tokenValue->number().number_value();
        if (!aNumber.is_zero())
            return _true->emit(emitContext);
        else
            return _false->emit(emitContext);
    }
}

size_t FogCondExpr::executable_tokens() const
{
    return 2 + expr().executable_tokens() + _true->executable_tokens() + _false->executable_tokens();
}

bool FogCondExpr::get_object(FogTokenRef& returnValue, FogScopeContext& inScope) const
{
    FogTokenRef numberValue;    
    if (!expr().get_number_token(numberValue, inScope))
        return false;
    const PrimNumber& aNumber = numberValue->number().number_value();
    return (!aNumber.is_zero() ? _true : _false)->get_object(returnValue, inScope);
}

void FogCondExpr::install(FogInstallContext& installContext) const
{
    Super::install(installContext);
    _true->install(installContext);
    _false->install(installContext);
}

bool FogCondExpr::is_actual(const FogScopeContext& scopeContext) const
{
    return Super::is_actual(scopeContext) && _true->is_actual(scopeContext) && _false->is_actual(scopeContext);
}

void FogCondExpr::make_actual_from(FogMakeActualContext& makeActualContext)
{
    Super::make_actual_from(makeActualContext);
    _true->make_actual(_true.to_const(), makeActualContext);
    _false->make_actual(_false.to_const(), makeActualContext);
}

void FogCondExpr::merge_from(FogMergeContext& mergeContext, const This& anExpr)
{
    Super::merge_from(mergeContext, anExpr);
    anExpr._true->merge_into(mergeContext, _true.to_const());
    anExpr._false->merge_into(mergeContext, _false.to_const());
}

const FogMerge& FogCondExpr::needs_merge_from(FogMergeContext& mergeContext, const This& anExpr) const
{
    FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, anExpr));
    needsMerge |= _true->needs_merge(mergeContext, *anExpr._true);
    needsMerge |= _false->needs_merge(mergeContext, *anExpr._false);
    return needsMerge;
}

std::ostream& FogCondExpr::print_depth(std::ostream& s, int aDepth) const
{
    Super::print_depth(s, aDepth);
    _true->print_deep(s, aDepth);
    _false->print_deep(s, aDepth);
    return s;
}

std::ostream& FogCondExpr::print_members(std::ostream& s, int aDepth) const
{
    Super::print_members(s, aDepth);
    _true->print_on(s, aDepth);
    _false->print_on(s, aDepth);
    return s;
}

char FogCondExpr::print_named(std::ostream& s, const PrimId *fullId, char tailChar) const
{
    tailChar = Super::print_named(s, 0, tailChar);
    tailChar = FogStream::space_and_emit(s, tailChar, " ? ");
    if (_true)
        tailChar = _true->print_named(s, 0, tailChar);
    tailChar = FogStream::space_and_emit(s, tailChar, " : ");
    if (_false)
        tailChar = _false->print_named(s, 0, tailChar);
    return tailChar;
}

bool FogCondExpr::put_value(FogScopeContext& putScope, const FogToken& tokenValue, FogScopeContext& getScope) const
{
    FogTokenRef numberValue;    
    if (!expr().get_number_token(numberValue, getScope))
        return false;
    const PrimNumber& aNumber = numberValue->number().number_value();
    return (!aNumber.is_zero() ? _true : _false)->put_value(putScope, tokenValue, getScope);
}

bool FogCondExpr::resolve_semantics(FogSemanticsContext& theSemantics) const
{
    FogChildSemanticsContext testSemantics(theSemantics);
    FogChildSemanticsContext trueSemantics(theSemantics);
    FogChildSemanticsContext falseSemantics(theSemantics);
    if (!Super::resolve_semantics(testSemantics)
     || !_true->resolve_semantics(trueSemantics)
     || !_false->resolve_semantics(falseSemantics))
        return false;
    theSemantics.reset();
    if (testSemantics.is_basic_expression() && trueSemantics.is_expression() && falseSemantics.is_assignment_expression())
        theSemantics.set(FogSemantics::IS_CONDITIONAL_EXPRESSION);
    return true;
}
