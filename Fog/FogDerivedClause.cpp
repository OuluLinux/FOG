#include <Fog/FogIncludeAll.h>











TYPEINFO_SINGLE(FogDerivedClause, Super)
FOGTOKEN_LEAF_IMPL(FogDerivedClause)

FogDerivedClause::FogDerivedClause(FogExpr& anExpr, FogRaw* theRaw)
	:
	_expr(anExpr),
	_raw(theRaw)
{}

FogDerivedClause::FogDerivedClause(const This& anExpr)
	:
	Super(anExpr),
	_expr(anExpr._expr),
	_raw(anExpr._raw)
{}

FogDerivedClause::~FogDerivedClause() {}

bool FogDerivedClause::compile_parsed(FogParseContext& parseContext) {
	ERRMSG("BUG did not expect to FogDerivedClause::compile_parsed for " << viz(*this));
	return true;
}

void FogDerivedClause::create_usage(FogUsageContext& aContext) const {
	ERRMSG("BUG did not expect to FogDerivedClause::create_usage for " << viz(*this));
	Super::create_usage(aContext);
	_expr->create_usage(aContext);
	
	if (_raw)
		_raw->create_usage(aContext);
}

const FogLazyBool& FogDerivedClause::derive_at(FogScopeContext& inScope) const {
	FogMetaScopeContext metaContext(inScope);
	FogTokenRef tokenValue;
	
	if (!expr().get_number_token(tokenValue, metaContext))
		return FogLazyBool::make_undecided();
		
	const PrimNumber& isDerived = tokenValue->number().number_value();
	
	if (isDerived.is_zero())
		return FogLazyBool::make_false();
		
	const FogDerivedClause* derivedClause = _raw->is_derived_clause();
	
	if (derivedClause)
		return derivedClause->derive_at(metaContext);
		
	return FogLazyBool::make_true();
}

bool FogDerivedClause::emit(FogEmitContext& emitContext) const {
	ERRMSG("BUG did not expect to FogDerivedClause::emit for " << viz(*this));
	return false;
}

size_t FogDerivedClause::executable_tokens() const {
	ERRMSG("INVESTIGATE -- should not invoke FogDerivedClause::executable_tokens for " << viz(*this));
	return _raw->executable_tokens();
}

FogName* FogDerivedClause::get_type() {
	ERRMSG("INVESTIGATE -- should not invoke FogDerivedClause::get_type for " << viz(*this));
	return _raw->get_type();
}

void FogDerivedClause::install(FogInstallContext& installContext) const {
	ERRMSG("BUG -- did not expect to FogDerivedClause::install for " << viz(*this));
	Super::install(installContext);
	_expr->install(installContext);
	
	if (_raw)
		_raw->install(installContext);
}

bool FogDerivedClause::is_actual(const FogScopeContext& scopeContext) const {
	return false;       //   Never actual since body needs transfer
	//    return Super::is_actual(scopeContext) && _raw->is_actual(scopeContext);
}

FogDerivedClause* FogDerivedClause::is_derived_clause() { return this; }

void FogDerivedClause::make_actual_from(FogMakeActualContext& makeActualContext) {
	Super::make_actual_from(makeActualContext);
	_expr->make_actual(_expr.to_const(), makeActualContext);
	_raw->make_actual(_raw.to_const(), makeActualContext);
}

bool FogDerivedClause::make_specifier(FogMakeSpecifierContext& makeSpecifierContext) {
	//      FogToken& inToken = makeSpecifierContext.dynamic_token();
	//      FogPotentialDeclaration *potentialDeclaration = inToken.is_potential_declaration();
	//      if (!potentialDeclaration)
	//      {
	//          ERRMSG("BUG - PotentialDeclaration expected as context of FogDerivedClause::make_specifier");
	//          return false;
	//      }
	return _raw->make_specifier(makeSpecifierContext);
	//      FogSpecifier& aSpecifier = (FogSpecifier&)potentialDeclaration->specifier();    //.bugbug cast
	//      FogName *aName = _raw->is_name();
	//      if (!aName)
	//          return false;
	//      makeSpecifierContext.make_specifier(*_raw);
	//      if (Super::make_specifier(makeSpecifierContext))
	//          return false;
	//    if (!_raw)
	//    {
	//        ERRMSG("INVESTIGATE - removal of Specifier::set_derived()");
	//  //        makeSpecifierContext.specifier().set_derived(expr());
	//    }
	//      return true;
}

void FogDerivedClause::merge_from(FogMergeContext& mergeContext, const This& thatExpr) {
	Super::merge_from(mergeContext, thatExpr);
	thatExpr._expr->merge_into(mergeContext, _expr.to_const());
	
	if (_raw)
		thatExpr._raw->merge_into(mergeContext, _raw.to_const());
}

const FogMerge& FogDerivedClause::needs_merge_from(FogMergeContext& mergeContext, const This& anExpr) const {
	FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, anExpr));
	needsMerge |= _expr->needs_merge(mergeContext, *anExpr._expr);
	needsMerge |= _raw->needs_merge(mergeContext, *anExpr._raw);
	return needsMerge;
}

std::ostream& FogDerivedClause::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	_expr->print_deep(s, aDepth);
	
	if (_raw)
		_raw->print_deep(s, aDepth);
		
	return s;
}

std::ostream& FogDerivedClause::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	_expr->print_on(s, aDepth);
	
	if (_raw)
		_raw->print_on(s, aDepth);
		
	return s;
}

char FogDerivedClause::print_named(std::ostream& s, const PrimId* fullId, char tailChar) const {
	tailChar = FogStream::space_and_emit(s, tailChar, "derived(");
	tailChar = _expr->print_named(s, fullId, tailChar);
	tailChar = FogStream::space_and_emit(s, tailChar, ") ");
	
	if (_raw)
		tailChar = _raw->print_named(s, 0, tailChar);
		
	tailChar = FogStream::space_and_emit(s, tailChar, ";");
	return tailChar;
}

std::ostream& FogDerivedClause::print_viz(std::ostream& s) const {
	s << "\"derived-clause: ";
	print_named(s, 0, ' ');
	return s << '\"';
}

bool FogDerivedClause::resolve_semantics(FogSemanticsContext& theSemantics) const {
	//    return expr().resolve_semantics(theSemantics, inScope);
	if (_raw)
		return _raw->resolve_semantics(theSemantics);
		
	theSemantics.assign(FogSemantics::IS_EPSILON);
	return true;
}

void FogDerivedClause::set_line(FogLine& aLine) {
	Super::set_line(aLine);
	_expr->set_line(aLine);
	
	if (_raw)
		_raw->set_line(aLine);
}
