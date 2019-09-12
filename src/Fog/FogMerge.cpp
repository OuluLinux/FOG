#include <Fog/FogIncludeAll.h>








static const char *const FogMerge_texts[FogMerge::IMPOSSIBLE+1] = {
	"BOTH_VALID",
	"RIGHT_INVALID",
	"LEFT_INVALID",
	"BOTH_INVALID",
	"INCOMPATIBLE_ACCESS",
	"INCOMPATIBLE_ALLOCATOR",
	"INCOMPATIBLE_BIT_FIELD_SIZE",
	"INCOMPATIBLE_CONST",
	"INCOMPATIBLE_DECLARATOR",
	"INCOMPATIBLE_DIMENSION",
	"INCOMPATIBLE_ELEMENT_TYPE",
	"INCOMPATIBLE_FRIEND",
	"INCOMPATIBLE_IDENTIFIER",
	"INCOMPATIBLE_INIT",
	"INCOMPATIBLE_INLINE",
	"INCOMPATIBLE_OVERLOAD",
	"INCOMPATIBLE_SEGMENT",
	"INCOMPATIBLE_SIGNATURE",
	"INCOMPATIBLE_STATIC",
	"INCOMPATIBLE_TAG",
	"INCOMPATIBLE_TEMPLATE_ARGUMENT",
	"INCOMPATIBLE_TEMPLATE_PARAMETER",
	"INCOMPATIBLE_TEMPLATE_RANK",
	"INCOMPATIBLE_TEMPLATING",
	"INCOMPATIBLE_TYPEDEF",
	"INCOMPATIBLE_TYPE_IDENTIFIER",
	"INCOMPATIBLE_USING",
	"INCOMPATIBLE_UTILITY",
	"INCOMPATIBLE_VALUE",
	"INCOMPATIBLE_VIRTUAL",
	"INCOMPATIBLE_VOLATILE",
	"INCOMPATIBLE",
	"IMPOSSIBLE"
};

const FogMerge FogMerge_Flyweights::_flyWeights[FogMerge::IMPOSSIBLE+1] = {
	FogMerge::BOTH_VALID,
	FogMerge::RIGHT_INVALID,
	FogMerge::LEFT_INVALID,
	FogMerge::BOTH_INVALID,
	FogMerge::INCOMPATIBLE_ACCESS,
	FogMerge::INCOMPATIBLE_ALLOCATOR,
	FogMerge::INCOMPATIBLE_BIT_FIELD_SIZE,
	FogMerge::INCOMPATIBLE_CONST,
	FogMerge::INCOMPATIBLE_DECLARATOR,
	FogMerge::INCOMPATIBLE_DIMENSION,
	FogMerge::INCOMPATIBLE_ELEMENT_TYPE,
	FogMerge::INCOMPATIBLE_FRIEND,
	FogMerge::INCOMPATIBLE_IDENTIFIER,
	FogMerge::INCOMPATIBLE_INIT,
	FogMerge::INCOMPATIBLE_INLINE,
	FogMerge::INCOMPATIBLE_OVERLOAD,
	FogMerge::INCOMPATIBLE_SEGMENT,
	FogMerge::INCOMPATIBLE_SIGNATURE,
	FogMerge::INCOMPATIBLE_STATIC,
	FogMerge::INCOMPATIBLE_TAG,
	FogMerge::INCOMPATIBLE_TEMPLATE_ARGUMENT,
	FogMerge::INCOMPATIBLE_TEMPLATE_PARAMETER,
	FogMerge::INCOMPATIBLE_TEMPLATE_RANK,
	FogMerge::INCOMPATIBLE_TEMPLATING,
	FogMerge::INCOMPATIBLE_TYPEDEF,
	FogMerge::INCOMPATIBLE_TYPE_IDENTIFIER,
	FogMerge::INCOMPATIBLE_USING,
	FogMerge::INCOMPATIBLE_UTILITY,
	FogMerge::INCOMPATIBLE_VALUE,
	FogMerge::INCOMPATIBLE_VIRTUAL,
	FogMerge::INCOMPATIBLE_VOLATILE,
	FogMerge::INCOMPATIBLE,
	FogMerge::IMPOSSIBLE
};

const PrimId& FogMerge::id() const {
	static PrimIdHandle idArray[PRIM_ARRAY_SIZE(FogMerge_texts)];
	PrimIdHandle& theId = idArray[_merge];
	
	if (!theId)
		theId = values()[_merge];
		
	return *theId;
}

const FogMerge& FogMerge::set_incompatible(const FogMerge& aMerge) {
	return aMerge;    //   A debug breakpoint target
}

const PrimEnum& FogMerge::values() {
	static const PrimEnum theValues("FogMerge", FogMerge_texts, PRIM_ARRAY_SIZE(FogMerge_texts));
	return theValues;
}

//
//  	Return the flyweight defining the merged validity. This is a simple maximum except in the
//  	case of LEFT_INVALID merged with RIGHT_INVALID which must progress to BOTH_INVALID.
//
const FogMerge& operator|(const FogMerge& firstMerge, const FogMerge& secondMerge) {
	FogMerge::Enum maxInvalidity = FogMerge::Enum(std::max(firstMerge._merge, secondMerge._merge));
	
	if (maxInvalidity == FogMerge::LEFT_INVALID) {
		FogMerge::Enum minInvalidity = FogMerge::Enum(std::min(firstMerge._merge, secondMerge._merge));
		
		if (minInvalidity == FogMerge::RIGHT_INVALID)
			maxInvalidity = FogMerge::BOTH_INVALID;
	}
	
	if (maxInvalidity == firstMerge._merge)
		return firstMerge;
		
	if (maxInvalidity == secondMerge._merge)
		return secondMerge;
		
	const FogMerge& theMerge = FogMerge::flyweight(maxInvalidity);
	
	if (theMerge.is_incompatible())
		FogMerge::set_incompatible(theMerge);
		
	return theMerge;
}

std::ostream& operator<<(std::ostream& s, const FogMerge& aMerge) {
	return s << FogMerge::values()[aMerge._merge];
}

std::ostream& operator<<(std::ostream& s, const FogMergeHandle& aMerge) {
	return s << aMerge._merge;
}

std::istream& operator>>(std::istream& s, FogMergeHandle& aMerge) {
	aMerge = FogMerge::flyweight(FogMerge::Enum(FogMerge::values()[s]));
	return s;
}
