#include <Fog/FogIncludeAll.h>












TYPEINFO_SINGLE(FogParseContext, Super)
TYPEINFO_SINGLE(FogBreakableParseContext, Super)
TYPEINFO_SINGLE(FogDeclarationParseContext, Super)
TYPEINFO_SINGLE(FogDecoratedParseContext, Super)
TYPEINFO_SINGLE(FogDynamicParseContext, Super)
TYPEINFO_SINGLE(FogEmitParseContext, Super)
TYPEINFO_SINGLE(FogMembersParseContext, Super)
TYPEINFO_SINGLE(FogMetaParseContext, Super)
TYPEINFO_SINGLE(FogScopeParseContext, Super)
TYPEINFO_SINGLE(FogStaticParseContext, Super)
TYPEINFO_SINGLE(FogSubspaceParseContext, Super)
TYPEINFO_SINGLE(FogTemplateParameterParseContext, Super)
TYPEINFO_SINGLE(FogTokenParseContext, Super)
TYPEINFO_SINGLE(FogUnbreakableParseContext, Super)

const FogRaw& FogParseContext::declaration() const { return FogRaw::immutable_null(); }
const FogTemplateParameterSpecifiers& FogParseContext::explicit_template_parameters() const {
	return FogTemplateParameterSpecifiers::immutable_null();
}

std::ostream& FogParseContext::print_this(std::ostream& s) const {
	print_resolution(s);
	dynamic_token().print_long_id(s);
	
	if (!declaration().is_null()) {
		s << " <- ";
		declaration().print_named(s, &dynamic_scope().global_signature_id(), 0);
	}
	
	return s;
}

std::ostream& FogParseContext::print_viz(std::ostream& s) const {
	return s << viz(declaration()) << " in " << viz(dynamic_token());
}

FogScopeContext::Resolution FogParseContext::resolution() const { return RESOLVE_DOLLARS; }

void FogParseContext::set_do_break() {
	ERRMSG("BUG -- should not set_do_break for " << viz(*this));
}

void FogParseContext::set_do_continue() {
	ERRMSG("BUG -- should not set_do_continue for " << viz(*this));
}

FogBreakableParseContext::FogBreakableParseContext(FogParseContext& parseContext, const FogRaw& aDeclaration)
	:
	Super(parseContext),
	FogTokenMonitor(aDeclaration),
	_declaration(aDeclaration),
	_do_break(false),
	_do_continue(false)
{}

FogBreakableParseContext::~FogBreakableParseContext() {}
const FogRaw& FogBreakableParseContext::declaration() const { return *_declaration; }

std::ostream& FogBreakableParseContext::print_message(std::ostream& s) const {
	const FogLine& aLine = _declaration->line();
	s << "At " << viz(aLine) << ": " << viz(*_declaration) << ".";
	//      FogStream strStream((FogScope&)dynamic_scope());                            //.bugbug cast and unchecked
	//      FogStaticEmitContext emitContext(strStream, (FogToken&)dynamic_token(), FogEmitContext::RAW);   //.bugbug cast
	//      emitContext.emit_space_and_text("Parsing\n");
	//      {
	//          FogSeparatedEmitContext separatedContext(emitContext, "", "\n");
	//          FogIndentEmitContext indentedEmitContext(separatedContext, 1, FogEmitContext::INDENTS);
	//          _declaration->emit(indentedEmitContext);
	//      }
	//      print_message_text(s, strStream.str(), 1);
	return s;
}

void FogBreakableParseContext::set_do_break() { _do_break = true; }
void FogBreakableParseContext::set_do_continue() { _do_continue = true; }

//  ---------------------------------------------------------------------------------------------------------------------

FogDeclarationParseContext::FogDeclarationParseContext(FogToken& aToken)
	:
	_static_token(aToken)
	//    _target_file(0)
{}

FogDeclarationParseContext::FogDeclarationParseContext(FogToken& aToken, FogScopeContext& _scope_context)
	:
	FogParseContext(_scope_context),
	_static_token(aToken)
	//    _target_file(0)
{}



