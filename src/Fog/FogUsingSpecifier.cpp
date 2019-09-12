#include <Fog/FogIncludeAll.h>







TYPEINFO_SINGLE(FogUsingSpecifier, Super)
PRIMREF_NULL_CLAS(FogUsingSpecifier)
    FOGTOKEN_NULL_LEAF
};
PRIMREF_DERIVED_NULL_IMPL(FogUsingSpecifier)
PRIMREFS_IMPL(FogUsingSpecifier)
FOGTOKEN_LEAF_IMPL(FogUsingSpecifier)
TMPL_HACK_FIX_DO(FogUsingSpecifier)

FogUsingSpecifier::FogUsingSpecifier() {}

FogUsingSpecifier::FogUsingSpecifier(FogName& nameName)
:
    Super(nameName)
{}

FogEntity *FogUsingSpecifier::make_name_entity(FogMakeEntityContext& makeEntityContext) const
{
    ERRMSG("ROUNDTUIT -- missing code for FogUsingSpecifier::make_name_entity.");
    return 0;
}

FogEntity *FogUsingSpecifier::make_type_entity(FogMakeEntityContext& makeEntityContext) const { return 0; }

const FogMetaType& FogUsingSpecifier::meta_type() const { return FogMetaType::using_directive_type(); }

std::ostream& FogUsingSpecifier::print_viz(std::ostream& s) const
{
    s << "\"using-directive: ";
    print_this(s);
    return s << '\"';
}

bool FogUsingSpecifier::resolve_semantics(FogSemanticsContext& theSemantics) const
{
    FogChildSemanticsContext nameSemantics(theSemantics);
    if (!name().resolve_semantics(nameSemantics))
        return false;
    theSemantics.reset();
    if (nameSemantics.is_scoped_identifier())
        theSemantics.assign(FogSemantics::IS_USING_DIRECTIVE);
    return true;
}
