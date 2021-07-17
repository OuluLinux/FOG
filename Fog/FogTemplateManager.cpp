#include <Fog/FogIncludeAll.h>















TYPEINFO_SINGLE(FogTemplateManager, Super)

FogTemplateManager::FogTemplateManager(FogEntity& primaryEntity)
	:
	_primary(primaryEntity),
	_classification(NON_TEMPLATE)
{}

FogTemplateManager::FogTemplateManager(FogEntity& primaryEntity, FogMakerContext& makerContext)
	:
	_primary(primaryEntity),
	_classification(classify_primary(makerContext)),
	_parameter_specifiers(is_templated() ? makerContext.template_parameters() : 0) {
	//    install_parameters(makerContext); -- must be in caller to ensure that parent has become at least a FogScope
	//        implementation here works on Sun C++, but fails under Visual C++.
}

FogTemplateManager::FogTemplateManager(FogMakeTemplateContext& makeTemplateContext)
	:
	_primary(makeTemplateContext.primary()),
	_classification(classify_secondary(makeTemplateContext)),
	_arguments(makeTemplateContext.template_args()),
	_parameter_specifiers(makeTemplateContext.template_parameters()) {
	//    install_parameters(makerContext); -- must be in caller to ensure that parent has become at least a FogScope
	//        implementation here works on Sun C++, but fails under Visual C++.
}

void FogTemplateManager::check_classification(const FogMakeTemplateContext& makeTemplateContext) const {
	Classification wantedClassification = classify_secondary(makeTemplateContext);
	
	if (wantedClassification == _classification)
		;
	else if ((_classification == IMPLICIT_INSTANTIATION) && (wantedClassification == PARTIAL_SPECIALISATION))
		;               //   e.g. found <T*> when <void*> wanted.
	else if ((_classification == IMPLICIT_INSTANTIATION) && (wantedClassification == EXPLICIT_SPECIALISATION))
		;               //   e.g. found specialised <T> when any <T> wanted.
	else if ((_classification == PRIMARY_TEMPLATE) && (wantedClassification == EXPLICIT_SPECIALISATION))
		;               //   e.g. found the target of an instantiation.
	else if ((_classification == PRIMARY_TEMPLATE) && (wantedClassification == PARTIAL_SPECIALISATION)
	         //                        && makeTemplateContext.template_args().matches(*_parameter_specifiers))
	         && (makeTemplateContext.template_args().id() == _parameter_specifiers->id()))
		;               //   e.g. found a requalified template.
	else if (((_classification == PRIMARY_TEMPLATE) || (_classification == EXPLICIT_SPECIALISATION))
	         && (wantedClassification == IMPLICIT_INSTANTIATION) && makeTemplateContext.is_reference())
		;               //   e.g. found the target of an instantiation.
	else if (!makeTemplateContext.template_parameters()
	         || !makeTemplateContext.template_parameters()->is_using_template())
		ERRMSG("Found " << classification(_classification) << " when " << classification(wantedClassification)
		       << " wanted for " << viz(makeTemplateContext));
}

const char* FogTemplateManager::classification(Classification aClassification) {
	static const char* const classificationTexts[] = {
		"non-template",
		"primary-template",
		"partial-specialisation",
		//          "complete-specialisation",
		"implicit-instantiation",
		"explicit-specialisation"
	};
	return classificationTexts[aClassification];
}

FogTemplateManager::Classification FogTemplateManager::classify_primary(const FogMakerContext& makerContext) {
	const FogTemplateParameterSpecifiers* templateParameters = makerContext.template_parameters();
	
	if (!templateParameters)
		return NON_TEMPLATE;
		
	const FogScope& inScope = makerContext.dynamic_scope();
	const FogTemplateParameterSpecifiers& templateParameterSpecifiers = inScope.template_parameters();
	size_t firstTemplateParameterNumber = templateParameterSpecifiers.last_template_parameter_number();
	size_t lastTemplateParameterNumber = templateParameters->last_template_parameter_number();
	
	if (firstTemplateParameterNumber >= lastTemplateParameterNumber)
		return NON_TEMPLATE;
		
	return PRIMARY_TEMPLATE;
}

