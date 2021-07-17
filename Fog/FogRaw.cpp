#include <Fog/FogIncludeAll.h>











TYPEINFO_SINGLE(FogRaw, Super)
PRIMREF_NULL_CLAS(FogRaw)
typedef FogRaw Super;
FOGTOKEN_SHAREDLEAF_DECLS
};
PRIMREF_DERIVED_NULL_IMPL(FogRaw)
FOGTOKEN_SHAREDLEAF_IMPL(FogRawNull)
PRIMREFS_IMPL(FogRaw)
TMPL_HACK_FIX_DO(FogRaw)

FogRaw::FogRaw()  {}

FogRaw::FogRaw(const This& aRaw)
	:
	Super(aRaw),
	_line(aRaw._line)
{}

FogRaw::~FogRaw() {}

bool FogRaw::compile_declaration(FogParseContext& parseContext) const {
	FogRawRef actualToken;
	make_actual(actualToken.to_const(), parseContext);
	VERBOSE(std::strstream str; actualToken->print_viz(str); printf("DEBUG %s\n", str.str());)
	return actualToken->compile_parsed(parseContext);
}

const FogLazyBool& FogRaw::derive_at(FogScopeContext& inScope) const {
	return inScope.at_root() ? FogLazyBool::make_true() : FogLazyBool::make_false();
}

FogDerivedClause* FogRaw::is_derived_clause() { return 0; }
FogRaw* FogRaw::is_raw() { return this; }
FogStatement* FogRaw::is_statement() { return 0; }
FogLine& FogRaw::line() const { return *_line; }
const FogMetaType& FogRaw::meta_type() const { return FogMetaType::declaration_type(); }

std::ostream& FogRaw::print_viz(std::ostream& s) const {
	s << "\"raw : ";
	print_this(s);
	return s << '\"';
}

//
//  	Apply declSpecifier to the name, returning this, or a wrapper upon this to aRaw.
//
void FogRaw::set_decl_specifier_raw(FogRawRef& aRaw, FogDeclSpecifier& declSpecifiers) {
	ERRMSG("Should not set_decl_specifier_raw for " << viz(*this));
}

void FogRaw::set_explicit_instantiation() {
	ERRMSG("Should not declare an explicit-instantiation of " << viz(*this));
}

void FogRaw::set_line(FogLine& aLine) {
	if (is_null())
		ERRMSG("BUG -- should not set_line for  " << viz(*this));
	else
		_line = aLine;
}

size_t FogRaw::source_lines() const { return 1; }
