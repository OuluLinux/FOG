#include <Fog/FogIncludeAll.h>


















TYPEINFO_SINGLE(FogMetaSlot, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogMetaSlot)
PRIMREFS_IMPL(FogMetaSlot)
PRIMMAPOFREFS_IMPL(FogMetaSlot)
FOGTOKEN_ACTUAL_IMPL(FogMetaSlot)
TMPL_HACK_FIX_DO(FogMetaSlot)

FogMetaSlot::FogMetaSlot()
	:
	_static_meta_scope(FogMetaType::mutable_null()),
	_type(FogMetaType::immutable_null()),
	_is_exposed(IS_ENCAPSULATED),
	_is_static(IS_STATIC)
{}

FogMetaSlot::FogMetaSlot(FogMakerContext& makerContext,
                         const FogStatic& aStatic, const FogMetaType& metaType, IsExposed isExposed)
	:
	_static_meta_scope(makerContext.dynamic_token()),
	_id(makerContext.short_id()),
	_type(metaType),
	_is_exposed(isExposed),
	_is_static(aStatic.is_static() ? IS_STATIC : IS_NOT_STATIC)
{}

FogMetaSlot::FogMetaSlot(FogToken& staticMetaScope,
                         IsStatic isStatic, const FogMetaType& metaType, IsExposed isExposed, const PrimId& anId)
	:
	_static_meta_scope(staticMetaScope),
	_id(anId),
	_type(metaType),
	_is_exposed(isExposed),
	_is_static(isStatic)
{}

FogMetaSlot::FogMetaSlot(FogToken& staticMetaScope,
                         IsStatic isStatic, const FogMetaType& metaType, IsExposed isExposed, const char* anId)
	:
	_static_meta_scope(staticMetaScope),
	_id(anId),
	_type(metaType),
	_is_exposed(isExposed),
	_is_static(isStatic)
{}

FogMetaSlot::~FogMetaSlot() {}

const PrimId& FogMetaSlot::constructor_id() {
	static const PrimIdHandle anId("meta-constructor");
	return *anId;
}

const PrimId& FogMetaSlot::destructor_id() {
	static const PrimIdHandle anId("meta-destructor");
	return *anId;
}

void FogMetaSlot::find_entities(FogEntityFinder& theFinder) {
	ERRMSG("INVESTIGATE use of FogMetaSlot::find_entities() for " << viz(*this));
	_static_meta_scope.find_entities(theFinder);
}

bool FogMetaSlot::find_slots(FogMetaSlotFinder& theFinder) {
	ERRMSG("INVESTIGATE use of FogMetaSlot::find_slots() for " << viz(*this));
	const FogMetaSlot* aSlot = find_local_slot(theFinder.id());                 //   Search this class
	
	if (!aSlot)
		return _static_meta_scope.find_slots(theFinder);
		
	theFinder.add(*this, *aSlot);
	return true;
}

FogToken* FogMetaSlot::get_scope_in(FogScopeContext& inScope) const {
	if (!is_static())
		return &inScope.dynamic_token();
		
	const FogToken& staticScope = static_meta_scope();
	const FogMetaType* metaType = staticScope.is_meta_type();
	
	if (metaType)
		return (FogMetaType*)metaType;                                      //  .bugbug cast
		
	FogInScopeContext inAnyContext(inScope, FogScopeContext::IN_ANY_SCOPE);
	return staticScope.get_entity_in(inAnyContext, FIND_SCOPE);
}

FogScope& FogMetaSlot::inner_scope() {
	//   Happens for inner $ { $ {Class} B::count } in arithmetic (maybe OPTIONAL scopeContext could suppress).
	//    ERRMSG("INVESTIGATE use of FogMetaSlot::inner_scope() for " << viz(*this));
	FogScope* aScope = _static_meta_scope.is_scope();
	
	if (aScope) {
		FogScope* mutableScope = aScope;
		return *mutableScope;
	}
	
	return Super::inner_scope();
}

const FogMetaArgument* FogMetaSlot::is_meta_argument() const { return 0; }
const FogMetaFunction* FogMetaSlot::is_meta_function() const { return 0; }
const FogMetaSlot* FogMetaSlot::is_meta_slot() const { return this; }

std::ostream& FogMetaSlot::print_long_id(std::ostream& s) const {
	_static_meta_scope.print_long_id(s);
	return s << "::" << id();
}

std::ostream& FogMetaSlot::print_this(std::ostream& s) const {
	if (_is_static)
		s << "static ";
		
	_type.print_long_id(s);
	
	if (_is_exposed)
		s << "[]";
		
	s << " ";
	return print_long_id(s);
}

std::ostream& FogMetaSlot::print_viz(std::ostream& s) const {
	s << "\"meta-slot: ";
	print_long_id(s);
	return s << '\"';
}

bool FogMetaSlot::put_value(FogScopeContext& putScope, const FogToken& tokenValue, FogScopeContext& getScope) const {
	ERRMSG("Should not attempt to put_value for " << viz(*this));
	return false;
}

//
//    Resolve a meta-object by applying it inScope to determine returnValue.
//
//    The return value is the left-value of the tree that represents the  resolved slot. For many
//    slots this is a FogMetaEntity, nut for built-in slots it may be a FogEntity.
//
bool FogMetaSlot::resolve_object(FogTokenRef& returnValue, FogScopeContext& inScope) const {
	ERRMSG("Should not attempt to resolve_object for " << viz(*this));
	returnValue = FogFailure::make();
	return false;
}
