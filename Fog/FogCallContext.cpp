#include <Fog/FogIncludeAll.h>













TYPEINFO_SINGLE(FogCallContext, Super)

FogCallContext::FogCallContext(FogScopeContext& scopeContext,
                               FogMetaFunctionEntity& metaFunctionEntity, const FogTokenListOfRef& actualArguments)
	:
	Super(scopeContext),
	_meta_function(metaFunctionEntity),
	_arguments(actualArguments)
{}

FogCallContext::~FogCallContext() {}

//
//  -- unnecessary since constructor caller validates
//
//  bool FogCallContext::check_argument_tally(size_t numArguments) const
//  {
//    if (_arguments.tally() == numArguments)
//        return true;
//    ERRMSG("BUG - expected " << numArguments << " arguments for " << viz(_meta_function));
//    return false;
//  }

FogToken& FogCallContext::dynamic_token() { return _meta_function.token(); }
const FogToken& FogCallContext::dynamic_token() const { return _meta_function.token(); }
FogCallContext* FogCallContext::find_call_context() { return this; }
FogMakeEntityContext* FogCallContext::find_make_entity_context() { return 0; }
bool FogCallContext::find_slots(FogMetaSlotFinder& theFinder) { return false; }

//
//    Convert theArgument to its value when FogMetaArgument::resolve_object invoked in the call phase.
//
FogToken* FogCallContext::get_argument(const FogMetaArgument& theArgument) {
	FogMetaArgument::Index anIndex = theArgument.index();
	
	if ((0 <= anIndex) && (anIndex < _arguments.tally()))
		return _arguments[anIndex].pointer();
		
	if (anIndex == FogMetaArgument::DYNAMIC)
		return &dynamic_token();
		
	if (anIndex == FogMetaArgument::STATIC)
		return (FogToken*)&theArgument.token();             //  .bugbug cast
		
	ERRMSG("Expected a " << (anIndex + 1) << "'th argument for " << viz(_meta_function));
	return 0;
}

FogBaseClass* FogCallContext::get_base_class() {
	FogBaseClass* baseClass = dynamic_token().is_base_class();
	
	if (baseClass)
		return baseClass;
		
	ERRMSG("Expected base-specifier for " << viz(_meta_function));
	return 0;
}

FogEntity* FogCallContext::get_entity() {
	FogEntity* anEntity = dynamic_token().is_entity();
	
	if (anEntity)
		return anEntity;
		
	ERRMSG("Expected entity for " << viz(_meta_function));
	return 0;
}

const FogExpr* FogCallContext::get_expr_argument(size_t argumentNumber) const {
	const FogExpr* anExpr = _arguments[argumentNumber]->is_expr();
	
	if (anExpr)
		return anExpr;
		
	ERRMSG("BUG - expected expression for argument " << argumentNumber << " of " << viz(_meta_function));
	return 0;
}

FogFunction* FogCallContext::get_function() {
	FogFunction* aFunction = dynamic_token().is_function();
	
	if (aFunction)
		return aFunction;
		
	ERRMSG("Expected function for " << viz(_meta_function));
	return 0;
}

const FogName* FogCallContext::get_name_argument(size_t argumentNumber) const {
	const FogName* aName = _arguments[argumentNumber]->is_name();
	
	if (aName)
		return aName;
		
	ERRMSG("BUG - expected name for argument " << argumentNumber << " of " << viz(_meta_function));
	return 0;
}

FogScope* FogCallContext::get_scope() {
	FogScope* theScope = dynamic_token().is_scope();
	
	if (theScope)
		return theScope;
		
	ERRMSG("Expected scope for " << viz(_meta_function));
	return 0;
}

FogSpecifier* FogCallContext::get_specifier() {
	FogSpecifier* aSpecifier = dynamic_token().is_specifier();
	
	if (aSpecifier)
		return aSpecifier;
		
	ERRMSG("Expected specifier for " << viz(_meta_function));
	return 0;
}

bool FogCallContext::get_string_argument(PrimIdHandle& aString, size_t argumentNumber) {
	return _arguments[argumentNumber]->get_string(aString, calling_context());
}

const FogToken* FogCallContext::get_token_argument(size_t argumentNumber) const {
	return _arguments[argumentNumber].pointer();
}

const FogScopeContext* FogCallContext::meta_context() const { return 0; }

std::ostream& FogCallContext::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	s << indent(aDepth) << _meta_function << '\n';
	_arguments.print_depth(s, aDepth);
	return s;
}

FogScopeContext::Resolution FogCallContext::resolution() const
{ return Resolution(SCOPE_UNKNOWN | RESOLVE_META_FUNCTION_FORMALS); }
