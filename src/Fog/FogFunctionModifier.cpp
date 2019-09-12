#include <Fog/FogIncludeAll.h>
















TYPEINFO_SINGLE(FogFunctionModifier, Super)
FOGTOKEN_LEAF_IMPL(FogFunctionModifier)

FogFunctionModifier::FogFunctionModifier(const FogCv& aCv)
:
 _cv(aCv),
 _ellipsis(false)
{}

FogFunctionModifier::FogFunctionModifier(const This& aDecl)
:
 Super(aDecl),
 _cv(aDecl._cv),
 _parameters(aDecl._parameters),
 _exceptions(aDecl._exceptions),
 _ellipsis(aDecl._ellipsis)
{}

FogFunctionModifier::~FogFunctionModifier() {}

void FogFunctionModifier::add_exception_specification(const FogExpr& typeId)
{
//  	typeId.compile();
//  	typeId.compile(AS_TYPE);
 _exceptions.add(typeId);
}

void FogFunctionModifier::add_parameter(FogParameterSpecifier& aParameter)
{
//  	aParameter.compile();
 _parameters.add(aParameter);
}

bool FogFunctionModifier::compile_specifier(FogMakeSpecifierContext& makeSpecifierContext)
{
 for (FogParameterSpecifierConstListOfRefIterator p1(_parameters); p1; ++p1)
  if (!p1->compile_specifier(makeSpecifierContext))
   return false;
 return true;
}

void FogFunctionModifier::create_usage(FogUsageContext& usageContext) const
{
 Super::create_usage(usageContext);
 FogChangedUsageContext localContext(usageContext, usageContext.of_use_by().of_name());
 for (FogParameterSpecifierConstListOfRefIterator p1(_parameters); p1; ++p1)
  p1->create_usage(localContext);
//  	for (FogExprConstListOfRefToConstIterator p2(_exceptions); p2; ++p2)
//  		p2->create_usage(localContext);
}

bool FogFunctionModifier::emit_prefix(FogEmitContext& emitContext, bool needsBrackets) const
{
 if (!needsBrackets)
  return false;
 emitContext.emit_space_and_text("(");
 return true;
}

bool FogFunctionModifier::emit_suffix(FogEmitContext& emitContext, bool needsBrackets) const
{
 if (needsBrackets)
  emitContext.emit_space_and_text(")");
 emitContext.emit_space_and_text("(");
 if (_parameters.tally())
 {
//  		FogEmitContext::ChangedForUseAs parameterContext(emitContext,
//  			emitContext.emit_default_initialisers() ? FogEmitContext::LOCAL_INTERFACE :
//  			emitContext.emit_parameter_names() ? FogEmitContext::LOCAL_SIGNATURE :
//  											   FogEmitContext::ABSTRACT_SIGNATURE);
  FogForUseAs forUseAs(emitContext.for_use_as() | FogForUseAs::NO_SCOPE);
  if (!forUseAs.emit_parameter_names())
   forUseAs |= FogForUseAs::NO_NAME;
  FogForUseAsEmitContext parameterContext(emitContext, forUseAs);
  for (FogParameterSpecifierConstListOfRefToConstIterator p(_parameters); p; ++p)
  {
   p->emit(parameterContext);
   if ((p.to_go() > 1) || _ellipsis)
    parameterContext.emit_space_and_text(",");
  }
 }
 if (_ellipsis)
  emitContext.emit_space_and_text("...");
 emitContext.emit_space_and_text(")");
 emitContext.emit_cv(_cv->decl_specifier_value());
 if (_exceptions.tally() && emitContext.emit_exceptions())
 {
//  		FogExprListOfRefToConst exceptionList(_exceptions);
//  		exceptionList.sort(&FogRawException::compare);
  FogForUseAsEmitContext exceptionContext(emitContext, FogEmitContext::ABSTRACT_SIGNATURE);
  exceptionContext.emit_space_and_text("throw (");
  for (FogExprConstListOfRefToConstIterator p(_exceptions); p; ++p)
  {
   p->emit(exceptionContext);
   if (p.to_go() > 1)
    exceptionContext.emit_space_and_text(",");
  }
  exceptionContext.emit_space_and_text(")");
 }
 return true;
}

void FogFunctionModifier::install(FogInstallContext& installContext) const
{
 Super::install(installContext);
 for (FogParameterSpecifierConstListOfRefToConstIterator p1(_parameters); p1; ++p1)
  p1->install(installContext);
 for (FogExprConstListOfRefToConstIterator p2(_exceptions); p2; ++p2)
  p2->install(installContext);
}

bool FogFunctionModifier::is_actual(const FogScopeContext& scopeContext) const
{
 for (FogParameterSpecifierConstListOfRefToConstIterator p1(_parameters); p1; ++p1)
  if (!p1->is_actual(scopeContext))
   return false;
 for (FogExprConstListOfRefToConstIterator p2(_exceptions); p2; ++p2)
  if (!p2->is_actual(scopeContext))
   return false;
 return Super::is_actual(scopeContext);
}

bool FogFunctionModifier::is_assign(const PrimId& typeId) const
{
 if (_parameters.tally() != 1)
  return false;
 return false; //   .bugbug _parameters[0]->is_copy_parameter(typeId);
}