FogTemplateManager::Classification FogTemplateManager::classify_secondary(const FogMakeTemplateContext& makeTemplateContext) {
	const FogTemplateArgs& templateArgs = makeTemplateContext.template_args();
	const FogTemplateParameterSpecifiers* templateParameters = makeTemplateContext.template_parameters();
	
	if (!templateArgs)
		return templateParameters ? PRIMARY_TEMPLATE : NON_TEMPLATE;
		
	//    const PrimId& templateArguments = templateArgs.id();
	//    if (!templateArguments)
	//        ERRMSG("Require arguments for template specialisation for " << viz(makeTemplateContext));
	//    if (!makeContext.is_declaration())
	//        return IMPLICIT_INSTANTIATION;
	//        return EXPLICIT_SPECIALISATION;
	//    else if (!templateParameters)
	//    if (nameContext.is_nesting())
	//        return EXPLICIT_SPECIALISATION;
	//    else if (makerContext.is_using_template())
	if (!templateParameters)
		return IMPLICIT_INSTANTIATION;
	else if (templateParameters->is_using_template())
		return IMPLICIT_INSTANTIATION;
	else if (makeTemplateContext.is_reference())
		return IMPLICIT_INSTANTIATION;
	else if (!templateParameters->parameters().tally())
		return EXPLICIT_SPECIALISATION;
	else
		return PARTIAL_SPECIALISATION;
}

void FogTemplateManager::create_usages(FogUsageContext& usageContext) {
	if (is_instantiation())
		_primary.add_use_by(usageContext);
		
	_arguments->create_usage(usageContext);
	
	if (!is_instantiation()) {
		_parameter_specifiers->create_usage(usageContext);
		
		for (FogEntityConstSetOfRefIterator p(_secondaries); p; ++p) {
			FogEntity& aSecondary = *p->value();
			aSecondary.create_usages();
		}
	}
}

void FogTemplateManager::derive_from(FogScope& derivedScope, FogBaseClass& baseClass) {}

void FogTemplateManager::derive_from(FogScope& derivedScope, FogFunction& baseFunction,
                                     FogPotentialDeclaration* potentialDeclaration) {
	for (FogEntitySetOfRefIterator p(_secondaries); p; ++p) {
		FogScope* theScope = p->value()->is_scope();
		const FogTemplateParameterSpecifiers* templateParameters = theScope ? &theScope->template_parameters() : 0;
		
		if (templateParameters && templateParameters->is_using_template())
			theScope->derive_from(baseFunction, potentialDeclaration);
	}
}

void FogTemplateManager::derive_from(FogScope& derivedScope, FogTypedef& aTypedef,
                                     FogPotentialDeclaration* potentialDeclaration) {}

void FogTemplateManager::derive_from(FogScope& derivedScope, FogVariable& aVariable,
                                     FogPotentialDeclaration* potentialDeclaration) {}

void FogTemplateManager::destroy() {
	for (FogEntityConstSetOfRefIterator p(_secondaries); p; ++p)
		p->value()->destroy();
		
	_secondaries.vacate();
}

bool FogTemplateManager::do_compile(FogCompileContext& inScope) {
	for (FogEntityConstSetOfRefIterator p(_secondaries); p; ++p) {
		FogEntity& aSecondary = *p->value();
		aSecondary.do_compile(inScope);
	}
	
	return true;
}

bool FogTemplateManager::do_derive(FogDeriveContext& inScope) {
	for (FogEntityConstSetOfRefIterator p(_secondaries); p; ++p) {
		FogEntity& aSecondary = *p->value();
		
		if (!aSecondary.done_derive())          //   Base class dependencies can break natural ordering.
			aSecondary.do_derive(inScope);
	}
	
	return true;
}

//
//    Emit the template prefix for a declaration.
//
void FogTemplateManager::emit_prefix(FogEmitContext& emitContext) const {
	switch (_classification) {
	case PRIMARY_TEMPLATE:
	case PARTIAL_SPECIALISATION:
		_parameter_specifiers->emit(emitContext);
		break;
		
	case IMPLICIT_INSTANTIATION:
	case EXPLICIT_SPECIALISATION:
		if (Fog::no_specialisation_prefix())
			emitContext.emit_space_and_text("// ");
			
		emitContext.emit_space_and_text("template <>\n");
		break;
		
	//        case EXPLICIT_SPECIALISATION:
	//            if (Fog::no_specialisation_prefix())
	//                emitContext.emit_space_and_text("// ");
	//            emitContext.emit_space_and_text("template\n");
	//            break;
	case NON_TEMPLATE:
		break;
	}
}

