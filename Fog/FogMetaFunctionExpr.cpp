#include <Fog/FogIncludeAll.h>












TYPEINFO_SINGLE(FogMetaFunctionExpr, Super)
FOGTOKEN_LEAF_IMPL(FogMetaFunctionExpr)

FogMetaFunctionExpr::FogMetaFunctionExpr(FogExpr& anExpr, FogTokens& argList,
        IsExposed isExposed, FogTokenStatements& theBody)
	:
	Super(anExpr),
	_args(argList),
	_is_exposed(isExposed),
	_body(theBody) {
	theBody.set_exposed();
}

FogMetaFunctionExpr::FogMetaFunctionExpr(const This& anExpr)
	:
	Super(anExpr),
	_args(anExpr._args),
	_is_exposed(anExpr._is_exposed),
	_body(anExpr._body)
{}

FogMetaFunctionExpr::~FogMetaFunctionExpr() {}

void FogMetaFunctionExpr::create_usage(FogUsageContext& aContext) const {
	Super::create_usage(aContext);
	//    _code->create_usage(aContext);
}

bool FogMetaFunctionExpr::emit(FogEmitContext& emitContext) const {
	bool part1 = Super::emit(emitContext);
	bool part2 = false; //   _code->emit(emitContext); -- only invoked for signatures.
	
	if (!_is_exposed)
		return part1 || part2;
		
	emitContext.emit_space_and_text("[]");
	return true;
}

size_t FogMetaFunctionExpr::executable_tokens() const {
	return Super::executable_tokens();
}

const FogToken* FogMetaFunctionExpr::find(FogScopeContext& scopeContext, const PrimId& anId) const {
	for (FogTokenConstListOfRefToConstIterator p(_args->tokens()); p; ++p) {
		const FogName* argName = p->is_name();
		const FogTypedName* typedArgName = argName ? argName->is_typed_name() : 0;
		
		if (!typedArgName) {
			ERRMSG("Expected name-like argument for " << viz(*p));
			return 0;
		}
		
		PrimIdHandle theId;
		typedArgName->name().resolve_id(theId, scopeContext);
		
		if (theId == anId)
			return argName;
	}
	
	return 0;
}

void FogMetaFunctionExpr::install(FogInstallContext& installContext) const {
	Super::install(installContext);
	//    _code->install(installContext);
}

bool FogMetaFunctionExpr::is_actual(const FogScopeContext& scopeContext) const {
	return Super::is_actual(scopeContext) && _args->is_actual(scopeContext) && _body->is_actual(scopeContext);
}

void FogMetaFunctionExpr::make_actual_from(FogMakeActualContext& makeActualContext) {
	Super::make_actual_from(makeActualContext);
	_args->make_actual(_args.to_const(), makeActualContext);
	
	if (makeActualContext.is_normalise_templates())
		_body->make_actual(_body.to_const(), makeActualContext);
	else if (makeActualContext.is_locate_formals()) {
		FogBracedScopeContext bracedContext(makeActualContext, *this);
		FogUnresolvableFunctionScopeContext metaFunctionContext(bracedContext, *this);
		FogUnresolvableScopeContext resolutionContext(metaFunctionContext);
		_body->make_actual(_body.to_const(), resolutionContext);
	}
	else if (makeActualContext.is_resolve_dollars()) {
		FogBracedScopeContext bracedContext(makeActualContext, *this);
		FogMultiDollarScopeContext resolutionContext(bracedContext);
		_body->make_actual(_body.to_const(), resolutionContext);
	}
	else if (makeActualContext.is_resolve_multi_dollars()) {
		FogBracedScopeContext bracedContext(makeActualContext, *this);
		FogMultiDollarScopeContext resolutionContext(bracedContext);
		_body->make_actual(_body.to_const(), resolutionContext);
	}
	else if (makeActualContext.is_resolve_formals()) {
		FogBracedScopeContext bracedContext(makeActualContext, *this);
		FogUnresolvableScopeContext resolutionContext(bracedContext);
		_body->make_actual(_body.to_const(), resolutionContext);
	}
	else
		ERRMSG("INVESTIGATE -- use of unclassified FogMetaFunctionExpr::make_actual_from.");
}

bool FogMetaFunctionExpr::make_specifier(FogMakeSpecifierContext& makeSpecifierContext) {
	if (!expr().make_specifier(makeSpecifierContext))
		return false;
		
	FogMetaFunctionSpecifier* metaFunctionSpecifier = makeSpecifierContext.get_meta_function_specifier(*this);
	
	if (metaFunctionSpecifier) {
		//        FogMakeSemantics makeSemantics(FogMakeSemantics::MAKE_META_PARAMETER);
		for (FogTokenConstListOfRefIterator p(_args->tokens()); p; ++p) {
			FogExpr* pExpr = p->is_expr();
			
			if (!pExpr) {
				ERRMSG("Expected expression-like argument for " << viz(*p));
				return false;
			}
			
			FogBaseMakeSpecifierContext nestedMakeSpecifierContext(makeSpecifierContext,
			        pExpr->decl_specifiers(), FogMakeSemantics::MAKE_META_PARAMETER);
			        
			if (!pExpr->make_specifier(nestedMakeSpecifierContext)) {
				ERRMSG("Failed to make_specifier for " << viz(*pExpr));
				return false;
			}
			
			FogMetaParameterSpecifier* metaParameter = nestedMakeSpecifierContext.get_meta_parameter_specifier(*pExpr);
			
			if (metaParameter)
				metaFunctionSpecifier->add_parameter(*metaParameter);
		}
		
		if (_is_exposed)
			metaFunctionSpecifier->adopt(new FogArrayModifier(0));
			
		metaFunctionSpecifier->set_body(*_body);
	}
	
	return true;
}

