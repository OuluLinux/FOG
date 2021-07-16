#include <Fog/FogIncludeAll.h>









TYPEINFO_SINGLE(FogBreakStatement, Super)
FOGTOKEN_LEAF_IMPL(FogBreakStatement)

FogBreakStatement::FogBreakStatement(BreakEnum) : _is_continue(IS_BREAK) {}

FogBreakStatement::FogBreakStatement(ContinueEnum) : _is_continue(IS_CONTINUE) {}

FogBreakStatement::FogBreakStatement(const This& aStatement) : _is_continue(aStatement._is_continue)  {}

FogBreakStatement::~FogBreakStatement() {}

bool FogBreakStatement::compile_parsed(FogParseContext& parseContext) {
	if (_is_continue)
		parseContext.set_do_continue();
	else
		parseContext.set_do_break();
		
	return false;
}

void FogBreakStatement::create_usage(FogUsageContext& aContext) const {
	Super::create_usage(aContext);
}

bool FogBreakStatement::emit(FogEmitContext& emitContext) const {
	if (is_meta()) {
		if (_is_continue)
			emitContext.set_do_continue();
		else
			emitContext.set_do_break();
			
		return false;
	}
	
	else {
		emitContext.emit_hash_line(line());
		emitContext.emit_space_and_text(_is_continue ? "continue;\n" : "break;\n");
		return true;
	}
}

size_t FogBreakStatement::executable_tokens() const {
	return 1;
}

void FogBreakStatement::install(FogInstallContext& installContext) const {
	Super::install(installContext);
}

bool FogBreakStatement::is_actual(const FogScopeContext& scopeContext) const {
	return Super::is_actual(scopeContext);
}

void FogBreakStatement::make_actual_from(FogMakeActualContext& makeActualContext) {
	Super::make_actual_from(makeActualContext);
}

void FogBreakStatement::merge_from(FogMergeContext& mergeContext, const This& thatExpr) {
	Super::merge_from(mergeContext, thatExpr);
}

const FogMerge& FogBreakStatement::needs_merge_from(FogMergeContext& mergeContext, const This& thatExpr) const {
	FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, thatExpr));
	
	if (_is_continue != thatExpr._is_continue)
		needsMerge |= FogMerge::incompatible();
		
	return needsMerge;
}

char FogBreakStatement::print_named(std::ostream& s, const PrimId *fullId, char tailChar) const {
	if (is_meta())
		tailChar = FogStream::space_and_emit(s, tailChar, "auto ");
		
	tailChar = FogStream::space_and_emit(s, tailChar, _is_continue ? "continue;" : "break;");
	
	return tailChar;
}
