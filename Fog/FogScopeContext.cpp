#include <Fog/FogIncludeAll.h>
















TYPEINFO_SINGLE(FogScopeContext, Super)
TYPEINFO_SINGLE(FogBracedScopeContext, Super)
TYPEINFO_SINGLE(FogDecoratedScopeContext, Super)
TYPEINFO_SINGLE(FogInScopeContext, Super)
TYPEINFO_SINGLE(FogMergeContext, Super)
TYPEINFO_SINGLE(FogMetaScopeContext, Super)
TYPEINFO_SINGLE(FogMultiDollarScopeContext, Super)
TYPEINFO_SINGLE(FogNestedScopeContext, Super)
TYPEINFO_SINGLE(FogPotentialDeclarationScopeContext, Super)
TYPEINFO_SINGLE(FogResolutionScopeContext, Super)
TYPEINFO_SINGLE(FogStaticScopeContext, Super)
TYPEINFO_SINGLE(FogTemplatedScopeContext, Super)
TYPEINFO_SINGLE(FogUnresolvableScopeContext, Super)
TYPEINFO_SINGLE(FogUnresolvableFunctionScopeContext, Super)
TYPEINFO_SINGLE(FogUnresolvableTypeScopeContext, Super)

//
//    Return true if this context is_root with respect to its potentialDeclaration. This method is invoked from
//    FogObjectSpecifier to interrogate its potentialDeclaration caller.
//
bool FogScopeContext::at_root() const { return true; }

//
//    Return the entity associated with the current context.
//  .bugbug This is a tacky workaround for poor ObjectStatement polymorphism.
FogEntity& FogScopeContext::dynamic_entity() {
	FogToken& dynamicToken = dynamic_token();
	FogEntity* anEntity = dynamicToken.is_entity();
	
	if (anEntity)
		return *anEntity;
		
	FogPotentialDeclaration* potentialDeclaration = dynamicToken.is_potential_declaration();
	
	if (potentialDeclaration)
		return potentialDeclaration->entity();
		
	ERRMSG("BUG -- failed to resolve dynamic_entity() for " << viz(*this));
	return dynamic_scope();
}

FogScope& FogScopeContext::dynamic_scope() { return dynamic_token().inner_scope(); }
const FogScope& FogScopeContext::dynamic_scope() const { return dynamic_token().inner_scope(); }
FogFileManager& FogScopeContext::file_manager() { return dynamic_token().global_scope().file_manager(); }
FogRoot& FogScopeContext::global_scope() { return dynamic_token().global_scope(); }

//
//    Traverse the scope contexts to locate the outermost call context.
//
FogCallContext* FogScopeContext::find_call_context() { return 0; }

//
//    Return the context in which a symbol with dollarCount $ prefixes should be resolved.
//
FogScopeContext* FogScopeContext::find_context(size_t dollarCount) { return dollarCount == 0 ? this : 0; }

void FogScopeContext::find_entities(FogEntityFinder& theFinder) {
	dynamic_token().find_entities(theFinder);
	//  .bugbug diagnose missing template arguments and/or select those corresponding to default template
}

bool FogScopeContext::find_formal_slots(FogMetaSlotFinder& theFinder) { return false; }

//
//    Traverse the scope contexts to locate the outermost make-entity context.
//
FogMakeEntityContext* FogScopeContext::find_make_entity_context() { return 0; }

//
//    Return the TemplateParameterSpecifier visible under the unnormalised-name anId.
//
const FogTemplateParameterSpecifier* FogScopeContext::find_template_parameter_specifier(const PrimId& anId)
{ return 0; }

//
//    Return the TemplateParameterSpecifier visible under the normalised-name anId.
//
const FogTemplateParameterSpecifier* FogScopeContext::find_template_parameter_specifier
(const FogTemplateParameterSpecifierId& anId) {
	ERRMSG("INVESTIGATE -- failed to find " << viz(anId) << " in " << viz(*this));
	return 0;
}

