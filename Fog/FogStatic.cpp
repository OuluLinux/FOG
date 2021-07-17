#include <Fog/FogIncludeAll.h>







TYPEINFO_SINGLE(FogStatic, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogStatic)
PRIMREFS_IMPL(FogStatic)
TMPL_HACK_FIX_DO(FogStatic)

FogStatic* FogStatic_Flyweights::_flyweights[FogStaticEnums::ARRAY_SIZE] = { 0 };
bool FogStatic_Flyweights::_initialised = initialise();

struct FogStatic_Flyweights_Initializer {
	const char* _name;
	FogDeclSpecifier::Mask _decl_specifier_enum;
	FogStaticMask::Enum _static_enum;
};

bool FogStatic_Flyweights::initialise() {
	static const FogStatic_Flyweights_Initializer flyweightInits[] = {
		{ "", FogDeclSpecifier::ZERO, FogStatic::DEFAULT },
		{ "/*!static*/", FogDeclSpecifier::NOT_STATIC_MASK, FogStatic::NOT_STATIC },
		{ "/*INVALID-static*/", FogDeclSpecifier::INVALID_MASK, FogStatic::INVALID },
		{ 0, FogDeclSpecifier::INVALID_MASK, FogStatic::INVALID }
	};
	static FogStaticRef flyWeights[FogStaticEnums::ARRAY_SIZE];
	
	for (const FogStatic_Flyweights_Initializer* p = flyweightInits; p->_name; ++p) {
		flyWeights[p->_static_enum].adopt(new FogStatic(p->_name, p->_decl_specifier_enum, p->_static_enum));
		_flyweights[p->_static_enum] = flyWeights[p->_static_enum].pointer();
	}
	
	_flyweights[FogStatic::STATIC] = &FogTokenType::get_static(FogTokenType::STATIC);
	return true;
}

FogStatic::FogStatic(const char* aName, Mask aMask, Enum anEnum)
	:
	Super(aName, aMask),
	_static(anEnum)
{}

FogStatic::FogStatic(const FogStatic_Initializer& anInit)
	:
	Super(anInit._decl_specifier),
	_static(anInit._static_enum)
{}

FogStatic& FogStatic::flyweight(const FogDeclSpecifierValue& aValue) {
	FogStatic* flyWeight = 0;
	
	if (aValue.is_static())
		flyWeight = &yes_static();
		
	if (aValue.is_not_static())
		flyWeight = flyWeight ? &invalid_static() : &not_static();
		
	if (!flyWeight)
		flyWeight = &invalid_static();
		
	return *flyWeight;
}

const FogStatic& FogStatic::merge_static(const FogStatic& aStatic) const {
	FogStaticMask mergedStatic = _static | aStatic._static;
	
	if (mergedStatic.value() >= INVALID)
		ERRMSG("BUG - should not merge incompatible statics \"" << *this << "\" and \"" << aStatic << '\"');
		
	return flyweight(mergedStatic.value());
}

const FogMerge& FogStatic::needs_merge_static(const FogStatic& aStatic) const {
	if (_static == aStatic._static)
		return FogMerge::both_valid();
		
	FogStaticMask mergedStatic = _static | aStatic._static;
	
	if (_static == mergedStatic)
		return FogMerge::right_invalid();
		
	if (aStatic._static == mergedStatic)
		return FogMerge::left_invalid();
		
	if (mergedStatic.value() < INVALID)
		return FogMerge::both_invalid();
		
	return FogMerge::incompatible_static();
}

