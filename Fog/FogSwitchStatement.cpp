#include <Fog/FogIncludeAll.h>










TYPEINFO_SINGLE(FogSwitchStatement, Super)
FOGTOKEN_LEAF_IMPL(FogSwitchStatement)

FogSwitchStatement::FogSwitchStatement(const FogExpr& testExpr, FogRaw& bodyStatement)
	:
	_test(testExpr),
	_body(bodyStatement)
{}

FogSwitchStatement::FogSwitchStatement(const This& aStatement)
	:
	Super(aStatement),
	_test(aStatement._test),
	_body(aStatement._body)
{}

FogSwitchStatement::~FogSwitchStatement() {}

bool FogSwitchStatement::compile_parsed(FogParseContext& parseContext) {
	FogMetaParseContext metaContext(parseContext);
	FogTokenRef theCase;
	
	if (_test->get_object(theCase, metaContext)) {
		FogRaws* rawList = _body->is_raw_list();
		
		if (rawList)
			return rawList->compile_parsed_case(parseContext, *theCase);
			
		return false;
	}
	else
		return true;
}

void FogSwitchStatement::create_usage(FogUsageContext& aContext) const {
	Super::create_usage(aContext);
	_test->create_usage(aContext);
	_body->create_usage(aContext);
}

bool FogSwitchStatement::emit(FogEmitContext& emitContext) const {
	if (is_meta()) {
		FogEmitParseContext parseContext(emitContext, *this);
		FogTokenRef theCase;
		
		if (_test->get_object(theCase, parseContext)) {
			const FogRaws* rawList = _body->is_raw_list();
			
			if (rawList)
				return rawList->emit_case(emitContext, *theCase);
				
			return false;
		}
		else
			return true;
	}
	else {
		emitContext.emit_hash_line(line());
		emitContext.emit_space_and_text("switch (");
		{
			FogSeparatedEmitContext separatedContext(emitContext, ",", 0);
			_test->emit(separatedContext);
		}
		emitContext.emit_space_and_text(")");
		{
			FogSeparatedEmitContext separatedContext(emitContext, "", "\n");
			_body->emit(separatedContext);
		}
	}
	
	return true;
}

size_t FogSwitchStatement::executable_tokens() const {
	size_t executableTokens = 1 + _test->executable_tokens() + _body->executable_tokens();
	return executableTokens;
}

void FogSwitchStatement::install(FogInstallContext& installContext) const {
	Super::install(installContext);
	_test->install(installContext);
	_body->install(installContext);
}

bool FogSwitchStatement::is_actual(const FogScopeContext& scopeContext) const {
	return Super::is_actual(scopeContext) && _test->is_actual(scopeContext) && _body->is_actual(scopeContext);
}

void FogSwitchStatement::make_actual_from(FogMakeActualContext& makeActualContext) {
	Super::make_actual_from(makeActualContext);
	_test->make_actual(_test.to_const(), makeActualContext);
	_body->make_actual(_body.to_const(), makeActualContext);
}

void FogSwitchStatement::merge_from(FogMergeContext& mergeContext, const This& thatExpr) {
	Super::merge_from(mergeContext, thatExpr);
	thatExpr._test->merge_into(mergeContext, _test.to_const());
	thatExpr._body->merge_into(mergeContext, _body.to_const());
}

const FogMerge& FogSwitchStatement::needs_merge_from(FogMergeContext& mergeContext, const This& thatExpr) const {
	FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, thatExpr));
	needsMerge |= _test->needs_merge(mergeContext, *thatExpr._test);
	needsMerge |= _body->needs_merge(mergeContext, *thatExpr._body);
	return needsMerge;
}

std::ostream& FogSwitchStatement::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	_test->print_deep(s, aDepth);
	_body->print_deep(s, aDepth);
	return s;
}

std::ostream& FogSwitchStatement::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	_test->print_on(s, aDepth);
	_body->print_on(s, aDepth);
	return s;
}

char FogSwitchStatement::print_named(std::ostream& s, const PrimId* fullId, char tailChar) const {
	if (is_meta())
		tailChar = FogStream::space_and_emit(s, tailChar, "auto ");
		
	tailChar = FogStream::space_and_emit(s, tailChar, "switch (");
	tailChar = _test->print_named(s, 0, tailChar);
	tailChar = FogStream::space_and_emit(s, tailChar, ")");
	return tailChar;
}

void FogSwitchStatement::set_is_meta() {
	Super::set_is_meta();
	_body->set_exposed();
}