FogScopeContext::InScope FogScopeContext::in_scope() const { return IN_ANY_SCOPE; }
FogEmitContext* FogScopeContext::is_emit_context() { return 0; }

bool FogScopeContext::is_unresolvable() const {
	const Resolution aResolution = resolution();
	return (aResolution & SCOPE_UNKNOWN) != 0;
}

//  FogScopeContext::Resolution FogScopeContext::merge_resolution(Resolution aResolution, Resolution bResolution)
//  {
//      int aBits = aResolution & RESOLVE_MASK;
//      int bBits = bResolution & RESOLVE_MASK;
//      return Resolution((aResolution & SCOPE_MASK) | (bResolution & SCOPE_MASK) | (aBits >= bBits ? aBits : bBits));
//  }

const FogScopeContext* FogScopeContext::meta_context() const { return 0; }

FogScopeContext& FogScopeContext::null() {
	static FogStaticScopeContext nullContext(FogScope::mutable_null());
	return nullContext;
}

std::ostream& FogScopeContext::print_resolution(std::ostream& s) const {
	if (at_meta())
		s << "META+";
		
	Resolution aResolution = resolution();
	s << (aResolution & SCOPE_UNKNOWN ? "SCOPE_UNKNOWN+" : "SCOPE_KNOWN+");
	
	switch (aResolution & RESOLVE_MASK) {
	case RESOLVE_ATS: s << "RESOLVE_ATS"; break;
	
	case RESOLVE_DOLLARS: s << "RESOLVE_DOLLARS"; break;
	
	case RESOLVE_MULTI_DOLLARS: s << "RESOLVE_MULTI_DOLLARS"; break;
	
	case RESOLVE_META_FUNCTION_FORMALS: s << "RESOLVE_META_FUNCTION_FORMALS"; break;
	
	case LOCATE_META_FUNCTION_FORMALS: s << "LOCATE_META_FUNCTION_FORMALS"; break;
	
	case LOCATE_TEMPLATE_FORMALS: s << "LOCATE_TEMPLATE_FORMALS"; break;
	
	case REPAIR_SEMANTICS: s << "REPAIR_SEMANTICS"; break;
	
	case RESOLVE_TYPE_STRONGLY: s << "RESOLVE_TYPE_STRONGLY"; break;
	
	case RESOLVE_TYPE_WEAKLY: s << "RESOLVE_TYPE_WEAKLY"; break;
	
	case RESOLVE_EMISSION: s << "RESOLVE_EMISSION"; break;
	
	case RESOLVE_USAGE: s << "RESOLVE_USAGE"; break;
	
	case RESOLVE_LAZY: s << "RESOLVE_LAZY"; break;
	}
	
	const FogTemplateParameterSpecifiers* templateParameterSpecifiers = template_parameters();
	
	if (templateParameterSpecifiers) {
		s << ", ";
		templateParameterSpecifiers->print_named(s, 0, 0);
	}
	
	return s << ", ";
}

std::ostream& FogScopeContext::print_this(std::ostream& s) const {
	print_resolution(s);
	dynamic_token().print_long_id(s);
	s << " <- ";
	static_token().print_long_id(s);
	return s;
}

std::ostream& FogScopeContext::print_viz(std::ostream& s) const { return dynamic_token().print_viz(s); }

bool FogScopeContext::requires_formal_location_or_resolution() const {
	const Resolution aResolution = resolution();
	return ((aResolution & RESOLVE_MASK) == LOCATE_META_FUNCTION_FORMALS)
	       ||  ((aResolution & RESOLVE_MASK) == LOCATE_TEMPLATE_FORMALS)
	       ||  ((aResolution & RESOLVE_MASK) == RESOLVE_META_FUNCTION_FORMALS)
	       ||  ((aResolution & RESOLVE_MASK) == RESOLVE_MULTI_DOLLARS);
}

