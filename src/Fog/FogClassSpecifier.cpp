#include <Fog/FogIncludeAll.h>













TYPEINFO_SINGLE(FogClassSpecifier, Super)
FOGTOKEN_READ_ONLY_IMPL(FogClassSpecifier)

FogClassSpecifier::FogClassSpecifier(const FogTag& aTag, FogName *aName, FogBaseSpecifiers *baseSpecifiers)
		:
		Super(aTag, aName ? *aName : FogName::make_anon(),
			  aTag.is_class_tag() ? FogAccess::private_access() : FogAccess::public_access()),
		_bases(baseSpecifiers) {}
		
FogClassSpecifier::FogClassSpecifier(const This &thatSpecifier)
		:
		Super(thatSpecifier),
		_bases(thatSpecifier._bases) {}
		
FogClassSpecifier::~FogClassSpecifier() {}

bool FogClassSpecifier::is_actual(const FogScopeContext& scopeContext) const {
	return Super::is_actual(scopeContext) && _bases->is_actual(scopeContext);
}

void FogClassSpecifier::make_actual_from(FogMakeActualContext& makeActualContext) {
	Super::make_actual_from(makeActualContext);
	_bases->make_actual(_bases.to_const(), makeActualContext);
}

FogEntity *FogClassSpecifier::make_type_entity(FogMakeEntityContext& makeEntityContext) const {
	FogScopeContext::InScope inScope = tag() ? FogMakeEntityContext::IN_THIS_SCOPE : makeEntityContext.in_scope();
	FogInScopeMakeEntityContext inScopeContext(makeEntityContext, inScope);
	FogEntityMakerContext makerContext(inScopeContext, *this, &FogScope::make_scope);
	FogEntity *theEntity = make_entity(makerContext);
	
	if (!theEntity)
		return 0;
		
	FogScope *madeScope = theEntity->is_scope();
	
	if (!madeScope) {
		ERRMSG("BUG - made " << viz(*theEntity) << " when scope expected.");
		return 0;
	}
	
	madeScope->set_access(access());
	
	FogNestedMakeEntityContext madeEntityContext(makeEntityContext, *madeScope);
	
	for (FogBaseSpecifierConstListOfRefToConstIterator p(_bases->bases()); p; ++p)
		p->make_base(madeEntityContext, *madeScope);
		
	make_body(madeEntityContext, *madeScope);
	
	FogRoot& globalScope  = theEntity->global_scope();
	
	const FogSourceFile *sourceFile = globalScope.source_file();
	
	if (sourceFile) {
		FogTargetFile *targetFile = globalScope.file_manager().make_target_file(*sourceFile);
		
		if (targetFile)
			targetFile->add_internal(theEntity->name_usage());
	}
	
	return theEntity;
}

std::ostream& FogClassSpecifier::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	_bases->print_deep(s, aDepth);
	return s;
}

std::ostream& FogClassSpecifier::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	_bases->print_on(s, aDepth);
	return s;
}

std::ostream& FogClassSpecifier::print_viz(std::ostream& s) const {
	s << "\"class-specifier: ";
	print_this(s);
	return s << '\"';
}

bool FogClassSpecifier::resolve_semantics(FogSemanticsContext& theSemantics) const {
	FogChildSemanticsContext nameSemantics(theSemantics);
	
	if (!name().resolve_semantics(nameSemantics))
		return false;
		
	/*
    std::strstream str;
    print_viz(str);
    std::cout << str.str() << endl;
    */
    
	theSemantics.reset();
	
	if (nameSemantics.is_scoped_identifier_opt() || nameSemantics.is_scoped_class_name()) {
		theSemantics.set(FogSemantics::IS_SCOPE_SPECIFIER);
		
		if (tag().is_type_specifier())
			theSemantics.set(FogSemantics::IS_CLASS_SPECIFIER);
	}
	
	if (nameSemantics.is_meta_variable_declaration_elem()) {
		theSemantics.set(FogSemantics::IS_SCOPE_SPECIFIER);
		
		if (tag().is_meta_tag())
			theSemantics.set(FogSemantics::IS_CLASS_SPECIFIER);
	}
	
	// TODO if deeply in FogTemplateParameterParseContext
	// - find the name what was replaced with _1, _2, etc.
	// - add variable to waiting for meta function declare
	// - 
	return true;
}
