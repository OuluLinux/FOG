#include <Fog/FogIncludeAll.h>








TYPEINFO_SINGLE(FogSpacing, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogSpacing)
PRIMREFS_IMPL(FogSpacing)
PRIMLISTOFREFS_IMPL(FogSpacing)
FOGTOKEN_SHAREDLEAF_IMPL(FogSpacing)
TMPL_HACK_FIX_DO(FogSpacing)

bool FogSpacing::emit(FogEmitContext& emitContext) const {
	if (_spaces > 0)
		emitContext.emit_spaces(_spaces);
		
	return spaces() > 0;
}

size_t FogSpacing::executable_tokens() const { return 0; }
const FogSpacing* FogSpacing::is_spacing() const { return this; }

FogSpacing& FogSpacing::make_spacing(int someSpaces) {
	static FogSpacingListOfRef plusTable(32);
	static FogSpacingListOfRef minusTable(8);
	FogSpacingListOfRef& tokenTable = someSpaces >= 0 ? plusTable : minusTable;
	size_t theSpacing = someSpaces >= 0 ? someSpaces : -someSpaces;
	
	if (tokenTable.tally() <= theSpacing)
		tokenTable.set_tally(theSpacing + 16);
		
	if (theSpacing >= tokenTable.capacity())
		return FogSpacing::mutable_null();
		
	if (!tokenTable[theSpacing])
		tokenTable[theSpacing].adopt(new FogSpacing(someSpaces));
		
	if (tokenTable[theSpacing])
		return *tokenTable[theSpacing];
	else
		return FogSpacing::mutable_null();
}

FogTokenType::TokenType FogSpacing::pp_token_type_enum() const { return FogTokenType::character(' '); }

std::ostream& FogSpacing::print_diagnostic(std::ostream& s, int aDepth) const {
	return s << "Spacing " << _spaces;
}

char FogSpacing::print_named(std::ostream& s, const PrimId* scopeId, char tailChar) const {
	return FogStream::space_and_emit(s, tailChar, " ");
}

std::ostream& FogSpacing::print_source(std::ostream& s, int aDepth) const {
	return s << (_spaces ? " " : "");
}

std::ostream& FogSpacing::print_stringized(std::ostream& s) const {
	return s << (_spaces ? " " : "");
}

std::ostream& FogSpacing::print_this(std::ostream& s) const { return s << _spaces; }

std::ostream& FogSpacing::print_tokens(std::ostream& s, int aDepth) const {
	return s << "Spacing " << _spaces;
}

size_t FogSpacing::source_columns() const { return _spaces; }
FogTokenType::TokenType FogSpacing::token_type_enum() const { return FogTokenType::character(' '); }
