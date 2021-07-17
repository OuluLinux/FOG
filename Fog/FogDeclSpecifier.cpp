#include <Fog/FogIncludeAll.h>









TYPEINFO_SINGLE(FogDeclSpecifier, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogDeclSpecifier)
PRIMREFS_IMPL(FogDeclSpecifier)
TMPL_HACK_FIX_DO(FogDeclSpecifier)

const FogDeclSpecifier_Initializer FogDeclSpecifier_Flyweights::_invalid_init = {
	"/*invalid*/class",
	FogTokenType::init(), FogTokenType::init(), FogTokenType::init(),
	FogDeclSpecifier::INVALID_MASK
};

FogDeclSpecifier* FogDeclSpecifier_Flyweights::_explicit = &FogTokenType::get_decl_specifier(FogTokenType::EXPLICIT);
FogDeclSpecifier* FogDeclSpecifier_Flyweights::_ex_port = &FogTokenType::get_decl_specifier(FogTokenType::EXPORT);
FogDeclSpecifier* FogDeclSpecifier_Flyweights::_extern = &FogTokenType::get_decl_specifier(FogTokenType::EXTERN);
FogDeclSpecifier* FogDeclSpecifier_Flyweights::_friend = &FogTokenType::get_decl_specifier(FogTokenType::FRIEND);
FogDeclSpecifier* FogDeclSpecifier_Flyweights::_invalid = &FogDeclSpecifier_Flyweights::invalid();
FogDeclSpecifier* FogDeclSpecifier_Flyweights::_mutable = &FogTokenType::get_decl_specifier(FogTokenType::MUTABLE);
FogDeclSpecifier* FogDeclSpecifier_Flyweights::_register = &FogTokenType::get_decl_specifier(FogTokenType::REGISTER);
FogDeclSpecifier* FogDeclSpecifier_Flyweights::_template = &FogTokenType::get_decl_specifier(FogTokenType::TEMPLATE);
FogDeclSpecifier* FogDeclSpecifier_Flyweights::_typedef = &FogTokenType::get_decl_specifier(FogTokenType::TYPEDEF);
FogDeclSpecifier* FogDeclSpecifier_Flyweights::_using = &FogTokenType::get_decl_specifier(FogTokenType::USING);

FogDeclSpecifier& FogDeclSpecifier_Flyweights::invalid() {
	static const FogDeclSpecifierRef aDeclSpecifier(new FogDeclSpecifier(_invalid_init), FogDeclSpecifierRef::ADOPT);
	return *aDeclSpecifier;
}

FogDeclSpecifier::FogDeclSpecifier(const FogDeclSpecifier_Initializer& declSpecifier)
	:
	Super(declSpecifier._name, declSpecifier._pp_enum, declSpecifier._enum),
	_decl_specifier(declSpecifier._decl_specifier_mask) {}

FogDeclSpecifier::FogDeclSpecifier(const char* anId, Mask aMask)
	:
	Super(anId, FogTokenType::init(), FogTokenType::init()),
	_decl_specifier(aMask) {}

const FogDeclSpecifierValue& FogDeclSpecifier::decl_specifiers() const {
	return _decl_specifier;
}

FogDeclSpecifier* FogDeclSpecifier::is_decl_specifier() {
	return this;
}

const FogMetaType& FogDeclSpecifier::meta_type() const {
	return FogMetaType::decl_specifier_type();
}

bool FogDeclSpecifier::resolve_semantics(FogSemanticsContext& theSemantics) const {
	return _decl_specifier.resolve_semantics(theSemantics);
}