//
//    Emit the template suffix for a name.
//
void FogTemplateManager::emit_suffix(FogEmitContext& emitContext) const {
	switch (_classification) {
	case PARTIAL_SPECIALISATION:
	case IMPLICIT_INSTANTIATION:
		emitContext.emit_space_and_text(_arguments->id().str());
		break;
		
	case PRIMARY_TEMPLATE:
		_parameter_specifiers->emit_suffix(emitContext);
		break;
		
	case EXPLICIT_SPECIALISATION:
	case NON_TEMPLATE:
		break;
	}
}

FogEntity* FogTemplateManager::find_template(FogMakeTemplateContext& makeTemplateContext) {
	const FogTemplateArgs& templateArguments = makeTemplateContext.template_args();
	const PrimId& templateArgumentsId = templateArguments.id();
	const PrimId& templateParametersId = _parameter_specifiers->id();
	
	if (is_specialisation())
		ERRMSG("BUG - should not find_template of specialisation " << viz(_primary));
	else if (templateArgumentsId == templateParametersId)
		return &_primary;
		
	FogEntitySetOfRefElement* aSecondary = _secondaries.find(templateArgumentsId);
	
	if (aSecondary)
		return aSecondary->value().pointer();
		
	FogEntity* bestEntity = 0;
	
	for (FogEntitySetOfRefIterator p(_secondaries); p; ++p) {
		FogEntity& theEntity = *p->value();
		const FogTemplateParameterSpecifiers& templateParameters = theEntity.template_parameters();
		FogMergeContext mergeContext(makeTemplateContext);
		
		if (templateArguments.matches(mergeContext, templateParameters)) {
			if (bestEntity)
				ERRMSG("Ambiguous match of " << viz(*bestEntity) << " and " << viz(theEntity));
				
			bestEntity = &theEntity;
		}
	}
	
	return bestEntity;
}

void FogTemplateManager::install(FogEntity& anEntity) {}

//
//    Deferred part of construction that cannot occur until aggregating context has become a FogScope.
//
void FogTemplateManager::install_parameters(FogMakeEntityContext& makeEntityContext) {
	switch (_classification) {
	case NON_TEMPLATE:
		if (_parameter_specifiers)
			ERRMSG("Should not have template specifiers for non-template.");
			
		break;
		
	case EXPLICIT_SPECIALISATION:
		if (!_parameter_specifiers)
			ERRMSG("Missing template <> for explicit specialisation.");
			
		if (_parameter_specifiers && (_parameter_specifiers->last_template_parameter_number() > 0))
			ERRMSG("Should not have template parameter specifiers for explicit specialisation.");
			
		break;
		
	case IMPLICIT_INSTANTIATION:
		//            if (!_parameter_specifiers)
		//                ERRMSG("Missing template <> for complete specialisation.");
		//            else
		//            {
		//                size_t i = _parameter_specifiers->first_template_parameter_number();
		//                size_t iLast = _parameter_specifiers->last_template_parameter_number();
		//                if (i != iLast)
		//                    ERRMSG("Should have empty template <> for complete specialisation.");
		//            }
		break;
		
	case PRIMARY_TEMPLATE: {
		if (_parameter_specifiers) {
			size_t i = _parameter_specifiers->first_template_parameter_number();
			size_t iLast = _parameter_specifiers->last_template_parameter_number();
			
			if (iLast > i) {
				FogNestedMakeEntityContext nestedContext(makeEntityContext, _primary);
				
				//                      _parameters.set_tally(iLast - i);
				while (++i <= iLast) {
					const FogTemplateParameterSpecifier& q = _parameter_specifiers->template_parameter_number(i);
					//                          FogEntity *anEntity = q.make_name_entity(nestedContext);
					//                          if (!anEntity)
					//                              ERRMSG("Failed to make_entity for " << viz(q));
				}
			}
		}
		else
			ERRMSG("Missing template specifiers for primary template.");
			
		break;
	}
	
	case PARTIAL_SPECIALISATION: {
		const FogTemplateParameterSpecifiers& primaryTemplateParameters = primary().template_parameters();
		size_t i = _parameter_specifiers->first_template_parameter_number();
		size_t iLast = _parameter_specifiers->last_template_parameter_number();
		size_t j = primaryTemplateParameters.first_template_parameter_number();
		size_t jLast = primaryTemplateParameters.last_template_parameter_number();
		
		if ((i != j) || (iLast != jLast))
			ERRMSG("Inconsistent number of template specifiers for partial specialisation.");
		else {
			if (iLast > i) {
				FogNestedMakeEntityContext nestedContext(makeEntityContext, _primary);
				
				while (++i < iLast) {
					const FogTemplateParameterSpecifier& q1 = _parameter_specifiers->template_parameter_number(i);
					const FogTemplateParameterSpecifier& q2 = primaryTemplateParameters.template_parameter_number(i);
				}
				
				//                      ERRMSG("BUG -- missing code for partial parameters");
			}
		}
	}
	}
}

