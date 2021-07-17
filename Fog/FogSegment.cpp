#include <Fog/FogIncludeAll.h>






TYPEINFO_SINGLE(FogSegment, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogSegment)
PRIMREFS_IMPL(FogSegment)
TMPL_HACK_FIX_DO(FogSegment)

const FogSegment_Initializer FogSegment_Flyweights::_invalid_init =
{ "/*invalid*/segment", FogTokenType::init(), FogSegment::INVALID };

FogSegment* FogSegment_Flyweights::_flyweights[FogSegmentEnums::ARRAY_SIZE] = {
	&FogTokenType::get_segment(FogTokenType::ENTRY),
	&FogTokenType::get_segment(FogTokenType::PRE),
	&FogTokenType::get_segment(FogTokenType::BODY),
	&FogTokenType::get_segment(FogTokenType::POST),
	&FogTokenType::get_segment(FogTokenType::EXIT),
	&FogSegment_Flyweights::invalid()
};

FogSegment& FogSegment_Flyweights::invalid() {
	static const FogSegmentRef aSegment(new FogSegment(_invalid_init), FogSegmentRef::ADOPT);
	return *aSegment;
}

FogSegment::FogSegment(const FogSegment_Initializer& segmentInit)
	:
	Super(*PrimIdHandle(segmentInit._name)),
	_segment(segmentInit._segment_enum),
	_token_type(segmentInit._table_enum)
{}

const FogSegment& FogSegment::merge_segment(const FogSegment& aSegment) const {
	if (!aSegment.is_valid() || (*this == aSegment))
		return *this;
		
	if (!is_valid())
		return aSegment;
		
	ERRMSG("BUG - should not merge incompatible segments \"" << *this << "\" and \"" << aSegment << '\"');
	return flyweight(INVALID);
}

const FogMerge& FogSegment::needs_merge_segment(const FogSegment& aSegment) const {
	if (_segment == aSegment._segment)
		return FogMerge::both_valid();
		
	FogSegmentValue mergedSegment = _segment | aSegment._segment;
	
	if (is_valid())
		return FogMerge::right_invalid();
		
	if (aSegment.is_valid())
		return FogMerge::left_invalid();
		
	return FogMerge::incompatible_segment();
}

std::ostream& FogSegment::print_viz(std::ostream& s) const {
	return s << "\"segment-identifier: " << id() << '\"';
}

FogTokenType::TokenType FogSegment::token_type_enum() const { return _token_type; }
