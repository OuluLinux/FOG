#include <Fog/FogIncludeAll.h>











TYPEINFO_SINGLE(FogTokens, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogTokens)
PRIMREFS_IMPL(FogTokens)
FOGTOKEN_READ_ONLY_IMPL(FogTokens)
TMPL_HACK_FIX_DO(FogTokens)

FogTokens::FogTokens(IsExposed isExposed)
	:
	_is_exposed(isExposed)
{}

FogTokens::FogTokens(const This& tokenList)
	:
	Super(tokenList),
	_tokens(tokenList._tokens),
	_is_exposed(tokenList._is_exposed)
{}

FogTokens::~FogTokens() {}

void FogTokens::add(FogToken& aToken) {
	const FogMetaType& thatMetaType = aToken.meta_type();
	const FogMetaType& thisMetaType = meta_type();
	
	if (!thatMetaType.is(thisMetaType))
		ERRMSG("INVESTIGATE -- addition of " << viz(thatMetaType) << " to list of " << viz(thisMetaType));
		
	if (!aToken.is_exposed())
		_tokens.add(aToken);
	else if (aToken.is_list() != is_list())         //   Must be same type of list //.bugbug should be more abstract
		_tokens.add(aToken);                        //     -- needs emit to use correct separator.
	else {
		FogListOfExpr* listOfExpr = aToken.is_list_of_expr();
		
		if (listOfExpr)
			for (FogExprConstListOfRefIterator p1(listOfExpr->exprs()); p1; ++p1)
				_tokens.add(*p1);
		else {
			FogTokens* listOfToken = aToken.is_tokens();
			
			if (listOfToken)
				for (FogTokenConstListOfRefIterator p2(listOfToken->tokens()); p2; ++p2)
					_tokens.add(*p2);
			else {
				ERRMSG("INVESTIGATE -- addition of unknown exposed list in FogTokens::add.");
				_tokens.add(aToken);
			}
		}
	}
}

void FogTokens::add(const FogTokenListOfRef& someTokens) {
	_tokens.add(someTokens);
}

bool FogTokens::compile_declaration(FogParseContext& parseContext) const {
	bool allOk = true;
	
	for (FogTokenConstListOfRefToConstIterator p(_tokens); p; ++p) {
		const FogRaw* aRaw = p->is_raw();
		
		if (aRaw) {
			FogDynamicParseContext childParseContext(parseContext, *(FogRaw*)aRaw);     //  .bugbug cast
			
			if (!aRaw->compile_declaration(childParseContext))
				allOk = false;
		}
		else {
			if (!p->compile_declaration(parseContext))
				allOk = false;
		}
	}
	
	return allOk;
}

