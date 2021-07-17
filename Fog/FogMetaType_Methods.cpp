#include <Fog/FogIncludeAll.h>






















FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_Base_Base, scope, "base", IS_ENCAPSULATED) {
	FogBaseClass* baseClass = callContext.get_base_class();
	
	if (!baseClass)
		return false;
		
	returnValue.assign(baseClass->base());
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_Base_IsAuto, bool, "is_auto", IS_ENCAPSULATED) {
	FogBaseClass* baseClass = callContext.get_base_class();
	
	if (!baseClass)
		return false;
		
	returnValue.assign(FogNumber::make_logical(baseClass->is_auto()));
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_Base_IsPrivate, bool, "is_private", IS_ENCAPSULATED) {
	FogBaseClass* baseClass = callContext.get_base_class();
	
	if (!baseClass)
		return false;
		
	returnValue.assign(FogNumber::make_logical(baseClass->is_private()));
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_Base_IsProtected, bool, "is_protected", IS_ENCAPSULATED) {
	FogBaseClass* baseClass = callContext.get_base_class();
	
	if (!baseClass)
		return false;
		
	returnValue.assign(FogNumber::make_logical(baseClass->is_protected()));
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_Base_IsPublic, bool, "is_public", IS_ENCAPSULATED) {
	FogBaseClass* baseClass = callContext.get_base_class();
	
	if (!baseClass)
		return false;
		
	returnValue.assign(FogNumber::make_logical(baseClass->is_public()));
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_Base_IsVirtual, bool, "is_virtual", IS_ENCAPSULATED) {
	FogBaseClass* baseClass = callContext.get_base_class();
	
	if (!baseClass)
		return false;
		
	returnValue.assign(FogNumber::make_logical(baseClass->is_virtual()));
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_BaseSpecifier_Base, scope, "base", IS_ENCAPSULATED) {
	FogBaseClass* baseClass = callContext.get_base_class();
	
	if (!baseClass)
		return false;
		
	returnValue.assign(baseClass->base());
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_BaseSpecifier_IsAuto, bool, "is_auto", IS_ENCAPSULATED) {
	FogBaseClass* baseClass = callContext.get_base_class();
	
	if (!baseClass)
		return false;
		
	returnValue.assign(FogNumber::make_logical(baseClass->is_auto()));
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_BaseSpecifier_IsPrivate, bool, "is_private", IS_ENCAPSULATED) {
	FogBaseClass* baseClass = callContext.get_base_class();
	
	if (!baseClass)
		return false;
		
	returnValue.assign(FogNumber::make_logical(baseClass->is_private()));
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_BaseSpecifier_IsProtected, bool, "is_protected", IS_ENCAPSULATED) {
	FogBaseClass* baseClass = callContext.get_base_class();
	
	if (!baseClass)
		return false;
		
	returnValue.assign(FogNumber::make_logical(baseClass->is_protected()));
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_BaseSpecifier_IsPublic, bool, "is_public", IS_ENCAPSULATED) {
	FogBaseClass* baseClass = callContext.get_base_class();
	
	if (!baseClass)
		return false;
		
	returnValue.assign(FogNumber::make_logical(baseClass->is_private()));
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_BaseSpecifier_IsVirtual, bool, "is_virtual", IS_ENCAPSULATED) {
	FogBaseClass* baseClass = callContext.get_base_class();
	
	if (!baseClass)
		return false;
		
	returnValue.assign(FogNumber::make_logical(baseClass->is_virtual()));
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_Entity_GlobalName, identifier, "global_name", IS_ENCAPSULATED) {
	const FogEntity* anEntity = callContext.get_entity();
	
	if (!anEntity)
		return false;
		
	returnValue.assign(FogIdentifier::make(anEntity->global_signature_id()));
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_Entity_IsBoundary, bool, "is_boundary", IS_ENCAPSULATED) {
	const FogEntity* anEntity = callContext.get_entity();
	
	if (!anEntity)
		return false;
		
	const FogLazyBool& lazyBool = anEntity->is_boundary();
	
	if (lazyBool.is_undecided())
		return false;
		
	returnValue.assign(FogNumber::make_logical(lazyBool.is_true()));
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_Entity_IsPure, bool, "is_pure", IS_ENCAPSULATED) {
	const FogEntity* anEntity = callContext.get_entity();
	
	if (!anEntity)
		return false;
		
	const FogLazyBool& lazyBool = anEntity->is_pure();
	
	if (lazyBool.is_undecided())
		return false;
		
	returnValue.assign(FogNumber::make_logical(lazyBool.is_true()));
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_Entity_LongName, identifier, "long_name", IS_ENCAPSULATED) {
	const FogEntity* anEntity = callContext.get_entity();
	
	if (!anEntity)
		return false;
		
	returnValue.assign(FogIdentifier::make(anEntity->long_id()));
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_Enum_Enumerators, enumerator, "enumerators", IS_EXPOSED) {
	FogEnum* anEnum = callContext.dynamic_token().is_enum();
	
	if (!anEnum) {
		ERRMSG("Expected enum for " << viz(*this));
		return false;
	}
	
	FogIterationDomainRef iterationDomain(new FogIterationDomain(FogMetaType::enumerator_type()), FogIterationDomainRef::ADOPT);
	
	for (FogEnumeratorConstListOfRefIterator p(anEnum->enumerators()); p; ++p)
		iterationDomain->add(*p);
		
	returnValue.assign(*iterationDomain);
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_Enumerator_Value, number, "value", IS_ENCAPSULATED) {
	const FogEnumerator* anEnumerator = callContext.dynamic_token().is_enumerator();
	
	if (!anEnumerator) {
		ERRMSG("Expected enumerator for " << viz(*this));
		return false;
	}
	
	return anEnumerator->get_number_token(returnValue, callContext);
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_Function_IsPureVirtual, bool, "is_pure_virtual", IS_ENCAPSULATED) {
	FogFunction* aFunction = callContext.get_function();
	
	if (!aFunction)
		return false;
		
	returnValue.assign(FogNumber::make_logical(aFunction->get_virtual().is_pure_virtual()));
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_Function_IsVirtual, bool, "is_virtual", IS_ENCAPSULATED) {
	FogFunction* aFunction = callContext.get_function();
	
	if (!aFunction)
		return false;
		
	returnValue.assign(FogNumber::make_logical(aFunction->get_virtual().is_virtual()));
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_Function_Parameters, parameter, "parameters", IS_EXPOSED) {
	FogFunction* aFunction = callContext.get_function();
	
	if (!aFunction)
		return false;
		
	FogIterationDomainRef iterationDomain(new FogIterationDomain(FogMetaType::parameter_type()), FogIterationDomainRef::ADOPT);
	
	for (FogExprConstListOfRefIterator p(aFunction->parameters()); p; ++p) {
		FogExpr& aParameter = *p;
		iterationDomain->add(aParameter);
	}
	
	returnValue.assign(*iterationDomain);
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_Function_Return, string, "return", IS_ENCAPSULATED) {
	FogFunction* aFunction = callContext.get_function();
	
	if (!aFunction)
		return false;
		
	const FogObjectSpecifier* aSpecifier = aFunction->specifier().is_object_specifier();
	
	if (!aSpecifier) {
		ERRMSG("Expected object specifier for " << viz(*this));
		return false;
	}
	
	const FogName* typeName = aSpecifier->get_type_name();
	const FogName* typeId = typeName; //   ? &typeName->id() : 0;
	static const FogBuiltInTypeId& voidType = FogBuiltInTypeId::void_type();
	static FogString& returnString = FogString::make("return", 6, FogString::NARROW);
	returnValue.assign((typeId == &voidType) ? returnString : FogString::mutable_null());
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_Function_Signature, string, "signature", IS_ENCAPSULATED) {
	FogFunction* aFunction = callContext.get_function();
	
	if (!aFunction)
		return false;
		
	returnValue.assign(FogString::make(aFunction->local_signature_id(), FogString::NARROW));
	return true;
}

