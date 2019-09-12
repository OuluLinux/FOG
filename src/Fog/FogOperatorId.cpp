#include <Fog/FogIncludeAll.h>




PRIM_WITH_MAP(FogOperatorId)

TYPEINFO_SINGLE(FogOperatorId, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogOperatorId)
PRIMREFS_IMPL(FogOperatorId)
PRIMMAPOFREFS_IMPL(FogOperatorId)
FOGTOKEN_SHAREDLEAF_IMPL(FogOperatorId)
TMPL_HACK_FIX_DO(FogOperatorId)

FogOperatorId::FogOperatorId() {}

FogOperatorId::FogOperatorId(const PrimId& anId)
: 
 _id(anId)
{}

FogOperatorId::~FogOperatorId() {}
bool FogOperatorId::is_operator() const { return true; }
const PrimId *FogOperatorId::is_resolved() const { return _id.pointer(); }

FogOperatorId& FogOperatorId::make(const PrimId& anId)
{
 static FogOperatorIdMapOfRef identifierMap;
 FogOperatorId *anIdentifier = identifierMap.find(anId);
 if (!anIdentifier)
 {
  anIdentifier = new FogOperatorId(anId);
  if (anIdentifier)
   identifierMap.adopt(anIdentifier);
 }
 if (anIdentifier)
  return *anIdentifier;
 else
  return FogOperatorId::mutable_null();
}

const FogMetaType& FogOperatorId::meta_type() const { return FogMetaType::keyword_type(); }

char FogOperatorId::print_named(std::ostream& s, const PrimId *scopeId, char tailChar) const
{
 if (scopeId)
 {
  const char *p = scopeId->str();
  tailChar = FogStream::space_and_emit(s, tailChar, p);
  if (isalnum(tailChar) && p && !*p)
   tailChar = FogStream::space_and_emit(s, tailChar, " ");
  tailChar = FogStream::space_and_emit(s, tailChar, "::");  
 }
 return FogStream::space_and_emit(s, tailChar, _id->str());
}

std::ostream& FogOperatorId::print_viz(std::ostream& s) const
{
 return s << "\"operator-function-id: " << id() << '\"';
}
bool FogOperatorId::resolve_id(PrimIdHandle& returnId, FogScopeContext& scopeContext) const
 { returnId = _id; return true; }

bool FogOperatorId::resolve_semantics(FogSemanticsContext& theSemantics) const
{
 theSemantics.assign(FogSemantics::IS_OPERATOR_FUNCTION_ID);
 return true;
}

FogToken::NameSort FogOperatorId::sort_class() const { return OPERATOR_NAME; }
size_t FogOperatorId::source_columns() const { return _id->length(); }