bool FogScopeContext::requires_resolution() const {
	const Resolution aResolution = resolution();
	//      return aResolution <= DOLLAR_REQUIRED;
	//      if ((aResolution & SCOPE_UNKNOWN) != 0)
	//          ERRMSG("INVESTIGATE -- SCOPE_UNKNOWN in FogScopeContext::requires_resolution.");
	return ((aResolution & SCOPE_MASK) == SCOPE_KNOWN)
	       && (((aResolution & RESOLVE_MASK) == RESOLVE_ATS)
	           || ((aResolution & RESOLVE_MASK) == RESOLVE_DOLLARS));
	//           || ((aResolution & RESOLVE_MASK) == RESOLVE_MULTI_DOLLARS)); -- excluded to avoid double diagnostic
	//           || ((aResolution & RESOLVE_MASK) == RESOLVE_META_FUNCTION_FORMALS)); -- never SCOPE_KNOWN
}

FogScopeContext::Resolution FogScopeContext::resolution() const { return RESOLVE_ATS; }

FogScope& FogScopeContext::static_scope() { return static_token().inner_scope(); }
const FogScope& FogScopeContext::static_scope() const { return static_token().inner_scope(); }

//
//    Return the innermost template parameterisation (from which the outers may be deduced).
//    No trimming occurs, so the caller must how the parameters correlate with scopes.
//
const FogTemplateParameterSpecifiers* FogScopeContext::template_parameters() const { return 0; }

FogScopeContext& FogScopeContext::weak() {
	static FogStaticScopeContext nullContext(FogScope::mutable_null());
	static FogResolutionScopeContext weakContext(nullContext, RESOLVE_TYPE_WEAKLY);
	return weakContext;
}

FogBracedScopeContext::FogBracedScopeContext(FogScopeContext& parseContext, FogToken& aToken)
	:
	Super(parseContext),
	_dynamic_token(aToken)
{}

FogToken& FogBracedScopeContext::dynamic_token() { return _dynamic_token; }
const FogToken& FogBracedScopeContext::dynamic_token() const { return _dynamic_token; }
FogScopeContext* FogBracedScopeContext::find_context(size_t dollarCount)
{ return dollarCount > 0 ? scope_context().find_context(dollarCount - 1) : this; }

std::ostream& FogBracedScopeContext::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	_dynamic_token.print_deep(s, aDepth);
	return s;
}

//  ---------------------------------------------------------------------------------------------------------------------

bool FogDecoratedScopeContext::at_root() const { return _context.at_root(); }
FogToken& FogDecoratedScopeContext::dynamic_token() { return _context.dynamic_token(); }
const FogToken& FogDecoratedScopeContext::dynamic_token() const { return scope_context().dynamic_token(); }
FogCallContext* FogDecoratedScopeContext::find_call_context() { return scope_context().find_call_context(); }
FogScopeContext* FogDecoratedScopeContext::find_context(size_t dollarCount)
{ return dollarCount > 0 ? _context.find_context(dollarCount) : this; }
bool FogDecoratedScopeContext::find_formal_slots(FogMetaSlotFinder& theFinder)
{ return _context.find_formal_slots(theFinder); }
FogMakeEntityContext* FogDecoratedScopeContext::find_make_entity_context()
{ return scope_context().find_make_entity_context(); }
bool FogDecoratedScopeContext::find_slots(FogMetaSlotFinder& theFinder) { return _context.find_slots(theFinder); }
const FogTemplateParameterSpecifier* FogDecoratedScopeContext::find_template_parameter_specifier
(const PrimId& anId) { return _context.find_template_parameter_specifier(anId); }
const FogTemplateParameterSpecifier* FogDecoratedScopeContext::find_template_parameter_specifier
(const FogTemplateParameterSpecifierId& anId) { return _context.find_template_parameter_specifier(anId); }
FogScopeContext::InScope FogDecoratedScopeContext::in_scope() const { return _context.in_scope(); }
const FogScopeContext* FogDecoratedScopeContext::meta_context() const { return _context.meta_context(); }

