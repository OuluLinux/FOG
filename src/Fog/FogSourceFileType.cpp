#include <Fog/FogIncludeAll.h>







const FogSourceFileType FogSourceFileType_Flyweights::_flyWeights[FogSourceFileType::INVALID+1] =
{
 FogSourceFileType::TOP_INPUT,
 FogSourceFileType::HASH_INPUT,
 FogSourceFileType::USING_INPUT,
 FogSourceFileType::UNREAD_INPUT,
 FogSourceFileType::INVALID
};

static const char *const FogSourceFileType_texts[] =
{
 "top-input", "hash-input", "using-input", "unread-input", "invalid-input"
};

const PrimEnum& FogSourceFileType::values()
{
 static const PrimEnum theValues("FogSourceFileType", FogSourceFileType_texts,
  PRIM_ARRAY_SIZE(FogSourceFileType_texts));
 return theValues;
}

std::ostream& operator<<(std::ostream& s, const FogSourceFileType& fileType)
{
 return s << FogSourceFileType::values()[fileType._file_type];
}
