#include <Fog/FogIncludeAll.h>














TYPEINFO_SINGLE(FogTreeCallExpr, Super)
FOGTOKEN_LEAF_IMPL(FogTreeCallExpr)

FogTreeCallExpr::FogTreeCallExpr(FogExpr& anExpr, FogTokens& theArgs)
	:
	Super(anExpr),
	_args(theArgs)
{}

FogTreeCallExpr::FogTreeCallExpr(const This& anExpr)
	:
	Super(anExpr),
	_args(anExpr._args)
{}

FogTreeCallExpr::~FogTreeCallExpr() {}

void FogTreeCallExpr::create_usage(FogUsageContext& aContext) const {
	Super::create_usage(aContext);
	//    _args->create_usage(aContext);
}

bool FogTreeCallExpr::emit(FogEmitContext& emitContext) const {
	bool doneSomething = expr().emit(emitContext);
	
	if (emitContext.emit_expression()) {
		emitContext.emit_space_and_text("(");
		
		for (FogTokenConstListOfRefIterator p(_args->tokens()); p; ++p) {
			p->emit(emitContext);
			
			if (p.to_go() > 1)
				emitContext.emit_space_and_text(", ");
		}
		
		emitContext.emit_space_and_text(")");
		doneSomething = true;
	}
	else {
		FogForUseAs forUseAs(emitContext.for_use_as() | FogEmitContext::NO_SCOPE);
		
		if (!forUseAs.emit_parameter_names())
			forUseAs |= FogForUseAs::NO_NAME;
			
		FogForUseAsEmitContext parameterContext(emitContext, forUseAs);
		doneSomething |= _args->emit(parameterContext);
	}
	
	return doneSomething;
}

size_t FogTreeCallExpr::executable_tokens() const {
	return 1 + expr().executable_tokens() + _args->executable_tokens();
}

bool FogTreeCallExpr::get_object(FogTokenRef& tokenValue, FogScopeContext& inScope) const {
	if (inScope.is_unresolvable())
		return false;
		
	//          ERRMSG("INVESTIGATE -- Invoking FogTreeCallExpr::get_object in unexpected phase");
	if (!expr().get_object(tokenValue, inScope))
		return false;
		
	FogMetaFunctionEntity* metaFunctionEntity = tokenValue->is_meta_function_entity();
	FogTokenRef tempValue(tokenValue);
	
	if (!tempValue->get_dyadic(tokenValue, inScope, FogTokenType::character('('), *_args))
		return false;
		
	if (!metaFunctionEntity)        //   Error message comes from virtual failure in get_dyadic
		return true;
		
	return tokenValue->morph_to(tokenValue, metaFunctionEntity->type(), metaFunctionEntity->is_exposed(), inScope);
}

void FogTreeCallExpr::install(FogInstallContext& installContext) const {
	Super::install(installContext);
	_args->install(installContext);
}

bool FogTreeCallExpr::is_actual(const FogScopeContext& scopeContext) const {
	return Super::is_actual(scopeContext) && _args->is_actual(scopeContext);
}

FogTreeCallExpr* FogTreeCallExpr::is_tree_call_expr() { return this; }

void FogTreeCallExpr::make_actual_from(FogMakeActualContext& makeActualContext) {
	Super::make_actual_from(makeActualContext);
	_args->make_actual(_args.to_const(), makeActualContext);
}

void FogTreeCallExpr::merge_from(FogMergeContext& mergeContext, const This& thatExpr) {
	Super::merge_from(mergeContext, thatExpr);
	thatExpr._args->merge_into(mergeContext, _args.to_const());
}

const FogMerge& FogTreeCallExpr::needs_merge_from(FogMergeContext& mergeContext, const This& anExpr) const {
	FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, anExpr));
	needsMerge |= _args->needs_merge(mergeContext, *anExpr._args);
	return needsMerge;
}

std::ostream& FogTreeCallExpr::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	_args->print_deep(s, aDepth);
	return s;
}

std::ostream& FogTreeCallExpr::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	_args->print_on(s, aDepth);
	return s;
}

char FogTreeCallExpr::print_named(std::ostream& s, const PrimId* fullId, char tailChar) const {
	tailChar = Super::print_named(s, fullId, tailChar);
	tailChar = FogStream::space_and_emit(s, tailChar, "(");
	
	for (FogTokenConstListOfRefToConstIterator p(_args->tokens()); p; ++p) {
		tailChar = p->print_named(s, 0, tailChar);
		
		if (p.to_go() > 1)
			tailChar = FogStream::space_and_emit(s, tailChar, ", ");
	}
	
	tailChar = FogStream::space_and_emit(s, tailChar, ")");
	return tailChar;
}

std::ostream& FogTreeCallExpr::print_viz(std::ostream& s) const {
	s << "\"tree-call-expression: ";
	print_this(s);
	return s << '\"';
}

bool FogTreeCallExpr::resolve_slot(FogTokenRef& returnValue, FogScopeContext& scopeContext,
                                   const FogToken& objectValue) const {
	//  .bugbug ?? is this code ever used ??
	//      if (!scopeContext.is_make_entity_phase() && !scopeContext.is_unscopable())
	//          return objectValue.get_dyadic(returnValue, scopeContext, FogTokenType::character('('), *_args);
	if (scopeContext.is_resolve_dollars() || scopeContext.is_resolve_ats())
		//      {
		//          if (scopeContext.is_make_entity_phase() || scopeContext.is_unscopable())
		//              ERRMSG("INVESTIGATE -- unexpected FogTreeCallExpr::resolve_slot for " << viz(*this));
		return objectValue.get_dyadic(returnValue, scopeContext, FogTokenType::character('('), *_args);
		
	//      }
	//      else
	//      {
	//          if (!scopeContext.is_make_entity_phase() && !scopeContext.is_unscopable())
	//              ERRMSG("INVESTIGATE -- unexpected non-FogTreeCallExpr::resolve_slot for " << viz(*this));
	//      }
	//    returnValue = FogFailure::make();
	returnValue.reset();
	return false;
}