FogToken& FogDeclarationParseContext::dynamic_token() { return _static_token; }
const FogToken& FogDeclarationParseContext::dynamic_token() const { return _static_token; }
bool FogDeclarationParseContext::find_slots(FogMetaSlotFinder& theFinder)
{ return _static_token.find_slots(theFinder); }
FogToken& FogDeclarationParseContext::static_token() { return _static_token; }
const FogToken& FogDeclarationParseContext::static_token() const { return _static_token; }
FogTargetFile* FogDeclarationParseContext::target_file() const { return 0 /*  _target_file*/; }

const FogAccess& FogDecoratedParseContext::access() const { return _parse_context.access(); }
const FogRaw& FogDecoratedParseContext::declaration() const { return _parse_context.declaration(); }
FogToken& FogDecoratedParseContext::dynamic_token() { return _parse_context.dynamic_token(); }
const FogToken& FogDecoratedParseContext::dynamic_token() const { return _parse_context.dynamic_token(); }
const FogTemplateParameterSpecifiers& FogDecoratedParseContext::explicit_template_parameters() const
{ return _parse_context.explicit_template_parameters(); }
FogScopeContext* FogDecoratedParseContext::find_context(size_t dollarCount)
{ return dollarCount > 0 ? _parse_context.find_context(dollarCount) : this; }
bool FogDecoratedParseContext::find_slots(FogMetaSlotFinder& theFinder)
{ return _parse_context.find_slots(theFinder); }
const FogTemplateParameterSpecifier* FogDecoratedParseContext::find_template_parameter_specifier
(const PrimId& anId) { return _parse_context.find_template_parameter_specifier(anId); }
const FogTemplateParameterSpecifier* FogDecoratedParseContext::find_template_parameter_specifier
(const FogTemplateParameterSpecifierId& anId) { return _parse_context.find_template_parameter_specifier(anId); }
FogScopeContext::InScope FogDecoratedParseContext::in_scope() const { return _parse_context.in_scope(); }
const FogScopeContext* FogDecoratedParseContext::meta_context() const { return _parse_context.meta_context(); }

std::ostream& FogDecoratedParseContext::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	_parse_context.print_deep(s, aDepth);
	return s;
}

std::ostream& FogDecoratedParseContext::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	_parse_context.print_on(s, aDepth);
	return s;
}

FogScopeContext::Resolution FogDecoratedParseContext::resolution() const { return _parse_context.resolution(); }
void FogDecoratedParseContext::set_access(const FogAccess& anAccess) { _parse_context.set_access(anAccess); }
void FogDecoratedParseContext::set_do_break() { _parse_context.set_do_break(); }
void FogDecoratedParseContext::set_do_continue() { _parse_context.set_do_continue(); }
void FogDecoratedParseContext::set_utility(const FogUtility& aUtility) { _parse_context.set_utility(aUtility); }
FogToken& FogDecoratedParseContext::static_token() { return _parse_context.static_token(); }
const FogToken& FogDecoratedParseContext::static_token() const { return _parse_context.static_token(); }
FogTargetFile* FogDecoratedParseContext::target_file() const { return _parse_context.target_file(); }
const FogTemplateParameterSpecifiers* FogDecoratedParseContext::template_parameters() const
{ return _parse_context.template_parameters(); }
const FogUtility& FogDecoratedParseContext::utility() const { return _parse_context.utility(); }

FogDynamicParseContext::FogDynamicParseContext(FogParseContext& parseContext, const FogRaw& aDeclaration)
	:
	Super(parseContext),
	FogTokenMonitor(aDeclaration),
	_declaration(aDeclaration)
{}

FogDynamicParseContext::~FogDynamicParseContext() {}
const FogRaw& FogDynamicParseContext::declaration() const { return *_declaration; }

const FogTemplateParameterSpecifiers& FogDynamicParseContext::explicit_template_parameters() const {
	if (!_template_parameters) {
		mutate()._template_parameters.adopt(new FogTemplateParameterSpecifiers(Super::template_parameters(),
		                                    NON_EXPORT_TEMPLATE, NON_USING_TEMPLATE));
	}
	
	return *_template_parameters;
}

const FogTemplateParameterSpecifiers* FogDynamicParseContext::template_parameters() const { return 0; }

//  ---------------------------------------------------------------------------------------------------------------------

FogEmitParseContext::FogEmitParseContext(FogEmitContext& emitContext, const FogRaw& aDeclaration)
	:
	Super(emitContext.dynamic_token(), aDeclaration, FogAccess::invalid_access(), FogUtility::invalid_utility())
{}

