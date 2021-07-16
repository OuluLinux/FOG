#include <Fog/FogIncludeAll.h>














TYPEINFO_SINGLE(FogListOfExpr, Super)
PRIMREF_NULL_CLAS(FogListOfExpr)
FOGTOKEN_NULL_LEAF
};

PRIMREF_DERIVED_NULL_IMPL(FogListOfExpr)
PRIMREFS_IMPL(FogListOfExpr)
FOGTOKEN_LEAF_IMPL(FogListOfExpr)
TMPL_HACK_FIX_DO(FogListOfExpr)

FogListOfExpr::FogListOfExpr(IsExposed isExposed)
		:
		_is_exposed(isExposed) {}
		
FogListOfExpr::FogListOfExpr(const This& anExpr)
		:
		Super(anExpr),
		_exprs(anExpr._exprs),
		_is_exposed(anExpr._is_exposed) {}
		
void FogListOfExpr::add(FogExpr& anExpr) {
	if (anExpr.is_exposed())
		ERRMSG("INVESTIGATE -- addition of exposed " << viz(anExpr) << " to " << viz(*this));
		
	_exprs.add(anExpr);
}

bool FogListOfExpr::compile_parsed(FogParseContext& parseContext) {
	bool allOk = true;
	FogStrongSemanticsContext theSemantics(parseContext);
	
	if (!resolve_semantics(theSemantics))
		;
	else
		if (theSemantics.is_init_declaration_list()) {
			FogSpecifierListOfRef theSpecifiers;
			FogExprListOfRefIterator p(_exprs);
			
			if (p) {
				if (!p->resolve_semantics(theSemantics))
					ERRMSG("BUG - failed to reresolve semantics for " << viz(*p));
				else
					if (theSemantics.is_epsilon())
						;
					else {
						FogBaseMakeSpecifierContext firstMakeSpecifierContext(parseContext, theSemantics,
								decl_specifiers(), FogMakeSemantics::DECLARATION_LIST);
						        
						if (!p->make_specifier(firstMakeSpecifierContext)) {
							ERRMSG("Failed to make_specifier for " << viz(*p));
							allOk = false;
						}
						
						else
							if (firstMakeSpecifierContext.compile_specifier())
								theSpecifiers.add(firstMakeSpecifierContext.specifier());
								
						FogName *typeName = p->get_type();
						
						for (++p; p; ++p) {
							if (typeName)
								p->make_typed_expression(p.ref(), *typeName);
								
							if (!p->resolve_semantics(theSemantics))
								ERRMSG("BUG - failed to reresolve semantics for " << viz(*p));
							else
								if (theSemantics.is_epsilon())
									;
								else {
									FogBaseMakeSpecifierContext makeSpecifierContext(parseContext, theSemantics,
											decl_specifiers(), FogMakeSemantics::DECLARATION_LIST);
									        
									if (!p->make_specifier(makeSpecifierContext)) {
										ERRMSG("Failed to make_specifier for " << viz(*p));
										allOk = false;
									}
									
									else
										if (makeSpecifierContext.compile_specifier())
											theSpecifiers.add(makeSpecifierContext.specifier());
								}
						}
						
						firstMakeSpecifierContext.compile_type_specifier();     //   Wait till possibly named by a deferred typedef
						
						bool isFirst = true;
						
						for (FogSpecifierConstListOfRefToConstIterator q(theSpecifiers); q; ++q, isFirst = false) {
							FogCompileMakeEntityContext compileContext(firstMakeSpecifierContext);
							
							if (isFirst)
								q->make_type_entity(compileContext);
								
							FogEntity *anEntity = q->make_name_entity(compileContext);
							
							if (!anEntity) {
								ERRMSG("Failed to make_entity for " << viz(*q));
								allOk = false;
							}
						}
						
						return allOk;
					}
			}
		}
		
		else
			if (theSemantics.is_expression()) {
				FogTokenRef tokenValue;
				return get_object(tokenValue, parseContext);
			}
			
//    else if (theSemantics.is_epsilon())
//        return true;
	ERRMSG("Failed to resolve semantics of " << viz(*this));
	
	return false;
}

void FogListOfExpr::create_usage(FogUsageContext& aContext) const {
	Super::create_usage(aContext);
	
	for (FogExprConstListOfRefToConstIterator p(_exprs); p; ++p)
		p->create_usage(aContext);
}

