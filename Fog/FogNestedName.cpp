#include <Fog/FogIncludeAll.h>
















TYPEINFO_SINGLE(FogNestedName, Super)
FOGTOKEN_LEAF_IMPL(FogNestedName)

//
//    Construct a nested name, representing 'scopeName'::'childName'.
//
FogNestedName::FogNestedName(FogName& scopeName, FogName& childName)
	:
	Super(childName),
	_scope(scopeName)
{}

FogNestedName::FogNestedName(const This& aName)
	:
	Super(aName),
	_scope(aName._scope)
{}

FogNestedName::~FogNestedName() {}

void FogNestedName::create_usage(FogUsageContext& usageContext) const {
	_scope->create_usage(usageContext);
	FogToken* theScope = find_scope(usageContext);
	
	if (theScope) {
		FogNestedUsageContext nestedContext(usageContext, *theScope);
		Super::create_usage(nestedContext);
		nestedContext.entity().add_use_by(nestedContext.entity(), nestedContext.of_use_by().of_tail());
	}
}

bool FogNestedName::emit(FogEmitContext& emitContext) const {
	_scope->emit(emitContext);
	emitContext.emit_space_and_text("::");
	
	if (emitContext.emit_expression())
		name().emit(emitContext);
	else {
		FogToken* theScope = find_scope(emitContext);
		
		if (0) {if (!theScope) theScope = find_entity_in(emitContext, FIND_SCOPE);}
		
		if (theScope) {
			FogNestedEmitContext nestedContext(emitContext, *theScope);
			name().emit(nestedContext);
		}
	}
	
	return true;
}

FogEntity* FogNestedName::find_entity_in(FogScopeContext& inScope, FindStrategy aStrategy) const {
	if (inScope.is_unresolvable())
		return 0;
		
	FogToken* theScope = find_scope(inScope);
	
	if (!theScope)
		return 0;
		
	FogNestedScopeContext nestedContext(inScope, *theScope);
	return name().find_entity_in(nestedContext, aStrategy);
}

FogToken* FogNestedName::find_scope(FogScopeContext& inScope) const {
	if (0) {
		FogScope& dynamicScope = inScope.dynamic_scope();
		
		if (dynamicScope.is_null()) {
			//   If an any old scope form of resolve_semantics
			return 0;
		}
		
		if (inScope.is_unresolvable())
			ERRMSG("INVESTIGATE -- Invoking FogNestedName::find_scope in unexpected phase");
			
		FogToken* newScope = _scope->find_entity_in(inScope, FIND_SCOPE);
		//    if (newScope && !hasArgs && newScope->is_templated())
		//    {
		//        ERRMSG("ROUNDTUIT -- missing code to select default secondary template.");
		//    }
		return newScope;
	}
	else {
		FogScopeContext* ctx = &inScope;
		
		if (ctx->is_unresolvable())
			ERRMSG("INVESTIGATE -- Invoking FogNestedName::find_scope in unexpected phase");
			
		int depth = 0;
		
		while (ctx) {
			FogEntity* newScope = _scope->find_entity_in(*ctx, FIND_SCOPE);
			
			if (newScope) {
				if (!depth)
					return newScope;
				else {
					newScope = find_entity_in(*ctx, FIND_SCOPE);
					
					if (newScope)
						return newScope;
				}
			}
			
			const FogScopeContext* par = ctx->get_parent();
			
			if (par)
				ctx = const_cast<FogScopeContext*>(par);
			else {
				FogDecoratedEmitContext* emitctx = dynamic_cast<FogDecoratedEmitContext*>(ctx);
				
				if (emitctx)
					ctx = &emitctx->get_context();
				else
					ctx = 0;
			}
			
			depth++;
		}
		
		return 0;
	}
}

const FogMetaSlot* FogNestedName::find_slot_in(FogScopeContext& scopeContext) const {
	if (scopeContext.is_unresolvable())
		return 0;
		
	FogToken* theScope = find_scope(scopeContext);
	
	if (!theScope)
		return 0;
		
	FogNestedScopeContext nestedContext(scopeContext, *theScope);
	return name().find_slot_in(nestedContext);
}

bool FogNestedName::get_object(FogTokenRef& returnValue, FogScopeContext& inScope) const {
	if (inScope.is_unresolvable())
		return 0;
		
	FogToken* theScope = find_scope(inScope);
	
	if (!theScope) {
		if (inScope.requires_resolution())
			ERRMSG("Failed to find " << viz(*this) << " in " << viz(inScope));
			
		returnValue = FogFailure::make();
		return 0;
	}
	
	FogNestedScopeContext nestedContext(inScope, *theScope);
	return name().get_object(returnValue, nestedContext);
}

void FogNestedName::install(FogInstallContext& installContext) const {
	FogToken* theScope = find_scope(installContext);
	
	if (!theScope)
		return;
		
	FogNestedScopeContext nestedContext(installContext, *theScope);
	Super::install(nestedContext);
}

bool FogNestedName::is_actual(const FogScopeContext& scopeContext) const {
	return _scope->is_actual(scopeContext) && Super::is_actual(scopeContext);
}

void FogNestedName::make_actual_from(FogMakeActualContext& makeActualContext) {
	_scope->make_actual(_scope.to_const(), makeActualContext);
	//      if (makeActualContext.is_potential_phase())         // In $ { X } ::$ { Y } Y resolution is not nested in X
	Super::make_actual_from(makeActualContext);
	//      else
	//      {
	//          FogToken *theScope = find_scope(makeActualContext);
	//          if (!theScope)
	//              return;
	//          FogNestedScopeContext nestedContext(makeActualContext, *theScope);
	//          Super::make_actual_from(nestedContext);
	//      }
}

