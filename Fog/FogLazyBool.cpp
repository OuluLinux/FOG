#include <Fog/FogIncludeAll.h>









const FogLazyBool FogLazyBool_Flyweights::_flyWeights[FogLazyBool::DEFAULT+1] =
{
 FogLazyBool::IS_FALSE,
 FogLazyBool::IS_TRUE,
 FogLazyBool::IS_UNDECIDED
};

static const char *const FogLazyBool_texts[] =
{
 "is_false", "is_true", "is_undecided"
};

const PrimEnum& FogLazyBool::values()
{
 static const PrimEnum theValues("FogLazyBool", FogLazyBool_texts,
  PRIM_ARRAY_SIZE(FogLazyBool_texts));
 return theValues;
}

std::ostream& operator<<(std::ostream& s, const FogLazyBool& elementType)
{
 return s << FogLazyBool::values()[elementType._lazy_bool];
}
