#include <Fog/FogIncludeAll.h>










TYPEINFO_SINGLE(FogFunctionExpr, Super)
FOGTOKEN_LEAF_IMPL(FogFunctionExpr)

FogFunctionExpr::FogFunctionExpr(FogExpr& anExpr, FogCode& theCode)
:
 Super(anExpr),
 _code(theCode)
{}

FogFunctionExpr::FogFunctionExpr(const This& anExpr)
:
 Super(anExpr),
 _code(anExpr._code)
{}

FogFunctionExpr::~FogFunctionExpr() {}

void FogFunctionExpr::create_usage(FogUsageContext& aContext) const
{
 ERRMSG("BUG did not expect to FogFunctionExpr::create_usage for " << viz(*this));
 Super::create_usage(aContext);
 _code->create_usage(aContext);
}

bool FogFunctionExpr::emit(FogEmitContext& emitContext) const
{
 ERRMSG("BUG did not expect to FogFunctionExpr::emit for " << viz(*this));
 bool part1 = Super::emit(emitContext);
 bool part2 = false; //   _code->emit(emitContext); -- only invoked for signatures.
 return part1 || part2;
}

size_t FogFunctionExpr::executable_tokens() const
{
    return Super::executable_tokens();
}

void FogFunctionExpr::install(FogInstallContext& installContext) const
{
 ERRMSG("BUG did not expect to FogFunctionExpr::install for " << viz(*this));
 Super::install(installContext);
 _code->install(installContext);
}

bool FogFunctionExpr::is_actual(const FogScopeContext& scopeContext) const
{
 return Super::is_actual(scopeContext) && _code->is_actual(scopeContext);
}

void FogFunctionExpr::make_actual_from(FogMakeActualContext& makeActualContext)
{
 Super::make_actual_from(makeActualContext);
//  	FogResolutionScopeContext weakContext(makeActualContext, FogResolutionScopeContext::LOCATE_META_FUNCTION_FORMALS);
 if (makeActualContext.requires_formal_location_or_resolution()
     || makeActualContext.is_resolve_dollars())
     _code->make_actual(_code.to_const(), makeActualContext);
}

bool FogFunctionExpr::make_specifier(FogMakeSpecifierContext& makeSpecifierContext)
{
 if (!expr().make_specifier(makeSpecifierContext))
  return false;
 FogFunctionSpecifier *functionSpecifier = makeSpecifierContext.get_function_specifier(*this);
 if (functionSpecifier)
 {
//  		FogNestedScopeContext nestedContext(makeSpecifierContext, *functionSpecifier, FogNestedScopeContext::AS_IS);
//  		FogResolutionScopeContext weakContext(makeSpecifierContext, FogResolutionScopeContext::LOCATE_META_FUNCTION_FORMALS);
//  		FogCodeRef normalisedCode;
//  		_code->make_actual(normalisedCode.to_const(), weakContext);	// Normalise template parameters.
//  		functionSpecifier->add_to_subspace(*normalisedCode);
  functionSpecifier->add_to_subspace(*_code);
 }
 return true;
}

void FogFunctionExpr::merge_from(FogMergeContext& mergeContext, const This& thatExpr)
{
 Super::merge_from(mergeContext, thatExpr);
 thatExpr._code->merge_into(mergeContext, _code.to_const());
}

const FogMerge& FogFunctionExpr::needs_merge_from(FogMergeContext& mergeContext, const This& anExpr) const
{
 FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, anExpr));
 needsMerge |= _code->needs_merge(mergeContext, *anExpr._code);
 return needsMerge;
}

std::ostream& FogFunctionExpr::print_depth(std::ostream& s, int aDepth) const
{
 Super::print_depth(s, aDepth);
 _code->print_depth(s, aDepth+1);
 return s;
}

std::ostream& FogFunctionExpr::print_members(std::ostream& s, int aDepth) const
{
 Super::print_members(s, aDepth);
 _code->print_members(s, aDepth+1);
 return s;
}

char FogFunctionExpr::print_named(std::ostream& s, const PrimId *fullId, char tailChar) const
{
 tailChar = Super::print_named(s, fullId, tailChar);
//  	tailChar = _code->print_named(s, 0, tailChar);
 return tailChar;
}

std::ostream& FogFunctionExpr::print_viz(std::ostream& s) const
{
 s << "\"function-expression: ";
 print_named(s, 0, ' ');
 return s << '\"';
}

bool FogFunctionExpr::resolve_semantics(FogSemanticsContext& theSemantics) const
{
 FogChildSemanticsContext exprSemantics(theSemantics);
 if (!expr().resolve_semantics(exprSemantics))
  return false;
 theSemantics.reset();
 if (exprSemantics.is_function_declarative()
  || exprSemantics.is_constructor_declarative()
  || exprSemantics.is_destructor_declarative())
  theSemantics.set(FogSemantics::IS_FUNCTION_DEFINITION);
 if (exprSemantics.is_using_declarative())
  theSemantics.set(FogSemantics::IS_USING_DECLARATION);
//  	if (exprSemantics.is_function_elem_declarator()
//  	 || exprSemantics.is_pure_function_declarator()
//  	 || exprSemantics.is_constructor_declarator())
//  		theSemantics.set(FogSemantics::IS_FUNCTION_DEFINITOR);
 return true;
}

void FogFunctionExpr::set_decl_specifier_expression(FogExprRef& anExpr, FogDeclSpecifier& declSpecifiers)
{
 set_decorated_decl_specifier_expression(anExpr, declSpecifiers);
}
