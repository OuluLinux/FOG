#include <Fog/FogIncludeAll.h>


























TYPEINFO_SINGLE(FogFunctionSpecifier, Super)
PRIMREF_NULL_CLAS(FogFunctionSpecifier)
FOGTOKEN_NULL_LEAF
};
PRIMREF_DERIVED_NULL_IMPL(FogFunctionSpecifier)
PRIMREFS_IMPL(FogFunctionSpecifier)
FOGTOKEN_LEAF_IMPL(FogFunctionSpecifier)
TMPL_HACK_FIX_DO(FogFunctionSpecifier)

FogFunctionSpecifier::FogFunctionSpecifier() {}

FogFunctionSpecifier::FogFunctionSpecifier(FogMakeSpecifierContext& makeSpecifierContext, FogName& nameName)
	:
	Super(makeSpecifierContext, nameName)
{}

FogFunctionSpecifier::FogFunctionSpecifier(const This& functionSpecifier)
	:
	Super(functionSpecifier),
	_reverse_friends(functionSpecifier._reverse_friends)
{}

FogFunctionSpecifier::~FogFunctionSpecifier() {}

void FogFunctionSpecifier::add_friend(FogScope& aScope) {
	if (!_reverse_friends.find(aScope.unique_id()))
		_reverse_friends.add(aScope);
}

bool FogFunctionSpecifier::compile_specifier(FogMakeSpecifierContext& makeSpecifierContext) {
	if (!decl_specifiers().is_explicit_access() && !decl_specifiers().is_friend())
		set_decl_specifier(makeSpecifierContext.access());
		
	if (decl_specifiers().is_explicit_inline_if_short()) {
		if (is_nested())                    //   inline outside 'class {}' goes in implementation,
			set_decl_specifier(FogInline::in_implementation());
		else                                //   inline inside 'class {}' is class-dependent,
			set_decl_specifier(makeSpecifierContext.dynamic_scope().default_inline());
	}
	else if (decl_specifiers().is_implicit_inline_if_short()) {
		if (is_nested())                    //   non-inline outside 'class {}' goes out-of-line,
			set_decl_specifier(FogInline::out_of_line());
			
		//        else                                // non-inline inside 'class {}' gets weighed
	}
	
	//
	//    Now deduce any implied function template arguments.
	//
	FogScope& thisScope = makeSpecifierContext.dynamic_scope();
	const FogTemplateParameterSpecifiers* makerTemplateParameters = makeSpecifierContext.template_parameters();
	const size_t makerTemplateParameterDepth =
	        makerTemplateParameters ? makerTemplateParameters->template_parameter_depth() : 0;
	const FogTemplateParameterSpecifiers& scopeTemplateParameters = thisScope.template_parameters();
	const size_t scopeTemplateParameterDepth = scopeTemplateParameters.template_parameter_depth();
	
	if (thisScope.is_instantiation())
		;
	else if (name().is_templated_name())
		;
	else if (makerTemplateParameterDepth > scopeTemplateParameterDepth) {
		FogEntityFinding theFinding(FogEntityFinder::FIND_NAME);
		name().find_entities_in(makeSpecifierContext, theFinding);
		//        FogEntityFinder theFinder(theFinding, makeSpecifierContext);
		//        thisScope.find_local_name(theFinder);
		FogEntity* anEntity = theFinding.found();
		
		//        if (theFinding.is_ambiguous())
		//            ERRMSG("BUG - missing support for resolution of overloaded template specialisation.");
		if (anEntity) {
			FogFunction* aFunction = anEntity->is_function();
			
			if (!aFunction) {
				ERRMSG("Cannot specialise " << viz(*anEntity) << " as a function.");
				return 0;
			}
			
			FogFunction* theFunction = aFunction->deduce_specialisation(makeSpecifierContext);
			
			if (theFunction) {
				const FogTemplateArgs& ta = theFunction->template_arguments();
				const FogTemplateArgListOfRefToConst& tArguments = ta.arguments();
				FogListOfExprRef argList(new FogListOfExpr, FogListOfExprRef::ADOPT);
				
				for (FogTemplateArgConstListOfRefToConstIterator p(tArguments); p; ++p)
					argList->add((FogTemplateArg&)*p);                                              //  .bugbug cast
					
				FogNameRef templatedName(new FogTemplatedName(name(), *argList), FogNameRef::ADOPT);
				set_name(*templatedName);
			}
		}
	}
	
	return Super::compile_specifier(makeSpecifierContext);
}

void FogFunctionSpecifier::instantiate() {
	Super::instantiate();
	_reverse_friends.vacate();
}

bool FogFunctionSpecifier::is_constructor(FogScopeContext& inScope) const {
	PrimIdHandle functionId;
	
	if (!resolve_id(functionId, inScope))
		return false;
		
	return *functionId == inScope.dynamic_scope().short_id();   //  .bugbug or root_scope too ?
}

