#include <Fog/FogIncludeAll.h>















TYPEINFO_SINGLE(FogWordName, Super)
PRIMREF_NULL_CLAS(FogWordName)
FOGTOKEN_NULL_LEAF
virtual bool resolve_id(PrimIdHandle& returnId, FogScopeContext& inScope) const
{ returnId = PrimId::null(); return true; }
};
PRIMREF_DERIVED_NULL_IMPL(FogWordName)
PRIMREFS_IMPL(FogWordName)
TMPL_HACK_FIX_DO(FogWordName)

void FogWordName::create_usage(FogUsageContext& usageContext) const {
	PrimIdHandle theId;
	
	if (!resolve_id(theId, usageContext) || !is_valid(*theId))
		return;                                 //   Happens for numeric results of tree-literals etc.
		
	FogEntity* anEntity = find_type_in(usageContext);
	
	if (anEntity)
		anEntity->add_use_by(usageContext);
}

bool FogWordName::emit(FogEmitContext& emitContext) const {
	if (is_null())
		return false;
		
	PrimIdHandle anId;
	resolve_id(anId, emitContext);
	return emitContext.emit_id(*anId, *this);
}

//
//    Find the formal argument matching this name inScope.
//
const FogMetaArgument* FogWordName::find_formal_in(FogScopeContext& inScope) const {
	const FogMetaSlot* aSlot = find_slot_in(inScope);
	const FogMetaArgument* metaArgument = aSlot ? aSlot->is_meta_argument() : 0;
	return metaArgument;
}

const FogMetaSlot* FogWordName::find_slot_in(FogScopeContext& scopeContext) const {
	FogMetaSlotFinding metaSlotFinding;
	
	if (find_slots_in(scopeContext, metaSlotFinding) && !metaSlotFinding.is_found())
		metaSlotFinding.add(FogMetaSlot::immutable_null());                     //   Propagate return truth.
		
	return metaSlotFinding.get_unambiguous_finding(*this, scopeContext);
}

//
//    Add all slots matching this name inScope to theFinding.
//
bool FogWordName::find_slots_in(FogScopeContext& inScope, FogMetaSlotFinding& theFinding) const {
	PrimIdHandle anId;
	
	if (!resolve_id(anId, inScope))
		return false;
		
	FogMetaSlotFinder theFinder(theFinding, *anId, inScope.in_scope());
	return inScope.find_slots(theFinder);
}

bool FogWordName::get_character(PrimIdHandle& returnId, FogScopeContext& inScope) {
	//    if (inScope.at_meta())
	//    {
	//        FogTokenRefToConst resolvedObject;
	//        if (get_object(resolvedObject, inScope) && (this != resolvedObject.pointer())
	//            return resolvedObject->get_character(returnId, inScope);
	//    }
	return resolve_id(returnId, inScope);
}

bool FogWordName::get_identifier(PrimIdHandle& returnId, FogScopeContext& inScope) {
	//    if (inScope.at_meta())          -- .bugbug this code should work but has const problems
	//    {
	//        FogTokenRefToConst resolvedObject;
	//        if (get_object(resolvedObject, inScope) && (this != resolvedObject.pointer())
	//            return resolvedObject->get_identifier(returnId, inScope);
	//    }
	return resolve_id(returnId, inScope);
}

bool FogWordName::get_number_token(FogTokenRef& returnValue, FogScopeContext& inScope) const {
	//    if (inScope.at_meta())
	//    {
	//        if (get_object(returnValue, inScope) && (this != returnValue.pointer())
	//            return returnValue->get_number_token(returnValue, inScope);
	//    }
	PrimIdHandle anId;
	
	if (!resolve_id(anId, inScope)) {
		returnValue = FogFailure::make();
		return false;
	}
	
	bool isUndiagnosed = !inScope.requires_resolution() && inScope.is_emit_context();
	FogNumber& theNumber = FogNumber::make(*anId, isUndiagnosed);
	returnValue.assign(theNumber);
	return !theNumber.is_null();
}

bool FogWordName::get_object(FogTokenRef& returnValue, FogScopeContext& inScope) const {
	if (!inScope.at_meta())
		return Super::get_object(returnValue, inScope);
		
	return get_meta_entity_in(returnValue, inScope);
}

bool FogWordName::get_string(PrimIdHandle& returnId, FogScopeContext& inScope) {
	//    if (inScope.at_meta())
	//    {
	//        FogTokenRefToConst resolvedObject;
	//        if (get_object(resolvedObject, inScope) && (this != resolvedObject.pointer())
	//            return resolvedObject->get_string(inScope);
	//    }
	return resolve_id(returnId, inScope);
}

bool FogWordName::is_actual(const FogScopeContext& scopeContext) const {
	if (!Super::is_actual(scopeContext))
		return false;
		
	if (scopeContext.is_normalise_templates()
	        && is_unnormalised_template_parameter((FogScopeContext&)scopeContext))         //  .bugbug cast
		return false;
		
	return true;
}

const FogTemplateParameterSpecifier* FogWordName::is_unnormalised_template_parameter
(FogScopeContext& scopeContext) const { return 0; }

bool FogWordName::is_valid(const PrimId& anId) {
	size_t toGo = anId.length();
	const char* p = anId.str();
	
	if (!toGo || (!isalpha(*p) && (*p != '_')))
		return false;
		
	for (++p; --toGo > 0; ++p)
		if (!isalnum(*p) && (*p != '_'))
			return false;
			
	return true;
}

