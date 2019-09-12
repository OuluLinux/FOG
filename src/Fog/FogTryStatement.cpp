#include <Fog/FogIncludeAll.h>









TYPEINFO_SINGLE(FogTryStatement, Super)
FOGTOKEN_LEAF_IMPL(FogTryStatement)

FogTryStatement::FogTryStatement(FogRaw& aStatement, FogTokens& exceptionHandlers)
:
    _statement(aStatement),
    _handlers(new FogTokens)
{
    FogTokenListOfRef& theTokens = exceptionHandlers.tokens();
    for (unsigned int i = theTokens.tally(); i-- > 0; )
        _handlers->add(*theTokens[i]);
}

FogTryStatement::FogTryStatement(const This& aStatement)
:
    Super(aStatement),
    _statement(aStatement._statement),
    _handlers(aStatement._handlers)
{}

FogTryStatement::~FogTryStatement() {}

void FogTryStatement::create_usage(FogUsageContext& aContext) const
{
    Super::create_usage(aContext);
    _statement->create_usage(aContext);
    _handlers->create_usage(aContext);
}

bool FogTryStatement::emit(FogEmitContext& emitContext) const
{
    if (is_meta())
        ERRMSG("BUG -- should not emit meta-try-block.");
    emitContext.emit_hash_line(line());
    emitContext.emit_space_and_text("try\n");       
    FogSeparatedEmitContext separatedContext(emitContext, "", "\n");
    _statement->emit(separatedContext);
    _handlers->emit(separatedContext);
    return true;
}

size_t FogTryStatement::executable_tokens() const
{
    return 1 + _statement->executable_tokens() /*  + _handlers->install(installContext)*/;
}

void FogTryStatement::install(FogInstallContext& installContext) const
{
    Super::install(installContext);
    _statement->install(installContext);
    _handlers->install(installContext);
}

bool FogTryStatement::is_actual(const FogScopeContext& scopeContext) const
{
    return Super::is_actual(scopeContext) && _statement->is_actual(scopeContext)
       && _handlers->is_actual(scopeContext);
}

void FogTryStatement::make_actual_from(FogMakeActualContext& makeActualContext)
{
    Super::make_actual_from(makeActualContext);
    _statement->make_actual(_statement.to_const(), makeActualContext);
    _handlers->make_actual(_handlers.to_const(), makeActualContext);
}

void FogTryStatement::merge_from(FogMergeContext& mergeContext, const This& thatExpr)
{
    Super::merge_from(mergeContext, thatExpr);
    thatExpr._statement->merge_into(mergeContext, _statement.to_const());
    thatExpr._handlers->merge_into(mergeContext, _handlers.to_const());
}

const FogMerge& FogTryStatement::needs_merge_from(FogMergeContext& mergeContext, const This& thatExpr) const
{
    FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, thatExpr));
    needsMerge |= _statement->needs_merge(mergeContext, *thatExpr._statement);
    needsMerge |= _handlers->needs_merge(mergeContext, *thatExpr._handlers);
    return needsMerge;
}

std::ostream& FogTryStatement::print_depth(std::ostream& s, int aDepth) const
{
    Super::print_depth(s, aDepth);
    _statement->print_deep(s, aDepth);
    _handlers->print_deep(s, aDepth);
    return s;
}

std::ostream& FogTryStatement::print_members(std::ostream& s, int aDepth) const
{
    Super::print_members(s, aDepth);
    _statement->print_on(s, aDepth);
    _handlers->print_on(s, aDepth);
    return s;
}

char FogTryStatement::print_named(std::ostream& s, const PrimId *fullId, char tailChar) const
{
    if (is_meta())
        tailChar = FogStream::space_and_emit(s, tailChar, "auto ");
    tailChar = FogStream::space_and_emit(s, tailChar, "try { ");     
    tailChar = _statement->print_named(s, 0, tailChar);
    tailChar = FogStream::space_and_emit(s, tailChar, " } ...");
//      tailChar = _handlers->print_named(s, 0, tailChar);
    return tailChar;
}

void FogTryStatement::set_is_meta()
{
    ERRMSG("Meta-try-blocks are not supported.");
}
