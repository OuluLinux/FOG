#include <Fog/FogIncludeAll.h>
























TYPEINFO_SINGLE(FogName, Super)
PRIMREF_NULL_CLAS(FogName)
FOGTOKEN_NULL_LEAF
virtual bool emit(FogEmitContext& emitContext) const { return false; }
virtual bool resolve_id(PrimIdHandle& returnId, FogScopeContext& inScope) const
{ returnId = PrimId::null(); return true; }
};
PRIMREF_DERIVED_NULL_IMPL(FogName)
PRIMREFS_IMPL(FogName)
TMPL_HACK_FIX_DO(FogName)

const FogDeclSpecifierValue& FogName::decl_specifiers() const {
	static const FogDeclSpecifierValue defaultDeclSpecifiers;
	return defaultDeclSpecifiers;
}

//
//    Contribute to the deduction of the equivalence tree for a template parameter by updating deducedType to reflect
//    the compatiblity of this formal type name and actualTypeName to deducedType, returniung the template
//    parameter and its equivalence type to deducedType.
//
const FogTemplateParameterSpecifierId* FogName::deduce_template_parameter_type(FogName& actualTypeName,
        FogNameRef& deducedType) const {
	ERRMSG("BUG - Should not invoke FogName::deduce_template_parameter_type for " << viz(*this));
	return 0;
}

bool FogName::emit(FogEmitContext& emitContext) const { return Super::emit(emitContext); }

bool FogName::find_meta_entity_in(FogTokenRef& returnValue, FogScopeContext& inScope) const {
	//    FogInScopeContext inScopeContext(inScope, IN_ANY_SCOPE);
	//    ERRMSG("BUG - should not invoke FogToken::find_meta_entity_in for " << viz(*this));
	const FogMetaSlot* aSlot = find_slot_in(inScope);
	
	if (!aSlot || aSlot->is_null() || !aSlot->resolve_object(returnValue, inScope))
		return false;
		
	return true;
}

//
//    Add the slot matching this name inScope to theFinding.
//
const FogMetaSlot* FogName::find_slot_in(FogScopeContext& scopeContext) const {
	ERRMSG("BUG - Should not invoke FogName::find_slot_in for " << viz(*this));
	return 0;
}

//
//    Add all slots matching this name inScope to theFinding.
//
bool FogName::find_slots_in(FogScopeContext& inScope, FogMetaSlotFinding& theFinding) const {
	ERRMSG("BUG - Should not invoke FogName::find_slots_in for " << viz(*this));
	return false;
}

FogEntity* FogName::get_entity_in(FogScopeContext& inScope, FindStrategy aStrategy) const {
	FogEntity* anEntity = Super::get_entity_in(inScope, aStrategy);
	
	if (!anEntity) {
		switch (aStrategy) {
		case FIND_SCOPE:
		case FIND_TYPE: {
			ERRMSG("Failed to locate type " << viz(*this) << " in " << viz(inScope));
			FogStaticMakeEntityContext makeEntityContext(inScope);
			anEntity = make_type_entity(makeEntityContext);
			
			if (anEntity)
				inScope.dynamic_scope().add_type(*anEntity, *anEntity);
				
			anEntity = find_type_in(inScope);
			break;
		}
		
		default:
			break;
		}
	}
	
	return anEntity;
}

bool FogName::get_meta_entity_in(FogTokenRef& returnValue, FogScopeContext& inScope) const {
	bool gotSlot = find_meta_entity_in(returnValue, inScope);
	
	if (!gotSlot && inScope.requires_resolution() && !returnValue->is_failure()) {
		ERRMSG("Failed to resolve " << viz(*this) << " in " << viz(inScope));
		
		if (Fog::debug_repeat())
			find_meta_entity_in(returnValue, inScope);  //   Another go for debugging
			
		returnValue = FogFailure::make();
	}
	
	return gotSlot;
}

//
//    Return this name as a viable type-specifier. The relatively few derived classes for which
//    this is too simplistic re-implement.
//
FogName* FogName::get_type() { return this; }

//
//    Return true if an @-expression must be resolved to resolve_id().
//
bool FogName::has_at() const { return false; }

//
//    Return true if a $-expression must be resolved to resolve_id().
//
bool FogName::has_dollar() const { return false; }

FogDeclSpecifier* FogName::is_decl_specifier() { return 0; }
FogEntityName* FogName::is_entity_name() { return 0; }
FogName* FogName::is_name() { return this; }

//
//    Return the id if this name is free from all dependency on scope context
//    (or unevaluated @ invocations), and in addition has been compressed to a simple identifier.
//
const PrimId* FogName::is_resolved() const {
	ERRMSG("BUG - should not invoke FogName::is_resolved for " << viz(*this));
	return 0;
}