#if 0
FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_Iterator_Next, void, "next", IS_ENCAPSULATED) {
	FogMetaEntity* metaEntity = callContext.dynamic_token().is_meta_entity();
	
	if (!metaEntity) {
		ERRMSG("Expected meta-entity for " << viz(*this));
		return false;
	}
	
	FogScopeContext scopeContext(metaEntity->inner_scope());
	FogMakeContext makeContext(scopeContext, metaEntity->short_id(), FogTemplateArgs::immutable_null());
	metaEntity->increment(makeContext);
	returnValue.reset();
	return true;
}
#endif

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_Iterator_Token, token, "token", IS_ENCAPSULATED) {
	FogMetaIteratorEntity* metaIterator = callContext.dynamic_token().is_meta_iterator_entity();
	
	if (!metaIterator) {
		ERRMSG("Expected iterator for " << viz(*this));
		return false;
	}
	
	if (!metaIterator->is_ok()) {
		ERRMSG("BUG - out of domain for " << viz(*this));
		return false;
	}
	
	returnValue.assign(metaIterator->token());
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_Object_IsConst, bool, "is_const", IS_ENCAPSULATED) {
	const FogDerivable* aDerivable = callContext.dynamic_token().is_derivable();
	bool isResult = aDerivable && aDerivable->decl_specifiers().is_const();
	returnValue.assign(FogNumber::make_logical(isResult));
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_Object_IsExtern, bool, "is_extern", IS_ENCAPSULATED) {
	const FogDerivable* aDerivable = callContext.dynamic_token().is_derivable();
	bool isResult = aDerivable && aDerivable->decl_specifiers().is_extern();
	returnValue.assign(FogNumber::make_logical(isResult));
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_Object_IsStatic, bool, "is_static", IS_ENCAPSULATED) {
	const FogDerivable* aDerivable = callContext.dynamic_token().is_derivable();
	bool isResult = aDerivable && aDerivable->decl_specifiers().is_static();
	returnValue.assign(FogNumber::make_logical(isResult));
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_Object_IsVolatile, bool, "is_volatile", IS_ENCAPSULATED) {
	const FogDerivable* aDerivable = callContext.dynamic_token().is_derivable();
	bool isResult = aDerivable && aDerivable->decl_specifiers().is_volatile();
	returnValue.assign(FogNumber::make_logical(isResult));
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_Object_Specifier, specifier, "specifier", IS_ENCAPSULATED) {
	const FogDerivable* aDerivable = callContext.dynamic_token().is_derivable();
	
	if (!aDerivable)
		return false;
		
	const FogObjectSpecifier& aSpecifier = aDerivable->specifier();
	FogObjectSpecifier* newSpecifier = aSpecifier.derived_clone();
	
	if (newSpecifier)
		newSpecifier->instantiate();
		
	returnValue.adopt(newSpecifier);
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_Object_Type, type, "type", IS_ENCAPSULATED) {
	const FogDerivable* aDerivable = callContext.dynamic_token().is_derivable();
	
	if (!aDerivable)
		return false;
		
	const FogObjectSpecifier& aSpecifier = aDerivable->specifier();
	const FogName* typeName = aSpecifier.get_type();
	
	if (typeName) {
		const PrimId* typeId = typeName->is_resolved();         //  .bugbug scoping and templating
		
		if (!typeId) {
			ERRMSG("Failed to resolve type name " << *typeId << " for " << viz(*this));
			return false;
		}
		
		FogEntity* theType = callContext.dynamic_scope().find_type(*typeId, IN_ANY_SCOPE);
		
		if (!theType) {
			ERRMSG("Failed to locate type " << *typeId << " for " << viz(*this));
			return false;
		}
		
		returnValue.assign(*theType);
	}
	else {
		ERRMSG("BUG - no type for " << viz(*this));
		return false;
	}
	
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_Object_DeriveType, type, "derive_type", IS_ENCAPSULATED) {
	FogDerivable* aDerivable = callContext.dynamic_token().is_derivable();
	
	if (!aDerivable)
		return false;
		
	const FogObjectSpecifier& aSpecifier = aDerivable->specifier(callContext.calling_context());
	const FogName* typeName = aSpecifier.get_type();
	
	if (typeName) {
		const PrimId* typeId = typeName->is_resolved();         //  .bugbug scoping and templating
		
		if (!typeId) {
			ERRMSG("Failed to resolve type name " << *typeId << " for " << viz(*this));
			return false;
		}
		
		FogEntity* theType = callContext.dynamic_scope().find_type(*typeId, IN_ANY_SCOPE);
		
		if (!theType) {
			ERRMSG("Failed to locate type " << *typeId << " for " << viz(*this));
			return false;
		}
		
		returnValue.assign(*theType);
	}
	else {
		ERRMSG("BUG - no type for " << viz(*this));
		return false;
	}
	
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_ObjectStatement_IsBoundary, bool, "is_boundary", IS_ENCAPSULATED) {
	const FogPotentialDeclaration* potentialDeclaration = callContext.calling_context().dynamic_token().is_potential_declaration();
	
	if (!potentialDeclaration) {
		ERRMSG("Expected potential declaration for " << viz(*this));
		return false;
	}
	
	const FogLazyBool& lazyBool = potentialDeclaration->is_boundary();
	
	if (lazyBool.is_undecided())
		return false;
		
	returnValue.assign(FogNumber::make_logical(lazyBool.is_true()));
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_ObjectStatement_IsLeaf, bool, "is_leaf", IS_ENCAPSULATED) {
	const FogPotentialDeclaration* potentialDeclaration = callContext.calling_context().dynamic_token().is_potential_declaration();
	
	if (!potentialDeclaration) {
		ERRMSG("Expected potential declaration for " << viz(*this));
		return false;
	}
	
	const FogLazyBool& lazyBool = potentialDeclaration->is_leaf();
	
	if (lazyBool.is_undecided())
		return false;
		
	returnValue.assign(FogNumber::make_logical(lazyBool.is_true()));
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_ObjectStatement_IsPure, bool, "is_pure", IS_ENCAPSULATED) {
	const FogPotentialDeclaration* potentialDeclaration = callContext.calling_context().dynamic_token().is_potential_declaration();
	
	if (!potentialDeclaration) {
		ERRMSG("Expected potential declaration for " << viz(*this));
		return false;
	}
	
	const FogLazyBool& lazyBool = potentialDeclaration->is_pure();
	
	if (lazyBool.is_undecided())
		return false;
		
	returnValue.assign(FogNumber::make_logical(lazyBool.is_true()));
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_ObjectStatement_IsRoot, bool, "is_root", IS_ENCAPSULATED) {
	const FogPotentialDeclaration* potentialDeclaration = callContext.calling_context().dynamic_token().is_potential_declaration();
	
	if (!potentialDeclaration) {
		ERRMSG("Expected potential declaration for " << viz(*this));
		return false;
	}
	
	bool isResult = potentialDeclaration->is_root();
	returnValue.assign(FogNumber::make_logical(isResult));
	return true;
}

