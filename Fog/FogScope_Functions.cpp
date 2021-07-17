#include <Fog/FogIncludeAll.h>














FogScope_Functions::FogScope_Functions() {}

//
//    Return true if none of the functions that could eventually be derived for use in thisScope will be pure
//    assuming thisScope is concrete. This return determines the actual purity of thisScope during
//    compilation enabling subsequent derivation to occur with respect to the necessary scope purity.
//
//    If there is any explicitly declared pure virtual, the scope will be pure.
//    If any derived function is pure, and there is no derivation to remove purity, the scope will be pure.
//
bool FogScope_Functions::can_be_concrete(FogScopeContext& inScope) {
	FogScope& thisScope = inScope.dynamic_scope();
	PrimIdMap concreteDerivations;              //   Map of all concrete functions.
	PrimIdMap undecidedDerivations;             //   Map of all possibly concrete functions.
	PrimIdMap pureDerivations;                  //   Map of functions that were pure on first encounter.
	
	for (FogFunctionConstListOfRefIterator p(_distinct_function_list); p; ++p) {
		FogFunction& localFunction = *p;
		const FogLazyBool& isConcrete = localFunction.can_be_concrete(inScope);
		
		if (isConcrete.is_false())
			return false;
			
		if (isConcrete.is_true())
			concreteDerivations.add(localFunction.local_signature_id());
		else
			undecidedDerivations.add(localFunction.local_signature_id());
	}
	
	//
	//    Accumulate all pure-virtuals from immediate base classes,
	//
	for (FogScopeConstListOfRefIterator p1(thisScope.ctor_bases()); p1; ++p1)
		for (FogFunctionConstListOfRefIterator p2(p1->distinct_function_list()); p2; ++p2) {
			FogFunction& baseFunction = *p2;
			const PrimId& baseFunctionId = baseFunction.local_signature_id();
			const FogLazyBool& isConcrete = baseFunction.can_be_concrete(inScope);
			
			//              if (isConcrete.is_undecided())
			//                  ERRMSG("BUG -- use of undecided can_be_concrete for " << viz(baseFunction));
			if (isConcrete.is_false())
				pureDerivations.add_filtered(baseFunctionId);
		}
		
	//
	//    Accumulate all derived non-pure-virtuals from base classes,
	//
	for (FogScopeConstListOfRefIterator p3(thisScope.all_bases()); p3; ++p3)
		for (FogFunctionConstListOfRefIterator p4(p3->distinct_function_list()); p4; ++p4) {
			const PrimId& anId = p4->local_signature_id();
			
			if (!concreteDerivations.find(anId)) {            //   Don't bother if already got it.
				const FogLazyBool& isConcrete = p4->can_be_concrete(inScope);
				
				//                  if (isConcrete.is_undecided())
				//                      ERRMSG("BUG -- use of undecided can_be_concrete for " << viz(*p4));
				if (!isConcrete.is_false())
					concreteDerivations.add(anId);
			}
		}
		
	//
	//    Fail if any pure-virtual lacks a non-pure-virtual,
	//
	for (PrimIdConstMapIterator p5(pureDerivations); p5; ++p5)
		if (!concreteDerivations.find(*p5) && !undecidedDerivations.find(*p5))
			return false;
			
	return true;
}

void FogScope_Functions::create_usages() {
	for (FogFunctionConstListOfRefIterator p(_distinct_function_list); p; ++p)
		if (p->is_active())
			p->create_usages();
}

void FogScope_Functions::derive_from(FogScope& derivedScope, FogBaseClass& baseClass) {
	for (FogFunctionConstListOfRefIterator p(baseClass.base().distinct_function_list()); p; ++p)
		derive_from(derivedScope, *p, 0);
}

void FogScope_Functions::derive_from(FogScope& derivedScope, FogFunction& baseFunction,
                                     FogPotentialDeclaration* potentialDeclaration) {
	FogStaticScopeContext derivedScopeContext(derivedScope);
	FogDerivable* aDerivable = _distinct_function_map.find(baseFunction.local_signature_id());
	
	if (!potentialDeclaration)
		aDerivable = baseFunction.make_derived(derivedScopeContext, aDerivable);
	else if (!aDerivable)
		aDerivable = baseFunction.make_derived(derivedScopeContext, aDerivable);
	else
		potentialDeclaration->make_derivation(derivedScopeContext, *aDerivable);
}

