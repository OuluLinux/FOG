#include <Fog/FogIncludeAll.h>











TYPEINFO_SINGLE(FogIfStatement, Super)
FOGTOKEN_LEAF_IMPL(FogIfStatement)

FogIfStatement::FogIfStatement(FogExpr& testExpr, FogRaw& trueStatement, FogRaw* falseStatement)
	:
	_test(testExpr),
	_true(trueStatement),
	_false(falseStatement)
{}

FogIfStatement::FogIfStatement(const This& aStatement)
	:
	Super(aStatement),
	_test(aStatement._test),
	_true(aStatement._true),
	_false(aStatement._false)
{}

FogIfStatement::~FogIfStatement() {}

bool FogIfStatement::compile_parsed(FogParseContext& parseContext) {
	FogMetaParseContext metaContext(parseContext);
	const PrimNumber& testValue = _test->get_number(metaContext);
	
	if (!testValue.is_null() && !testValue.is_zero()) {
		FogUnbreakableParseContext trueContext(parseContext, *_true);
		return _true->compile_declaration(trueContext);
	}
	else if (_false) {
		FogUnbreakableParseContext falseContext(parseContext, *_false);
		return _false->compile_declaration(falseContext);
	}
	else
		return true;
}

void FogIfStatement::create_usage(FogUsageContext& aContext) const {
	Super::create_usage(aContext);
	_test->create_usage(aContext);
	_true->create_usage(aContext);
	_false->create_usage(aContext);
}

bool FogIfStatement::emit(FogEmitContext& emitContext) const {
	if (is_meta()) {
		FogEmitParseContext parseContext(emitContext, *this);
		const PrimNumber& testValue = _test->get_number(parseContext);
		
		if (!testValue.is_null() && !testValue.is_zero()) {
			FogUnbreakableEmitContext trueContext(emitContext, *_true);
			return _true->emit(trueContext);
		}
		else if (_false) {
			FogUnbreakableEmitContext falseContext(emitContext, *_false);
			return _false->emit(falseContext);
		}
		else
			return true;
	}
	else {
		emitContext.emit_hash_line(line());
		emitContext.emit_space_and_text("if (");
		{
			FogSeparatedEmitContext separatedContext(emitContext, ",", 0);
			_test->emit(separatedContext);
		}
		emitContext.emit_space_and_text(")\n");
		
		if (_true->is_compound()) {
			FogSeparatedEmitContext separatedContext(emitContext, "", "\n");
			_true->emit(separatedContext);
		}
		else {
			FogIndentEmitContext indentedContext(emitContext, 1, FogEmitContext::INDENTS);
			_true->emit(indentedContext);
		}
		
		if (!_false)
			;
		else if (ptr_cast(FogIfStatement, _false.pointer())) {
			emitContext.emit_space_and_text("\nelse ");
			_false->emit(emitContext);
		}
		else if (_false->is_compound()) {
			emitContext.emit_space_and_text("\nelse\n");
			FogSeparatedEmitContext separatedContext(emitContext, "", "\n");
			_false->emit(separatedContext);
		}
		else {
			emitContext.emit_space_and_text("\nelse\n");
			FogIndentEmitContext indentedContext(emitContext, 1, FogEmitContext::INDENTS);
			_false->emit(indentedContext);
		}
	}
	
	return true;
}

size_t FogIfStatement::executable_tokens() const {
	size_t executableTokens = 1 + _test->executable_tokens() + _true->executable_tokens();
	
	if (_false)
		executableTokens += 1 + _false->executable_tokens();
		
	return executableTokens;
}

void FogIfStatement::install(FogInstallContext& installContext) const {
	Super::install(installContext);
	_test->install(installContext);
	_true->install(installContext);
	_false->install(installContext);
}

bool FogIfStatement::is_actual(const FogScopeContext& scopeContext) const {
	return Super::is_actual(scopeContext) && _test->is_actual(scopeContext)
	       && _true->is_actual(scopeContext) && _false->is_actual(scopeContext);
}

void FogIfStatement::make_actual_from(FogMakeActualContext& makeActualContext) {
	Super::make_actual_from(makeActualContext);
	_test->make_actual(_test.to_const(), makeActualContext);
	
	if (makeActualContext.requires_formal_location_or_resolution()) {
		_true->make_actual(_true.to_const(), makeActualContext);
		_false->make_actual(_false.to_const(), makeActualContext);
	}
}

void FogIfStatement::merge_from(FogMergeContext& mergeContext, const This& thatExpr) {
	Super::merge_from(mergeContext, thatExpr);
	thatExpr._test->merge_into(mergeContext, _test.to_const());
	thatExpr._true->merge_into(mergeContext, _true.to_const());
	thatExpr._false->merge_into(mergeContext, _false.to_const());
}

const FogMerge& FogIfStatement::needs_merge_from(FogMergeContext& mergeContext, const This& thatExpr) const {
	FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, thatExpr));
	needsMerge |= _test->needs_merge(mergeContext, *thatExpr._test);
	needsMerge |= _true->needs_merge(mergeContext, *thatExpr._true);
	needsMerge |= _false->needs_merge(mergeContext, *thatExpr._false);
	return needsMerge;
}

std::ostream& FogIfStatement::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	_test->print_deep(s, aDepth);
	_true->print_deep(s, aDepth);
	_false->print_deep(s, aDepth);
	return s;
}

std::ostream& FogIfStatement::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	_test->print_on(s, aDepth);
	_true->print_on(s, aDepth);
	_false->print_on(s, aDepth);
	return s;
}

char FogIfStatement::print_named(std::ostream& s, const PrimId* fullId, char tailChar) const {
	if (is_meta())
		tailChar = FogStream::space_and_emit(s, tailChar, "auto ");
		
	tailChar = FogStream::space_and_emit(s, tailChar, "if (");
	tailChar = _test->print_named(s, 0, tailChar);
	tailChar = FogStream::space_and_emit(s, tailChar, ")");
	return tailChar;
}

void FogIfStatement::set_is_meta() {
	Super::set_is_meta();
	_true->set_exposed();
	_false->set_exposed();
}