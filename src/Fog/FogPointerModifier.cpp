#include <Fog/FogIncludeAll.h>










TYPEINFO_SINGLE(FogPointerModifier, Super)
PRIMREF_NULL_CLAS(FogPointerModifier)
    FOGTOKEN_NULL_LEAF
};
PRIMREF_DERIVED_NULL_IMPL(FogPointerModifier)
PRIMREFS_IMPL(FogPointerModifier)
//  FOGTOKEN_LEAF_IMPL(FogPointerModifier)
FOGTOKEN_SHAREDLEAF_IMPL(FogPointerModifier)
TMPL_HACK_FIX_DO(FogPointerModifier)

FogPointerModifier::FogPointerModifier(const FogCv& aCv)
:
    _cv(aCv)
{}

FogPointerModifier::FogPointerModifier(const This& aDecl)
:
    Super(aDecl),
    _cv(aDecl._cv)
{}

FogPointerModifier::~FogPointerModifier() {}

void FogPointerModifier::deduce_template_parameter(FogExprRef& deducedExpr) const
{
    deducedExpr.adopt(new FogStarExpr(make(*_cv), *deducedExpr));
}

void FogPointerModifier::deduce_template_parameter(FogName& deducedType, FogExprRef& deducedExpr) const
{
    deducedExpr.adopt(new FogMulExpr(deducedType, make(*_cv), *deducedExpr));
}

bool FogPointerModifier::emit_prefix(FogEmitContext& emitContext, bool needsBrackets) const
{
    emitContext.emit_space_and_text("*");
    emitContext.emit_cv(_cv->decl_specifier_value());
    return true;
}

bool FogPointerModifier::is_formal_template_parameter_for(const FogModifier& actualModifier) const
{
    if (actualModifier.is_star())
        return true;
    if (actualModifier.is_array_modifier())
        return true;
    return false;
}

bool FogPointerModifier::is_indirect() const { return true; }
bool FogPointerModifier::is_star() const { return !_cv->is_cv(); }

FogPointerModifier& FogPointerModifier::make(const FogCv& aCv)
{
    static bool initFlags[FogCv::ARRAY_SIZE] = { false };
    static FogPointerModifierRef declTab[FogCv::ARRAY_SIZE];
    const FogCv::Enum cvValue = aCv.mask().value();
    if (!initFlags[cvValue])
    {
        declTab[cvValue].adopt(new FogPointerModifier(aCv));
        initFlags[cvValue] = true;
    }
    return *declTab[cvValue];
}

void FogPointerModifier::merge_from(FogMergeContext& mergeContext, const This& aToken)
{
    Super::merge_from(mergeContext, aToken);
    _cv = _cv->merge_cv(*aToken._cv);
}

const FogMetaType& FogPointerModifier::meta_type() const { return FogMetaType::pointer_modifier_type(); }

const FogMerge& FogPointerModifier::needs_merge_from(FogMergeContext& mergeContext, const This& aToken) const
{
    FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, aToken));
    needsMerge |= _cv->needs_merge_cv(*aToken._cv);
    return needsMerge;
}

FogPointerModifier& FogPointerModifier::new_cv(const FogPointerModifier& aDecl, const FogCv& aCv)
{
    FogPointerModifier& aModifier = make(FogCv::flyweight(aDecl._cv->merge_cv(aCv)));
    aModifier.share();
    return aModifier;
}

char FogPointerModifier::print_prefix(std::ostream& s, char tailChar, bool needsBrackets) const
{
    tailChar = FogStream::space_and_emit(s, tailChar, "*");
    if (_cv->is_cv())
        tailChar = FogStream::space_and_emit(s, tailChar, _cv->str());
    return tailChar;
}