void FogScope_Functions::destroy() {
	for (FogFunctionConstListOfRefIterator p(_distinct_function_list); p; ++p)
		p->destroy();
		
	_distinct_function_list.vacate();
	_distinct_function_map.vacate();
}

void FogScope_Functions::do_compile(FogCompileContext& inScope) {
	for (FogFunctionListOfRefIterator p(_distinct_function_list); p; ++p)
		p->do_compile(inScope);
}

void FogScope_Functions::do_derive(FogDeriveContext& inScope) {
	_distinct_function_list.sort(&FogFunction::compare);
	
	for (FogFunctionListOfRefIterator p(_distinct_function_list); p; ++p)
		p->do_derive(inScope);
}

FogFunction* FogScope_Functions::find_local_function(const FogFunction& aFunction) {
	const PrimId& anId = aFunction.local_signature_id();
	FogFunction* foundFunction = _distinct_function_map.find(anId);
	return foundFunction && foundFunction->is_active() ? foundFunction : 0;
}

FogFunction* FogScope_Functions::find_local_function(const PrimId& anId) {
	FogFunction* foundFunction = _distinct_function_map.find(anId);
	return foundFunction && foundFunction->is_active() ? foundFunction : 0;
}

bool FogScope_Functions::has_inline_functions() const {
	for (FogFunctionConstListOfRefToConstIterator p(_distinct_function_list); p; ++p) {
		const FogFunction& aFunction = *p;
		
		if (aFunction.is_active()
		        && !aFunction.specifier().is_null()                //   Avoid error message for non-local functions.
		        && aFunction.get_inline().is_in_interface())
			return true;
	}
	
	return false;
}

void FogScope_Functions::instantiate_into(FogScope& thisScope, FogScope& instantiatingScope) {
	for (FogFunctionConstListOfRefIterator p(distinct_function_list()); p; ++p)
		derive_from(instantiatingScope, *p, 0);
}

//
//    Determine actual purity from the absence of any pure-virtual functions.
//
bool FogScope_Functions::is_concrete() const {
	for (FogFunctionConstListOfRefToConstIterator p(_distinct_function_list); p; ++p) {
		const FogFunction& aFunction = *p;
		
		//        if (aFunction.is_active() && !aFunction.is_concrete())      -- ignore active since inheritance matters
		if (!aFunction.is_concrete())
			return false;
	}
	
	return true;
}

