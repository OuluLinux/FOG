#include <Fog/FogIncludeAll.h>








TYPEINFO_SINGLE(FogMetaIterator, Super)

FogMetaIterator::FogMetaIterator(FogMakerContext& makerContext,
        const FogStatic& aStatic, const FogMetaType& metaType, IsExposed isExposed, const FogToken& aToken)
:
    Super(makerContext, aStatic, metaType, isExposed, aToken)
{}

FogMetaIterator::~FogMetaIterator() {}

FogMetaEntity *FogMetaIterator::new_meta_entity(FogToken& inToken) const
{
    return new FogMetaIteratorEntity(inToken, *this);
}

std::ostream& FogMetaIterator::print_viz(std::ostream& s) const
{
    s << "\"meta-iteration: ";
    print_long_id(s);
    return s << '\"';
}
