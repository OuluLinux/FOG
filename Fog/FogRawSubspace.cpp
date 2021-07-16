#include <Fog/FogIncludeAll.h>








TYPEINFO_SINGLE(FogRawSubspace, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogRawSubspace)
PRIMREFS_IMPL(FogRawSubspace)
FOGTOKEN_LEAF_IMPL(FogRawSubspace)
TMPL_HACK_FIX_DO(FogRawSubspace)

FogRawSubspace::FogRawSubspace() {}
FogRawSubspace::~FogRawSubspace() {}

const FogLazyBool& FogRawSubspace::derive_at(FogScopeContext& inScope) const
{
    bool hasDormantClause = false;
    bool hasNoDerivedClause = true;
    for (FogRawConstListOfRefToConstIterator p(raws()); p; ++p)
    {
        const FogDerivedClause *derivedClause = p->is_derived_clause();
        if (derivedClause)
        {
            hasNoDerivedClause = false;
            const FogLazyBool& isActive = derivedClause->derive_at(inScope);
            if (isActive.is_true())
                return FogLazyBool::make_true();
            if (isActive.is_undecided())
                hasDormantClause = true;
        }
    }
    if (hasDormantClause)
        return FogLazyBool::make_undecided();
    return FogLazyBool::make_bool(hasNoDerivedClause && inScope.at_root());
}

bool FogRawSubspace::emit(FogEmitContext& emitContext) const
{
    emitContext.emit_space_and_text(":{");
    Super::emit(emitContext);
    emitContext.emit_space_and_text("}");
    return true;
}

FogToken::IsExposed FogRawSubspace::is_exposed() const
{
    return IS_EXPOSED;     //   A fudge that makes emit work nicely.
}

std::ostream& FogRawSubspace::print_viz(std::ostream& s) const
{
    s << "\"subspace : ";
    print_named(s, 0, 0);
    return s << '\"';
}
