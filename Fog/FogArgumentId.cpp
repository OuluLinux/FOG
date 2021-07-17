#include <Fog/FogIncludeAll.h>














TYPEINFO_SINGLE(FogArgumentId, Super)
FOGTOKEN_LEAF_IMPL(FogArgumentId)

FogArgumentId::FogArgumentId(FogExpr& anExpr, size_t dollarCount, const FogMetaArgument& aSlot)
	:
	_expr(anExpr),
	_dollar_count(dollarCount),
	_argument(aSlot) {}

FogArgumentId::FogArgumentId(const This& thatId)
	:
	Super(thatId),
	_expr(thatId._expr),
	_dollar_count(thatId._dollar_count),
	_argument(thatId._argument),
	_actual(thatId._actual) {
	if (_actual)
		ERRMSG("BUG - did not expect to copy " << viz(*this));
}

FogArgumentId::~FogArgumentId() {}

bool FogArgumentId::compile_parsed(FogParseContext& parseContext) {
	if (_actual) {
		//ERRMSG("INVESTIGATE did not expect to FogArgumentId::compile_parsed with actual.");
		// hack fix: "basics.fog" fails with "auto declaration cached = $nest(Nested, class Potential {});"
		// and the "class Potential {}" probably sets the actual.
		// Is tested to give correct result with that example.
		return _actual->compile_declaration(parseContext);
	}
	else {
		FogTokenRef tokenValue;
		
		if (!get_object(tokenValue, parseContext))
			return false;
			
		FogTokenRef actualValue;
		tokenValue->make_actual(actualValue.to_const(), parseContext);
		return actualValue->compile_declaration(parseContext);
	}
}

void FogArgumentId::create_usage(FogUsageContext& aContext) const {
	if (!_actual)
		ERRMSG("BUG - did not expect to FogArgumentId::create_usage for non-actual " << viz(*this));
		
	_actual->create_usage(aContext);
}

bool FogArgumentId::emit(FogEmitContext& emitContext) const {
	if (_actual)
		return _actual->emit(emitContext);
		
	if (emitContext.emit_raw()) {
		if (!_dollar_count)
			emitContext.emit_space_and_text("@");
		else
			for (size_t i = _dollar_count; i-- > 0;)
				emitContext.emit_space_and_text("$");
				
		emitContext.emit_space_and_text("{");
		_expr->emit(emitContext);
		emitContext.emit_space_and_text("}");
		return true;
	}
	
	FogTokenRef returnValue;
	FogMetaScopeContext metaContext(emitContext);
	
	if (!_expr->get_object(returnValue, metaContext))
		return false;;
		
	return returnValue->emit(emitContext);
}

size_t FogArgumentId::executable_tokens() const {
	return 1000;    //  .bugbug arbitrary large number
}

void FogArgumentId::find_entities_in(FogScopeContext& inScope, FogEntityFinding& theFinding) const {
	if (_actual)
		_actual->find_entities_in(inScope, theFinding);
	else
		Super::find_entities_in(inScope, theFinding);
}

bool FogArgumentId::find_meta_entity_in(FogTokenRef& returnValue, FogScopeContext& inScope) const {
	ERRMSG("INVESTIGATE - invocation of FogArgumentId::find_meta_entity_in for " << viz(*this));
	return Super::find_meta_entity_in(returnValue, inScope);
}

bool FogArgumentId::get_object(FogTokenRef& returnValue, FogScopeContext& inScope) const {
	if (_actual) {
		returnValue = _actual;
		return true;
	}
	else {
		FogMetaScopeContext metaScopeContext(inScope);
		return _expr->get_object(returnValue, metaScopeContext);
	}
}

FogName* FogArgumentId::get_type() {
	ERRMSG("INVESTIGATE - did not expect to FogArgumentId::get_type for " << viz(*this));
	
	if (_actual)
		return _actual->get_type();
	else
		return _expr->get_type();     //  .bugbug indirection ?
}

void FogArgumentId::install(FogInstallContext& installContext) const {
	Super::install(installContext);
	
	if (!_actual)
		ERRMSG("BUG - did not expect to FogArgumentId::install for non-actual " << viz(*this));
		
	//  	_expr->install(installContext);
	_actual->install(installContext);
}

bool FogArgumentId::is_actual(const FogScopeContext& scopeContext) const {
	return !_actual->is_null();
	//  	return !_actual->is_null() && _actual->is_actual(scopeContext);
}

const PrimId* FogArgumentId::is_resolved() const {
	const FogName* aName = _actual->is_name();
	return aName ? aName->is_resolved() : 0;
}

const FogTemplateParameterSpecifier* FogArgumentId::is_unnormalised_template_parameter
(FogScopeContext& scopeContext) const {
	return 0;
}

void FogArgumentId::make_actual_from(FogMakeActualContext& makeActualContext) {
	if (_actual)
		ERRMSG("BUG - did not expect to FogArgumentId::make_actual_from for actualised " << viz(*this));
	else {
		_expr->make_actual(_expr.to_const(), makeActualContext);
		
		if (_dollar_count || makeActualContext.requires_resolution()) {
			_expr->make_actual(_expr.to_const(), makeActualContext);
			FogTokenRef slotValue;
			FogTokenRef tokenValue;
			
			if (_argument.resolve_object(slotValue, makeActualContext)
			        && _expr->resolve_slot(tokenValue, makeActualContext, *slotValue))
				tokenValue->make_actual(_actual.to_const(), makeActualContext);
			else
				_actual = FogFailure::make();          //   Suppress further attempts
		}
	}
}