FOGMETAVARIABLE_CLASS_METHOD(FogMetaType_ObjectStatement_Scope, scope, "Scope", IS_ENCAPSULATED) {
	if (inScope.is_unresolvable())
		return false;
		
	FogPotentialDeclaration* potentialDeclaration = inScope.dynamic_token().is_potential_declaration();
	
	if (!potentialDeclaration) {
		ERRMSG("Expected potential declaration for " << viz(*this));
		return false;
	}
	
	returnValue.assign(potentialDeclaration->inner_scope());
	return true;
}

FOGMETAVARIABLE_CLASS_METHOD(FogMetaType_ObjectStatement_This, entity, "This", IS_ENCAPSULATED) {
	FogPotentialDeclaration* potentialDeclaration = inScope.dynamic_token().is_potential_declaration();
	
	if (!potentialDeclaration) {
		ERRMSG("Expected potential declaration for " << viz(*this));
		return false;
	}
	
	returnValue.assign(potentialDeclaration->entity());
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_Scope_Functions, function, "functions", IS_EXPOSED) {
	FogScope* theScope = callContext.get_scope();
	
	if (!theScope)
		return false;
		
	FogIterationDomainRef iterationDomain(new FogIterationDomain(FogMetaType::function_type()), FogIterationDomainRef::ADOPT);
	
	for (FogFunctionConstListOfRefIterator p(theScope->distinct_function_list()); p; ++p)
		if (p->is_active())
			iterationDomain->add(*p);
			
	returnValue.assign(*iterationDomain);
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_Scope_ClassKey, class_key, "class_key", IS_ENCAPSULATED) {
	FogScope* theScope = callContext.get_scope();
	
	if (!theScope)
		return false;
		
	returnValue.assign(theScope->tag().mutate());
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_Scope_IsAuto, bool, "is_auto", IS_ENCAPSULATED) {
	FogScope* theScope = callContext.get_scope();
	
	if (!theScope)
		return false;
		
	returnValue.assign(FogNumber::make_logical(theScope->is_auto()));
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_Scope_IsClass, bool, "is_class", IS_ENCAPSULATED) {
	FogScope* theScope = callContext.get_scope();
	
	if (!theScope)
		return false;
		
	const FogTag& theTag = theScope->tag();
	returnValue.assign(FogNumber::make_logical(theTag.is_class_tag()));
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_Scope_IsFilespace, bool, "is_filespace", IS_ENCAPSULATED) {
	FogScope* theScope = callContext.get_scope();
	
	if (!theScope)
		return false;
		
	const FogTag& theTag = theScope->tag();
	returnValue.assign(FogNumber::make_logical(theTag.is_filespace_tag()));
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_Scope_IsLeaf, bool, "is_leaf", IS_ENCAPSULATED) {
	FogScope* theScope = callContext.get_scope();
	
	if (!theScope)
		return false;
		
	const FogLazyBool& lazyBool = theScope->is_leaf();
	
	if (lazyBool.is_undecided())
		return false;
		
	returnValue.assign(FogNumber::make_logical(lazyBool.is_true()));
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_Scope_IsLinkage, bool, "is_linkage", IS_ENCAPSULATED) {
	FogScope* theScope = callContext.get_scope();
	
	if (!theScope)
		return false;
		
	const FogTag& theTag = theScope->tag();
	returnValue.assign(FogNumber::make_logical(theTag.is_linkage_tag()));
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_Scope_IsNamespace, bool, "is_namespace", IS_ENCAPSULATED) {
	FogScope* theScope = callContext.get_scope();
	
	if (!theScope)
		return false;
		
	const FogTag& theTag = theScope->tag();
	returnValue.assign(FogNumber::make_logical(theTag.is_namespace_tag()));
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_Scope_IsStruct, bool, "is_struct", IS_ENCAPSULATED) {
	FogScope* theScope = callContext.get_scope();
	
	if (!theScope)
		return false;
		
	const FogTag& theTag = theScope->tag();
	returnValue.assign(FogNumber::make_logical(theTag.is_struct_tag()));
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_Scope_IsTypename, bool, "is_typename", IS_ENCAPSULATED) {
	FogScope* theScope = callContext.get_scope();
	
	if (!theScope)
		return false;
		
	const FogTag& theTag = theScope->tag();
	returnValue.assign(FogNumber::make_logical(theTag.is_typename_tag()));
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_Scope_IsUnion, bool, "is_union", IS_ENCAPSULATED) {
	FogScope* theScope = callContext.get_scope();
	
	if (!theScope)
		return false;
		
	const FogTag& theTag = theScope->tag();
	returnValue.assign(FogNumber::make_logical(theTag.is_union_tag()));
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_Scope_Variables, variable, "variables", IS_EXPOSED) {
	FogScope* theScope = callContext.get_scope();
	
	if (!theScope)
		return false;
		
	FogIterationDomainRef iterationDomain(new FogIterationDomain(FogMetaType::variable_type()), FogIterationDomainRef::ADOPT);
	
	for (FogVariableConstListOfRefIterator p(theScope->distinct_variable_list()); p; ++p)
		if (p->is_active())
			iterationDomain->add(*p);
			
	returnValue.assign(*iterationDomain);
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_ScopeSpecifier_IsClass, bool, "is_class", IS_ENCAPSULATED) {
	FogSpecifier* theSpecifier = callContext.get_specifier();
	
	if (!theSpecifier)
		return false;
		
	FogScopeSpecifier* scopeSpecifier = theSpecifier->is_scope_specifier();
	const FogTag& theTag = scopeSpecifier->tag();
	returnValue.assign(FogNumber::make_logical(theTag.is_class_tag()));
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_Specifier_Modifiers, modifier, "modifiers", IS_EXPOSED) {
	FogSpecifier* aSpecifier = callContext.dynamic_token().is_specifier();
	FogObjectSpecifier* objectSpecifier = aSpecifier ? aSpecifier->is_object_specifier() : 0;
	
	if (!aSpecifier) {
		ERRMSG("Expected object-specifier for " << viz(*this));
		return false;
	}
	
	FogIterationDomainRef iterationDomain(new FogIterationDomain(FogMetaType::modifier_type()), FogIterationDomainRef::ADOPT);
	
	for (FogModifierConstListOfRefIterator p(objectSpecifier->modifiers()); p; ++p) {
		FogModifier& aModifier = *p;
		iterationDomain->add(aModifier);
	}
	
	returnValue.assign(*iterationDomain);
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_Specifier_Name, identifier, "name", IS_ENCAPSULATED) {
	FogSpecifier* aSpecifier = callContext.get_specifier();
	
	if (!aSpecifier)
		return false;
		
	FogName& aName = aSpecifier->name();
	returnValue.assign(aName);
	return true;
}