bool FogFunctionSpecifier::is_copy(const PrimId& anId) const { return false; }
FogFunctionSpecifier* FogFunctionSpecifier::is_function_specifier() { return this; }

FogEntity* FogFunctionSpecifier::make_name_entity(FogMakeEntityContext& makeEntityContext) const {
	if (makeEntityContext.decl_specifiers().is_using()) {
		FogEntityMakerContext makerContext(makeEntityContext, *this, &FogScope::make_using_entity);
		return make_entity(makerContext);
	}
	else if (makeEntityContext.decl_specifiers().is_friend()) {
		FogScope& targetScope = makeEntityContext.dynamic_scope();
		//        FogStaticScopeContext nameSpaceContext(targetScope.name_space());
		//        FogStaticMakeEntityContext makeEntityContext(nameSpaceContext);
		FogNestedMakeEntityContext nameSpaceContext(makeEntityContext, targetScope.name_space());
		FogEntityMakerContext makerContext(nameSpaceContext, *this, &FogScope::make_function_entity);
		FogEntity* anEntity = make_entity(makerContext);
		FogFunction* aFunction = anEntity ? anEntity->is_function() : 0;
		
		if (aFunction && makerContext.has_context())
			aFunction->add(makerContext);
			
		if (anEntity)
			targetScope.add_friend(*anEntity);
			
		return anEntity;
	}
	else {
		FogEntityMakerContext makerContext(makeEntityContext, *this, &FogScope::make_function_entity);
		FogEntity* anEntity = make_entity(makerContext);
		FogFunction* aFunction = anEntity ? anEntity->is_function() : 0;
		
		if (aFunction && makerContext.has_context())
			aFunction->add(makerContext);
			
		return anEntity;
	}
}

void FogFunctionSpecifier::merge_from(FogMergeContext& mergeContext, const This& aSpecifier) {
	FogMergeContext nestedMergeContext(mergeContext, subspace().is_null(), aSpecifier.subspace().is_null());
	Super::merge_from(nestedMergeContext, aSpecifier);
	_reverse_friends |= aSpecifier.reverse_friends();
}

const FogMetaType& FogFunctionSpecifier::meta_type() const { return FogMetaType::function_specifier_type(); }

const FogMerge& FogFunctionSpecifier::needs_merge_from(FogMergeContext& mergeContext, const This& thatDecl) const {
	FogMergeContext nestedMergeContext(mergeContext, subspace().is_null(), thatDecl.subspace().is_null());
	FogMergeHandle needsMerge(Super::needs_merge_from(nestedMergeContext, thatDecl));
	
	if (_reverse_friends.tally() || thatDecl.reverse_friends().tally()) {
		if (!_reverse_friends.tally())
			needsMerge |= FogMerge::left_invalid();
		else if (!thatDecl.reverse_friends().tally())
			needsMerge |= FogMerge::right_invalid();
		else {
			FogScopeMapOfRef netMap(_reverse_friends);
			netMap |= thatDecl.reverse_friends();
			
			if (_reverse_friends.tally() == netMap.tally())
				needsMerge |= FogMerge::left_invalid();
			else if (thatDecl.reverse_friends().tally() == netMap.tally())
				needsMerge |= FogMerge::right_invalid();
			else
				needsMerge |= FogMerge::both_invalid();
		}
	}
	
	return needsMerge;
}

std::ostream& FogFunctionSpecifier::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	
	if (_reverse_friends.tally()) {
		s << indent(aDepth) << "reverse friends\n";
		_reverse_friends.print_members(s, aDepth + 1);
	}
	
	return s;
}

std::ostream& FogFunctionSpecifier::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	
	if (_reverse_friends.tally()) {
		s << indent(aDepth) << "reverse friends\n";
		_reverse_friends.print_members(s, aDepth + 1);
	}
	
	return s;
}

std::ostream& FogFunctionSpecifier::print_viz(std::ostream& s) const {
	s << "\"function-specifier: ";
	print_this(s);
	return s << '\"';
}

const FogScopeMapOfRef& FogFunctionSpecifier::reverse_friends() const { return _reverse_friends; }

void FogFunctionSpecifier::set_colon_value(FogExpr& anExpr) {
	FogRawCtorInitRef ctorInit;
	
	if (anExpr.make_ctor_init(ctorInit)) {
		FogRawCtorInitsRef ctorInits(new FogRawCtorInits, FogRawCtorInitsRef::ADOPT);
		ctorInits->add(*ctorInit);
		FogCodeRef someCode(new FogCode, FogCodeRef::ADOPT);
		someCode->set_ctor_inits(*ctorInits);
		add_to_subspace(*someCode);
	}
}

void FogFunctionSpecifier::set_initializer(const FogExpr& anExpr) {
	static const FogExpr& zeroExpr = FogNumber::make_zero();
	
	if (&anExpr == &zeroExpr)
		set_decl_specifiers(FogDeclSpecifierValue::PURE_MASK);
	else
		ERRMSG("Should not set_initializer " << viz(anExpr) << " to " << viz(*this));
}