bool FogArgumentId::make_specifier(FogMakeSpecifierContext& makeSpecifierContext) {
	FogTokenRef tokenValue;
	
	if (!get_object(tokenValue, makeSpecifierContext))
		return false;
		
	return tokenValue->make_specifier(makeSpecifierContext);
}

void FogArgumentId::merge_from(FogMergeContext& mergeContext, const This& thatDecl) {
	Super::merge_from(mergeContext, thatDecl);
	ERRMSG("BUG - did not expect to FogArgumentId::merge_from for " << viz(*this));
	thatDecl._expr->merge_into(mergeContext, _expr.to_const());
}

bool FogArgumentId::morph_to(FogTokenRef& returnValue, const FogMetaType& metaType, IsExposed isExposed,
                             FogScopeContext& inScope) const {
	FogTokenRef gotValue;
	
	if (!get_object(gotValue, inScope))
		return false;
	else
		return gotValue->morph_to(returnValue, metaType, isExposed, inScope);
}

const FogMerge& FogArgumentId::needs_merge_from(FogMergeContext& mergeContext, const This& thatId) const {
	FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, thatId));
	
	if (_actual.pointer() != thatId._actual.pointer())
		needsMerge |= FogMerge::incompatible();
	else if (&_argument != &thatId._argument)
		needsMerge |= FogMerge::incompatible();
		
	needsMerge |= _expr->needs_merge(mergeContext, *thatId._expr);
	return needsMerge;
}

FogTokenType::TokenType FogArgumentId::pp_token_type_enum() const {
	return FogTokenType::TreeLiteral;
}

std::ostream& FogArgumentId::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	
	if (_actual)
		_actual->print_on(s, aDepth);
	else
		_argument.print_on(s, aDepth);
		
	return s;
}

std::ostream& FogArgumentId::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	
	if (_actual)
		_actual->print_on(s, aDepth);
	else
		_argument.print_on(s, aDepth);
		
	return s;
}

char FogArgumentId::print_named(std::ostream& s, const PrimId* scopeId, char tailChar) const {
	if (Fog::debug_actual() && _actual)
		tailChar = _actual->print_named(s, scopeId, tailChar);
	else if (_expr) {
		if (scopeId) {
			tailChar = FogStream::space_and_emit(s, tailChar, scopeId->str());
			tailChar = FogStream::space_and_emit(s, tailChar, "::");
		}
		
		if (!_dollar_count)
			tailChar = FogStream::space_and_emit(s, tailChar, "@");
		else
			for (size_t i = _dollar_count; i-- > 0;)
				tailChar = FogStream::space_and_emit(s, tailChar, "$");
				
		tailChar = FogStream::space_and_emit(s, tailChar, "{");
		tailChar = _expr->print_named(s, 0, tailChar);
		tailChar = FogStream::space_and_emit(s, tailChar, "}");
	}
	else
		tailChar = _argument.print_named(s, scopeId, tailChar);
		
	return tailChar;
}

std::ostream& FogArgumentId::print_viz(std::ostream& s) const {
	s << "\"formal-argument-id: ";
	
	if (Fog::debug_actual() && _actual)
		_actual->print_this(s);
	else if (_expr) {   //   expr first since more intelligible to user.
		if (!_dollar_count)
			s << '@';
		else
			for (size_t i = _dollar_count; i-- > 0;)
				s << '$';
				
		_expr->print_this(s);
	}
	else
		_argument.print_this(s);
		
	return s << '\"';
}

bool FogArgumentId::resolve_id(PrimIdHandle& returnId, FogScopeContext& inScope) const {
	if (!_dollar_count && !inScope.requires_resolution())
		return false;
		
	if (_actual)
		return _actual->get_identifier(returnId, inScope);
		
	const FogEmitContext* emitContext = inScope.is_emit_context();
	
	if (emitContext && emitContext->emit_raw())     //   If resolving within a diagnostic
		return false;
		
	FogInScopeContext inScopeContext(inScope, IN_ANY_SCOPE);
	FogTokenRef slotValue;
	FogTokenRef returnValue;
	
	if (_argument.resolve_object(slotValue, inScope)
	        && _expr->resolve_slot(returnValue, inScope, *slotValue))
		return returnValue->get_identifier(returnId, inScope);
		
	returnId = PrimId::null();
	return false;     //   Happens during premature resolution of e.g $ { $ formal }
}

bool FogArgumentId::resolve_semantics(FogSemanticsContext& theSemantics) const {
	//  	if (!inScope.requires_resolution())
	{
		theSemantics.assign_identifier(FogSemantics::POTENTIAL);
		return true;
	}
	//  	FogTokenRef tempValue;
	//  	if (!get_object(tempValue, inScope))
	//  		return false;
	//  	return tempValue->resolve_semantics(theSemantics, inScope);
}

FogTokenType::TokenType FogArgumentId::token_type_enum() const {
	return FogTokenType::TreeLiteral;
}