FogEntity* FogNestedName::make_entity(FogMakerContext& makerContext) const {
	FogToken* theScope = find_scope(makerContext);
	
	if (!theScope)
		return 0;
		
	FogNestedMakerContext nestedContext(makerContext, *theScope);
	return name().make_entity(nestedContext);
}

FogEntity* FogNestedName::make_name_entity(FogMakeEntityContext& makeEntityContext) const {
	FogToken* theScope = find_scope(makeEntityContext);
	
	if (!theScope)
		return 0;
		
	FogNestedMakeEntityContext nestedContext(makeEntityContext, *theScope);
	return name().make_name_entity(nestedContext);
}

bool FogNestedName::make_specifier(FogMakeSpecifierContext& makeSpecifierContext) {
	FogToken* theToken = find_scope(makeSpecifierContext);
	
	if (!theToken) {
		ERRMSG("Failed to locate scope for " << viz(*this));
		return 0;
	}
	
	FogScope* theScope = theToken ? theToken->is_scope() : 0;
	
	if (theScope && theScope->is_instantiation()) {
		const FogTemplateParameterSpecifiers* templateParameterSpecifiers = makeSpecifierContext.template_parameters();
		
		if (!templateParameterSpecifiers)
			WRNMSG("Missing \"template <>\" for explicit specialisation of " << viz(*theScope));
	}
	
	makeSpecifierContext.set_nested_scope(*theToken);       //   No restoration; caller wants to know where we are.
	return name().make_specifier(makeSpecifierContext);
}

bool FogNestedName::make_type_specifier(FogMakeSpecifierContext& makeSpecifierContext) {
	FogObjectSpecifier* objectSpecifier = makeSpecifierContext.get_object_specifier(*this);
	
	if (!objectSpecifier)
		return false;
		
	FogDecoratedNameRef actualName;
	make_actual(actualName.to_const(), makeSpecifierContext);   //  .bugbug to_const
	objectSpecifier->set_type(*actualName);
	return true;
}

void FogNestedName::merge_from(FogMergeContext& mergeContext, const This& thatName) {
	Super::merge_from(mergeContext, thatName);
	thatName._scope->merge_into(mergeContext, _scope.to_const());
}

const FogMerge& FogNestedName::needs_merge_from(FogMergeContext& mergeContext, const This& thatName) const {
	FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, thatName));
	needsMerge |= _scope->needs_merge(mergeContext, *thatName._scope);
	return needsMerge;
}

std::ostream& FogNestedName::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	_scope->print_deep(s, aDepth);
	return s;
}

std::ostream& FogNestedName::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	_scope->print_on(s, aDepth);
	return s;
}

char FogNestedName::print_named(std::ostream& s, const PrimId* scopeId, char tailChar) const {
	tailChar = _scope->print_named(s, 0, tailChar);
	tailChar = FogStream::space_and_emit(s, tailChar, "::");
	return Super::print_named(s, 0, tailChar);
}

bool FogNestedName::put_value(FogScopeContext& putScope, const FogToken& tokenValue, FogScopeContext& getScope) const {
	FogToken* theScope = find_scope(putScope);
	
	if (!theScope)
		return false;
		
	FogNestedScopeContext nestedContext(putScope, *theScope);
	return name().put_value(nestedContext, tokenValue, getScope);
}

bool FogNestedName::resolve_semantics(FogSemanticsContext& theSemantics) const {
	if (theSemantics.is_unresolvable())
		return false;
		
	FogToken* theScope = find_scope(theSemantics);
	
	if (!theScope)
		return false;
		
	FogChildSemanticsContext scopeSemantics(theSemantics);
	FogNestedSemanticsContext nameSemantics(theSemantics, *theScope);
	
	if (!theScope->resolve_semantics(scopeSemantics) || !name().resolve_semantics(nameSemantics))
		return false;
		
	if (scopeSemantics.is_class_or_namespace_name()) {
		if (nameSemantics.is_scoped_class_name())
			theSemantics.set(FogSemantics::IS_SCOPED_CLASS_NAME);
			
		if (nameSemantics.is_scoped_identifier())
			theSemantics.set(FogSemantics::IS_SCOPED_IDENTIFIER);
			
		if (nameSemantics.is_scoped_type_name())
			theSemantics.set(FogSemantics::IS_SCOPED_TYPE_NAME);
			
		if (nameSemantics.is_pseudo_destructor_name())
			theSemantics.set(FogSemantics::IS_PSEUDO_DESTRUCTOR_NAME);
			
		if (nameSemantics.is_template_unqualified_id())
			theSemantics.set(FogSemantics::IS_QUALIFIED_ID);
			
		if (nameSemantics.is_unqualified_id())
			theSemantics.set(FogSemantics::IS_QUALIFIED_ID);
			
		if (nameSemantics.is_qualified_id())
			theSemantics.set(FogSemantics::IS_QUALIFIED_ID);
	}
	
	if (scopeSemantics.is_type_name()) {
		if (nameSemantics.is_pseudo_destructor_id())                //  .bugbug validate same type-name
			theSemantics.set(FogSemantics::IS_PSEUDO_DESTRUCTOR_NAME);
	}
	
	if (scopeSemantics.is_meta_type_name()) {
		if (nameSemantics.is_scoped_meta_name())
			theSemantics.set(FogSemantics::IS_SCOPED_META_NAME);
			
		if (nameSemantics.is_scoped_meta_type_name())
			theSemantics.set(FogSemantics::IS_SCOPED_META_TYPE_NAME);
			
		if (nameSemantics.is_meta_dtor_name())
			theSemantics.set(FogSemantics::IS_META_DTOR_NAME);
	}
	
	return true;
}