const FogDeclSpecifierValue& FogListOfExpr::decl_specifiers() const {
	if (!_exprs.tally())
		return Super::decl_specifiers();
	else        //   Used to get decl_specifiers of first element of init-declaration-list
		return _exprs[0]->decl_specifiers();
}

bool FogListOfExpr::emit(FogEmitContext& emitContext) const {
	const char *oldSeparator = emitContext.separator();
	const char *oldTerminator = emitContext.terminator();
	
	if (!oldSeparator)
		ERRMSG("INVESTIGATE -- no separator defined for FogListOfExpr::emit " << viz(*this));
		
	bool gotSomething = true;
	
	if (is_exposed()) {
		FogSeparatedEmitContext separatedContext(emitContext, oldSeparator ? oldSeparator : ",", oldTerminator);
		gotSomething = false;
		
		for (FogExprConstListOfRefToConstIterator p(_exprs); p; ++p) {
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
	
	else
		if (!_exprs.tally())
			emitContext.emit_space_and_text(" {}");
		else {
			emitContext.start();
			emitContext.emit_space_and_text("{");
			FogSeparatedEmitContext separatedContext(emitContext, oldSeparator ? oldSeparator : ",", "\n");
			{
				FogIndentEmitContext indentedContext(separatedContext, 1, FogEmitContext::INDENTS);
				
				for (FogExprConstListOfRefToConstIterator p(_exprs); p; ++p) {
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

size_t FogListOfExpr::executable_tokens() const {
	size_t executableTokens = 0;
	
	for (FogExprConstListOfRefToConstIterator p(_exprs); p; ++p)
		executableTokens += p->executable_tokens();
		
	return executableTokens;
}

bool FogListOfExpr::get_object(FogTokenRef& returnValue, FogScopeContext& inScope) const {
	if (exprs().tally() > 0) {
		if (exprs().tally() > 1)
			ERRMSG("Should not use sequence of expressions in constant expression.");
			
		return exprs()[exprs().tally()-1]->get_object(returnValue, inScope);
	}
	
	else {
		ERRMSG("INVESTIGATE -- transisitising unit-list for get_object of " << viz(*this));
		return Super::get_object(returnValue, inScope);
	}
}

void FogListOfExpr::install(FogInstallContext& installContext) const {
	Super::install(installContext);
	
	for (FogExprConstListOfRefToConstIterator p(_exprs); p; ++p)
		p->install(installContext);
}

bool FogListOfExpr::is_actual(const FogScopeContext& scopeContext) const {
	for (FogExprConstListOfRefToConstIterator p(_exprs); p; ++p)
		if (!p->is_actual(scopeContext))
			return false;
			
	return Super::is_actual(scopeContext);
}

FogToken::IsExposed FogListOfExpr::is_exposed() const {
	return _is_exposed;
}

FogToken::IsList FogListOfExpr::is_list() const {
	return IS_COMMA_LIST;
}

FogListOfExpr *FogListOfExpr::is_list_of_expr() {
	return this;
}

void FogListOfExpr::make_actual_from(FogMakeActualContext& makeActualContext) {
	Super::make_actual_from(makeActualContext);
	
	for (FogExprListOfRefToConstIterator p(_exprs); p; ++p)
		p->make_actual(p.ref(), makeActualContext);
}

bool FogListOfExpr::make_parameters(FogFunctionModifier& functionModifier, FogMakeSpecifierContext& makeSpecifierContext) {
	for (FogExprConstListOfRefIterator p(_exprs); p; ++p) {
		FogBaseMakeSpecifierContext nestedMakeSpecifierContext(makeSpecifierContext,
				p->decl_specifiers(), FogMakeSemantics::MAKE_PARAMETER);
		FogStrongSemanticsContext theSemantics(nestedMakeSpecifierContext);
		
		if (!p->resolve_semantics(theSemantics)) {
			ERRMSG("BUG -- failed to re-resolve_semantics for " << viz(*p));
			return false;
		}
		
		if (theSemantics.is_typed_decl_specifier_seq()) {
			FogName *aName = p->is_name();
			
			if (aName) {
				if (!nestedMakeSpecifierContext.make_specifier(FogName::make_anon())
					|| !aName->make_type_specifier(nestedMakeSpecifierContext)) {
					ERRMSG("Failed to make_type_specifier for " << viz(*p));
					return false;
				}
			}
			
			else {
				ERRMSG("BUG expected Name for make_type_specifier from typed_decl_specifier_seqfor " << viz(*p));
				
				if (!p->make_specifier(nestedMakeSpecifierContext)) {
					ERRMSG("Failed to make_specifier for " << viz(*p));
					return false;
				}
			}
		}
		
		else {
			if (!p->make_specifier(nestedMakeSpecifierContext)) {
				ERRMSG("Failed to make_specifier for " << viz(*p));
				return false;
			}
		}
		
		FogParameterSpecifier *parameterSpecifier = nestedMakeSpecifierContext.get_parameter_specifier(*p);
		
		if (!parameterSpecifier)
			return false;
			
		functionModifier.add_parameter(*parameterSpecifier);
	}
	
	return true;
}

bool FogListOfExpr::make_specifier(FogMakeSpecifierContext& makeSpecifierContext) {
	if (!_exprs.tally())
		return false;
		
	if (!_exprs[0]->make_specifier(makeSpecifierContext))
		return false;
		
	FogFunctionSpecifier *functionSpecifier = makeSpecifierContext.get_function_specifier(*this);
	
	if (functionSpecifier) {    //   Class() : a(0), b(0), ... is ListExpr(BitFieldExpr( a(0) ), b(0), ...)
		if (_exprs.tally() > 1) {
			FogExprConstListOfRefIterator p(_exprs);
			
			for (; p; ++p)
				functionSpecifier->set_colon_value(*p);
		}
	}
	
	else
		ERRMSG("Caller should have de-listed prior to make_specifier for " << viz(*this));
		
	return true;
}

bool FogListOfExpr::make_typed_expression(FogExprRef& theExpr, FogName& theType) {
	ERRMSG("Caller should have de-listed prior to make_typed_expression for " << viz(*this));
	
	if (_exprs.tally())
		return _exprs[0]->make_typed_expression(theExpr, theType);
	else
		return Super::make_typed_expression(theExpr, theType);
}

void FogListOfExpr::merge_from(FogMergeContext& mergeContext, const This& thatExpr) {
	Super::merge_from(mergeContext, thatExpr);
//    Super::merge_long_into(mergeContext, _exprs, thatExpr._exprs);
	FogExpr::merge_long_into(mergeContext, _exprs, thatExpr._exprs);
}

bool FogListOfExpr::morph_to(FogTokenRef& returnValue, const FogMetaType& metaType, IsExposed isExposed,
		FogScopeContext& inScope) const {
	FogListOfExprRef morphedExprs(new FogListOfExpr, FogListOfExprRef::ADOPT);   //  .bugbug list exposure
	FogTokenRef morphedToken;
	
	for (FogExprConstListOfRefToConstIterator p(_exprs); p; ++p) {
		if (!p->morph_to(morphedToken, metaType, isExposed, inScope))
			return false;
			
		FogExpr *morphedExpr = morphedToken->is_expr();
		
		if (!morphedExpr) {
			ERRMSG("BUG -- morphed " << viz(*p) << " is not an expression");
			return false;
		}
		
		morphedExprs->add(*morphedExpr);
	}
	
	returnValue = *morphedExprs;
	
	return true;
}

const FogMerge& FogListOfExpr::needs_merge_from(FogMergeContext& mergeContext, const This& anExpr) const {
	FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, anExpr));
	FogExprConstListOfRefToConstIterator p1(_exprs);
	FogExprConstListOfRefToConstIterator p2(anExpr._exprs);
	
	for (; p1 && p2; ++p1, ++p2) {
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
	else
		if (p2)
			needsMerge |= FogMerge::left_invalid();
			
	return needsMerge;
}

std::ostream& FogListOfExpr::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	
	for (FogExprConstListOfRefToConstIterator p(_exprs); p; ++p)
		p->print_deep(s, aDepth);
		
	return s;
}

std::ostream& FogListOfExpr::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	
	for (FogExprConstListOfRefToConstIterator p(_exprs); p; ++p)
		p->print_on(s, aDepth);
		
	return s;
}

char FogListOfExpr::print_named(std::ostream& s, const PrimId *fullId, char tailChar) const {
	if (!_is_exposed)
		tailChar = FogStream::space_and_emit(s, tailChar, "{");
		
	for (FogExprConstListOfRefToConstIterator p(_exprs); p; ++p) {
		if (p->is_null())
			tailChar = FogStream::space_and_emit(s, tailChar, "0");
		else
			tailChar = p->print_named(s, 0, tailChar);
			
		if (p.to_go() > 1)
			tailChar = FogStream::space_and_emit(s, tailChar, ", ");
	}
	
	if (!_is_exposed)
		tailChar = FogStream::space_and_emit(s, tailChar, "}");
		
	return tailChar;
}

std::ostream& FogListOfExpr::print_viz(std::ostream& s) const {
	s << "\"expression-list: ";
	
	if (!_is_exposed)
		s << "{ ";
		
	print_this(s);
	
	if (!_is_exposed)
		s << " }";
		
	return s << '\"';
}

bool FogListOfExpr::resolve_semantics(FogSemanticsContext& theSemantics) const {
	if (_exprs.tally() == 0) {
//        theSemantics.assign(FogSemantics::IS_SIMPLE_DECLARATION);
		theSemantics.assign(FogSemantics::IS_EPSILON);
		return true;
	}
	
	bool constructorDeclarativeList = true;
	
	bool initDeclarationList = true;
	bool initDeclaratorList = true;
	bool isExpression = true;
	bool isTemplateArgumentList = true;
	bool isTemplateParameterList = true;
	bool isTypeIdList = true;
	bool isFirst = true;
	
	for (FogExprConstListOfRefIterator p(_exprs); p; ++p) {
		FogChildSemanticsContext termSemantics(theSemantics);
		
		if (!p->resolve_semantics(termSemantics))
			return false;
			
		if (!termSemantics.is_assignment_expression())
			isExpression = false;
			
		if (isFirst) {
			if (!termSemantics.is_constructor_declarative())
				constructorDeclarativeList = false;
				
			if (!termSemantics.is_init_declaration())
				initDeclarationList = false;
				
			if (!termSemantics.is_init_declarator())
				initDeclaratorList = false;
		}
		
		else {
			if (!termSemantics.is_mem_initializer())
				constructorDeclarativeList = false;
				
			if (!termSemantics.is_init_declarator()) {
				initDeclarationList = false;
				initDeclaratorList = false;
			}
		}
		
		if (!termSemantics.is_template_argument())
			isTemplateArgumentList = false;
			
		if (!termSemantics.is_template_parameter())
			isTemplateParameterList = false;
			
		if (!termSemantics.is_type_id())
			isTypeIdList = false;
			
		isFirst = false;
	}
	
	theSemantics.reset();
	
	if (constructorDeclarativeList)
		theSemantics.set(FogSemantics::IS_CONSTRUCTOR_DECLARATIVE);
		
	if (initDeclarationList)
		theSemantics.set(FogSemantics::IS_INIT_DECLARATION_LIST);
		
	if (initDeclaratorList)
		theSemantics.set(FogSemantics::IS_INIT_DECLARATOR_LIST);
		
	if (isExpression)
		theSemantics.set(FogSemantics::IS_EXPRESSION);
		
	if (isTemplateArgumentList)
		theSemantics.set(FogSemantics::IS_TEMPLATE_ARGUMENT_LIST);
		
	if (isTemplateParameterList)
		theSemantics.set(FogSemantics::IS_TEMPLATE_PARAMETER_LIST);
		
	if (isTypeIdList)
		theSemantics.set(FogSemantics::IS_TYPE_ID_LIST);
		
	return true;
}

void FogListOfExpr::set_decl_specifier_expression(FogExprRef& anExpr, FogDeclSpecifier& declSpecifiers) {
	if (_exprs.tally()) {       //   Used to get set_decl_specifier_expression of first element of init-declaration-list/ctor_head
		_exprs[0]->set_decl_specifier_expression(_exprs[0], declSpecifiers);
		anExpr = this;
	}
	
	else
		Super::set_decl_specifier_expression(anExpr, declSpecifiers);
}

FogTokenListOfRef& FogListOfExpr::tokens()  {
	return Super::tokens();
}

const FogTokenListOfRefToConst& FogListOfExpr::tokens() const {
	return (const FogTokenListOfRefToConst&)_exprs.to_const();
}