void FogWordName::make_actual_token(FogTokenRefToConst& actualToken, FogMakeActualContext& makeActualContext) const {
	const FogTemplateParameterSpecifier* templateParameter = is_unnormalised_template_parameter(makeActualContext);
	
	if (templateParameter && makeActualContext.is_normalise_templates()) {
		PrimIdHandle resolveId;
		templateParameter->resolve_id(resolveId, makeActualContext);
		actualToken = FogTemplateParameterSpecifierId::make(*resolveId, *templateParameter);
	}
	else
		Super::make_actual_token(actualToken, makeActualContext);
}

//
//    Append a builtin-typeid to this name.
//
void FogWordName::make_built_in(FogNameRef& aName, const FogBuiltInTypeId& anId) {
	aName.adopt(new FogDeclSpecifiedName(*this));
	aName->make_built_in(aName, anId);
}

FogEntity* FogWordName::make_type_entity(FogMakeEntityContext& makeEntityContext) const { return 0; }

bool FogWordName::make_type_specifier(FogMakeSpecifierContext& makeSpecifierContext) {
	FogObjectSpecifier* objectSpecifier = makeSpecifierContext.get_object_specifier(*this);
	
	if (!objectSpecifier)
		return false;
		
	FogWordNameRef actualName;
	make_actual(actualName.to_const(), makeSpecifierContext);   //  .bugbug to_const
	objectSpecifier->set_type(*actualName);
	return true;
}

bool FogWordName::put_value(FogScopeContext& putScope, const FogToken& tokenValue, FogScopeContext& getScope) const {
	FogTokenRef returnValue;
	
	if (!get_meta_entity_in(returnValue, putScope))
		return false;
		
	FogMetaEntity* metaEntity = returnValue->is_meta_entity();
	
	if (!metaEntity) {
		ERRMSG("BUG -- expected meta-entity " << viz(*returnValue) << " in FogWordName::put_value");
		return false;
	}
	
	return metaEntity->set_value(tokenValue, getScope);
}

bool FogWordName::resolve_identifier_semantics(FogSemanticsContext& theSemantics) const {
	theSemantics.reset();
	PrimIdHandle theId;
	
	//    if (!resolve_id(theId, inScope) || !is_valid(*theId))
	//    {
	//        if (!inScope.is_normalise_templates() && inScope.requires_resolution())
	//            ERRMSG("Illegal identifier \"" << *theId << '"');
	//        return false;
	//    }
	if (!resolve_id(theId, theSemantics))
		return false;
		
	if (!is_valid(*theId)) {
		if (theSemantics.is_resolve_type_weakly()) {
			theSemantics.set_diagnostic(FogSemanticsContext::BAD_IDENTIFIER);
			theSemantics.assign_identifier(FogSemantics::POTENTIAL);
			return true;
		}
		else
			return false;
	}
	
	theSemantics.assign_identifier(FogSemantics::ACTUAL);
	//    bool isPotential = inScope.dynamic_token().is_null();
	//    theSemantics.assign_identifier(isPotential ? FogSemantics::POTENTIAL : FogSemantics::ACTUAL);
	//    if (isPotential)                                        // If only looking for potential semantics.
	//        return true;
	const FogTemplateParameterSpecifier* templateParameter = theSemantics.find_template_parameter_specifier(*theId);
	
	if (templateParameter) {
		if (templateParameter->is_type_parameter_specifier())
			theSemantics.set(FogSemantics::IS_TEMPLATE_TYPE_NAME);
		else
			theSemantics.set(FogSemantics::IS_TEMPLATE_VALUE_NAME);
	}
	else {
		FogEntityFinding typeFinding(FIND_TYPE);
		find_entities_in(theSemantics, typeFinding);
		const FogEntity* typeEntity = typeFinding.found();
		
		if (typeEntity) {                                       //  .bugbug ignores multiplicity - just uses first one.
			if (typeEntity->is_enum())
				theSemantics.set(FogSemantics::IS_ENUM_NAME);
				
			if (typeEntity->is_namespace()) {
				ERRMSG("INVESTIGATE -- namespace name " << viz(*this)
				       << " installed in " << viz(theSemantics) << " as type.");
				theSemantics.set(FogSemantics::IS_NAMESPACE_NAME);
			}
			
			if (typeEntity->is_templated())
				theSemantics.set(FogSemantics::IS_TEMPLATE_TYPE_NAME);
				
			const FogTypedef* aTypedef = typeEntity->is_typedef();
			
			if (aTypedef) {
				theSemantics.set(FogSemantics::IS_TYPEDEF_NAME);
				const FogEntity* typedefEntity = aTypedef->resolve_alias();
				
				if (typedefEntity)
					typeEntity = typedefEntity;
					
				//   No typedef of function needed since any IDENTIFIER will do.
			}
			
			if (typeEntity->is_class())
				theSemantics.set(FogSemantics::IS_CLASS_NAME);
		}
		
		FogEntityFinding nameFinding(FIND_NAME);
		find_entities_in(theSemantics, nameFinding);
		const FogEntity* nameEntity = nameFinding.found();
		
		if (nameEntity) {                                       //  .bugbug ignores multiplicity - just uses first one.
			if (nameEntity->is_namespace())
				theSemantics.set(FogSemantics::IS_NAMESPACE_NAME);
				
			if (nameEntity->is_templated())
				theSemantics.set(FogSemantics::IS_TEMPLATE_VALUE_NAME);
		}
	}
	
	return true;
}

void FogWordName::set_destructor_name(FogNameRef& aName) {
	aName.adopt(new FogDestructorId(*this));
}
