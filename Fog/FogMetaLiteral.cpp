#include <Fog/FogIncludeAll.h>











TYPEINFO_SINGLE(FogMetaLiteral, Super)
FOGTOKEN_LEAF_IMPL(FogMetaLiteral)

FogMetaLiteral::FogMetaLiteral(FogToken& aToken)
	:
	_potential(aToken)
{}

FogMetaLiteral::FogMetaLiteral(const This& thatId)
	:
	Super(thatId),
	_potential(thatId._potential),
	_actual(thatId._actual) {
	if (_actual)
		ERRMSG("BUG - did not expect to copy " << viz(*this));
}

FogMetaLiteral::~FogMetaLiteral() {}

void FogMetaLiteral::create_usage(FogUsageContext& aContext) const {
	if (!_actual)
		ERRMSG("BUG - did not expect to FogMetaLiteral::create_usage for non-actual " << viz(*this));
		
	_actual->create_usage(aContext);
}

bool FogMetaLiteral::emit(FogEmitContext& emitContext) const {
	if (_actual)
		return _actual->emit(emitContext);
	else {
		FogTokenRef returnValue;
		
		if (!_potential->get_object(returnValue, emitContext))
			return false;;
			
		return returnValue->emit(emitContext);
	}
	
	//    FogTokenRef returnValue;
	//    if (!get_object(returnValue, emitContext))
	//        return false;
	//    else
	//        return returnValue->emit(emitContext);
}

bool FogMetaLiteral::get_object(FogTokenRef& returnValue, FogScopeContext& inScope) const {
	if (_actual)
		return _actual->get_object(returnValue, inScope);
	else {
		//        FogMetaScopeContext metaScopeContext(inScope);
		return _potential->get_object(returnValue, inScope);
	}
}

FogName* FogMetaLiteral::get_type() {
	ERRMSG("INVESTIGATE - did not expect to FogMetaLiteral::get_type for " << viz(*this));
	
	if (_actual)
		return _actual->get_type();
	else
		return _potential->get_type();                  //  .bugbug indirection ?
}

void FogMetaLiteral::install(FogInstallContext& installContext) const {
	Super::install(installContext);
	
	if (!_actual)
		ERRMSG("BUG - did not expect to FogMetaLiteral::install for non-actual " << viz(*this));
		
	_actual->install(installContext);
}

bool FogMetaLiteral::is_actual(const FogScopeContext& scopeContext) const {
	return !_actual->is_null();
}

void FogMetaLiteral::make_actual_from(FogMakeActualContext& makeActualContext) {
	if (_actual)
		ERRMSG("BUG - did not expect to FogMetaLiteral::make_actual_from for actualised " << viz(*this));
	else
		_potential->make_actual(_potential.to_const(), makeActualContext);
}

bool FogMetaLiteral::make_specifier(FogMakeSpecifierContext& makeSpecifierContext) {
	FogTokenRef tokenValue;
	
	if (!get_object(tokenValue, makeSpecifierContext))
		return false;
		
	return tokenValue->make_specifier(makeSpecifierContext);
}

void FogMetaLiteral::merge_from(FogMergeContext& mergeContext, const This& thatDecl) {
	Super::merge_from(mergeContext, thatDecl);
	ERRMSG("BUG - did not expect to FogMetaLiteral::merge_from for " << viz(*this));
	thatDecl._potential->merge_into(mergeContext, _potential.to_const());
}

bool FogMetaLiteral::morph_to(FogTokenRef& returnValue, const FogMetaType& metaType, IsExposed isExposed,
                              FogScopeContext& inScope) const {
	FogMetaScopeContext unMetaScope(inScope, false);
	FogTokenRef gotValue;
	
	if (!get_object(gotValue, unMetaScope))
		return false;
	else
		return gotValue->morph_to(returnValue, metaType, isExposed, unMetaScope);
}

const FogMerge& FogMetaLiteral::needs_merge_from(FogMergeContext& mergeContext, const This& thatId) const {
	FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, thatId));
	needsMerge |= _potential->needs_merge(mergeContext, *thatId._potential);
	return needsMerge;
}

FogTokenType::TokenType FogMetaLiteral::pp_token_type_enum() const { return FogTokenType::TreeLiteral; }

std::ostream& FogMetaLiteral::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	
	if (_actual)
		_actual->print_deep(s, aDepth);
	else
		_potential->print_deep(s, aDepth);
		
	return s;
}

std::ostream& FogMetaLiteral::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	
	if (_actual)
		_actual->print_on(s, aDepth);
	else
		_potential->print_on(s, aDepth);
		
	return s;
}

char FogMetaLiteral::print_named(std::ostream& s, const PrimId* scopeId, char tailChar) const {
	if (_potential)
		tailChar = _potential->print_named(s, 0, tailChar);
	else
		tailChar = _actual->print_named(s, 0, tailChar);
		
	return tailChar;
}

std::ostream& FogMetaLiteral::print_viz(std::ostream& s) const {
	s << "\"meta-literal: ";
	
	if (_potential)                     //   expr first since more intelligible to user.
		_potential->print_this(s);
	else
		_actual->print_this(s);
		
	//    if (_actual)
	//        _actual->print_this(s);
	//    else
	//        _potential->print_this(s);
	return s << '\"';
}

bool FogMetaLiteral::resolve_semantics(FogSemanticsContext& theSemantics) const {
	theSemantics.assign_identifier(FogSemantics::POTENTIAL);
	return true;
}

FogTokenType::TokenType FogMetaLiteral::token_type_enum() const { return FogTokenType::TreeLiteral; }