// identical to following function
FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_Specifier_TypeName, identifier, "type_name", IS_EXPOSED) {
	FogSpecifier* aSpecifier = callContext.get_specifier();
	
	if (!aSpecifier)
		return false;
		
	const FogName* typeName = aSpecifier->get_type();
	
	if (typeName) {
		const PrimId* typeId = typeName->is_resolved();         //  .bugbug scoping and templating
		
		if (!typeId) {
			ERRMSG("Failed to resolve type name " << *typeId << " for " << viz(*this));
			return false;
		}
		
		FogEntity* theType = callContext.dynamic_scope().find_type(*typeId, IN_ANY_SCOPE);
		
		if (!theType) {
			ERRMSG("Failed to locate type " << *typeId << " for " << viz(*this));
			return false;
		}
		
		returnValue.assign(*theType);
	}
	else {
		ERRMSG("BUG - no type for " << viz(*this));
		return false;
	}
	
	return true;
}


FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_Specifier_Type, type, "type", IS_ENCAPSULATED) {
	FogSpecifier* aSpecifier = callContext.get_specifier();
	
	if (!aSpecifier)
		return false;
		
	const FogName* typeName = aSpecifier->get_type();
	
	if (typeName) {
		const PrimId* typeId = typeName->is_resolved();         //  .bugbug scoping and templating
		
		if (!typeId) {
			ERRMSG("Failed to resolve type name " << *typeId << " for " << viz(*this));
			return false;
		}
		
		FogEntity* theType = callContext.dynamic_scope().find_type(*typeId, IN_ANY_SCOPE);
		
		if (!theType) {
			ERRMSG("Failed to locate type " << *typeId << " for " << viz(*this));
			return false;
		}
		
		returnValue.assign(*theType);
	}
	else {
		ERRMSG("BUG - no type for " << viz(*this));
		return false;
	}
	
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_Token_Bases, base, "bases", IS_EXPOSED) {
	FogScope* theScope = callContext.get_scope();
	
	if (!theScope)
		return false;
		
	FogIterationDomainRef iterationDomain(new FogIterationDomain(FogMetaType::base_type()), FogIterationDomainRef::ADOPT);
	
	for (FogBaseClassConstListOfRefIterator p(theScope->direct_bases()); p; ++p) {
		FogBaseClass& aBase = *p;
		iterationDomain->add(aBase);
	}
	
	returnValue.assign(*iterationDomain);
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_Token_Enums, enum, "enums", IS_EXPOSED) {
	FogScope* theScope = callContext.get_scope();
	
	if (!theScope)
		return false;
		
	FogIterationDomainRef iterationDomain(new FogIterationDomain(FogMetaType::enum_type()), FogIterationDomainRef::ADOPT);
	
	for (FogEnumConstListOfRefIterator p(theScope->enums()); p; ++p)
		iterationDomain->add(*p);
		
	returnValue.assign(*iterationDomain);
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_Token_Id, identifier, "id", IS_ENCAPSULATED) {
	returnValue.assign(FogIdentifier::make(callContext.dynamic_token().short_id()));
	return true;
}

