#include <Fog/FogIncludeAll.h>











TYPEINFO_SINGLE(FogMetaArgument, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogMetaArgument)
PRIMREFS_IMPL(FogMetaArgument)
PRIMLISTOFREFS_IMPL(FogMetaArgument)
PRIMMAPOFREFS_IMPL(FogMetaArgument)
TMPL_HACK_FIX_DO(FogMetaArgument)

FogMetaArgument::~FogMetaArgument() {}
FogToken* FogMetaArgument::clone() const { return /*  &mutate();*/ Super::clone(); }

bool FogMetaArgument::get_object(FogTokenRef& tokenValue, FogScopeContext& inScope) const {
	ERRMSG("INVESTIGATE - did not expect to FogMetaArgument::get_object for " << viz(*this));
	FogCallContext* callContext = inScope.find_call_context();
	FogToken* aToken = callContext ? callContext->get_argument(*this) : 0;
	
	if (!aToken)
		return false;
		
	tokenValue = aToken;
	return true;
}

bool FogMetaArgument::is_actual(const FogScopeContext& scopeContext) const
{ return /*  false;*/ Super::is_actual(scopeContext); }

const FogMetaArgument* FogMetaArgument::is_meta_argument() const { return this; }

bool FogMetaArgument::matches(const FogMetaArgument& firstArgument, const FogMetaArgument& secondArgument) {
	return (&firstArgument.type() == &secondArgument.type()) && (&firstArgument.id() == &secondArgument.id());
}

bool FogMetaArgument::matches(const FogMetaArgumentListOfRefToConst& firstArgument,
                              const FogMetaArgumentListOfRefToConst& secondArgument) {
	FogMetaArgumentConstListOfRefToConstIterator p1(firstArgument);
	FogMetaArgumentConstListOfRefToConstIterator p2(secondArgument);
	
	if (p1.to_go() != p2.to_go())
		return false;
		
	for ( ; p1; ++p1, ++p2)
		if (!matches(*p1, *p2))
			return false;
			
	return true;
}

std::ostream& FogMetaArgument::print_viz(std::ostream& s) const {
	return s << "\"meta-argument: " << id() << '\"';
}

bool FogMetaArgument::resolve_object(FogTokenRef& returnValue, FogScopeContext& inScope) const {
	FogCallContext* callContext = inScope.find_call_context();
	FogToken* aToken = callContext ? callContext->get_argument(*this) : 0;
	
	if (!aToken) {
		returnValue = FogFailure::make();
		return false;
	}
	else {
		returnValue = aToken;
		return true;
	}
	
	//        return aToken->morph_to(returnValue, type(), inScope);  -- morph occurs prior to call.
}
