#include <Fog/FogIncludeAll.h>














TYPEINFO_SINGLE(FogMetaFunction, Super)

FogMetaFunction::FogMetaFunction(FogMakerContext& makerContext,
                                 const FogStatic& aStatic, const FogMetaType& metaType, IsExposed isExposed,
                                 const FogMetaParameterSpecifierListOfRefToConst& formalParameters, const FogTokenStatements& theToken)
	:
	Super(makerContext, aStatic, metaType, isExposed),
	_token(theToken),
	_is_exclusive(false/*  rawMetaFunction.is_exclusive()*/) {                                        //  .bugbug
	FogResolvableFunctionMakeEntityContext nestedMakeEntityContext(makerContext, *this);
	
	for (FogMetaParameterSpecifierConstListOfRefToConstIterator p(formalParameters); p; ++p) {
		const FogMetaArgument* metaArgument = p->new_parameter(nestedMakeEntityContext, formals().tally());
		
		if (metaArgument)
			adopt(*metaArgument);
	}
	
	_token->make_actual(_token, nestedMakeEntityContext);
}

FogMetaFunction::~FogMetaFunction() {}
const FogMetaFunction* FogMetaFunction::is_meta_function() const { return this; }

std::ostream& FogMetaFunction::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	_token->print_deep(s, aDepth);
	return s;
}

std::ostream& FogMetaFunction::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	_token->print_on(s, aDepth);
	return s;
}

bool FogMetaFunction::resolve_function(FogTokenRef& returnValue, FogCallContext& callContext) const {
	FogTokenStatementsRef tempValue;
	_token->make_actual(tempValue.to_const(), callContext);         //  .bugbug cast
	returnValue = *tempValue;
	return true;
}