FOGMETAFUNCTION_CLASS_METHOD(FogMetaType_Token_Name, identifier, "name", IS_ENCAPSULATED) {
	returnValue.assign(FogIdentifier::make(callContext.dynamic_token().short_id()));
	return true;
}

FOGMETAVARIABLE_CLASS_METHOD(FogMetaType_Token_Namespace, namespace, "Namespace", IS_ENCAPSULATED) {
	returnValue.assign(inScope.dynamic_token().name_space());
	return true;
}

FOGMETAVARIABLE_CLASS_METHOD(FogMetaType_Token_OuterNamespace, namespace, "OuterNamespace", IS_ENCAPSULATED) {
	DIAMSG("INVESTIGATE -- using experimental implementation of FogMetaType_Token_OuterNamespace::resolve_object");
	returnValue.assign(inScope.dynamic_token().name_space().name_space());
	return true;
}

FOGMETAVARIABLE_CLASS_METHOD(FogMetaType_Token_OuterScope, scope, "OuterScope", IS_ENCAPSULATED) {
	DIAMSG("INVESTIGATE -- using experimental implementation of FogMetaType_Token_OuterScope::resolve_object");
	returnValue.assign(inScope.dynamic_token().inner_scope().outer_scope());
	return true;
}

FOGMETAVARIABLE_CLASS_METHOD(FogMetaType_Token_Scope, scope, "Scope", IS_ENCAPSULATED) {
	if (inScope.is_unresolvable())
		return false;
		
	returnValue.assign(inScope.dynamic_token().inner_scope());
	return true;
}