const FogScopeContext* FogEmitParseContext::meta_context() const { return this; }

FogMembersParseContext::FogMembersParseContext(FogMakeEntityContext& makeEntityContext, const FogRaw& aDeclaration,
        FogScope& aScope, const FogAccess& anAccess)
	:
	Super(aScope, aDeclaration, anAccess, makeEntityContext.utility()),
	_make_entity_context(makeEntityContext) {
	//      const FogUtility& scopeUtility = aScope.utility();
	//      const FogUtility& contextUtility = makeEntityContext.utility();
	//      if (scopeUtility != contextUtility)
	//          ERRMSG("INVESTIGATE -- inconsistent " << scopeUtility.str() << " and " << contextUtility.str());
}

FogMembersParseContext::~FogMembersParseContext() {}
FogScopeContext* FogMembersParseContext::find_context(size_t dollarCount)
{ return dollarCount > 0 ? _make_entity_context.find_context(dollarCount) : this; }
bool FogMembersParseContext::find_slots(FogMetaSlotFinder& theFinder)
{ return dynamic_token().find_slots(theFinder); }
const FogTemplateParameterSpecifier* FogMembersParseContext::find_template_parameter_specifier
(const PrimId& anId) { return _make_entity_context.find_template_parameter_specifier(anId); }
const FogTemplateParameterSpecifier* FogMembersParseContext::find_template_parameter_specifier
(const FogTemplateParameterSpecifierId& anId)
{ return _make_entity_context.find_template_parameter_specifier(anId); }

std::ostream& FogMembersParseContext::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	_make_entity_context.print_deep(s, aDepth);
	return s;
}

std::ostream& FogMembersParseContext::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	_make_entity_context.print_on(s, aDepth);
	return s;
}

const FogTemplateParameterSpecifiers* FogMembersParseContext::template_parameters() const
{ return _make_entity_context.template_parameters(); }

FogMetaParseContext::FogMetaParseContext(FogParseContext& parseContext)
	:
	Super(parseContext)
{}

const FogScopeContext* FogMetaParseContext::meta_context() const { return this; }

//  ---------------------------------------------------------------------------------------------------------------------

FogScopeParseContext::FogScopeParseContext(FogToken& aToken, const FogRaw& aDeclaration,
        const FogAccess& anAccess, const FogUtility& aUtility)
	:
	Super(aToken),
	_monitor(aDeclaration),
	_declaration(aDeclaration),
	_access(anAccess),
	_utility(aUtility)
{}

FogScopeParseContext::~FogScopeParseContext() {}
const FogAccess& FogScopeParseContext::access() const { return *_access; }
const FogRaw& FogScopeParseContext::declaration() const { return *_declaration; }
void FogScopeParseContext::set_access(const FogAccess& anAccess) { _access = anAccess; }
void FogScopeParseContext::set_utility(const FogUtility& aUtility) { _utility = aUtility; }
const FogUtility& FogScopeParseContext::utility() const { return *_utility; }

//  ---------------------------------------------------------------------------------------------------------------------

FogStaticParseContext::FogStaticParseContext(FogToken& aToken, const FogRaw& aDeclaration,
        const FogAccess& anAccess, const FogUtility& aUtility)
	:
	Super(aToken, aDeclaration, anAccess, aUtility),
	_target_file(aUtility.is_hash() ? file_manager().make_target_file(aDeclaration.line().file()) : 0)
{}

FogScopeContext* FogStaticParseContext::find_context(size_t dollarCount) {
	if (dollarCount == 0)
		return this;
	//      else if (dollarCount == 1)
	//          return &dynamic_token().global_scope().root_context();
	else
		return 0;
}

FogTargetFile* FogStaticParseContext::target_file() const { return _target_file; }

FogSubspaceParseContext::FogSubspaceParseContext(FogScopeContext& scopeContext,
        FogPotentialDeclaration& potentialDeclaration)
	:
	Super(potentialDeclaration, scopeContext),
	_scope_context(scopeContext),
	_potential_declaration(potentialDeclaration)
{}

FogSubspaceParseContext::~FogSubspaceParseContext() {}