bool FogFunctionModifier::is_copy(const PrimId& typeId) const
{
 if (_parameters.tally() != 1)
  return false;
 return false; //   .bugbug _parameters[0]->is_copy_parameter(typeId);
}

const FogFunctionModifier *FogFunctionModifier::is_function_modifier() const { return this; }

void FogFunctionModifier::make_actual_from(FogMakeActualContext& makeActualContext)
{
 Super::make_actual_from(makeActualContext);
 for (FogParameterSpecifierListOfRefToConstIterator p1(_parameters); p1; ++p1)
  p1->make_actual(p1.ref(), makeActualContext);
 for (FogExprListOfRefToConstIterator p2(_exceptions); p2; ++p2)
  p2->make_actual(p2.ref(), makeActualContext);
}

void FogFunctionModifier::merge_from(FogMergeContext& mergeContext, const This& thatDecl)
{
 Super::merge_from(mergeContext, thatDecl);
 _cv = _cv->merge_cv(*thatDecl._cv);
 _ellipsis = merge_bool("ellipsis", _ellipsis, thatDecl._ellipsis);
 FogParameterSpecifier::merge_short_into(mergeContext, _parameters, thatDecl._parameters);
 _exceptions.add(thatDecl._exceptions);
//  	for (FogDeclaratorConstMapOfRefIterator e1(_exceptions); e1; ++e1)
//  	{
//  		FogDeclarator& thisDeclaration = *e1;
//  		FogDeclarator *thatDeclaration = thatDecl._exceptions.find(thisDeclaration.unique_id());
//  		if (!thatDeclaration)
//  			_exceptions.add(thisDeclaration);
//  		else
//  			_exceptions.add((FogDeclarator&)*thisDeclaration.merge_exception(*thatDeclaration));
//  	}
//  	for (FogDeclaratorConstMapOfRefIterator e2(thatDecl._exceptions); e2; ++e2)
//  	{
//  		FogDeclarator& thatDeclaration = *e2;
//  		FogDeclarator *thisDeclaration = _exceptions.find(thatDeclaration.unique_id());
//  		if (!thisDeclaration)
//  			_exceptions.add(thatDeclaration);
//  	}
}

const FogMetaType& FogFunctionModifier::meta_type() const { return FogMetaType::function_modifier_type(); }

const FogMerge& FogFunctionModifier::needs_merge_from(FogMergeContext& mergeContext,
  const FogFunctionModifier& thatFunction) const
{
 FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, thatFunction));
 needsMerge |= FogParameterSpecifier::needs_merge_exact(mergeContext, _parameters, thatFunction._parameters);
 needsMerge |= _cv->needs_merge_cv(*thatFunction._cv);
 if (_ellipsis != thatFunction._ellipsis)
  needsMerge |= FogMerge::incompatible_signature();
 if (_exceptions.tally())
 {
  if (thatFunction._exceptions.tally())
   needsMerge |= FogMerge::both_invalid();
  else
   needsMerge |= FogMerge::right_invalid();
 }
 else
 {
  if (thatFunction._exceptions.tally())
   needsMerge |= FogMerge::left_invalid();
 }
 return needsMerge;
}

std::ostream& FogFunctionModifier::print_depth(std::ostream& s, int aDepth) const
{
 Super::print_depth(s, aDepth);
 if (_parameters.tally())
 {
  s << indent(aDepth) << "parameters\n";
  _parameters.print_depth(s, aDepth+1);
 }
 if (_exceptions.tally())
 {
  s << indent(aDepth) << "exceptions\n";
  _exceptions.print_depth(s, aDepth+1);
 }
 return s;
}

std::ostream& FogFunctionModifier::print_members(std::ostream& s, int aDepth) const
{
 Super::print_members(s, aDepth);
 if (_parameters.tally())
 {
  s << indent(aDepth) << "parameters\n";
  _parameters.print_members(s, aDepth+1);
 }
 if (_exceptions.tally())
 {
  s << indent(aDepth) << "exceptions\n";
  _exceptions.print_members(s, aDepth+1);
 }
 return s;
}

char FogFunctionModifier::print_prefix(std::ostream& s, char tailChar, bool needsBrackets) const
{
 if (needsBrackets)
  tailChar = FogStream::space_and_emit(s, tailChar, "(");
 return tailChar;
}

char FogFunctionModifier::print_suffix(std::ostream& s, char tailChar, bool needsBrackets) const
{
 if (needsBrackets)
  tailChar = FogStream::space_and_emit(s, tailChar, ")");
 tailChar = FogStream::space_and_emit(s, tailChar, "(");
 for (FogParameterSpecifierConstListOfRefToConstIterator p(_parameters); p; ++p)
 {
  tailChar = p->print_named(s, 0, tailChar);
  if ((p.to_go() > 1) || _ellipsis)
   tailChar = FogStream::space_and_emit(s, tailChar, ",");
 }
 if (_ellipsis)
  tailChar = FogStream::space_and_emit(s, tailChar, "...");
 tailChar = FogStream::space_and_emit(s, tailChar, ")");
 if (_cv->is_cv())
  tailChar = FogStream::space_and_emit(s, tailChar, _cv->str());
 return tailChar;
}
