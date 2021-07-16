#include <Fog/FogIncludeAll.h>







TYPEINFO_SINGLE(FogMetaVariable, Super)

FogMetaVariable::FogMetaVariable(FogMakerContext& makerContext,
    const FogStatic& aStatic, const FogMetaType& metaType, IsExposed isExposed, const FogToken& aToken)
:
    Super(makerContext, aStatic, metaType, isExposed, aToken)
{}

FogMetaVariable::~FogMetaVariable() {}

FogMetaEntity *FogMetaVariable::new_meta_entity(FogToken& inToken) const
{
    return new FogMetaVariableEntity(inToken, *this);
}

std::ostream& FogMetaVariable::print_viz(std::ostream& s) const
{
    s << "\"meta-variable: ";
    print_long_id(s);
    return s << '\"';
}
