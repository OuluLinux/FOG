#include <Fog/FogIncludeAll.h>









const FogElementType FogElementType_Flyweights::_flyWeights[FogElementType::WIDE_STRING + 1] = {
	FogElementType::DEFAULT,
	FogElementType::CHARACTER,
	FogElementType::WIDE_CHARACTER,
	FogElementType::IDENTIFIER,
	FogElementType::NUMBER,
	FogElementType::STRING,
	FogElementType::WIDE_STRING
};

static const char* const FogElementType_texts[] = {
	"unclassified", "character", "wide-character", "identifier", "number", "string", "wide-string"
};

const FogElementType& FogElementType::merge(const FogElementType& elementType) const {
	if (_element_type != elementType._element_type)
		ERRMSG("BUG - should not merge incompatible element-types \"" << *this
		       << "\" and \"" << elementType << '\"');
		       
	return *this;
}

const FogMetaType& FogElementType::meta_type() const {
	switch (_element_type) {
	case CHARACTER:
	case WIDE_CHARACTER: return FogMetaType::character_type();
	
	case STRING:
	case WIDE_STRING: return FogMetaType::string_type();
	
	case NUMBER: return FogMetaType::number_type();
	
	case IDENTIFIER: return FogMetaType::identifier_type();
	
	default: return FogMetaType::immutable_null();
	}
}

const FogMerge& FogElementType::needs_merge(const FogElementType& elementType) const {
	if (_element_type == elementType._element_type)
		return FogMerge::both_valid();
		
	return FogMerge::incompatible_element_type();
}

FogTokenType::TokenType FogElementType::token_type() const {
	switch (_element_type) {
	case CHARACTER:
	case WIDE_CHARACTER: return FogTokenType::CharacterLiteral;
	
	case STRING:
	case WIDE_STRING: return FogTokenType::StringLiteral;
	
	case NUMBER: return FogTokenType::NumberLiteral;
	
	case IDENTIFIER: return FogTokenType::Identifier;
	
	default: return FogTokenType::ERROR;
	}
}

const PrimEnum& FogElementType::values() {
	static const PrimEnum theValues("FogElementType", FogElementType_texts,
	                                PRIM_ARRAY_SIZE(FogElementType_texts));
	return theValues;
}

std::ostream& operator<<(std::ostream& s, const FogElementType& elementType) {
	return s << FogElementType::values()[elementType._element_type];
}
