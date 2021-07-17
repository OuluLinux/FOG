#include <Fog/FogIncludeAll.h>







TYPEINFO_SINGLE(FogAccess, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogAccess)
PRIMREFS_IMPL(FogAccess)
TMPL_HACK_FIX_DO(FogAccess)

FogAccess* FogAccess_Flyweights::_flyweights[FogAccessEnums::ARRAY_SIZE] = { 0 };
bool FogAccess_Flyweights::_initialised = initialise();

struct FogAccess_Flyweights_Initializer {
	const char* _name;
	FogDeclSpecifier::Mask _decl_specifier_enum;
	FogAccessValue::Enum _access_enum;
};

bool FogAccess_Flyweights::initialise() {
	static const FogAccess_Flyweights_Initializer flyweightInits[] = {
		{ "/*INITIAL-access*/", FogDeclSpecifier::ZERO, FogAccess::INITIAL },
		{ "/*INVALID-access*/", FogDeclSpecifier::INVALID_ACCESS_MASK, FogAccess::INVALID },
		{ 0, FogDeclSpecifier::INVALID_MASK, FogAccess::INVALID }
	};
	static FogAccessRef flyWeights[FogAccessEnums::ARRAY_SIZE];
	
	for (const FogAccess_Flyweights_Initializer* p = flyweightInits; p->_name; ++p) {
		flyWeights[p->_access_enum].adopt(new FogAccess(p->_name, p->_decl_specifier_enum, p->_access_enum));
		_flyweights[p->_access_enum] = flyWeights[p->_access_enum].pointer();
	}
	
	_flyweights[FogAccess::AUTO] = &FogTokenType::get_access(FogTokenType::AUTO);
	_flyweights[FogAccess::PRIVATE] = &FogTokenType::get_access(FogTokenType::PRIVATE);
	_flyweights[FogAccess::PROTECTED] = &FogTokenType::get_access(FogTokenType::PROTECTED);
	_flyweights[FogAccess::PUBLIC] = &FogTokenType::get_access(FogTokenType::PUBLIC);
	return true;
}

FogAccess::FogAccess(const char* aName, Mask aMask, Enum anEnum)
	:
	Super(aName, aMask),
	_access(anEnum) {}

FogAccess::FogAccess(const FogAccess_Initializer& anInit)
	:
	Super(anInit._decl_specifier),
	_access(anInit._access_enum) {}

FogAccess& FogAccess::flyweight(const FogDeclSpecifierValue& aValue) {
	FogAccess* flyWeight = 0;
	
	if (aValue.is_private())
		flyWeight = &private_access();
		
	if (aValue.is_protected())
		flyWeight = flyWeight ? &invalid_access() : &protected_access();
		
	if (aValue.is_public())
		flyWeight = flyWeight ? &invalid_access() : &public_access();
		
	if (aValue.is_auto())
		flyWeight = flyWeight ? &invalid_access() : &auto_access();
		
	if (!flyWeight)
		flyWeight = &invalid_access();
		
	return *flyWeight;
}

const FogAccess& FogAccess::merge_access(const FogAccess& anAccess) const {
	if (!anAccess.is_valid() || (value() == anAccess.value()))
		return *this;
		
	if (!is_valid())
		return anAccess;
		
	ERRMSG("Should not merge incompatible accesses \"" << *this << "\" and \"" << anAccess << '\"');
	return *this;
}

const FogMerge& FogAccess::needs_merge_access(const FogAccess& anAccess) const {
	if (value() == anAccess.value())
		return FogMerge::both_valid();
		
	if (!anAccess.is_valid())
		return FogMerge::right_invalid();
		
	if (!is_valid())
		return FogMerge::left_invalid();
		
	return FogMerge::incompatible_access();
}
