#include <Fog/FogIncludeAll.h>








TYPEINFO_SINGLE(FogCatchStatement, Super)
PRIMREF_NULL_CLAS(FogCatchStatement)
    FOGTOKEN_NULL_LEAF
};
PRIMREF_DERIVED_NULL_IMPL(FogCatchStatement)
PRIMREFS_IMPL(FogCatchStatement)
FOGTOKEN_LEAF_IMPL(FogCatchStatement)
TMPL_HACK_FIX_DO(FogCatchStatement)

FogCatchStatement::FogCatchStatement() {}

FogCatchStatement::FogCatchStatement(FogExpr& anExpr, FogRaw& aStatement)
:
    _exception(anExpr),
    _statement(aStatement)
{}

FogCatchStatement::FogCatchStatement(const This& aStatement)
:
    Super(aStatement),
    _exception(aStatement._exception),
    _statement(aStatement._statement)
{}

FogCatchStatement::~FogCatchStatement() {}

void FogCatchStatement::create_usage(FogUsageContext& aContext) const
{
    Super::create_usage(aContext);
    _exception->create_usage(aContext);
    _statement->create_usage(aContext);
}

bool FogCatchStatement::emit(FogEmitContext& emitContext) const
{
    if (is_meta())
        ERRMSG("BUG -- should not emit meta-handler.");
//      emitContext.emit_hash_line(line());
    emitContext.start();
    emitContext.emit_space_and_text("catch (");       
    _exception->emit(emitContext);
    emitContext.emit_space_and_text(")\n");
    FogSeparatedEmitContext separatedContext(emitContext, "", "\n");
    _statement->emit(separatedContext);
    return true;
}

size_t FogCatchStatement::executable_tokens() const
{
    return 1 + _exception->executable_tokens() + _statement->executable_tokens();
}

void FogCatchStatement::install(FogInstallContext& installContext) const
{
    Super::install(installContext);
    _exception->install(installContext);
    _statement->install(installContext);
}

bool FogCatchStatement::is_actual(const FogScopeContext& scopeContext) const
{
    return Super::is_actual(scopeContext) && _exception->is_actual(scopeContext) && _statement->is_actual(scopeContext);
}

void FogCatchStatement::make_actual_from(FogMakeActualContext& makeActualContext)
{
    Super::make_actual_from(makeActualContext);
    _exception->make_actual(_exception.to_const(), makeActualContext);
    _statement->make_actual(_statement.to_const(), makeActualContext);
}

void FogCatchStatement::merge_from(FogMergeContext& mergeContext, const This& thatExpr)
{
    Super::merge_from(mergeContext, thatExpr);
    thatExpr._exception->merge_into(mergeContext, _exception.to_const());
    thatExpr._statement->merge_into(mergeContext, _statement.to_const());
}

const FogMerge& FogCatchStatement::needs_merge_from(FogMergeContext& mergeContext, const This& thatExpr) const
{
    FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, thatExpr));
    needsMerge |= _exception->needs_merge(mergeContext, *thatExpr._exception);
    needsMerge |= _statement->needs_merge(mergeContext, *thatExpr._statement);
    return needsMerge;
}

std::ostream& FogCatchStatement::print_depth(std::ostream& s, int aDepth) const
{
    Super::print_depth(s, aDepth);
    _exception->print_deep(s, aDepth);
    _statement->print_deep(s, aDepth);
    return s;
}

std::ostream& FogCatchStatement::print_members(std::ostream& s, int aDepth) const
{
    Super::print_members(s, aDepth);
    _exception->print_on(s, aDepth);
    _statement->print_on(s, aDepth);
    return s;
}

char FogCatchStatement::print_named(std::ostream& s, const PrimId *fullId, char tailChar) const
{
    if (is_meta())
        tailChar = FogStream::space_and_emit(s, tailChar, "auto ");
    tailChar = FogStream::space_and_emit(s, tailChar, "catch (");     
    tailChar = _exception->print_named(s, 0, tailChar);
    tailChar = FogStream::space_and_emit(s, tailChar, ") ");
    tailChar = _statement->print_named(s, 0, tailChar);
    return tailChar;
}

void FogCatchStatement::set_is_meta()
{
    ERRMSG("Meta-handlers are not supported.");
}
