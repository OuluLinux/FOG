#include <Fog/FogIncludeAll.h>















TYPEINFO_SINGLE(FogTypedName, Super)
FOGTOKEN_LEAF_IMPL(FogTypedName)

FogTypedName::FogTypedName(FogName& typeName, FogName& nameName)
		:
		Super(nameName),
		_type(typeName) {}
		
FogTypedName::~FogTypedName() {}

FogName *FogTypedName::append_name(FogNameRef& frontName, FogName& backName) {
	FogNameRef *pTypedName = &frontName;
	FogTypedName *typedName = 0;
	
	while ((typedName = (*pTypedName)->is_typed_name()) != 0)
		pTypedName = &typedName->name_ref();
		
	pTypedName->adopt(new FogTypedName(**pTypedName, backName));
	
	return frontName.pointer();
}

void FogTypedName::create_usage(FogUsageContext& aContext) const {
	Super::create_usage(aContext);
	_type->create_usage(aContext);
}

const FogDeclSpecifierValue& FogTypedName::decl_specifiers() const {
	return _type->decl_specifiers();
}

bool FogTypedName::emit(FogEmitContext& emitContext) const {
	bool doneSomething = false;
	{
		const FogForUseAs forUseAs(emitContext.for_use_as() | FogEmitContext::NO_SCOPE);
		FogForUseAsEmitContext changedContext(emitContext, forUseAs);
		doneSomething |= _type->emit(changedContext);
	}
	
	if (emitContext.emit_declarator())
		doneSomething |= name().emit(emitContext);
		
	return doneSomething;
}

FogName *FogTypedName::get_type() {
	return &type();
}

void FogTypedName::install(FogInstallContext& installContext) const {
	Super::install(installContext);
	_type->install(installContext);
}

bool FogTypedName::is_actual(const FogScopeContext& scopeContext) const {
	return Super::is_actual(scopeContext) && _type->is_actual(scopeContext);
}

FogTypedName *FogTypedName::is_typed_name() {
	return this;
}

void FogTypedName::make_actual_from(FogMakeActualContext& makeActualContext) {
	Super::make_actual_from(makeActualContext);
	_type->make_actual(_type.to_const(), makeActualContext);
}

bool FogTypedName::make_specifier(FogMakeSpecifierContext& makeSpecifierContext) {
	if (!name().make_specifier(makeSpecifierContext))
		return false;
		
	makeSpecifierContext.specifier().set_decl_specifiers(decl_specifiers());
	
//  	if (makeSpecifierContext.decl_specifiers().is_using())
//  		return true;
	if (makeSpecifierContext.semantics().is_parameter()) //   Parameters are part of signature and need resolution
//  	 || makeSpecifierContext.semantics().is_meta())			// Meta statements need resolution now
		return _type->make_type_specifier(makeSpecifierContext);
		
	if (!makeSpecifierContext.is_resolve_dollars())
		ERRMSG("INVESTIGATE -- unexpected non-is_resolves_dollars in FogTypedName::make_specifier.");
		
//  	FogResolutionMakeSpecifierContext resolutionContext(makeSpecifierContext, FogScopeContext::RESOLVE_DOLLARS);
//  	return _type->make_type_specifier(resolutionContext);	// Types are not and so can (should) be deferred
	return _type->make_type_specifier(makeSpecifierContext);
}

bool FogTypedName::make_type_specifier(FogMakeSpecifierContext& makeSpecifierContext) {
	/*if (!_type)
		return false;
	return _type->make_type_specifier(makeSpecifierContext);*/
	return false;
}

bool FogTypedName::make_typed_expression(FogExprRef& theExpr, FogName& theType) {
	const FogDeclSpecifier *declSpecifier = theType.is_decl_specifier();
	
	if (declSpecifier) {
		FogNameRef theName;
		set_decl_specifier_name(theName, *declSpecifier);
	}
	
	else
		theExpr.adopt(new FogTypedName(theType, *this));
		
	return true;
}

void FogTypedName::merge_from(FogMergeContext& mergeContext, const This& anExpr) {
	Super::merge_from(mergeContext, anExpr);
	anExpr._type->merge_into(mergeContext, _type.to_const());
}

const FogMerge& FogTypedName::needs_merge_from(FogMergeContext& mergeContext, const This& anExpr) const {
	FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, anExpr));
	needsMerge |= _type->needs_merge(mergeContext, *anExpr._type);
	return needsMerge;
}

std::ostream& FogTypedName::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	_type->print_deep(s, aDepth);
	return s;
}