std::ostream& FogDecoratedScopeContext::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	_context.print_deep(s, aDepth);
	return s;
}

std::ostream& FogDecoratedScopeContext::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	_context.print_on(s, aDepth);
	return s;
}

FogScopeContext::Resolution FogDecoratedScopeContext::resolution() const { return _context.resolution(); }
FogToken& FogDecoratedScopeContext::static_token() { return _context.static_token(); }
const FogToken& FogDecoratedScopeContext::static_token() const { return scope_context().static_token(); }
const FogTemplateParameterSpecifiers* FogDecoratedScopeContext::template_parameters() const
{ return _context.template_parameters(); }

FogInScopeContext::FogInScopeContext(FogScopeContext& scopeContext, InScope inScope)
	:
	Super(scopeContext),
	_in_scope(inScope)
{}

FogScopeContext::InScope FogInScopeContext::in_scope() const { return _in_scope; }

//  ---------------------------------------------------------------------------------------------------------------------

FogMergeContext::FogMergeContext(FogScopeContext& scopeContext, bool leftIsAnon, bool rightIsAnon)
	:
	Super(scopeContext),
	_left_is_anon(leftIsAnon),
	_right_is_anon(rightIsAnon)
{}

//  ---------------------------------------------------------------------------------------------------------------------

FogMetaScopeContext::FogMetaScopeContext(FogScopeContext& scopeContext, bool isMeta)
	:
	Super(scopeContext),
	_is_meta(isMeta)
{}

const FogScopeContext* FogMetaScopeContext::meta_context() const { return _is_meta ? this : 0; }

//  ---------------------------------------------------------------------------------------------------------------------

FogMultiDollarScopeContext::FogMultiDollarScopeContext(FogScopeContext& scopeContext)
	:
	Super(scopeContext)
{}

FogScopeContext::Resolution FogMultiDollarScopeContext::resolution() const
{ return Resolution(RESOLVE_MULTI_DOLLARS | SCOPE_UNKNOWN); }

FogNestedScopeContext::FogNestedScopeContext(FogScopeContext& scopeContext, FogToken& inToken)
	:
	Super(scopeContext),
	_dynamic_token(inToken),
	_in_scope(scopeContext.in_scope() == IN_THIS_SCOPE ? IN_THIS_SCOPE : IN_BASE_SCOPE)
{}

FogNestedScopeContext::FogNestedScopeContext(FogScopeContext& scopeContext, FogToken& inToken, InScope inScope)
	:
	Super(scopeContext),
	_dynamic_token(inToken),
	_in_scope(inScope)
{}

FogToken& FogNestedScopeContext::dynamic_token() { return _dynamic_token; }
const FogToken& FogNestedScopeContext::dynamic_token() const { return _dynamic_token; }

bool FogNestedScopeContext::find_slots(FogMetaSlotFinder& theFinder) {
	FogMakeEntityContext* makeEntityContext = find_make_entity_context();
	
	if (makeEntityContext && makeEntityContext->find_formal_slots(theFinder))
		return true;
		
	if (_dynamic_token.find_slots(theFinder))
		return true;
		
	return Super::find_slots(theFinder);
}

FogScopeContext::InScope FogNestedScopeContext::in_scope() const { return _in_scope; }

//  ---------------------------------------------------------------------------------------------------------------------

FogPotentialDeclarationScopeContext::FogPotentialDeclarationScopeContext(FogScopeContext& scopeContext,
        FogPotentialDeclaration& potentialDeclaration, bool atRoot)
	:
	Super(scopeContext, potentialDeclaration),
	_at_root(atRoot)
{}

bool FogPotentialDeclarationScopeContext::at_root() const { return _at_root; }

//  ---------------------------------------------------------------------------------------------------------------------

FogResolutionScopeContext::FogResolutionScopeContext(FogScopeContext& scopeContext, Resolution aResolution)
	:
	Super(scopeContext),
	_resolution(Resolution((scopeContext.resolution() & SCOPE_MASK) | (aResolution & RESOLVE_MASK)))
{}

