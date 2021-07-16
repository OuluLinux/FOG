#include <Fog/FogIncludeAll.h>









TYPEINFO_SINGLE(FogGotoStatement, Super)
FOGTOKEN_LEAF_IMPL(FogGotoStatement)

FogGotoStatement::FogGotoStatement(FogName& aLabel)
:
    _label(aLabel)
{}

FogGotoStatement::FogGotoStatement(const This& aStatement)
:
    Super(aStatement),
    _label(aStatement._label)
{}

FogGotoStatement::~FogGotoStatement() {}

void FogGotoStatement::create_usage(FogUsageContext& aContext) const
{
    Super::create_usage(aContext);
    _label->create_usage(aContext);
}

bool FogGotoStatement::emit(FogEmitContext& emitContext) const
{
    emitContext.emit_hash_line(line());
    emitContext.emit_space_and_text("goto ");
    _label->emit(emitContext);
    emitContext.emit_space_and_text(";");
    return true;
}

size_t FogGotoStatement::executable_tokens() const { return _label->executable_tokens(); }

void FogGotoStatement::install(FogInstallContext& installContext) const
{
    Super::install(installContext);
    _label->install(installContext);
}

bool FogGotoStatement::is_actual(const FogScopeContext& scopeContext) const
{
    return Super::is_actual(scopeContext) && _label->is_actual(scopeContext);
}

void FogGotoStatement::make_actual_from(FogMakeActualContext& makeActualContext)
{
    Super::make_actual_from(makeActualContext);
    _label->make_actual(_label, makeActualContext);
}

void FogGotoStatement::merge_from(FogMergeContext& mergeContext, const This& thatExpr)
{
    Super::merge_from(mergeContext, thatExpr);
    thatExpr._label->merge_into(mergeContext, _label.to_const());
}

const FogMerge& FogGotoStatement::needs_merge_from(FogMergeContext& mergeContext, const This& thatExpr) const
{
    FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, thatExpr));
    needsMerge |= _label->needs_merge(mergeContext, *thatExpr._label);
    return needsMerge;
}

std::ostream& FogGotoStatement::print_depth(std::ostream& s, int aDepth) const
{
    Super::print_depth(s, aDepth);
    _label->print_deep(s, aDepth);
    return s;
}

std::ostream& FogGotoStatement::print_members(std::ostream& s, int aDepth) const
{
    Super::print_members(s, aDepth);
    _label->print_on(s, aDepth);
    return s;
}

char FogGotoStatement::print_named(std::ostream& s, const PrimId *fullId, char tailChar) const
{
    if (is_meta())
        tailChar = FogStream::space_and_emit(s, tailChar, "auto ");
    tailChar = FogStream::space_and_emit(s, tailChar, "goto ");
    tailChar = _label->print_named(s, 0, tailChar);
    tailChar = FogStream::space_and_emit(s, tailChar, ";");
    return tailChar;
}

void FogGotoStatement::set_is_meta()
{
    ERRMSG("Meta-goto-statements are not supported.");
}
