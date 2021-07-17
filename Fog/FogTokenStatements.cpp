#include <Fog/FogIncludeAll.h>







TYPEINFO_SINGLE(FogTokenStatements, Super)
PRIMREF_NULL_CLAS(FogTokenStatements)
FOGTOKEN_NULL_LEAF
};
PRIMREF_DERIVED_NULL_IMPL(FogTokenStatements)
PRIMREFS_IMPL(FogTokenStatements)
FOGTOKEN_LEAF_IMPL(FogTokenStatements)
TMPL_HACK_FIX_DO(FogTokenStatements)

FogTokenStatements::FogTokenStatements()
	:
	Super(IS_ENCAPSULATED),
	_meta_type(FogMetaType::void_type())
{}

FogTokenStatements::FogTokenStatements(const FogMetaType& metaType, FogTokens* someTokens)
	:
	Super(IS_ENCAPSULATED),
	_meta_type(metaType) {
	if (someTokens)
		add(someTokens->tokens());
}

FogTokenStatements::~FogTokenStatements() {}

bool FogTokenStatements::get_dyadic(FogTokenRef& returnValue, FogScopeContext& inScope,
                                    FogTokenType::TokenType dyadicOp, const FogToken& rightValue) const {
	switch (dyadicOp) {
	case '+':
	case FogTokenType::ASS_ADD: {           //   Assign in FogAssignExpr.
		FogTokenRef tempValue;
		
		if (!rightValue.get_object(tempValue, inScope))
			return false;
			
		if (!tempValue->morph_to(tempValue, _meta_type, is_exposed(), inScope))
			return false;
			
		FogTokenStatements* newList = new FogTokenStatements(*this);
		
		if (newList)
			newList->add(*tempValue);
			
		returnValue.adopt(newList);
		return true;
	}
	
	default:
		return Super::get_dyadic(returnValue, inScope, dyadicOp, rightValue);;
	}
}

void FogTokenStatements::make_actual_from(FogMakeActualContext& makeActualContext) {
	//    Super::make_actual_from(makeActualContext);
	for (FogTokenListOfRefToConstIterator p(tokens()); p; ++p) {
		FogTokenMonitor aMonitor(*p);
		p->make_actual(p.ref(), makeActualContext);
	}
}

const FogMetaType& FogTokenStatements::meta_type() const { return _meta_type; }

bool FogTokenStatements::morph_to(FogTokenRef& returnValue, const FogMetaType& metaType, IsExposed isExposed,
                                  FogScopeContext& inScope) const {
	//   .bugbug list exposure
	FogTokenConstListOfRefToConstIterator p(tokens());
	
	if (is_exposed() && !isExposed && (p.to_go() == 1))
		return p->morph_to(returnValue, metaType, isExposed, inScope);
		
	FogTokenStatementsRef morphedTokens(new FogTokenStatements(metaType), FogTokenStatementsRef::ADOPT);
	
	if (is_exposed())
		morphedTokens->set_exposed();
		
	FogTokenRef morphedToken;
	
	for ( ; p; ++p) {
		if (!p->morph_to(morphedToken, metaType, isExposed, inScope))
			return false;
			
		morphedTokens->add(*morphedToken);
	}
	
	returnValue = *morphedTokens;
	return true;
}

char FogTokenStatements::print_named(std::ostream& s, const PrimId* scopeId, char tailChar) const {
	const bool isExposed = is_exposed() ? true : false;
	
	if (!isExposed) {
		tailChar = FogStream::space_and_emit(s, tailChar, "{");
		
		if (tokens().tally())
			tailChar = FogStream::space_and_emit(s, tailChar, "; ");
	}
	
	for (FogTokenConstListOfRefToConstIterator p(tokens()); p; ++p)
		tailChar = p->print_named(s, 0, tailChar);
		
	if (!isExposed) {
		if (tokens().tally())
			tailChar = FogStream::space_and_emit(s, tailChar, "; ");
			
		tailChar = FogStream::space_and_emit(s, tailChar, "}");
	}
	
	return tailChar;
}

std::ostream& FogTokenStatements::print_viz(std::ostream& s) const {
	s << "\"tree-statement-seq : ";
	print_named(s, 0, 0);
	return s << '\"';
}
