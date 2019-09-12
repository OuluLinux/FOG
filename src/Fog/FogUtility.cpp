#include <Fog/FogIncludeAll.h>







TYPEINFO_SINGLE(FogUtility, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogUtility)
PRIMREFS_IMPL(FogUtility)
TMPL_HACK_FIX_DO(FogUtility)

const FogUtility_Initializer FogUtility_Flyweights::_invalid_init =
 { "/*invalid*/utility", FogTokenType::init(), FogUtility::INVALID };

FogUtility *FogUtility_Flyweights::_emit = &FogTokenType::get_utility(FogTokenType::EMIT);
FogUtility *FogUtility_Flyweights::_frozen = &FogTokenType::get_utility(FogTokenType::FROZEN);
FogUtility *FogUtility_Flyweights::_invalid = &FogUtility_Flyweights::invalid();
FogUtility *FogUtility_Flyweights::_pool = &FogTokenType::get_utility(FogTokenType::POOL);
FogUtility *FogUtility_Flyweights::_utility = &FogTokenType::get_utility(FogTokenType::UTILITY);

FogUtility& FogUtility_Flyweights::invalid()
{
 static const FogUtilityRef aUtility(new FogUtility(_invalid_init), FogUtilityRef::ADOPT);
 return *aUtility;
}

FogUtility::FogUtility(const FogUtility_Initializer& utilityInit)
:
 Super(*PrimIdHandle(utilityInit._name)),
 _utility(utilityInit._utility_enum),
 _token_type(utilityInit._table_enum)
{}

const FogUtility& FogUtility::merge_utility(const FogUtility& aUtility) const
{
 if (!aUtility.is_valid() || (*this == aUtility))
  return *this;
 if (!is_valid())
  return aUtility;
//  	ERRMSG("Should not merge incompatible utilities \"" << *this << "\" and \"" << aUtility << '"');
//  	return flyweight(INVALID);
 return _utility > aUtility._utility ? *this : aUtility;
}

const FogMerge& FogUtility::needs_merge_utility(const FogUtility& aUtility) const
{
 if (*this == aUtility)
  return FogMerge::both_valid();
 if (!aUtility.is_valid())
  return FogMerge::right_invalid();
 if (!is_valid())
  return FogMerge::left_invalid();
 return FogMerge::incompatible_utility();
}

std::ostream& FogUtility::print_viz(std::ostream& s) const
{
 return s << "\"utility-identifier: " << id() << '\"';
}

FogTokenType::TokenType FogUtility::token_type_enum() const { return _token_type; }
