#include <Fog/FogIncludeAll.h>










TYPEINFO_SINGLE(FogReferenceModifier, Super)
FOGTOKEN_LEAF_IMPL(FogReferenceModifier)

FogReferenceModifier::FogReferenceModifier() {}

FogReferenceModifier::FogReferenceModifier(const This& aDecl)
:
    Super(aDecl)
{}

FogReferenceModifier::~FogReferenceModifier() {}

void FogReferenceModifier::deduce_template_parameter(FogExprRef& deducedExpr) const
{
    deducedExpr.adopt(new FogAddrExpr(*deducedExpr));
}

void FogReferenceModifier::deduce_template_parameter(FogName& deducedType, FogExprRef& deducedExpr) const
{
    deducedExpr.adopt(new FogAndExpr(deducedType, *deducedExpr));
}

bool FogReferenceModifier::emit_prefix(FogEmitContext& emitContext, bool needsBrackets) const
{
    emitContext.emit_space_and_text("& ");
    return true;
}

bool FogReferenceModifier::is_formal_template_parameter_for(const FogModifier& actualModifier) const
{
    if (actualModifier.is_reference())
        return true;
    return false;
}

bool FogReferenceModifier::is_indirect() const { return true; }
bool FogReferenceModifier::is_reference() const { return true; }

FogModifier& FogReferenceModifier::make()
{
    static FogModifierRef aModifier(new FogReferenceModifier, FogModifierRef::ADOPT);
    return *aModifier;
}

const FogMetaType& FogReferenceModifier::meta_type() const { return FogMetaType::reference_modifier_type(); }

char FogReferenceModifier::print_prefix(std::ostream& s, char tailChar, bool needsBrackets) const
{
    return FogStream::space_and_emit(s, tailChar, "& ");
}