FogScopeContext::Resolution FogResolutionScopeContext::resolution() const { return _resolution; }

FogStaticScopeContext::FogStaticScopeContext(FogToken& aToken, InScope inScope)
	:
	_static_token(aToken),
	_in_scope(inScope)
{}

FogToken& FogStaticScopeContext::dynamic_token() { return _static_token; }
const FogToken& FogStaticScopeContext::dynamic_token() const { return _static_token; }
bool FogStaticScopeContext::find_slots(FogMetaSlotFinder& theFinder) { return _static_token.find_slots(theFinder); }
FogScopeContext::InScope FogStaticScopeContext::in_scope() const { return _in_scope; }
FogToken& FogStaticScopeContext::static_token() { return _static_token; }
const FogToken& FogStaticScopeContext::static_token() const { return _static_token; }

//  ---------------------------------------------------------------------------------------------------------------------

FogTemplatedScopeContext::FogTemplatedScopeContext(FogScopeContext& scopeContext, const FogTemplatedName& templatedName)
	:
	Super(scopeContext),
	_templated_name(templatedName)
{}

void FogTemplatedScopeContext::find_entities(FogEntityFinder& theFinder) {
	FogEntityFinding theFinding(theFinder.strategy());
	FogEntityFinder nestedFinder(theFinding, theFinder);
	dynamic_token().find_entities(nestedFinder);
	FogEntity* scopeEntity = theFinding.get_unambiguous_finding(dynamic_token(), *this);
	
	if (scopeEntity) {
		FogTemplateArgsRefToConst templateArgs;
		
		if (_templated_name.resolve_template(scopeEntity->template_manager().primary(), templateArgs, *this)) {
			FogMakeTemplateContext makeContext(*this, *scopeEntity, *templateArgs, IS_REFERENCE);
			FogEntity* actualEntity = scopeEntity->find_template(makeContext);
			
			if (actualEntity)
				theFinder.add_find(*actualEntity);
		}
	}
}

std::ostream& FogTemplatedScopeContext::print_resolution(std::ostream& s) const {
	Super::print_resolution(s);
	s << " < ";
	_templated_name.exprs().print_named(s, 0, 0);
	s << " >, ";
	return s;
}

FogUnresolvableScopeContext::FogUnresolvableScopeContext(FogScopeContext& scopeContext)
	:
	Super(scopeContext),
	_resolution(Resolution(scopeContext.resolution() | SCOPE_UNKNOWN))
{}

FogScopeContext::Resolution FogUnresolvableScopeContext::resolution() const { return _resolution; }

//  ---------------------------------------------------------------------------------------------------------------------

FogUnresolvableFunctionScopeContext::FogUnresolvableFunctionScopeContext(
        FogScopeContext& scopeContext, FogMetaFunctionExpr& anExpr)
	:
	Super(scopeContext),
	_function_expr(anExpr)
{}

bool FogUnresolvableFunctionScopeContext::find_formal_slots(FogMetaSlotFinder& theFinder) {
	const FogToken* metaArgument = _function_expr.find(*this, theFinder.id());
	return metaArgument != 0;
}

bool FogUnresolvableFunctionScopeContext::find_slots(FogMetaSlotFinder& theFinder) {
	if (!find_formal_slots(theFinder))
		_function_expr.find_slots(theFinder);
		
	return true;
}

//  ---------------------------------------------------------------------------------------------------------------------

FogUnresolvableTypeScopeContext::FogUnresolvableTypeScopeContext(FogScopeContext& scopeContext,
        FogTypeSpecifier& typeSpecifier)
	:
	Super(scopeContext),
	_type_specifier(typeSpecifier)
{}

bool FogUnresolvableTypeScopeContext::find_slots(FogMetaSlotFinder& theFinder) {
	find_formal_slots(theFinder);                   //   Need to search further but only to resolve formals
	return true;
}
