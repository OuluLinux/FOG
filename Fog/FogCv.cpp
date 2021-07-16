#include <Fog/FogIncludeAll.h>








TYPEINFO_SINGLE(FogCv, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogCv)
PRIMREFS_IMPL(FogCv)
TMPL_HACK_FIX_DO(FogCv)

FogCv *FogCv_Flyweights::_flyweights[FogCvEnums::ARRAY_SIZE] = { 0 };
bool FogCv_Flyweights::_initialised = initialise();

struct FogCv_Flyweights_Initializer {
	const char *_name;
	FogDeclSpecifier::Mask _decl_specifier_enum;
	FogCvMask::Enum _cv_enum;
};

bool FogCv_Flyweights::initialise() {
	static const FogCv_Flyweights_Initializer flyweightInits[] = {
		{ "?const ?volatile", FogDeclSpecifier::ZERO, FogCv::NONE },
		{ "!const ?volatile", FogDeclSpecifier::NOT_CONST_MASK, FogCv::NOT_CONST },
		{ "bad-const ?volatile", FogDeclSpecifier::INVALID_MASK, FogCv::Enum(3) },
		{ "const volatile", FogDeclSpecifier::INVALID_MASK, FogCv::CONST_VOLATILE },
		{ "!const volatile", FogDeclSpecifier::INVALID_MASK, FogCv::NOT_CONST_VOLATILE },
		{ "bad-const volatile", FogDeclSpecifier::INVALID_MASK, FogCv::Enum(7) },
		{ "?const !volatile", FogDeclSpecifier::NOT_VOLATILE_MASK, FogCv::NOT_VOLATILE },
		{ "const !volatile", FogDeclSpecifier::INVALID_MASK, FogCv::CONST_NOT_VOLATILE },
		{ "!const !volatile", FogDeclSpecifier::INVALID_MASK, FogCv::NOT_CONST_NOT_VOLATILE },
		{ "bad-const !volatile", FogDeclSpecifier::INVALID_MASK, FogCv::Enum(11) },
		{ "?const bad-volatile", FogDeclSpecifier::INVALID_MASK, FogCv::Enum(12) },
		{ "const bad-volatile", FogDeclSpecifier::INVALID_MASK, FogCv::Enum(13) },
		{ "!const bad-volatile", FogDeclSpecifier::INVALID_MASK, FogCv::Enum(14) },
		{ "bad-const bad-volatile", FogDeclSpecifier::INVALID_MASK, FogCv::INVALID },
		{ 0, FogDeclSpecifier::INVALID_MASK, FogCv::INVALID }
	};
	static FogCvRef flyWeights[FogCvEnums::ARRAY_SIZE];
	
	for (const FogCv_Flyweights_Initializer *p = flyweightInits; p->_name; ++p) {
		flyWeights[p->_cv_enum].adopt(new FogCv(p->_name, p->_decl_specifier_enum, p->_cv_enum));
		_flyweights[p->_cv_enum] = flyWeights[p->_cv_enum].pointer();
	}
	
	_flyweights[FogCv::CONST] = &FogTokenType::get_cv(FogTokenType::CONST);
	
	_flyweights[FogCv::VOLATILE] = &FogTokenType::get_cv(FogTokenType::VOLATILE);
	return true;
}

FogCv::FogCv(const char *aName, Mask aMask, Enum anEnum)
		:
		Super(aName, aMask),
		_cv(anEnum) {}
		
FogCv::FogCv(const FogCv_Initializer& anInit)
		:
		Super(anInit._decl_specifier),
		_cv(anInit._cv_enum) {}
		
const FogCv& FogCv::compile() const {
	FogCvMask compiledCv = _cv;
	
	if (!is_const())
		compiledCv |= NOT_CONST;
		
	if (!is_volatile())
		compiledCv |= NOT_VOLATILE;
		
	return flyweight(compiledCv.value());
}

FogCv::Enum FogCv::merge_cv(const FogCv& aCv) const {
	FogCvMask mergedCv = _cv | aCv._cv;
	
	if ((mergedCv.is_const() && mergedCv.is_not_const())
		|| (mergedCv.is_volatile() && mergedCv.is_not_volatile()))
		ERRMSG("Should not merge incompatible \"cv-qualifier: " << *this
			   << "\" and \"cv-qualifier: " << aCv << '\"');
		       
	return mergedCv.value();
}

FogCv::Enum FogCv::merge_cv(Enum aCv) const {
	FogCvMask mergedCv = _cv | aCv;
	
	if ((mergedCv.is_const() && mergedCv.is_not_const())
		|| (mergedCv.is_volatile() && mergedCv.is_not_volatile()))
		ERRMSG("BUG - should not merge incompatible \"cv-qualifier: "
			   << *this << "\" and \"cv-qualifier: " << aCv << '\"');
		       
	return mergedCv.value();
}

const FogMetaType& FogCv::meta_type() const {
	return FogMetaType::cv_qualifier_type();
}

const FogMerge& FogCv::needs_merge_cv(const FogCv& aCv) const {
	if (_cv == aCv._cv)
		return FogMerge::both_valid();
		
	FogCvMask mergedCv = _cv | aCv._cv;
	
	if (_cv == mergedCv)
		return FogMerge::right_invalid();
		
	if (aCv.mask() == mergedCv)
		return FogMerge::left_invalid();
		
	if (mergedCv.is_const() && mergedCv.is_not_const())
		return FogMerge::incompatible_const();
		
	if (mergedCv.is_volatile() && mergedCv.is_not_volatile())
		return FogMerge::incompatible_volatile();
		
	return FogMerge::both_invalid();
}

//  bool FogCv::resolve_semantics(FogSemantics& theSemantics, FogScopeContext& inScope) const
//  {
//  	if (is_cv())
//  		theSemantics.assign(FogSemantics::IS_TYPE_SPECIFIER);
//  	else
//  		theSemantics.reset();
//  	return true;
//  }
