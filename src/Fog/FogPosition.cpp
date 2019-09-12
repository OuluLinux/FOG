#include <Fog/FogIncludeAll.h>






TYPEINFO_SINGLE(FogPosition, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogPosition)
PRIMREFS_IMPL(FogPosition)
TMPL_HACK_FIX_DO(FogPosition)

FogPosition *FogPosition_Flyweights::_flyweights[FogPositionEnums::ARRAY_SIZE] = { 0 };
bool FogPosition_Flyweights::_initialised = initialise();

struct FogPosition_Flyweights_Initializer
{
 const char *_name;
 FogPositionValue::Enum _position_enum;
};
 
bool FogPosition_Flyweights::initialise()
{
 static const FogPosition_Flyweights_Initializer flyweightInits[] =
 {
  { "REPLACE", FogPosition::REPLACE },
  { "PREPEND", FogPosition::PREPEND },
  { "APPEND", FogPosition::APPEND },
  { "EXCLUSIVE", FogPosition::EXCLUSIVE },
  { "/*INVALID-position*/", FogPosition::INVALID },
  { 0, FogPosition::INVALID }
 };
 static FogPositionRef flyWeights[FogPositionEnums::ARRAY_SIZE];
 for (const FogPosition_Flyweights_Initializer *p = flyweightInits; p->_name; ++p)
 {
  flyWeights[p->_position_enum].adopt(new FogPosition(p->_name, p->_position_enum));
  _flyweights[p->_position_enum] = flyWeights[p->_position_enum].pointer();
 }
 return true;
}

FogPosition::FogPosition(const char *aName, Enum anEnum)
:
 Super(*PrimIdHandle(aName)),
 _position(anEnum)
{}
