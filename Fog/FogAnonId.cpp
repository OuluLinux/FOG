#include <Fog/FogIncludeAll.h>













PRIM_WITH_MAP(FogAnonId)

TYPEINFO_SINGLE(FogAnonId, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogAnonId)
PRIMREFS_IMPL(FogAnonId)
PRIMMAPOFREFS_IMPL(FogAnonId)
TMPL_HACK_FIX_DO(FogAnonId)

FogAnonId::FogAnonId()
	:
	_abstract(false)
{}

FogAnonId::FogAnonId(const PrimId& anId)
	:
	_id(anId),
	_abstract(true)
{}

FogAnonId::~FogAnonId() {}

//
//    Return the re-useable abstract singleton.
//
FogName& FogAnonId::abstract() {
	static const FogNameRef theId(make());
	return *theId;
}

FogToken* FogAnonId::clone() const {
	DIAMSG("INVESTIGATE - did not expect to invoke FogAnonId::clone()");
	share(); return (FogToken*)this;
}

bool FogAnonId::emit(FogEmitContext& emitContext) const {
	if (is_abstract())
		return false;
		
	emitContext.emit_scope();
	return emitContext.emit_identifier(anon_id());
}

FogAnonId* FogAnonId::is_anon() { return this; }
const PrimId* FogAnonId::is_resolved() const { return &anon_id(); }

//
//    Return another abstract id, which may later acquire a name.
//
FogName& FogAnonId::make() {
	static FogAnonIdMapOfRef identifierMap;
	static unsigned int count = 0;
	PrimOstrstream s;
	s << Fog::anon_prefix() << count++;
	size_t aSize = s.pcount();
	PrimIdHandle anId(s.str(), aSize);
	FogAnonId* anIdentifier = new FogAnonId(*anId);
	
	if (anIdentifier)
		identifierMap.adopt(anIdentifier);
		
	if (anIdentifier)
		return *anIdentifier;
	else
		return FogAnonId::mutable_null();
}

void FogAnonId::merge(FogMergeContext& mergeContext, const FogToken& aToken) {
	bool sameClass = &dynamic_type_info() == &aToken.dynamic_type_info();
	
	if (!sameClass) Super::merge(mergeContext, aToken);
}

const FogMetaType& FogAnonId::meta_type() const { return FogMetaType::keyword_type(); }

const FogMerge& FogAnonId::needs_merge(FogMergeContext& mergeContext, const FogToken& aToken) const {
	//   Right-hand side of algorithm in base class.
	bool sameClass = &dynamic_type_info() == &aToken.dynamic_type_info();
	
	if (!sameClass)
		return FogMerge::left_invalid();
		
	const FogAnonId& thatId = (const FogAnonId&)aToken;
	
	if (_explicit_id && thatId._explicit_id && (_explicit_id != thatId._explicit_id))
		return impossible_merge();
		
	return FogMerge::both_valid();
}

char FogAnonId::print_named(std::ostream& s, const PrimId* scopeId, char tailChar) const {
	if (is_abstract())
		return tailChar;
	else
		return FogStream::space_and_emit(s, tailChar, anon_id().str());
}

std::ostream& FogAnonId::print_viz(std::ostream& s) const {
	s << "\"anonymous-identifier: ";
	print_this(s);
	return s << '\"';
}

bool FogAnonId::resolve_id(PrimIdHandle& returnId, FogScopeContext& scopeContext) const {
	returnId = anon_id();
	return true;
}

bool FogAnonId::resolve_semantics(FogSemanticsContext& theSemantics) const {
	theSemantics.assign(FogSemantics::IS_EPSILON);
	return true;
}

void FogAnonId::set_concrete() {
	if (this == &abstract())
		ERRMSG("BUG -- should not set_concrete " << viz(*this));
	else
		_abstract = false;
}

void FogAnonId::set_id(const PrimId& anId) {
	if (this == &abstract())
		ERRMSG("BUG -- should not set_id of " << viz(*this));
	else if (is_named())
		ERRMSG("BUG -- should not set_id of " << viz(*this));
	else {
		_explicit_id = anId;
		_abstract = false;
	}
}

size_t FogAnonId::source_columns() const { return 0; }
