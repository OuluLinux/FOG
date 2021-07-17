#include <Fog/FogIncludeAll.h>













TYPEINFO_SINGLE(FogTokensExpr, Super)
//  PRIMREF_NULL_CLAS(FogTokensExpr)
//      FOGTOKEN_NULL_LEAF
//  };
//  PRIMREF_DERIVED_NULL_IMPL(FogTokensExpr)
//  PRIMREFS_IMPL(FogTokensExpr)
FOGTOKEN_LEAF_IMPL(FogTokensExpr)

FogTokensExpr::FogTokensExpr(FogTokens& someTokens)
	:
	_tokens(someTokens.tokens())
{}

FogTokensExpr::FogTokensExpr(const This& anExpr)
	:
	Super(anExpr),
	_tokens(anExpr._tokens)
{}

void FogTokensExpr::create_usage(FogUsageContext& aContext) const {
	Super::create_usage(aContext);
	
	for (FogTokenConstListOfRefToConstIterator p(_tokens); p; ++p)
		p->create_usage(aContext);
}

bool FogTokensExpr::emit(FogEmitContext& emitContext) const {
	const char* oldSeparator = emitContext.separator();
	const char* oldTerminator = emitContext.terminator();
	
	if (!oldSeparator)
		ERRMSG("INVESTIGATE -- no separator defined for FogTokensExpr::emit " << viz(*this));
		
	bool gotSomething = true;
	
	if (is_exposed()) {
		FogSeparatedEmitContext separatedContext(emitContext, oldSeparator ? oldSeparator : ",", oldTerminator);
		gotSomething = false;
		
		for (FogTokenConstListOfRefToConstIterator p(_tokens); p; ++p) {
			if (p->is_null()) {
				emitContext.emit_space_and_text("0");
				gotSomething = true;
			}
			else {
				if (p->emit(emitContext))
					gotSomething = true;
			}
			
			if ((p.to_go() > 1) && emitContext.separate())
				gotSomething = true;
		}
	}
	else if (!_tokens.tally())
		emitContext.emit_space_and_text(" {}");
	else {
		emitContext.start();
		emitContext.emit_space_and_text("{");
		FogSeparatedEmitContext separatedContext(emitContext, oldSeparator ? oldSeparator : ",", "\n");
		{
			FogIndentEmitContext indentedContext(separatedContext, 1, FogEmitContext::INDENTS);
			
			for (FogTokenConstListOfRefToConstIterator p(_tokens); p; ++p) {
				indentedContext.start();
				
				if (p->is_null())
					indentedContext.emit_space_and_text("0");
				else
					p->emit(indentedContext);
					
				if (p.to_go() > 1)
					indentedContext.separate();
			}
		}
		separatedContext.terminate();
		emitContext.emit_space_and_text("}");
	}
	
	return gotSomething;
}

size_t FogTokensExpr::executable_tokens() const {
	size_t executableTokens = 0;
	
	for (FogTokenConstListOfRefToConstIterator p(_tokens); p; ++p)
		executableTokens += p->executable_tokens();
		
	return executableTokens;
}

bool FogTokensExpr::get_object(FogTokenRef& returnValue, FogScopeContext& inScope) const {
	if (_tokens.tally() > 0) {
		if (_tokens.tally() > 1)
			ERRMSG("Should not use sequence of expressions in constant expression.");
			
		return _tokens[_tokens.tally() - 1]->get_object(returnValue, inScope);
	}
	else {
		ERRMSG("INVESTIGATE -- transisitising unit-list for get_object of " << viz(*this));
		return Super::get_object(returnValue, inScope);
	}
}

void FogTokensExpr::install(FogInstallContext& installContext) const {
	Super::install(installContext);
	
	for (FogTokenConstListOfRefToConstIterator p(_tokens); p; ++p)
		p->install(installContext);
}

bool FogTokensExpr::is_actual(const FogScopeContext& scopeContext) const {
	for (FogTokenConstListOfRefToConstIterator p(_tokens); p; ++p)
		if (!p->is_actual(scopeContext))
			return false;
			
	return Super::is_actual(scopeContext);
}