const FogAccess& FogSubspaceParseContext::access() const {
	ERRMSG("BUG - no implementation of FogSubspaceParseContext::access()");
	return FogAccess::invalid_access();
}

bool FogSubspaceParseContext::at_root() const { return _potential_declaration.is_root(); }
const FogRaw& FogSubspaceParseContext::declaration() const { return FogRaw::immutable_null(); }

void FogSubspaceParseContext::set_access(const FogAccess& anAccess) {
	ERRMSG("BUG - no implementation of FogSubspaceParseContext::set_access()");
}

void FogSubspaceParseContext::set_utility(const FogUtility& aUtility) {
	ERRMSG("BUG - no implementation of FogSubspaceParseContext::set_utility()");
}

const FogUtility& FogSubspaceParseContext::utility() const {
	//      ERRMSG("INVESTIGATE - trial implementation of FogSubspaceParseContext::utility()");
	//      return FogUtility::invalid_utility();
	return static_token().utility();
}

FogTemplateParameterParseContext::FogTemplateParameterParseContext(FogParseContext& parseContext,
        const FogTemplateParameterSpecifiers& templateParameters, const FogRaw& aDeclaration)
	:
	Super(parseContext),
	FogTokenMonitor(aDeclaration),
	_declaration(aDeclaration),
	_template_parameters(templateParameters) {
	FogResolutionScopeContext resolutionContext(*this, LOCATE_TEMPLATE_FORMALS);
	aDeclaration.make_actual(_declaration, resolutionContext);
}

FogTemplateParameterParseContext::~FogTemplateParameterParseContext() {}
const FogRaw& FogTemplateParameterParseContext::declaration() const { return *_declaration; }
const FogTemplateParameterSpecifiers& FogTemplateParameterParseContext::explicit_template_parameters() const
{ return *_template_parameters; }

const FogTemplateParameterSpecifier* FogTemplateParameterParseContext::find_template_parameter_specifier
(const PrimId& anId) {
	const FogTemplateParameterSpecifier* templateParameterSpecifier = _template_parameters->find(*this, anId);
	return templateParameterSpecifier ? templateParameterSpecifier : Super::find_template_parameter_specifier(anId);
}

const FogTemplateParameterSpecifier* FogTemplateParameterParseContext::find_template_parameter_specifier
(const FogTemplateParameterSpecifierId& anId) {
	const FogTemplateParameterSpecifier* templateParameterSpecifier = _template_parameters->find(anId);
	return templateParameterSpecifier ? templateParameterSpecifier : Super::find_template_parameter_specifier(anId);
}

const FogTemplateParameterSpecifiers* FogTemplateParameterParseContext::template_parameters() const
{ return _template_parameters.pointer(); }

//  ---------------------------------------------------------------------------------------------------------------------
FogTokenParseContext::FogTokenParseContext(FogParseContext& parseContext, const FogToken& aToken)
	:
	Super(parseContext),
	FogTokenMonitor(aToken)
{}

FogUnbreakableParseContext::FogUnbreakableParseContext(FogParseContext& parseContext, const FogRaw& aDeclaration)
	:
	Super(parseContext),
	FogTokenMonitor(aDeclaration),
	_declaration(aDeclaration)
{}

FogUnbreakableParseContext::~FogUnbreakableParseContext() {}
const FogRaw& FogUnbreakableParseContext::declaration() const { return *_declaration; }

std::ostream& FogUnbreakableParseContext::print_message(std::ostream& s) const {
	const FogLine& aLine = _declaration->line();
	s << "At " << viz(aLine) << ": " << viz(*_declaration) << ".";
	//      FogStream strStream((FogScope&)dynamic_scope());                            //.bugbug cast and unchecked
	//      FogStaticEmitContext emitContext(strStream, (FogToken&)dynamic_token(), FogEmitContext::RAW);   //.bugbug cast
	//      emitContext.emit_space_and_text("Parsing\n");
	//      {
	//          FogSeparatedEmitContext separatedContext(emitContext, "", "\n");
	//          FogIndentEmitContext indentedEmitContext(separatedContext, 1, FogEmitContext::INDENTS);
	//          _declaration->emit(indentedEmitContext);
	//      }
	//      print_message_text(s, strStream.str(), 1);
	return s;
}