void FogMetaFunctionExpr::merge_from(FogMergeContext& mergeContext, const This& thatExpr) {
	Super::merge_from(mergeContext, thatExpr);
	//    thatExpr._code->merge_into(mergeContext, _code.to_const());
}

const FogMerge& FogMetaFunctionExpr::needs_merge_from(FogMergeContext& mergeContext, const This& anExpr) const {
	FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, anExpr));
	//    needsMerge |= _code->needs_merge(mergeContext, *anExpr._code);
	return needsMerge;
}

std::ostream& FogMetaFunctionExpr::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	s << indent(aDepth) << "arguments\n";
	_args->tokens().print_depth(s, aDepth + 1);
	s << indent(aDepth) << "body\n";
	_body->tokens().print_depth(s, aDepth + 1);
	return s;
}

std::ostream& FogMetaFunctionExpr::print_long_id(std::ostream& s) const {
	expr().print_named(s, 0, 0);
	return s;
}

std::ostream& FogMetaFunctionExpr::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	s << indent(aDepth) << "arguments\n";
	_args->tokens().print_members(s, aDepth + 1);
	s << indent(aDepth) << "body\n";
	_body->tokens().print_members(s, aDepth + 1);
	return s;
}

char FogMetaFunctionExpr::print_named(std::ostream& s, const PrimId* fullId, char tailChar) const {
	tailChar = Super::print_named(s, fullId, tailChar);
	
	//    tailChar = _code->print_named(s, 0, tailChar);
	if (_is_exposed)
		tailChar = FogStream::space_and_emit(s, tailChar, "[]");
		
	return tailChar;
}

std::ostream& FogMetaFunctionExpr::print_viz(std::ostream& s) const {
	s << "\"meta-function-expression: ";
	print_named(s, 0, ' ');
	return s << '\"';
}

bool FogMetaFunctionExpr::resolve_semantics(FogSemanticsContext& theSemantics) const {
	FogChildSemanticsContext exprSemantics(theSemantics);
	FogChildSemanticsContext argSemantics(theSemantics);
	
	if (!expr().resolve_semantics(exprSemantics) || !_args->resolve_semantics(argSemantics))
		return false;
		
	bool isMetaParameterDeclarationClause = true;
	
	for (FogTokenConstListOfRefToConstIterator p(_args->tokens()); p; ++p) {
		FogChildSemanticsContext termSemantics(theSemantics);
		
		if (!p->resolve_semantics(termSemantics))
			return false;
			
		if (!termSemantics.is_meta_parameter_declaration())
			isMetaParameterDeclarationClause = false;
	}
	
	theSemantics.reset();
	
	if (isMetaParameterDeclarationClause) {
		if (exprSemantics.is_meta_ctor_name())
			theSemantics.set(FogSemantics::IS_META_CTOR_DEFINITION);
			
		if (exprSemantics.is_meta_ctor_declaration())
			theSemantics.set(FogSemantics::IS_META_CTOR_DEFINITION);
			
		if (exprSemantics.is_meta_dtor_name())
			theSemantics.set(FogSemantics::IS_META_DTOR_DEFINITION);
			
		if (exprSemantics.is_meta_dtor_declaration())
			theSemantics.set(FogSemantics::IS_META_DTOR_DEFINITION);
			
		if (exprSemantics.is_meta_function_declarator())
			theSemantics.set(FogSemantics::IS_META_FUNCTION_DEFINITOR);
			
		if (exprSemantics.is_meta_function_declaration())
			theSemantics.set(FogSemantics::IS_META_FUNCTION_DEFINITION);
			
		if (exprSemantics.is_meta_variable_declaration())
			theSemantics.set(FogSemantics::IS_META_FUNCTION_DEFINITION);
			
		if (exprSemantics.is_auto_ctor_name())
			theSemantics.set(FogSemantics::IS_AUTO_CTOR_DEFINITION);
			
		if (exprSemantics.is_auto_ctor_declaration())
			theSemantics.set(FogSemantics::IS_AUTO_CTOR_DEFINITION);
			
		if (exprSemantics.is_auto_dtor_name())
			theSemantics.set(FogSemantics::IS_AUTO_DTOR_DEFINITION);
			
		if (exprSemantics.is_auto_dtor_declaration())
			theSemantics.set(FogSemantics::IS_AUTO_DTOR_DEFINITION);
			
		if (exprSemantics.is_auto_function_declarator())
			theSemantics.set(FogSemantics::IS_AUTO_FUNCTION_DEFINITOR);
			
		if (exprSemantics.is_auto_function_declaration())
			theSemantics.set(FogSemantics::IS_AUTO_FUNCTION_DEFINITION);
			
		if (exprSemantics.is_auto_variable_declaration())
			theSemantics.set(FogSemantics::IS_AUTO_FUNCTION_DEFINITION);
	}
	
	return true;
}

void FogMetaFunctionExpr::set_decl_specifier_expression(FogExprRef& anExpr, FogDeclSpecifier& declSpecifiers) {
	set_decorated_decl_specifier_expression(anExpr, declSpecifiers);
}