void FogTokensExpr::make_actual_from(FogMakeActualContext& makeActualContext) {
	Super::make_actual_from(makeActualContext);
	
	for (FogTokenListOfRefToConstIterator p(_tokens); p; ++p)
		p->make_actual(p.ref(), makeActualContext);
}

void FogTokensExpr::merge_from(FogMergeContext& mergeContext, const This& thatExpr) {
	Super::merge_from(mergeContext, thatExpr);
	//    Super::merge_long_into(mergeContext, _tokens, thatExpr._tokens);
	FogToken::merge_long_into(mergeContext, _tokens, thatExpr._tokens);
}

#if 0
bool FogTokensExpr::morph_to(FogTokenRef& returnValue, const FogMetaType& metaType, IsExposed isExposed,
                             FogScopeContext& inScope) const {
	FogListOfExprRef morphedExprs(new FogTokensExpr, FogListOfExprRef::ADOPT);
	FogTokenRef morphedToken;
	
	for (FogTokenConstListOfRefToConstIterator p(_tokens); p; ++p) {
		if (!p->morph_to(morphedToken, metaType, isExposed, inScope))
			return false;
			
		FogExpr* morphedExpr = morphedToken->is_expr();
		
		if (!morphedExpr) {
			ERRMSG("BUG -- morphed " << viz(*p) << " is not an expression");
			return false;
		}
		
		morphedExprs->add(*morphedExpr);
	}
	
	returnValue = *morphedExprs;
	return true;
}
#endif

const FogMerge& FogTokensExpr::needs_merge_from(FogMergeContext& mergeContext, const This& anExpr) const {
	FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, anExpr));
	FogTokenConstListOfRefToConstIterator p1(_tokens);
	FogTokenConstListOfRefToConstIterator p2(anExpr._tokens);
	
	for ( ; p1 && p2; ++p1, ++p2) {
		if (p1->is_null()) {
			if (!p2->is_null())
				needsMerge |= FogMerge::left_invalid();
		}
		else {
			if (!p2->is_null())
				needsMerge |= p1->needs_merge(mergeContext, *p2);
			else
				needsMerge |= FogMerge::right_invalid();
		}
	}
	
	if (p1)
		needsMerge |= FogMerge::right_invalid();
	else if (p2)
		needsMerge |= FogMerge::left_invalid();
		
	return needsMerge;
}

std::ostream& FogTokensExpr::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	
	for (FogTokenConstListOfRefToConstIterator p(_tokens); p; ++p)
		p->print_deep(s, aDepth);
		
	return s;
}

std::ostream& FogTokensExpr::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	
	for (FogTokenConstListOfRefToConstIterator p(_tokens); p; ++p)
		p->print_on(s, aDepth);
		
	return s;
}

char FogTokensExpr::print_named(std::ostream& s, const PrimId* fullId, char tailChar) const {
	tailChar = FogStream::space_and_emit(s, tailChar, ")");
	
	for (FogTokenConstListOfRefToConstIterator p(_tokens); p; ++p) {
		if (p->is_null())
			tailChar = FogStream::space_and_emit(s, tailChar, "0");
		else
			tailChar = p->print_named(s, 0, tailChar);
			
		if (p.to_go() > 1)
			tailChar = FogStream::space_and_emit(s, tailChar, ", ");
	}
	
	tailChar = FogStream::space_and_emit(s, tailChar, ")");
	return tailChar;
}

std::ostream& FogTokensExpr::print_viz(std::ostream& s) const {
	s << "\"tokens-expression: ( ";
	print_this(s);
	return s << " )\"";
}

bool FogTokensExpr::resolve_semantics(FogSemanticsContext& theSemantics) const {
	if (_tokens.tally() == 0) {
		theSemantics.assign(FogSemantics::IS_EPSILON);
		return true;
	}
	
	for (FogTokenConstListOfRefIterator p(_tokens); p; ++p) {
		FogChildSemanticsContext termSemantics(theSemantics);
		
		if (!p->resolve_semantics(termSemantics))
			return false;
	}
	
	theSemantics.reset();
	return true;
}

FogTokenListOfRef& FogTokensExpr::tokens()  { return _tokens; }
const FogTokenListOfRefToConst& FogTokensExpr::tokens() const { return _tokens; }