FogFunction* FogScope_Functions::make_function(FogMakerContext& makerContext) {
	FogScope& thisScope = makerContext.dynamic_scope();
	const FogSpecifier& aSpecifier = makerContext.specifier();
	FogStream localStream(thisScope);
	FogStaticEmitContext localEmitContext(localStream, makerContext.dynamic_scope(), FogEmitContext::LOCAL_SIGNATURE);
	aSpecifier.emit(localEmitContext);
	FogFunction* theFunction = 0;
	const FogFunction* primaryFunction = 0;
	const FogTemplateParameterSpecifiers* makerTemplateParameters = makerContext.template_parameters();
	const size_t makerTemplateParameterDepth =
	        makerTemplateParameters ? makerTemplateParameters->template_parameter_depth() : 0;
	const FogTemplateParameterSpecifiers& scopeTemplateParameters = thisScope.template_parameters();
	const size_t scopeTemplateParameterDepth = scopeTemplateParameters.template_parameter_depth();
	
	if (thisScope.is_instantiation()) {
		FogEntityFinding theFinding(FogEntityFinder::FIND_NAME);        //  .bugbug use general purpose method.
		FogEntityFinder theFinder(theFinding, makerContext);
		FogScope& primaryScope = thisScope.primary();
		primaryScope.find_local_name(theFinder);                        //   If just a single name, no need to
		//        primaryScope.find_local_name(makeContext, theFinding);          // If just a single name, no need to
		const FogEntity* anEntity = theFinder.found();                  //    bother with signature mismatches
		
		if (theFinder.is_ambiguous()) {                                 //   If multiples then need to find a
			//    structurally compatible signature
			//            ERRMSG("BUG - missing support for resolution of overloaded template specialisation.");
			const FogEntityListOfRefToConst* entityList = primaryScope.find_local_names(makerContext.short_id());
#if 0
			const FogRawDerivable* theDeclaration = aDeclaration.is_raw_derivable();
			anEntity = 0;
			
			if (entityList && theDeclaration) {
				PrimAdopted<PrimOstrstream> s;
				
				for (FogEntityConstListOfRefToConstIterator p(*entityList); p; ++p) {
					const FogFunction* aFunction = p->is_function();
					
					if (aFunction) {                                    //   For all correctly named functions
						const FogDeclarative& functionDeclaration = aFunction->specifier();
						FogMergeContext mergeContext(primaryScope, thisScope);
						const FogMerge& needsMerge = functionDeclaration.needs_merge(mergeContext, *theDeclaration);
						
						if (needsMerge.is_incompatible())
							;
						else if (!anEntity)
							anEntity = aFunction;
						else {
							if (!s) {
								s.reset(new PrimOstrstream);
								*s << "Failed to resolve overload ambiguity for " << viz(makeContext);
								*s << "\n\t" << viz(*anEntity);
							}
							
							*s << "\n\t" << viz(*aFunction);
						}
					}
				}
				
				if (s)
					ERRMSGZ(s->str());
			}
			
#endif
		}
		
		if (anEntity) {
			primaryFunction = anEntity->is_function();
			
			if (!primaryFunction) {
				ERRMSG("Cannot specialise " << viz(*anEntity) << " as a function.");
				return 0;
			}
			
			const PrimId& localSignatureId = primaryFunction->local_signature_id();
			theFunction = _distinct_function_map.find(localSignatureId);
		}
	}
	else if (makerTemplateParameterDepth > scopeTemplateParameterDepth) {
		FogEntityFinding theFinding(FogEntityFinder::FIND_NAME);
		FogEntityFinder theFinder(theFinding, makerContext);
		thisScope.find_local_name(theFinder);
		FogEntity* anEntity = theFinder.found();
		
		//        if (theFinding.is_ambiguous())
		//            ERRMSG("BUG - missing support for resolution of overloaded template specialisation.");
		if (anEntity) {
			FogFunction* aFunction = anEntity->is_function();
			
			if (!aFunction) {
				ERRMSG("Cannot specialise " << viz(*anEntity) << " as a function.");
				return 0;
			}
			
			theFunction = aFunction->find_specialisation(makerContext);
		}
	}
	else {
		const PrimId& localStreamId = localStream.id();
		theFunction = _distinct_function_map.find(localStreamId);
	}
	
	if (theFunction)
		return theFunction;
		
	FogStream globalStream(thisScope.global_scope());
	FogStaticEmitContext globalEmitContext(globalStream, makerContext.dynamic_scope(), FogEmitContext::GLOBAL_SIGNATURE);
	aSpecifier.emit(globalEmitContext);
	const PrimId& localId = primaryFunction ? primaryFunction->local_signature_id() : localStream.id();
	const PrimId& globalId = globalStream.id();
	FogFunctionMakerContext functionMakerContext(makerContext, localId, globalId);
	FogFunction* newFunction = new FogFunction(functionMakerContext);
	
	if (!newFunction || !_distinct_function_map.adopt(*newFunction))
		return 0;
		
	_distinct_function_list.add(*newFunction);
	thisScope.add_function(*newFunction, 0);
	return newFunction;
}

std::ostream& FogScope_Functions::print_depth(std::ostream& s, int aDepth) const {
	if (_distinct_function_list.tally()) {
		s << indent(aDepth) << "functions\n";
		_distinct_function_list.print_depth(s, aDepth + 1);
	}
	
	return s;
}

std::ostream& FogScope_Functions::print_members(std::ostream& s, int aDepth) const {
	if (_distinct_function_list.tally()) {
		s << indent(aDepth) << "functions\n";
		_distinct_function_list.print_members(s, aDepth + 1);
	}
	
	return s;
}

void FogScope_Functions::validate() const {
	for (FogFunctionConstListOfRefToConstIterator p(_distinct_function_list); p; ++p)
		if (p->is_active())
			p->validate();
}
