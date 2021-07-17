#include <Fog/FogIncludeAll.h>







static const char* const FogUsingStyle_texts[] = {
	"", "using/*for-name*/", "using/*for-signature*/", "/*using-for-access*/", "/*invalid-using*/"
};

const FogUsingStyle FogUsingStyle_Flyweights::_flyWeights[FogUsingStyle::INVALID + 1] = {
	FogUsingStyle::NORMAL,
	FogUsingStyle::NAME,
	FogUsingStyle::SIGNATURE,
	FogUsingStyle::ACCESS,
	FogUsingStyle::INVALID
};

const PrimId& FogUsingStyle::id() const {
	static PrimIdHandle idArray[PRIM_ARRAY_SIZE(FogUsingStyle_texts)];
	PrimIdHandle& theId = idArray[_using];
	
	if (!theId)
		theId = values()[_using];
		
	return *theId;
}

//
//  	Note that SIGNATURE merges away.
//
const FogUsingStyle& FogUsingStyle::merge(const FogUsingStyle& aUsing) const {
	if (_using == aUsing._using)
		return aUsing;
		
	if (aUsing._using == SIGNATURE)
		return aUsing;
		
	if (_using == SIGNATURE)
		return *this;
		
	if (aUsing._using == NORMAL)
		return *this;
		
	if (_using == NORMAL)
		return aUsing;
		
	ERRMSG("BUG - should not merge incompatible usings \"" << *this << "\" and \"" << aUsing << '\"');
	return flyweight(INVALID);
}

const FogMerge& FogUsingStyle::needs_merge(const FogUsingStyle& aUsing) const {
	if (_using == aUsing._using)
		return FogMerge::both_valid();
		
	if (aUsing._using == SIGNATURE)
		return FogMerge::left_invalid();
		
	if (_using == SIGNATURE)
		return FogMerge::right_invalid();
		
	if (aUsing._using == NORMAL)
		return FogMerge::right_invalid();
		
	if (_using == NORMAL)
		return FogMerge::left_invalid();
		
	return FogMerge::incompatible_using();
}

const char* FogUsingStyle::str() const { return values()[_using]; }

const PrimEnum& FogUsingStyle::values() {
	static const PrimEnum theValues("FogUsingStyle", FogUsingStyle_texts, PRIM_ARRAY_SIZE(FogUsingStyle_texts));
	return theValues;
}

std::ostream& operator<<(std::ostream& s, const FogUsingStyle& aUsing) {
	return s << FogUsingStyle::values()[aUsing._using];
}