FOGMETAVARIABLE_CLASS_METHOD(FogMetaType_Token_Super, scope, "Super", IS_ENCAPSULATED) {
	FogScope& theScope = inScope.dynamic_token().inner_scope();
	
	for (FogBaseClassConstListOfRefIterator p(theScope.direct_bases()); p; ++p)
		if (!p->is_auto()) {
			FogScope& baseClass = p->base();
			returnValue.assign(baseClass);
			return true;
		}
		
	if (inScope.requires_resolution())
		ERRMSG("No base class to resolve Super for " << viz(*this));
		
	return false;
}

FOGMETAVARIABLE_CLASS_METHOD(FogMetaType_Token_This, entity, "This", IS_ENCAPSULATED) {
	returnValue.assign(inScope.dynamic_token());
	return true;
}


void FogMetaType::add_array_modifier_slots() {}
void FogMetaType::add_asm_definition_slots() {}

void FogMetaType::add_base_slots() {
	_slots.adopt(new FogMetaType_Base_Base(base_type()));
	_slots.adopt(new FogMetaType_Base_IsAuto(base_type()));
	_slots.adopt(new FogMetaType_Base_IsPrivate(base_type()));
	_slots.adopt(new FogMetaType_Base_IsProtected(base_type()));
	_slots.adopt(new FogMetaType_Base_IsPublic(base_type()));
	_slots.adopt(new FogMetaType_Base_IsVirtual(base_type()));
}

void FogMetaType::add_base_specifier_slots() {
	_slots.adopt(new FogMetaType_BaseSpecifier_Base(base_specifier_type()));
	_slots.adopt(new FogMetaType_BaseSpecifier_IsAuto(base_specifier_type()));
	_slots.adopt(new FogMetaType_BaseSpecifier_IsPrivate(base_specifier_type()));
	_slots.adopt(new FogMetaType_BaseSpecifier_IsProtected(base_specifier_type()));
	_slots.adopt(new FogMetaType_BaseSpecifier_IsPublic(base_specifier_type()));
	_slots.adopt(new FogMetaType_BaseSpecifier_IsVirtual(base_specifier_type()));
}

