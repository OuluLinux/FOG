#include <Fog/FogIncludeAll.h>











TYPEINFO_SINGLE(FogIteratorExpr, Super)
FOGTOKEN_LEAF_IMPL(FogIteratorExpr)

FogIteratorExpr::FogIteratorExpr(const FogMetaIteratorEntity& anIterator)
	:
	_iterator(anIterator)
{}

FogIteratorExpr::FogIteratorExpr(const FogIterationDomain& tokenList)
	:
	_tokens(tokenList)
{}

FogIteratorExpr::FogIteratorExpr(const This& anExpr)
	:
	Super(anExpr),
	_iterator(anExpr._iterator)
{}

FogIteratorExpr::~FogIteratorExpr() {}

bool FogIteratorExpr::emit(FogEmitContext& emitContext) const {
	return _iterator->emit(emitContext);
}

const PrimNumber& FogIteratorExpr::get_number(FogScopeContext& inScope) const {
	bool isOk = _iterator->is_ok();
	return PrimNumber::make_bool(isOk);
}

bool FogIteratorExpr::get_number_token(FogTokenRef& returnValue, FogScopeContext&) const {
	bool isOk = _iterator->is_ok();
	returnValue.assign(FogNumber::make_logical(isOk));
	return true;
}

bool FogIteratorExpr::get_object(FogTokenRef& tokenValue, FogScopeContext& inScope) const {
	return _iterator->get_object(tokenValue, inScope);
}

const FogMetaType& FogIteratorExpr::meta_type() const { return FogMetaType::iterator_type(); }

//  const FogMerge& FogIteratorExpr::needs_merge(FogMergeContext& mergeContext, const FogToken& aToken) const
//  {
//    ERRMSG("BUG -- should not merge meta-iterator expressions.");
//    return  FogMerge::incompatible();
//  }
//  const FogMerge& FogIteratorExpr::needs_merge_from(FogMergeContext& mergeContext, const This& anExpr) const
//  {
//    FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, anExpr));
//    needsMerge |= _decl->needs_merge(mergeContext, *anExpr._decl);
//    return needsMerge;
//  }

std::ostream& FogIteratorExpr::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	_iterator->print_deep(s, aDepth);
	return s;
}

std::ostream& FogIteratorExpr::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	_iterator->print_on(s, aDepth);
	return s;
}

char FogIteratorExpr::print_named(std::ostream& s, const PrimId* fullId, char tailChar) const {
	return _iterator->print_named(s, fullId, tailChar);
}
