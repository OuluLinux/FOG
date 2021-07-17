#include <Fog/FogIncludeAll.h>














TYPEINFO_SINGLE(FogGlobalName, Super)
FOGTOKEN_LEAF_IMPL(FogGlobalName)

//
//    Construct a global name, representing ::'childName'.
//
FogGlobalName::FogGlobalName(IsTemplate  isTemplate, FogName& childName)
	:
	Super(childName),
	_is_template(isTemplate)
{}

FogGlobalName::FogGlobalName(const This& aName)
	:
	Super(aName),
	_is_template(aName._is_template)
{}

FogGlobalName::~FogGlobalName() {}

void FogGlobalName::create_usage(FogUsageContext& usageContext) const {
	FogNestedUsageContext nestedContext(usageContext, usageContext.global_scope());
	Super::create_usage(nestedContext);
	nestedContext.entity().add_use_by(nestedContext.entity(), nestedContext.of_use_by().of_tail());
}

bool FogGlobalName::emit(FogEmitContext& emitContext) const {
	if (_is_template)
		emitContext.emit_space_and_text(" template");
		
	emitContext.emit_space_and_text(" ::");
	
	if (emitContext.emit_expression())
		name().emit(emitContext);
	else {
		FogNestedEmitContext nestedContext(emitContext, emitContext.global_scope());
		name().emit(nestedContext);
	}
	
	return true;
}

FogEntity* FogGlobalName::find_entity_in(FogScopeContext& inScope, FindStrategy aStrategy) const {
	if (inScope.is_unresolvable())
		return 0;
		
	FogNestedScopeContext nestedContext(inScope, inScope.global_scope());
	return name().find_entity_in(nestedContext, aStrategy);
}

const FogMetaSlot* FogGlobalName::find_slot_in(FogScopeContext& scopeContext) const {
	if (scopeContext.is_unresolvable())
		return 0;
		
	FogNestedScopeContext nestedContext(scopeContext, scopeContext.global_scope());
	return name().find_slot_in(nestedContext);
}

bool FogGlobalName::get_object(FogTokenRef& returnValue, FogScopeContext& inScope) const {
	if (inScope.is_unresolvable())
		return 0;
		
	FogNestedScopeContext nestedContext(inScope, inScope.global_scope());
	return name().get_object(returnValue, nestedContext);
}

void FogGlobalName::install(FogInstallContext& installContext) const {
	FogNestedScopeContext nestedContext(installContext, installContext.global_scope());
	Super::install(nestedContext);
}

bool FogGlobalName::is_actual(const FogScopeContext& scopeContext) const {
	return Super::is_actual(scopeContext);
}

void FogGlobalName::make_actual_from(FogMakeActualContext& makeActualContext) {
	//      if (makeActualContext.is_potential_phase())         // In $ { X } ::$ { Y } Y resolution is not nested in X
	Super::make_actual_from(makeActualContext);
	//      else
	//      {
	//          FogNestedScopeContext nestedContext(makeActualContext, makeActualContext.global_scope());
	//          Super::make_actual_from(nestedContext);
	//      }
}

FogEntity* FogGlobalName::make_entity(FogMakerContext& makerContext) const {
	FogNestedMakerContext nestedContext(makerContext, makerContext.global_scope());
	return name().make_entity(nestedContext);
}

FogEntity* FogGlobalName::make_name_entity(FogMakeEntityContext& makeEntityContext) const {
	FogNestedMakeEntityContext nestedContext(makeEntityContext, makeEntityContext.global_scope());
	return name().make_name_entity(nestedContext);
}

bool FogGlobalName::make_specifier(FogMakeSpecifierContext& makeSpecifierContext) {
	makeSpecifierContext.set_nested_scope(makeSpecifierContext.global_scope()); //   No restoration; caller wants to know where we are.
	return name().make_specifier(makeSpecifierContext);
}