void FogMetaType::add_bool_slots() {}
void FogMetaType::add_built_in_slots() {}
void FogMetaType::add_built_in_type_specifier_slots() {}
void FogMetaType::add_character_slots() {}
void FogMetaType::add_class_slots() {}
void FogMetaType::add_class_key_slots() {}
void FogMetaType::add_class_specifier_slots() {}
void FogMetaType::add_cv_qualifier_slots() {}
void FogMetaType::add_decl_specifier_slots() {}
void FogMetaType::add_declaration_slots() {}
void FogMetaType::add_double_slots() {}
void FogMetaType::add_elaborated_type_specifier_slots() {}

void FogMetaType::add_entity_slots() {
	_slots.adopt(new FogMetaType_Entity_GlobalName(entity_type()));
	_slots.adopt(new FogMetaType_Token_Id(entity_type()));
	_slots.adopt(new FogMetaType_Entity_IsBoundary(scope_type()));
	_slots.adopt(new FogMetaType_Entity_IsPure(scope_type()));
	_slots.adopt(new FogMetaType_Entity_LongName(entity_type()));
	_slots.adopt(new FogMetaType_Token_Name(entity_type()));
	_slots.adopt(new FogMetaType_Token_Namespace(entity_type()));
	_slots.adopt(new FogMetaType_Token_OuterNamespace(entity_type()));
	_slots.adopt(new FogMetaType_Token_OuterScope(entity_type()));
	_slots.adopt(new FogMetaType_Token_Scope(entity_type()));
	_slots.adopt(new FogMetaType_Token_This(entity_type()));
}

void FogMetaType::add_enum_slots() {
	_slots.adopt(new FogMetaType_Enum_Enumerators(enum_type()));
}

void FogMetaType::add_enum_specifier_slots() {
	//    _slots.adopt(new FogMetaType_Enum_Enumerators(enum_specifier_type()));
}

void FogMetaType::add_enumerator_slots() {
	_slots.adopt(new FogMetaType_Enumerator_Value(enumerator_type()));
}

void FogMetaType::add_enumerator_definition_slots() {
	//    _slots.adopt(new FogMetaType_Enumerator_Value(xxx_type()));
}

void FogMetaType::add_exception_slots() {}
//  void FogMetaType::add_exception_declaration_slots() {}
void FogMetaType::add_exception_specification_slots() {}
void FogMetaType::add_expression_slots() {}
void FogMetaType::add_expression_statement_slots() {}
void FogMetaType::add_file_dependency_specifier_slots() {}
void FogMetaType::add_file_placement_specifier_slots() {}
void FogMetaType::add_filespace_slots() {}
void FogMetaType::add_filespace_specifier_slots() {}

void FogMetaType::add_function_slots() {
	//    add_entity_slots();
	//    add_object_slots();
	_slots.adopt(new FogMetaType_Function_IsPureVirtual(function_type()));
	_slots.adopt(new FogMetaType_Function_IsVirtual(function_type()));
	_slots.adopt(new FogMetaType_Function_Parameters(function_type()));
	_slots.adopt(new FogMetaType_Function_Return(function_type()));
	_slots.adopt(new FogMetaType_Function_Signature(function_type()));
}

void FogMetaType::add_function_body_slots() {}
void FogMetaType::add_function_modifier_slots() {}
void FogMetaType::add_function_specifier_slots() {}
void FogMetaType::add_handler_slots() {}
void FogMetaType::add_identifier_slots() {}
void FogMetaType::add_initializer_clause_slots() {}

void FogMetaType::add_iterator_slots() {
	//    _slots.adopt(new FogMetaType_Iterator_Next(iterator_type()));
	_slots.adopt(new FogMetaType_Iterator_Token(iterator_type()));
}

void FogMetaType::add_keyword_slots() {}
void FogMetaType::add_linkage_slots() {}
void FogMetaType::add_linkage_specification_slots() {}
void FogMetaType::add_meta_class_slots() {}
void FogMetaType::add_meta_class_specifier_slots() {}
void FogMetaType::add_meta_function_slots() {}
void FogMetaType::add_meta_function_specifier_slots() {}
void FogMetaType::add_meta_parameter_slots() {}
void FogMetaType::add_meta_parameter_specifier_slots() {}
void FogMetaType::add_meta_type_slots() {}
void FogMetaType::add_meta_variable_slots() {}
void FogMetaType::add_meta_variable_specifier_slots() {}
void FogMetaType::add_modifier_slots() {}
void FogMetaType::add_name_slots() {}
void FogMetaType::add_namespace_slots() {}
void FogMetaType::add_namespace_alias_slots() {}
void FogMetaType::add_namespace_alias_definition_slots() {}
void FogMetaType::add_namespace_definition_slots() {}
void FogMetaType::add_number_slots() {}

void FogMetaType::add_object_slots() {
	_slots.adopt(new FogMetaType_Object_IsConst(object_type()));
	_slots.adopt(new FogMetaType_Object_IsExtern(object_type()));
	_slots.adopt(new FogMetaType_Object_IsStatic(object_type()));
	_slots.adopt(new FogMetaType_Object_IsVolatile(object_type()));
	_slots.adopt(new FogMetaType_Object_Specifier(object_type()));
	_slots.adopt(new FogMetaType_Object_Type(object_type()));
	_slots.adopt(new FogMetaType_Object_DeriveType(object_type()));
}

