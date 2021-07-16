#include <Fog/FogIncludeAll.h>









TYPEINFO_SINGLE(FogCaseStatement, Super)
FOGTOKEN_LEAF_IMPL(FogCaseStatement)

FogCaseStatement::FogCaseStatement(FogRaw& aStatement)
:
    _statement(aStatement)
{}

FogCaseStatement::FogCaseStatement(FogExpr& anExpr, FogRaw& aStatement)
:
    _case(anExpr),
    _statement(aStatement)
{}

FogCaseStatement::FogCaseStatement(const This& aStatement)
:
    Super(aStatement),
    _case(aStatement._case),
    _statement(aStatement._statement)
{}

FogCaseStatement::~FogCaseStatement() {}

void FogCaseStatement::create_usage(FogUsageContext& aContext) const
{
    Super::create_usage(aContext);
    if (_case)
        _case->create_usage(aContext);
    _statement->create_usage(aContext);
}

bool FogCaseStatement::emit(FogEmitContext& emitContext) const
{
    if (is_meta())
        ERRMSG("BUG -- should not emit meta-case-statement.");
    emitContext.emit_hash_line(line());
    if (_case)
    {
        emitContext.emit_space_and_text("case ");       
        _case->emit(emitContext);
    }
    else
        emitContext.emit_space_and_text("default");
    emitContext.emit_space_and_text(":\n");
    {
        FogSeparatedEmitContext separatedContext(emitContext, "", "\n");
        FogIndentEmitContext indentedContext(separatedContext, 1, FogEmitContext::INDENTS);
        _statement->emit(indentedContext);
    }
    return true;
}

size_t FogCaseStatement::executable_tokens() const { return 1 + _case->executable_tokens() + _statement->executable_tokens(); }

void FogCaseStatement::install(FogInstallContext& installContext) const
{
    Super::install(installContext);
    if (_case)
        _case->install(installContext);
    _statement->install(installContext);
}

bool FogCaseStatement::is_actual(const FogScopeContext& scopeContext) const
{
    return Super::is_actual(scopeContext) && (!_case || _case->is_actual(scopeContext)) && _statement->is_actual(scopeContext);
}

FogRaw *FogCaseStatement::is_case(FogScopeContext& scopeContext, const FogToken& theCase)
{
    if (!is_meta())
        return 0;
    if (theCase.is_null())
    {
        if (_case->is_null())
            return _statement.pointer();
        else
            return 0;
    }
    else if (_case->is_null())
        return _statement.pointer();
    else
    {
        const PrimNumber& thisCaseNumber = _case->get_number(scopeContext);
        const PrimNumber& thatCaseNumber = theCase.get_number(scopeContext);
        if (thisCaseNumber == thatCaseNumber)
            return _statement.pointer();
        else
            return 0;
    }
    return 0;
}

void FogCaseStatement::make_actual_from(FogMakeActualContext& makeActualContext)
{
    Super::make_actual_from(makeActualContext);
    if (_case)
        _case->make_actual(_case.to_const(), makeActualContext);
    _statement->make_actual(_statement.to_const(), makeActualContext);
}

void FogCaseStatement::merge_from(FogMergeContext& mergeContext, const This& thatExpr)
{
    Super::merge_from(mergeContext, thatExpr);
    thatExpr._case->merge_into(mergeContext, _case.to_const());
    thatExpr._statement->merge_into(mergeContext, _statement.to_const());
}

const FogMerge& FogCaseStatement::needs_merge_from(FogMergeContext& mergeContext, const This& thatExpr) const
{
    FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, thatExpr));
    needsMerge |= _case->needs_merge(mergeContext, *thatExpr._case);
    needsMerge |= _statement->needs_merge(mergeContext, *thatExpr._statement);
    return needsMerge;
}

std::ostream& FogCaseStatement::print_depth(std::ostream& s, int aDepth) const
{
    Super::print_depth(s, aDepth);
    _case->print_deep(s, aDepth);
    _statement->print_deep(s, aDepth);
    return s;
}

std::ostream& FogCaseStatement::print_members(std::ostream& s, int aDepth) const
{
    Super::print_members(s, aDepth);
    _case->print_on(s, aDepth);
    _statement->print_on(s, aDepth);
    return s;
}

char FogCaseStatement::print_named(std::ostream& s, const PrimId *fullId, char tailChar) const
{
    if (is_meta())
        tailChar = FogStream::space_and_emit(s, tailChar, "auto ");
    if (_case)
    {
        tailChar = FogStream::space_and_emit(s, tailChar, "case ");     
        tailChar = _case->print_named(s, 0, tailChar);
    }
    else
        tailChar = FogStream::space_and_emit(s, tailChar, "default");
    tailChar = FogStream::space_and_emit(s, tailChar, ": ");
    tailChar = _statement->print_named(s, 0, tailChar);
    return tailChar;
}
