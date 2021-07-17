#include <Fog/FogIncludeAll.h>






TYPEINFO_SINGLE(FogKeyword, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogKeyword)
PRIMREFS_IMPL(FogKeyword)
PRIMLISTOFREFS_IMPL(FogKeyword)
PRIMMAPOFREFS_IMPL(FogKeyword)
FOGTOKEN_SHAREDLEAF_IMPL(FogKeyword)
TMPL_HACK_FIX_DO(FogKeyword)

const PrimId* FogKeyword::is_resolved() const { return _id.pointer(); }
const PrimId* FogKeyword::is_word() const { return _id.pointer(); }
const FogMetaType& FogKeyword::meta_type() const { return FogMetaType::keyword_type(); }

char FogKeyword::print_named(std::ostream& s, const PrimId* scopeId, char tailChar) const {
	if (scopeId) {
		const char* p = scopeId->str();
		tailChar = FogStream::space_and_emit(s, tailChar, p);
		
		if (isalnum(tailChar) && p && !*p)
			tailChar = FogStream::space_and_emit(s, tailChar, " ");
			
		tailChar = FogStream::space_and_emit(s, tailChar, "::");
	}
	
	return FogStream::space_and_emit(s, tailChar, _id->str());
}

bool FogKeyword::resolve_id(PrimIdHandle& returnId, FogScopeContext& scopeContext) const
{ returnId = _id; return true; }
size_t FogKeyword::source_columns() const { return _id->length(); }
