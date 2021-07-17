#include <Fog/FogIncludeAll.h>




TYPEINFO_SINGLE(FogDecoratedExpr, Super)

FogDecoratedExpr::FogDecoratedExpr(const This& anExpr)
	:
	Super(anExpr),
	_expr(anExpr._expr)
{}

void FogDecoratedExpr::create_usage(FogUsageContext& aContext) const {
	Super::create_usage(aContext);
	_expr->create_usage(aContext);
}

const FogDeclSpecifierValue& FogDecoratedExpr::decl_specifiers() const {
	//    ERRMSG("INVESTIGATE -- should not invoke FogDecoratedExpr::decl_specifiers for " << viz(*this));
	return _expr->decl_specifiers();
}

bool FogDecoratedExpr::emit(FogEmitContext& emitContext) const {
	ERRMSG("INVESTIGATE -- should not invoke FogDecoratedExpr::emit for " << viz(*this));
	return _expr->emit(emitContext);
}

size_t FogDecoratedExpr::executable_tokens() const {
	ERRMSG("INVESTIGATE -- should not invoke FogDecoratedExpr::executable_tokens for " << viz(*this));
	return _expr->executable_tokens();
}

FogName* FogDecoratedExpr::get_type() { return _expr->get_type(); }

void FogDecoratedExpr::install(FogInstallContext& installContext) const {
	Super::install(installContext);
	_expr->install(installContext);
}

bool FogDecoratedExpr::is_actual(const FogScopeContext& scopeContext) const {
	return Super::is_actual(scopeContext) && _expr->is_actual(scopeContext);
}

void FogDecoratedExpr::make_actual_from(FogMakeActualContext& makeActualContext) {
	Super::make_actual_from(makeActualContext);
	_expr->make_actual(_expr.to_const(), makeActualContext);
}

bool FogDecoratedExpr::make_typed_expression(FogExprRef& theExpr, FogName& theType) {
	theExpr = *this;
	return _expr->make_typed_expression(_expr, theType);
}

void FogDecoratedExpr::merge_from(FogMergeContext& mergeContext, const This& thatExpr) {
	Super::merge_from(mergeContext, thatExpr);
	thatExpr._expr->merge_into(mergeContext, _expr.to_const());
}

const FogMerge& FogDecoratedExpr::needs_merge_from(FogMergeContext& mergeContext, const This& thatExpr) const {
	FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, thatExpr));
	needsMerge |= _expr->needs_merge(mergeContext, *thatExpr._expr);
	return needsMerge;
}

std::ostream& FogDecoratedExpr::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	_expr->print_deep(s, aDepth);
	return s;
}

std::ostream& FogDecoratedExpr::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	_expr->print_on(s, aDepth);
	return s;
}

char FogDecoratedExpr::print_named(std::ostream& s, const PrimId* fullId, char tailChar) const {
	return _expr->print_named(s, fullId, tailChar);
}

void FogDecoratedExpr::set_decorated_decl_specifier_expression(FogExprRef& anExpr, FogDeclSpecifier& declSpecifiers) {
	_expr->set_decl_specifier_expression(_expr, declSpecifiers);
	anExpr = *this;
}

void FogDecoratedExpr::set_destructor_expression() {
	//    ERRMSG("INVESTIGATE -- should not invoke FogDecoratedExpr::set_destructor_expression for " << viz(*this));
	_expr->set_destructor_expression(_expr);
}

void FogDecoratedExpr::set_line(FogLine& aLine) {
	Super::set_line(aLine);
	_expr->set_line(aLine);
}
