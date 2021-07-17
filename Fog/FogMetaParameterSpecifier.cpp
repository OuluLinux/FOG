#include <Fog/FogIncludeAll.h>







TYPEINFO_SINGLE(FogMetaParameterSpecifier, Super)
PRIMREF_NULL_CLAS(FogMetaParameterSpecifier)
FOGTOKEN_NULL_LEAF
};
PRIMREF_DERIVED_NULL_IMPL(FogMetaParameterSpecifier)
PRIMREFS_IMPL(FogMetaParameterSpecifier)
FOGTOKEN_UNIQUE_IMPL(FogMetaParameterSpecifier)
TMPL_HACK_FIX_DO(FogMetaParameterSpecifier)

FogMetaParameterSpecifier::FogMetaParameterSpecifier() {}

FogMetaParameterSpecifier::FogMetaParameterSpecifier(FogMakeSpecifierContext& makeSpecifierContext, FogName& nameName)
	:
	Super(makeSpecifierContext, nameName)
{}

FogMetaParameterSpecifier::~FogMetaParameterSpecifier() {}
FogMetaParameterSpecifier* FogMetaParameterSpecifier::is_meta_parameter_specifier() { return this; }

bool FogMetaParameterSpecifier::make_slot(FogMakerContext& makerContext) const {
	ERRMSG("Sould not make_slot for " << viz(*this));
	return false;
}

const FogMetaType& FogMetaParameterSpecifier::meta_type() const { return FogMetaType::meta_parameter_specifier_type(); }

const FogMetaArgument* FogMetaParameterSpecifier::new_parameter(FogScopeContext& scopeContext, size_t anIndex) const {
	const FogMetaType& metaType = resolve_type_meta_type(scopeContext);
	PrimIdHandle anId;
	
	if (!resolve_id(anId, scopeContext))
		return 0;
		
	return new FogMetaArgument(anIndex, metaType, is_exposed(), *anId, _initializer.pointer());
}

std::ostream& FogMetaParameterSpecifier::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	_initializer->print_deep(s, aDepth);
	return s;
}

std::ostream& FogMetaParameterSpecifier::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	_initializer->print_on(s, aDepth);
	return s;
}

std::ostream& FogMetaParameterSpecifier::print_viz(std::ostream& s) const {
	s << "\"meta-parameter-specifier: ";
	print_this(s);
	return s << '\"';
}

void FogMetaParameterSpecifier::set_initializer(const FogExpr& anExpr) { _initializer = anExpr; }
