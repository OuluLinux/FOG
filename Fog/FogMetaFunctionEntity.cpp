#include <Fog/FogIncludeAll.h>












TYPEINFO_SINGLE(FogMetaFunctionEntity, Super)

FogMetaFunctionEntity::FogMetaFunctionEntity(FogToken& inToken, const FogMetaFunctionSlot& metaFunction)
	:
	Super(inToken, metaFunction)
{}

FogMetaFunctionEntity::~FogMetaFunctionEntity() {}

bool FogMetaFunctionEntity::get_dyadic(FogTokenRef& returnValue, FogScopeContext& inScope,
                                       FogTokenType::TokenType dyadicOp, const FogToken& rightValue) const {
	switch (dyadicOp) {
	case '(': {
		const FogMetaArgumentListOfRefToConst& formalArguments = function().formals();
		const FogTokenListOfRefToConst& actualArguments = rightValue.tokens();
		
		if (actualArguments.tally() > formalArguments.tally())
			ERRMSG("Extra arguments ignored for " << viz(*this));
			
		FogTokenListOfRef morphedTokens;
		FogTokenRef tempToken;
		FogTokenConstListOfRefToConstIterator pActual(actualArguments);
		
		for (FogMetaArgumentConstListOfRefToConstIterator pFormal(formalArguments); pFormal; ++pFormal) {
			if (pActual) {
				FogTokenRef actualToken;
				pActual->make_actual(actualToken, inScope);
				
				if (!actualToken->morph_to(tempToken, pFormal->type(), pFormal->is_exposed(), inScope)) {
					returnValue = FogFailure::make();
					return false;
				}
				
				++pActual;
			}
			else if (!pFormal->token().is_null()) {
				FogTokenRef actualToken;
				pFormal->token().make_actual(actualToken, inScope);
				
				if (!actualToken->morph_to(tempToken, pFormal->type(), pFormal->is_exposed(), inScope)) {
					returnValue = FogFailure::make();
					return false;
				}
			}
			else {
				if (inScope.requires_resolution())
					ERRMSG("Missing " << viz(*pFormal) << " for " << viz(*this));
					
				returnValue = FogFailure::make();
				return false;
			}
			
			morphedTokens.add(*tempToken);
		}
		
		FogCallContext callContext(inScope, mutate(), morphedTokens);
		
		if (function().resolve_function(tempToken, callContext)
		        && tempToken->morph_to(returnValue, slot().type(), slot().is_exposed(), inScope))
			return true;
			
		returnValue = FogFailure::make();
		return false;
	}
	
	default:
		return Super::get_dyadic(returnValue, inScope, dyadicOp, rightValue);
	}
}

FogMetaFunctionEntity* FogMetaFunctionEntity::is_meta_function_entity() { return this; }
const FogMetaType& FogMetaFunctionEntity::meta_type() const { return FogMetaType::meta_function_type(); }

std::ostream& FogMetaFunctionEntity::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	slot().print_deep(s, aDepth);
	return s;
}

std::ostream& FogMetaFunctionEntity::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	slot().print_on(s, aDepth);
	return s;
}

std::ostream& FogMetaFunctionEntity::print_viz(std::ostream& s) const {
	s << "\"meta-function: ";
	print_this(s);
	return s << '\"';
}

bool FogMetaFunctionEntity::set_value(const FogToken& tokenValue, FogScopeContext& inScope) {
	ERRMSG("Cannot assign a value to " << viz(*this));
	return false;
}