FogScopeEnums::IsUsingTemplate FogTemplateManager::is_using() const
{ return _parameter_specifiers->is_using_template(); }

FogEntity* FogTemplateManager::make_template(FogMakeTemplateContext& makeTemplateContext) {
	const PrimId& templateArgumentsId = makeTemplateContext.template_args().id();
	
	if (!is_specialisable())
		ERRMSG("BUG - should not specialise " << classification(_classification) << " " << viz(_primary));
		
	FogEntitySetOfRefElement* aSecondary = _secondaries.find(templateArgumentsId);
	
	if (aSecondary)
		return &*aSecondary->value();
		
	//    FogScope *outerScope = _primary.outer_scope();
	//    if (!outerScope)
	//    {
	//        ERRMSG("BUG - no outer scope for " << viz(_primary));
	//        return 0;
	//    }
	//    FogNameContext nameContext(*outerScope, makerContext);
	FogEntity* instantiatingEntity = _primary.new_template(makeTemplateContext);
	
	if (instantiatingEntity) {
		_secondaries.adopt(new FogEntitySetOfRefElement(templateArgumentsId, *instantiatingEntity));
		instantiatingEntity->annul();
		_primary.instantiate_into(*instantiatingEntity);
	}
	
	return instantiatingEntity;
}

const FogTemplateParameterSpecifiers& FogTemplateManager::parameter_specifiers() const {
	//      if (has_distinct_primary())
	//          return _primary.template_parameters();
	return *_parameter_specifiers;
}

std::ostream& FogTemplateManager::print_depth(std::ostream& s, int aDepth) const {
	if (_secondaries.tally()) {
		s << indent(aDepth) << "secondaries\n";
		_secondaries.print_depth(s, aDepth + 1);
	}
	
	return s;
}

std::ostream& FogTemplateManager::print_members(std::ostream& s, int aDepth) const {
	if (_secondaries.tally()) {
		s << indent(aDepth) << "secondaries\n";
		_secondaries.print_members(s, aDepth + 1);
	}
	
	return s;
}

char FogTemplateManager::print_prefix(std::ostream& s, char tailChar) const {
	return _parameter_specifiers->print_named(s, 0, tailChar);
}

char FogTemplateManager::print_suffix(std::ostream& s, char tailChar) const {
	switch (_classification) {
	case PARTIAL_SPECIALISATION:
	case IMPLICIT_INSTANTIATION:
		tailChar = FogStream::space_and_emit(s, tailChar, _arguments->id().str());
		break;
		
	case PRIMARY_TEMPLATE:
		tailChar = _parameter_specifiers->print_suffix(s, tailChar);
		break;
		
	case EXPLICIT_SPECIALISATION:
	case NON_TEMPLATE:
		break;
	}
	
	return tailChar;
}

std::ostream& FogTemplateManager::print_this(std::ostream& s) const {
	return s << classification(_classification);
}

void FogTemplateManager::validate() const {
	for (FogEntityConstSetOfRefIterator p(_secondaries); p; ++p) {
		FogEntity& aSecondary = *p->value();
		aSecondary.validate();
	}
}