bool FogTokens::emit(FogEmitContext& emitContext) const {
	const char* oldSeparator = emitContext.separator();
	const char* oldTerminator = emitContext.terminator();
	
	if (!oldSeparator)
		ERRMSG("INVESTIGATE -- no separator defined for FogTokens::emit " << viz(*this));
		
	bool gotSomething = true;
	
	if (is_exposed()) {
		FogSeparatedEmitContext separatedContext(emitContext, oldSeparator ? oldSeparator : " ", oldTerminator);
		gotSomething = false;
		
		for (FogTokenConstListOfRefToConstIterator p(_tokens); p; ++p) {
			if (p->emit(separatedContext))
				gotSomething = true;
				
			if ((p.to_go() > 1) && separatedContext.separate())
				gotSomething = true;
		}
	}
	else if (!_tokens.tally())
		emitContext.emit_space_and_text(" {}");
	else {
		emitContext.start();
		emitContext.emit_space_and_text("{");
		FogSeparatedEmitContext separatedContext(emitContext, oldSeparator, "\n");
		{
			FogIndentEmitContext indentedContext(separatedContext, 1, FogEmitContext::INDENTS);
			
			for (FogTokenConstListOfRefToConstIterator p(_tokens); p; ++p) {
				indentedContext.start();
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

bool FogTokens::get_object(FogTokenRef& returnValue, FogScopeContext& inScope) const {
	if (!is_exposed())
		return Super::get_object(returnValue, inScope);
		
	switch (_tokens.tally()) {
	case 0:
		ERRMSG("BUG -- cannot FogTokens::get_object an empty list of objects to a single object.");
		return Super::get_object(returnValue, inScope);
		
	default:
		ERRMSG("BUG -- should not FogTokens::get_object a non-unit list of objects to a single object.");
		
	/*   dropthrough */
	case 1:
		return _tokens[0]->get_object(returnValue, inScope);
	}
}

//   -- used by FogStd_GetCpp_Id::resolve_function
bool FogTokens::get_string(PrimIdHandle& returnId, FogScopeContext& inScope) {
	PrimOstrstream s;
	
	for (FogTokenConstListOfRefIterator p(_tokens); p; ++p) {
		PrimIdHandle theId;
		
		if (p->get_string(theId, inScope))
			s.write(theId->str(), theId->length());
	}
	
	size_t aSize = s.pcount();
	returnId = PrimIdHandle(s.str(), aSize);
	return true;
}

bool FogTokens::is_actual(const FogScopeContext& scopeContext) const {
	for (FogTokenConstListOfRefToConstIterator p(_tokens); p; ++p)
		if (!p->is_actual(scopeContext))
			return false;
			
	return Super::is_actual(scopeContext);
}

bool FogTokens::is_compound() const { return is_exposed() == IS_ENCAPSULATED; }
FogToken::IsExposed FogTokens::is_exposed() const { return _is_exposed; }
FogToken::IsList FogTokens::is_list() const { return IS_ABUT_LIST; }
FogTokens* FogTokens::is_tokens() { return this; }

FogLine& FogTokens::line() const {
	if (is_exposed() && _tokens.tally())
		return _tokens[0]->line();
		
	return Super::line();
}

void FogTokens::make_actual_from(FogMakeActualContext& makeActualContext) {
	Super::make_actual_from(makeActualContext);
	
	for (FogTokenListOfRefToConstIterator p(_tokens); p; ++p)
		p->make_actual(p.ref(), makeActualContext);
}

bool FogTokens::morph_to(FogTokenRef& returnValue, const FogMetaType& metaType, IsExposed isExposed,
                         FogScopeContext& inScope) const {
	//  .buybug list exposure
	FogTokensRef morphedTokens(new FogTokens, FogTokensRef::ADOPT);
	FogTokenRef morphedToken;
	
	for (FogTokenConstListOfRefToConstIterator p(_tokens); p; ++p) {
		if (!p->morph_to(morphedToken, metaType, isExposed, inScope))
			return false;
			
		morphedTokens->add(*morphedToken);
	}
	
	returnValue = *morphedTokens;
	return true;
}

std::ostream& FogTokens::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	
	if (_tokens.tally()) {
		s << indent(aDepth) << "tokens\n";
		_tokens.print_depth(s, aDepth + 1);
	}
	
	return s;
}

std::ostream& FogTokens::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	
	if (_tokens.tally()) {
		s << indent(aDepth) << "tokens\n";
		_tokens.print_members(s, aDepth + 1);
	}
	
	return s;
}

char FogTokens::print_named(std::ostream& s, const PrimId* scopeId, char tailChar) const {
	const bool isExposed = is_exposed() ? true : false;
	
	if (!isExposed) {
		tailChar = FogStream::space_and_emit(s, tailChar, "{");
		
		if (_tokens.tally())
			tailChar = FogStream::space_and_emit(s, tailChar, " ");
	}
	
	for (FogTokenConstListOfRefToConstIterator p(_tokens); p; ++p)
		tailChar = p->print_named(s, 0, tailChar);
		
	if (!isExposed) {
		if (_tokens.tally())
			tailChar = FogStream::space_and_emit(s, tailChar, " ");
			
		tailChar = FogStream::space_and_emit(s, tailChar, "}");
	}
	
	return tailChar;
}

std::ostream& FogTokens::print_this(std::ostream& s) const {
	IsExposed isExposed = is_exposed();
	
	if (!isExposed)
		s << "{ ";
		
	s << meta_type().id() << '[' << _tokens.tally() << ']';
	
	if (!isExposed)
		s << " }";
		
	return s;
}

std::ostream& FogTokens::print_viz(std::ostream& s) const {
	s << "\"tokens: ";
	print_this(s);
	return s << '\"';
}

bool FogTokens::resolve_semantics(FogSemanticsContext& theSemantics) const {
	bool isDeclaration = true;
	
	for (FogTokenConstListOfRefIterator p(_tokens); p; ++p) {
		FogChildSemanticsContext termSemantics(theSemantics);
		
		if (!p->resolve_semantics(termSemantics))
			return false;
			
		if (!termSemantics.is_declaration())
			isDeclaration = false;
	}
	
	theSemantics.reset();
	
	if (isDeclaration)
		theSemantics.set(FogSemantics::IS_COMPOUND_DECLARATION);
		
	return true;
}

void FogTokens::set_exposed() { _is_exposed = IS_EXPOSED; }

void FogTokens::set_line(FogLine& aLine) {} //  .bugbug This suppresses an error from a compound_tree_stateemt

FogTokenListOfRef& FogTokens::tokens() { return _tokens; }
const FogTokenListOfRefToConst& FogTokens::tokens() const { return _tokens.to_const(); }

#if 0
bool operator==(const FogTokens& firstTokens, const FogTokens& secondTokens) {
	ERRMSG("BUG - should not operator== for FogTokens.");
	//    if (firstTokens._tokens.tally() != secondTokens._tokens.tally())
	return false;
	//    FogTokenConstListOfRefToConst p1 = firstTokens._tokens;
	//    FogTokenConstListOfRefToConst p2 = secondTokens._tokens;
	//    for ( ; p1 && p2; ++p1, ++p2)
	//
	//    return firstTokens._tokens == secondTokens._tokens;
}
#endif