std::ostream& FogTypedName::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	_type->print_on(s, aDepth);
	return s;
}

char FogTypedName::print_named(std::ostream& s, const PrimId *fullId, char tailChar) const {
	tailChar = _type->print_named(s, fullId, tailChar);
	tailChar = FogStream::space_and_emit(s, tailChar, " ");
	tailChar = Super::print_named(s, fullId, tailChar);
	return tailChar;
}

std::ostream& FogTypedName::print_viz(std::ostream& s) const {
	s << "\"typed-name: ";
	print_this(s);
	return s << '\"';
}

bool FogTypedName::resolve_semantics(FogSemanticsContext& theSemantics) const {
	FogChildSemanticsContext typeSemantics(theSemantics);
	FogChildSemanticsContext nameSemantics(theSemantics);
	
	if (!_type->resolve_semantics(typeSemantics) || !name().resolve_semantics(nameSemantics))
		return false;
		
	if (typeSemantics.is_scoped_identifier() && !typeSemantics.is_scoped_type_name() && !typeSemantics.is_scoped_meta_name()) {          //  .bugbug a bit wider than just ::id
		ERRMSG("Type name expected in " << viz(*this) << ".");
//  		return false;
	}
	
	theSemantics.reset();
	
	if (typeSemantics.is_typed_decl_specifier_seq()) {
		if (nameSemantics.is_bit_field_declarator_id_opt())
			theSemantics.set(FogSemantics::IS_BIT_FIELD_TYPE_ID);
			
		if (nameSemantics.is_function_declarator_id())
			theSemantics.set(FogSemantics::IS_FUNCTION_DECLARATION_ID);
			
		if (nameSemantics.is_function_declarator())
			theSemantics.set(FogSemantics::IS_FUNCTION_TYPE_ID);
			
		if (nameSemantics.is_init_declarator_list())
			theSemantics.set(FogSemantics::IS_INIT_DECLARATION_LIST);
			
		if (nameSemantics.is_object_declarator_id())
			theSemantics.set(FogSemantics::IS_OBJECT_TYPE_ID);
			
		if (nameSemantics.is_parameter_declarator_opt())
			theSemantics.set(FogSemantics::IS_PARAMETER_TYPE_ID);
			
		if (nameSemantics.is_typed_decl_specifier_seq())
			theSemantics.set(FogSemantics::IS_TYPED_DECL_SPECIFIER_SEQ);
			
		if (decl_specifiers().is_typedef()) {
			if (nameSemantics.is_object_declarator_id())
				theSemantics.set(FogSemantics::IS_TYPEDEF_DECLARATION);
				
			if (nameSemantics.is_template_name())
				theSemantics.set(FogSemantics::IS_TYPEDEF_DECLARATION);
		}
		
		if (nameSemantics.is_using_decl_specifier_seq())
			theSemantics.set(FogSemantics::IS_USING_NAME_DECL_SPECIFIER_SEQ);
	}
	
	if (typeSemantics.is_using_decl_specifier_seq()) {
		if (nameSemantics.is_constructor_name() || nameSemantics.is_function_name())
			theSemantics.set(FogSemantics::IS_USING_DECLARATIVE);
	}
	
	if (typeSemantics.is_using_name_decl_specifier_seq()) {
		if (nameSemantics.is_bit_field_declarator_id_opt())
			theSemantics.set(FogSemantics::IS_BIT_FIELD_TYPE_ID);
			
		if (nameSemantics.is_function_declarator_id())
			theSemantics.set(FogSemantics::IS_FUNCTION_TYPE_ID);
			
		if (nameSemantics.is_object_declarator_id())
			theSemantics.set(FogSemantics::IS_OBJECT_DECLARATION);
	}
	
	if (typeSemantics.is_decl_specifier_seq()) {
		if (nameSemantics.is_using_decl_specifier_seq())
			theSemantics.set(FogSemantics::IS_USING_DECL_SPECIFIER_SEQ);
	}
	
	if (typeSemantics.is_type_specifier_seq()) {
		if (nameSemantics.is_conversion_declarator_opt())
			theSemantics.set(FogSemantics::IS_CONVERSION_TYPE_ID);
			
		if (nameSemantics.is_type_id_declarator_opt())
			theSemantics.set(FogSemantics::IS_TYPE_ID);
			
		if (nameSemantics.is_type_specifier_seq())
			theSemantics.set(FogSemantics::IS_TYPE_SPECIFIER_SEQ);
	}
	
	if (typeSemantics.is_unmeta_decl_specifier_seq()) {
		if (nameSemantics.is_unmeta_decl_specifier_seq())
			theSemantics.set(FogSemantics::IS_UNMETA_DECL_SPECIFIER_SEQ);
			
		if (nameSemantics.is_meta_decl_specifier_seq())
			theSemantics.set(FogSemantics::IS_META_DECL_SPECIFIER_SEQ);
			
		if (nameSemantics.is_auto_decl_specifier_seq())
			theSemantics.set(FogSemantics::IS_AUTO_DECL_SPECIFIER_SEQ);
			
		if (nameSemantics.is_auto_meta_decl_specifier_seq())
			theSemantics.set(FogSemantics::IS_AUTO_META_DECL_SPECIFIER_SEQ);
	}
	
	if (typeSemantics.is_meta_decl_specifier_seq()) {
		if (nameSemantics.is_unmeta_decl_specifier_seq())
			theSemantics.set(FogSemantics::IS_META_DECL_SPECIFIER_SEQ);
			
		if (nameSemantics.is_auto_decl_specifier_seq())
			theSemantics.set(FogSemantics::IS_AUTO_META_DECL_SPECIFIER_SEQ);
			
		if (nameSemantics.is_meta_variable_declarator_elem())
			theSemantics.set(FogSemantics::IS_META_VARIABLE_DECLARATION_ELEM);
			
		if (nameSemantics.is_meta_variable_declarator_elem())
			theSemantics.set(FogSemantics::IS_META_VARIABLE_TYPE_ID);
			
		if (nameSemantics.is_meta_variable_declarator())
			theSemantics.set(FogSemantics::IS_META_VARIABLE_DECLARATION);
			
		if (nameSemantics.is_auto_variable_declaration_elem())
			theSemantics.set(FogSemantics::IS_AUTO_VARIABLE_TYPE_ID);
			
		if (nameSemantics.is_auto_variable_declarator())
			theSemantics.set(FogSemantics::IS_AUTO_VARIABLE_DECLARATION);
			
		if (nameSemantics.is_meta_function_declarator_elem())
			theSemantics.set(FogSemantics::IS_META_FUNCTION_DECLARATION_ELEM);
			
		if (nameSemantics.is_meta_function_declarator())
			theSemantics.set(FogSemantics::IS_META_FUNCTION_DECLARATION);
			
		if (nameSemantics.is_meta_function_definitor())
			theSemantics.set(FogSemantics::IS_META_FUNCTION_DEFINITION);
			
		if (nameSemantics.is_auto_function_declarator())
			theSemantics.set(FogSemantics::IS_AUTO_FUNCTION_DECLARATION);
			
		if (nameSemantics.is_auto_function_definitor())
			theSemantics.set(FogSemantics::IS_AUTO_FUNCTION_DEFINITION);
	}
	
	if (typeSemantics.is_auto_decl_specifier_seq()) {
		if (nameSemantics.is_unmeta_decl_specifier_seq())
			theSemantics.set(FogSemantics::IS_AUTO_DECL_SPECIFIER_SEQ);
			
		if (nameSemantics.is_meta_decl_specifier_seq())
			theSemantics.set(FogSemantics::IS_AUTO_META_DECL_SPECIFIER_SEQ);
			
		if (nameSemantics.is_meta_ctor_name())
			theSemantics.set(FogSemantics::IS_AUTO_CTOR_NAME);
			
		if (nameSemantics.is_meta_ctor_declaration())
			theSemantics.set(FogSemantics::IS_AUTO_CTOR_DECLARATION);
			
		if (nameSemantics.is_meta_ctor_definition())
			theSemantics.set(FogSemantics::IS_AUTO_CTOR_DEFINITION);
			
		if (nameSemantics.is_meta_dtor_name())
			theSemantics.set(FogSemantics::IS_AUTO_DTOR_NAME);
			
		if (nameSemantics.is_meta_dtor_declaration())
			theSemantics.set(FogSemantics::IS_AUTO_DTOR_DECLARATION);
			
		if (nameSemantics.is_meta_dtor_definition())
			theSemantics.set(FogSemantics::IS_AUTO_DTOR_DEFINITION);
			
		if (nameSemantics.is_meta_function_declaration_elem())
			theSemantics.set(FogSemantics::IS_AUTO_FUNCTION_DECLARATION_ELEM);
			
		if (nameSemantics.is_meta_function_declarator())
			theSemantics.set(FogSemantics::IS_AUTO_FUNCTION_DECLARATOR);
			
		if (nameSemantics.is_meta_function_declaration())
			theSemantics.set(FogSemantics::IS_AUTO_FUNCTION_DECLARATION);
			
		if (nameSemantics.is_meta_function_definitor())
			theSemantics.set(FogSemantics::IS_AUTO_FUNCTION_DEFINITOR);
			
		if (nameSemantics.is_meta_function_definition())
			theSemantics.set(FogSemantics::IS_AUTO_FUNCTION_DEFINITION);
			
		if (nameSemantics.is_meta_variable_declarator())
			theSemantics.set(FogSemantics::IS_AUTO_VARIABLE_DECLARATOR);
			
		if (nameSemantics.is_meta_variable_declarator_elem())
			theSemantics.set(FogSemantics::IS_AUTO_VARIABLE_DECLARATION_ELEM);
			
		if (nameSemantics.is_meta_variable_type_id())
			theSemantics.set(FogSemantics::IS_AUTO_VARIABLE_TYPE_ID);
			
		if (nameSemantics.is_meta_variable_declaration())
			theSemantics.set(FogSemantics::IS_AUTO_VARIABLE_DECLARATION);
	}
	
	if (typeSemantics.is_auto_meta_decl_specifier_seq()) {
		if (nameSemantics.is_unmeta_decl_specifier_seq())
			theSemantics.set(FogSemantics::IS_AUTO_META_DECL_SPECIFIER_SEQ);
			
		if (nameSemantics.is_meta_function_declarator_elem())
			theSemantics.set(FogSemantics::IS_AUTO_FUNCTION_DECLARATION_ELEM);
			
		if (nameSemantics.is_meta_function_declarator())
			theSemantics.set(FogSemantics::IS_AUTO_FUNCTION_DECLARATION);
			
		if (nameSemantics.is_meta_function_definitor())
			theSemantics.set(FogSemantics::IS_AUTO_FUNCTION_DEFINITION);
			
		if (nameSemantics.is_meta_variable_declarator_elem())
			theSemantics.set(FogSemantics::IS_AUTO_VARIABLE_TYPE_ID);
			
		if (nameSemantics.is_meta_variable_declarator())
			theSemantics.set(FogSemantics::IS_AUTO_VARIABLE_DECLARATION);
	}
	
#if 0
	if (typeSemantics.is_meta_decl_specifier_seq()) {
//  		if (decl_specifiers().is_auto() && nameSemantics.is_scoped_meta_name())
		if (nameSemantics.is_scoped_meta_name())
			theSemantics.set(FogSemantics::IS_META_OBJECT_NAME);
			
		if (nameSemantics.is_meta_variable_declarator_elem())
			theSemantics.set(FogSemantics::IS_META_VARIABLE_TYPE_ID);
			
		if (nameSemantics.is_meta_variable_declarator())
			theSemantics.set(FogSemantics::IS_META_VARIABLE_DECLARATION);
			
		if (nameSemantics.is_meta_function_declarator_elem())
			theSemantics.set(FogSemantics::IS_META_FUNCTION_DECLARATION_ELEM);
			
		if (nameSemantics.is_meta_function_declarator())
			theSemantics.set(FogSemantics::IS_META_FUNCTION_DECLARATION);
			
		if (nameSemantics.is_meta_function_definitor())
			theSemantics.set(FogSemantics::IS_META_FUNCTION_DEFINITION);
			
		if (nameSemantics.is_meta_decl_specifier_seq())
			theSemantics.set(FogSemantics::IS_META_DECL_SPECIFIER_SEQ);
	}
	
#endif
	if (typeSemantics.is_scoped_meta_type_name()) {
		if (nameSemantics.is_meta_parameter_name())
			theSemantics.set(FogSemantics::IS_META_PARAMETER_DECLARATION_ELEM);
			
		if (nameSemantics.is_meta_parameter_declarator_elem())
			theSemantics.set(FogSemantics::IS_META_PARAMETER_TYPE_ID);
			
		if (nameSemantics.is_meta_parameter_declarator())
			theSemantics.set(FogSemantics::IS_META_PARAMETER_DECLARATION);
	}
	
	if (nameSemantics.is_epsilon())
		theSemantics.assign(typeSemantics);
		
	return true;
}

void FogTypedName::set_decl_specifier_name(FogNameRef& aName, const FogDeclSpecifier& declSpecifiers) {
	_type->set_decl_specifier_name(_type, declSpecifiers);
	aName = this;
}
