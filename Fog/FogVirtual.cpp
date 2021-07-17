#include <Fog/FogIncludeAll.h>








TYPEINFO_SINGLE(FogVirtual, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogVirtual)
PRIMREFS_IMPL(FogVirtual)
TMPL_HACK_FIX_DO(FogVirtual)

FogVirtual* FogVirtual_Flyweights::_flyweights[FogVirtualEnums::ARRAY_SIZE] = { 0 };
bool FogVirtual_Flyweights::_initialised = initialise();

struct FogVirtual_Flyweights_Initializer {
	const char* _name;
	FogDeclSpecifier::Mask _decl_specifier_enum;
	FogVirtualMask::Enum _virtual_enum;
};

bool FogVirtual_Flyweights::initialise() {
	static const FogVirtual_Flyweights_Initializer flyweightInits[] = {
		{ "", FogDeclSpecifier::ZERO, FogVirtual::NONVIRTUAL },
		{ "/*pure(virtual)*/", FogDeclSpecifier::PURE_MASK, FogVirtual::PURE_NONVIRTUAL },
		{ "/*pure*/virtual", FogDeclSpecifier::PURE_VIRTUAL_MASK, FogVirtual::PURE_VIRTUAL },
		{ "/*!virtual*/", FogDeclSpecifier::NOT_VIRTUAL_MASK, FogVirtual::NOT_VIRTUAL },
		{ "/*!virtual*/ virtual", FogDeclSpecifier::INVALID_MASK, FogVirtual::Enum(FogVirtual::NOT_VIRTUAL | FogVirtual::VIRTUAL) },
		{ "/*!virtual pure(virtual)*/", FogDeclSpecifier::INVALID_MASK, FogVirtual::Enum(FogVirtual::NOT_VIRTUAL | FogVirtual::PURE_NONVIRTUAL) },
		{ "/*INVALID-virtual*/", FogDeclSpecifier::INVALID_MASK, FogVirtual::INVALID },
		{ 0, FogDeclSpecifier::INVALID_MASK, FogVirtual::INVALID }
	};
	static FogVirtualRef flyWeights[FogVirtualEnums::ARRAY_SIZE];
	
	for (const FogVirtual_Flyweights_Initializer* p = flyweightInits; p->_name; ++p) {
		flyWeights[p->_virtual_enum].adopt(new FogVirtual(p->_name, p->_decl_specifier_enum, p->_virtual_enum));
		_flyweights[p->_virtual_enum] = flyWeights[p->_virtual_enum].pointer();
	}
	
	_flyweights[FogVirtual::VIRTUAL] = &FogTokenType::get_virtual(FogTokenType::VIRTUAL);
	return true;
}

FogVirtual::FogVirtual(const char* aName, Mask aMask, Enum anEnum)
	:
	Super(aName, aMask),
	_virtual(anEnum)
{}

FogVirtual::FogVirtual(const FogVirtual_Initializer& anInit)
	:
	Super(anInit._decl_specifier),
	_virtual(anInit._virtual_enum)
{}

FogVirtual& FogVirtual::flyweight(const FogDeclSpecifierValue& aValue) {
	FogVirtual* flyWeight = 0;
	
	if (aValue.is_pure_virtual())
		flyWeight = &pure_virtual();
		
	if (aValue.is_virtual())
		flyWeight = flyWeight ? &invalid_virtual() : &yes_virtual();
		
	if (aValue.is_not_virtual())
		flyWeight = flyWeight ? &invalid_virtual() : &not_virtual();
		
	if (!flyWeight)
		flyWeight = &invalid_virtual();
		
	return *flyWeight;
}

const FogVirtual& FogVirtual::merge_virtual(const FogVirtual& aVirtual) const {
	const FogVirtualMask mergedVirtual = _virtual | aVirtual._virtual;
	
	if ((mergedVirtual.is_virtual() || mergedVirtual.is_pure()) && mergedVirtual.is_not_virtual())
		ERRMSG("BUG - should not merge incompatible virtuals \"" << *this << "\" and \"" << aVirtual << '\"');
		
	return flyweight(mergedVirtual.value());
}

const FogMerge& FogVirtual::needs_merge_virtual(const FogVirtual& aVirtual) const {
	if (_virtual == aVirtual._virtual)
		return FogMerge::both_valid();
		
	const FogVirtualMask mergedVirtual = _virtual | aVirtual._virtual;
	
	if (_virtual == mergedVirtual)
		return FogMerge::right_invalid();
		
	if (aVirtual._virtual == mergedVirtual)
		return FogMerge::left_invalid();
		
	if ((mergedVirtual.is_virtual() || mergedVirtual.is_pure()) && mergedVirtual.is_not_virtual())
		return FogMerge::incompatible_virtual();
		
	return FogMerge::both_invalid();
}
