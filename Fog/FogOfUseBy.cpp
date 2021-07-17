#include <Fog/FogIncludeAll.h>







static const char* const FogOfUseBy_texts[] = {
	"OF_NAME_BY_HEAD", "OF_TAIL_BY_HEAD",
	"OF_NAME_BY_IMPLEMENTATION", "OF_TAIL_BY_IMPLEMENTATION",
	"OF_NAME_BY_NAME", "OF_TAIL_BY_NAME",
	"OF_NAME_BY_?", "OF_TAIL_BY_?"
};

const FogOfUseBy FogOfUseBy_Flyweights::_flyWeights[] = {
	FogOfUseBy::OfUseBy(0), FogOfUseBy::OfUseBy(1), FogOfUseBy::OfUseBy(2), FogOfUseBy::OfUseBy(3),
	FogOfUseBy::OfUseBy(4), FogOfUseBy::OfUseBy(5), FogOfUseBy::OfUseBy(6), FogOfUseBy::OfUseBy(7)
};

const PrimId& FogOfUseBy::id() const {
	static PrimIdHandle idArray[PRIM_ARRAY_SIZE(FogOfUseBy_texts)];
	PrimIdHandle& theId = idArray[_of_use_by];
	
	if (!theId)
		theId = values()[_of_use_by];
		
	return *theId;
}

const PrimEnum& FogOfUseBy::values() {
	static const PrimEnum theValues("FogOfUseBy", FogOfUseBy_texts, PRIM_ARRAY_SIZE(FogOfUseBy_texts));
	return theValues;
}

std::ostream& operator<<(std::ostream& s, const FogOfUseBy& ofUseBy) {
	return s << FogOfUseBy::values()[ofUseBy._of_use_by];
}
