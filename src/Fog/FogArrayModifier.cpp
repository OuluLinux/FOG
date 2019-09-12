#include <Fog/FogIncludeAll.h>












TYPEINFO_SINGLE(FogArrayModifier, Super)
FOGTOKEN_LEAF_IMPL(FogArrayModifier)

FogArrayModifier::FogArrayModifier(const FogExpr *arrayDimension)
:
    _dimension(arrayDimension)
{}

FogArrayModifier::FogArrayModifier(const This& aDecl)
:
    Super(aDecl),
    _dimension(aDecl._dimension)
{}

FogArrayModifier::~FogArrayModifier() {}

void FogArrayModifier::create_usage(FogUsageContext& usageContext) const
{
    Super::create_usage(usageContext);
    _dimension->create_usage(usageContext);
}

void FogArrayModifier::deduce_template_parameter(FogExprRef& deducedExpr) const
{
    deducedExpr.adopt(new FogStarExpr(FogPointerModifier::make(), *deducedExpr));
}

void FogArrayModifier::deduce_template_parameter(FogName& deducedType, FogExprRef& deducedExpr) const
{
    deducedExpr.adopt(new FogMulExpr(deducedType, FogPointerModifier::make(), *deducedExpr));
}

bool FogArrayModifier::emit_prefix(FogEmitContext& emitContext, bool needsBrackets) const
{
    if (!needsBrackets)
        return false;
    emitContext.emit_space_and_text("(");
    return true;
}

bool FogArrayModifier::emit_suffix(FogEmitContext& emitContext, bool needsBrackets) const
{
    if (needsBrackets)
        emitContext.emit_space_and_text(")");
    emitContext.emit_space_and_text("[");
    _dimension->emit(emitContext);
    emitContext.emit_space_and_text("]");
    return true;
}

void FogArrayModifier::install(FogInstallContext& installContext) const
{
    Super::install(installContext);
    _dimension->install(installContext);
}

bool FogArrayModifier::is_actual(const FogScopeContext& scopeContext) const
{
    return Super::is_actual(scopeContext) && _dimension->is_actual(scopeContext);
}

const FogArrayModifier *FogArrayModifier::is_array_modifier() const { return this; }

void FogArrayModifier::make_actual_from(FogMakeActualContext& makeActualContext)
{
    Super::make_actual_from(makeActualContext);
    _dimension->make_actual(_dimension, makeActualContext);
}

void FogArrayModifier::merge_from(FogMergeContext& mergeContext, const This& thatDecl)
{
    Super::merge_from(mergeContext, thatDecl);
    thatDecl._dimension->merge_into(mergeContext, _dimension);
}

const FogMetaType& FogArrayModifier::meta_type() const { return FogMetaType::array_modifier_type(); }

const FogMerge& FogArrayModifier::needs_merge_from(FogMergeContext& mergeContext, const This& thatDecl) const
{
    FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, thatDecl));
    FogMergeHandle dimensionNeedsMerge(_dimension->needs_merge(mergeContext, *thatDecl._dimension));
    if (dimensionNeedsMerge->is_incompatible())
        needsMerge |= FogMerge::incompatible_dimension();
    else
        needsMerge |= dimensionNeedsMerge;
    return needsMerge;
}

char FogArrayModifier::print_prefix(std::ostream& s, char tailChar, bool needsBrackets) const
{
    if (needsBrackets)
        tailChar = FogStream::space_and_emit(s, tailChar, "(");
    return tailChar;
}

char FogArrayModifier::print_suffix(std::ostream& s, char tailChar, bool needsBrackets) const
{
    if (needsBrackets)
        tailChar = FogStream::space_and_emit(s, tailChar, ")");
    tailChar = FogStream::space_and_emit(s, tailChar, "[");
    if (_dimension)
        tailChar = _dimension->print_named(s, 0, tailChar);
    tailChar = FogStream::space_and_emit(s, tailChar, "]");
    return tailChar;
}