void FogMetaType::add_object_specifier_slots() {
	//    _slots.adopt(new FogMetaType_ObjectSpecifier_IsRoot(object_specifier_type()));
}

void FogMetaType::add_object_statement_slots() {
	_slots.adopt(new FogMetaType_ObjectStatement_IsBoundary(object_statement_type()));
	_slots.adopt(new FogMetaType_ObjectStatement_IsLeaf(object_statement_type()));
	_slots.adopt(new FogMetaType_ObjectStatement_IsPure(object_statement_type()));
	_slots.adopt(new FogMetaType_ObjectStatement_IsRoot(object_statement_type()));
	_slots.adopt(new FogMetaType_ObjectStatement_Scope(object_statement_type()));
	_slots.adopt(new FogMetaType_ObjectStatement_This(object_statement_type()));
}

void FogMetaType::add_parameter_slots() {}
void FogMetaType::add_parameter_specifier_slots() {}

void FogMetaType::add_pointer_modifier_slots() {}
void FogMetaType::add_punctuation_slots() {}
void FogMetaType::add_reference_modifier_slots() {}
void FogMetaType::add_reserved_slots() {}

void FogMetaType::add_scope_slots() {
	_slots.adopt(new FogMetaType_Scope_ClassKey(scope_type()));
	_slots.adopt(new FogMetaType_Scope_Functions(scope_type()));
	_slots.adopt(new FogMetaType_Scope_IsAuto(scope_type()));
	_slots.adopt(new FogMetaType_Scope_IsClass(scope_type()));
	_slots.adopt(new FogMetaType_Scope_IsFilespace(scope_type()));
	_slots.adopt(new FogMetaType_Scope_IsLeaf(scope_type()));
	_slots.adopt(new FogMetaType_Scope_IsLinkage(scope_type()));
	_slots.adopt(new FogMetaType_Scope_IsNamespace(scope_type()));
	_slots.adopt(new FogMetaType_Scope_IsStruct(scope_type()));
	_slots.adopt(new FogMetaType_Scope_IsTypename(scope_type()));
	_slots.adopt(new FogMetaType_Scope_IsUnion(scope_type()));
	_slots.adopt(new FogMetaType_Scope_Variables(scope_type()));
}

void FogMetaType::add_scope_specifier_slots() {
	_slots.adopt(new FogMetaType_ScopeSpecifier_IsClass(scope_specifier_type()));
}

void FogMetaType::add_scoped_modifier_slots() {}
void FogMetaType::add_signed_slots() {}

void FogMetaType::add_specifier_slots() {
	_slots.adopt(new FogMetaType_Specifier_Modifiers(specifier_type()));
	_slots.adopt(new FogMetaType_Specifier_Name(specifier_type()));
	_slots.adopt(new FogMetaType_Specifier_Type(specifier_type()));
}

void FogMetaType::add_statement_slots() {}
void FogMetaType::add_string_slots() {}
void FogMetaType::add_struct_slots() {}
void FogMetaType::add_template_argument_slots() {}
//  void FogMetaType::add_template_parameter_slots() {}
void FogMetaType::add_template_parameter_specifier_slots() {}
void FogMetaType::add_templated_parameter_specifier_slots() {}

void FogMetaType::add_token_slots() {
	_slots.adopt(new FogMetaType_Token_Bases(token_type()));
	_slots.adopt(new FogMetaType_Token_Enums(token_type()));
	_slots.adopt(new FogMetaType_Token_Super(token_type()));
}

void FogMetaType::add_tree_literal_slots() {
}

void FogMetaType::add_type_slots() {
	//    _slots.adopt(new FogMetaType_Token_Id(type_type()));
	//    _slots.adopt(new FogMetaType_Token_Name(type_type()));
}

//  void FogMetaType::add_type_parameter_slots() {}
void FogMetaType::add_type_parameter_specifier_slots() {}
void FogMetaType::add_typedef_slots() {}
void FogMetaType::add_typedef_specifier_slots() {}
void FogMetaType::add_type_specifier_slots() {}
void FogMetaType::add_typename_slots() {}
void FogMetaType::add_union_slots() {}
void FogMetaType::add_unsigned_slots() {}
void FogMetaType::add_using_slots() {}
void FogMetaType::add_using_declaration_slots() {}
void FogMetaType::add_using_directive_slots() {}
//  void FogMetaType::add_value_parameter_slots() {}
void FogMetaType::add_value_parameter_specifier_slots() {}
void FogMetaType::add_variable_slots() {}
void FogMetaType::add_variable_specifier_slots() {}
void FogMetaType::add_void_slots() {}