FogEntity* FogGlobalName::make_type_entity(FogMakeEntityContext& makeEntityContext) const { return 0; }

bool FogGlobalName::make_type_specifier(FogMakeSpecifierContext& makeSpecifierContext) {
	FogObjectSpecifier* objectSpecifier = makeSpecifierContext.get_object_specifier(*this);
	
	if (!objectSpecifier)
		return false;
		
	FogDecoratedNameRef actualName;
	make_actual(actualName.to_const(), makeSpecifierContext);   //  .bugbug to_const
	objectSpecifier->set_type(*actualName);
	return true;
}

void FogGlobalName::merge_from(FogMergeContext& mergeContext, const This& thatName) {
	Super::merge_from(mergeContext, thatName);
}

const FogMerge& FogGlobalName::needs_merge_from(FogMergeContext& mergeContext, const This& thatName) const {
	FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, thatName));
	//  .bugbug _is_template
	return needsMerge;
}

std::ostream& FogGlobalName::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	return s;
}

std::ostream& FogGlobalName::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	return s;
}

char FogGlobalName::print_named(std::ostream& s, const PrimId* scopeId, char tailChar) const {
	if (_is_template)
		tailChar = FogStream::space_and_emit(s, tailChar, " template");
		
	tailChar = FogStream::space_and_emit(s, tailChar, " ::");
	return Super::print_named(s, 0, tailChar);
}

bool FogGlobalName::put_value(FogScopeContext& putScope, const FogToken& tokenValue, FogScopeContext& getScope) const {
	FogNestedScopeContext nestedContext(putScope, putScope.global_scope());
	return name().put_value(nestedContext, tokenValue, getScope);
}

bool FogGlobalName::resolve_semantics(FogSemanticsContext& theSemantics) const {
	if (theSemantics.is_unresolvable())
		return false;
		
	//      FogNestedScopeContext nestedContext(inScope, theSemantics.global_scope());
	//      FogChildSemanticsContext nameSemantics(nestedContext, theSemantics);
	FogNestedSemanticsContext nameSemantics(theSemantics, theSemantics.global_scope());
	
	if (!name().resolve_semantics(nameSemantics))
		return false;
		
	if (nameSemantics.is_scoped_class_name())
		theSemantics.set(FogSemantics::IS_SCOPED_CLASS_NAME);
		
	if (nameSemantics.is_scoped_identifier())
		theSemantics.set(FogSemantics::IS_SCOPED_IDENTIFIER);
		
	if (nameSemantics.is_scoped_type_name())
		theSemantics.set(FogSemantics::IS_SCOPED_TYPE_NAME);
		
	if (nameSemantics.is_id_expression())
		theSemantics.set(FogSemantics::IS_GLOBAL_ID_EXPRESSION);
		
	if (nameSemantics.is_qualified_id())
		theSemantics.set(FogSemantics::IS_PRIMARY_EXPRESSION);
		
	if (nameSemantics.is_operator_function_id())
		theSemantics.set(FogSemantics::IS_PRIMARY_EXPRESSION);
		
	if (nameSemantics.is_id_expression())
		theSemantics.set(FogSemantics::IS_GLOBAL_ID_EXPRESSION);
		
	if (nameSemantics.is_pseudo_destructor_name()) {
		theSemantics.set(FogSemantics::IS_PSEUDO_DESTRUCTOR_NAME);
		theSemantics.set(FogSemantics::IS_MEMBER_ID_EXPRESSION);
	}
	
	if (nameSemantics.is_scoped_meta_name())
		theSemantics.set(FogSemantics::IS_SCOPED_META_NAME);
		
	if (nameSemantics.is_scoped_meta_type_name())
		theSemantics.set(FogSemantics::IS_SCOPED_META_TYPE_NAME);
		
	if (nameSemantics.is_meta_dtor_name())
		theSemantics.set(FogSemantics::IS_META_DTOR_NAME);
		
	return true;
}
