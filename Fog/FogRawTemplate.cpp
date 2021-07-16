#include <Fog/FogIncludeAll.h>















TYPEINFO_SINGLE(FogRawTemplate, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogRawTemplate)
PRIMREFS_IMPL(FogRawTemplate)
FOGTOKEN_LEAF_IMPL(FogRawTemplate)
TMPL_HACK_FIX_DO(FogRawTemplate)

FogRawTemplate::FogRawTemplate()
		:
		_is_export(NON_EXPORT_TEMPLATE),
		_is_using(NON_USING_TEMPLATE) {}
		
FogRawTemplate::FogRawTemplate(FogListOfExpr& parameterList, FogRaw& aDeclaration)
		:
		_is_export(NON_EXPORT_TEMPLATE),
		_is_using(NON_USING_TEMPLATE),
		_declaration(aDeclaration),
		_parameters(parameterList) {}
		
FogRawTemplate::~FogRawTemplate() {}

bool FogRawTemplate::compile_parsed(FogParseContext& parseContext) {
	const FogTemplateParameterSpecifiers *outerParameters = &parseContext.explicit_template_parameters();
	FogTemplateParameterSpecifiersRef templateParameters
	(new FogTemplateParameterSpecifiers(outerParameters, _is_export, _is_using),
	 FogTemplateParameterSpecifiersRef::ADOPT);
	const FogToken& inToken = parseContext.dynamic_token();
	
	for (FogExprConstListOfRefIterator p(_parameters->exprs()); p; ++p) {
		FogStrongSemanticsContext theSemantics(parseContext);
		
		if (!p->resolve_semantics(theSemantics))
			ERRMSG("Failed to resolve semantics of " << viz(*p));
		else
			if (!theSemantics.is_template_parameter())
				ERRMSG("Template parameter expected rather than " << viz(*p));
			else {
				static const FogDeclSpecifierValue declSpecifiers;
				FogBaseMakeSpecifierContext makeSpecifierContext(parseContext, theSemantics,
						declSpecifiers, FogMakeSemantics::TEMPLATE_DECLARATION);
				        
				if (!p->make_specifier(makeSpecifierContext))
					ERRMSG("Failed to make_specifier for " << viz(*p));
				else {
					FogTemplateParameterSpecifier *templateSpecifier =
						makeSpecifierContext.get_template_parameter_specifier(inToken);
					    
					if (templateSpecifier)
						templateParameters->add(*templateSpecifier);
						
					makeSpecifierContext.compile_specifier();
					
					makeSpecifierContext.compile_type_specifier();
				}
			}
	}
	
	FogTemplateParameterParseContext templatedParseContext(parseContext, *templateParameters, *_declaration);
	
	return templatedParseContext.declaration().compile_declaration(templatedParseContext);
}

void FogRawTemplate::create_usage(FogUsageContext& usageContext) const {
	Super::create_usage(usageContext);
	_declaration->create_usage(usageContext);
	_parameters->create_usage(usageContext);
}

bool FogRawTemplate::emit(FogEmitContext& emitContext) const {
	_parameters->emit(emitContext);
	_declaration->emit(emitContext);
	return true;
}

void FogRawTemplate::install(FogInstallContext& installContext) const {
	Super::install(installContext);
	_declaration->install(installContext);
	_parameters->install(installContext);
}

bool FogRawTemplate::is_actual(const FogScopeContext& scopeContext) const {
	return Super::is_actual(scopeContext)
		   && _declaration->is_actual(scopeContext)
		   && _parameters->is_actual(scopeContext);
}

void FogRawTemplate::make_actual_from(FogMakeActualContext& makeActualContext) {
	Super::make_actual_from(makeActualContext);
	_declaration->make_actual(_declaration.to_const(), makeActualContext);
	_parameters->make_actual(_parameters.to_const(), makeActualContext);
}

void FogRawTemplate::merge_from(FogMergeContext& mergeContext, const This& templatePar) {
	Super::merge_from(mergeContext, templatePar);
	templatePar._declaration->merge_into(mergeContext, _declaration.to_const());
	templatePar._parameters->merge_into(mergeContext, _parameters.to_const());
	_is_export = IsExportTemplate(merge_bool("is_export", _is_export != 0, templatePar._is_export != 0));
//  	_is_using = merge_bool("is_using", _is_using, templatePar._is_using);
}

const FogMerge& FogRawTemplate::needs_merge_from(FogMergeContext& mergeContext,
		const This& templatePar) const {
	if (_is_using != templatePar._is_using)
		return FogMerge::set_incompatible(FogMerge::incompatible_template_parameter());
		
	FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, templatePar));
	
	needsMerge |= _declaration->needs_merge(mergeContext, *templatePar._declaration);
	
	needsMerge |= _parameters->needs_merge(mergeContext, *templatePar._parameters);
	
	if (_is_export && !templatePar._is_export)
		needsMerge |= FogMerge::right_invalid();
	else
		if (!_is_export && templatePar._is_export)
			needsMerge |= FogMerge::left_invalid();
			
	return needsMerge;
}

std::ostream& FogRawTemplate::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	_declaration->print_deep(s, aDepth);
	return s;
}

std::ostream& FogRawTemplate::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	_declaration->print_on(s, aDepth);
	return s;
}

char FogRawTemplate::print_named(std::ostream& s, const PrimId *scopeId, char tailChar) const {
	if (_is_export)
		s << "export ";
		
	if (_is_using)
		s << "using ";
		
	s << "template ";
	
	s << "<";
	
	for (FogExprConstListOfRefToConstIterator p(_parameters->exprs()); p; ++p) {
		s << " ";
		p->print_this(s);
		FogStream::space_and_emit(s, 0, p.to_go() > 1 ? "," : " ");
	}
	
	s << ">";
	
	return _declaration->print_named(s, scopeId, '>');
}

std::ostream& FogRawTemplate::print_viz(std::ostream& s) const {
	s << "\"template-specifier : ";
	print_this(s);
	return s << '\"';
}

bool FogRawTemplate::resolve_semantics(FogSemanticsContext& theSemantics) const {
	FogChildSemanticsContext declSemantics(theSemantics);
	FogChildSemanticsContext paramSemantics(theSemantics);
	
	if (!_parameters->resolve_semantics(paramSemantics) || !_declaration->resolve_semantics(declSemantics))
		return false;
		
	theSemantics.reset();
	
	if (!_is_export && !_is_using && paramSemantics.is_template_parameter_list()
		&& declSemantics.is_class_type_parameter()) {
		theSemantics.set(FogSemantics::IS_TEMPLATED_PARAMETER);
	}
	
	return true;
}

void FogRawTemplate::set_explicit_instantiation() {
	_declaration->set_explicit_instantiation();
}

void FogRawTemplate::set_decl_specifier_raw(FogRawRef& aRaw, FogDeclSpecifier& declSpecifiers) {
	if (&declSpecifiers == &FogDeclSpecifier::export_decl_specifier()) {
		_is_export = IS_EXPORT_TEMPLATE;
		aRaw = this;
	}
	
	else
		if (&declSpecifiers == &FogDeclSpecifier::using_decl_specifier()) {
			_is_using = IS_USING_TEMPLATE;
			aRaw = this;
		}
		
		else
			Super::set_decl_specifier_raw(aRaw, declSpecifiers);
}

void FogRawTemplate::set_line(FogLine& aLine) {
	Super::set_line(aLine);
	_declaration->set_line(aLine);
}