FogTemplateParameterSpecifierId* FogName::is_template_parameter_specifier_id() { return 0; }
FogTemplatedName* FogName::is_templated_name() { return 0; }
FogTypedName* FogName::is_typed_name() { return 0; }
FogName& FogName::make_anon() { return FogAnonId::make(); }

//
//    Append a builtin-typeid to this name.
//
void FogName::make_built_in(FogNameRef& aName, const FogBuiltInTypeId& anId) {
	ERRMSG("BUG - should not invoke FogName::make_built_in for " << viz(*this));
	aName = FogFailure::make();
}

//
//    Create the entity defined by this name on behalf of a type-specifier.
//    Primarily defined by derived Specifiers but also for bland names used as types.
//
FogEntity* FogName::make_entity(FogMakerContext& makerContext) const {
	ERRMSG("BUG - should not invoke FogName::make_entity for " << viz(*this));
	return 0;
}

//
//    Convert this specifier into the entity that it names.
//
FogEntity* FogName::make_name_entity(FogMakeEntityContext& makeEntityContext) const {
	ERRMSG("BUG - should not invoke FogName::make_name_entity for " << viz(*this));
	return 0;
}

FogName& FogName::make_operator(const char* anId) { return FogOperatorId::make(*PrimIdHandle(anId)); }

//
//    Convert this specifier into the type that it names.
//
FogEntity* FogName::make_type_entity(FogMakeEntityContext& makeEntityContext) const {
	FogEntity* ent = find_entity_in(makeEntityContext, FIND_TYPE);
	
	if (!ent) {
		ent = find_entity_in(makeEntityContext, FIND_SCOPE);
	}
	
	return ent;
}

bool FogName::make_type_specifier(FogMakeSpecifierContext& makeSpecifierContext) {
	ERRMSG("BUG - should not invoke FogName::make_type_specifier for " << viz(*this));
	return false;
}

bool FogName::make_typed_expression(FogExprRef& theExpr, FogName& theType) {
	theExpr.adopt(new FogTypedName(theType, *this));
	return true;
}

const FogMetaType& FogName::meta_type() const { return FogMetaType::name_type(); }

std::ostream& FogName::print_long_id(std::ostream& s) const {
	print_named(s, 0, 0);
	return s;
}

std::ostream& FogName::print_viz(std::ostream& s) const {
	s << "\"name: ";
	print_this(s);
	return s << '\"';
}

bool FogName::put_value(FogScopeContext& putScope, const FogToken& tokenValue, FogScopeContext& getScope) const {
	ERRMSG("BUG -- should not put_value for " << viz(*this));
	return false;
}

//
//    Return the identifier part of this name (strip decl-specifier and template-arguments).
//
bool FogName::resolve_id(PrimIdHandle& returnId, FogScopeContext& inScope) const {
	ERRMSG("INVESTIGATE -- should not FogName::resolve_id for " << viz(*this));
	returnId.reset();
	return false;
}

bool FogName::resolve_slot(FogTokenRef& returnValue, FogScopeContext& scopeContext, const FogToken& objectValue) const {
	return objectValue.get_monadic(returnValue, scopeContext, FogTokenType::character('='));
}

void FogName::set_decl_specifier_expression(FogExprRef& anExpr, FogDeclSpecifier& declSpecifiers) {
	FogNameRef aName;
	set_decl_specifier_name(aName, declSpecifiers);
	anExpr = *aName;
}

void FogName::set_decl_specifier_name(FogNameRef& aName, const FogDeclSpecifier& declSpecifiers) {
	aName.adopt(new FogDeclSpecifiedName(*this));
	aName->set_decl_specifier_name(aName, declSpecifiers);
}

void FogName::set_decl_specifier_raw(FogRawRef& aRaw, FogDeclSpecifier& declSpecifiers) {
	FogNameRef aName;
	set_decl_specifier_name(aName, declSpecifiers);
	aRaw = *aName;
}

void FogName::set_destructor_expression(FogExprRef& anExpr) {
	FogNameRef aName;
	set_destructor_name(aName);
	anExpr = *aName;
}

void FogName::set_destructor_name(FogNameRef& aName) {
	ERRMSG("Cannot interpret ~ " << viz(*this) << " as the name of a destructor.");
	aName = FogFailure::make();
}

FogToken::NameSort FogName::sort_class() const {
	ERRMSG("BUG -- should not sort_class from " << viz(*this));
	return INVALID_NAME;
}

const FogTag& FogName::tag() const {
	ERRMSG("BUG -- should not tag from " << viz(*this));
	return FogTag::invalid_tag();
}
