#include <Fog/FogIncludeAll.h>











TYPEINFO_SINGLE(FogTemplateParameterSpecifiers, Super)
PRIMREF_NULL_CLAS(FogTemplateParameterSpecifiers)
 FOGTOKEN_NULL_LEAF
};
PRIMREF_DERIVED_NULL_IMPL(FogTemplateParameterSpecifiers)
PRIMREFS_IMPL(FogTemplateParameterSpecifiers)
FOGTOKEN_LEAF_IMPL(FogTemplateParameterSpecifiers)
TMPL_HACK_FIX_DO(FogTemplateParameterSpecifiers)

FogTemplateParameterSpecifiers::FogTemplateParameterSpecifiers()
:
 _last_outer_parameter_number(0),
 _is_export(NON_EXPORT_TEMPLATE),
 _is_using(NON_USING_TEMPLATE)
{}

FogTemplateParameterSpecifiers::FogTemplateParameterSpecifiers(const FogTemplateParameterSpecifiers *outerParameters,
 IsExportTemplate isExport, IsUsingTemplate isUsing)
:
 _outer_parameters(outerParameters),
 _last_outer_parameter_number(outerParameters ? outerParameters->last_template_parameter_number() : 0),
 _is_export(isExport),
 _is_using(isUsing)
{}

FogTemplateParameterSpecifiers::~FogTemplateParameterSpecifiers() {}

void FogTemplateParameterSpecifiers::add(FogTemplateParameterSpecifier& aParameter,
 const FogTemplateParameterSpecifier *parentSpecifier)
{
 if (!is_null())
 {
  aParameter.set_template_parameter_number(last_template_parameter_number()+1, parentSpecifier);
  _parameters.add(aParameter);
 }
}

void FogTemplateParameterSpecifiers::create_usage(FogUsageContext& aContext) const
{
 Super::create_usage(aContext);
 for (FogTemplateParameterSpecifierConstListOfRefToConstIterator p(_parameters); p; ++p)
  p->create_usage(aContext);
}

bool FogTemplateParameterSpecifiers::emit(FogEmitContext& emitContext) const
{
 if (_is_export)
  emitContext.emit_space_and_text("export ");
 if (_is_using)
  emitContext.emit_space_and_text("/* using */ ");
 emitContext.emit_space_and_text("template <");
 for (FogTemplateParameterSpecifierConstListOfRefToConstIterator p(_parameters); p; ++p)
 {
  p->emit(emitContext);
  if (p.to_go() > 1)
   emitContext.emit_space_and_text(",");
 }
 emitContext.emit_space_and_text(">");
 return true;
}

void FogTemplateParameterSpecifiers::emit_suffix(FogEmitContext& emitContext) const
{
 emitContext.emit_space_and_text("<");
 for (FogTemplateParameterSpecifierConstListOfRefToConstIterator p(_parameters); p; ++p)
 {
  p->emit_suffix(emitContext);
  if (p.to_go() > 1)
   emitContext.emit_space_and_text(",");
 }
 emitContext.emit_space_and_text(">");
}

//  const FogTemplateParameterSpecifiers& FogTemplateParameterSpecifiers::empty()
//  {
//  	static FogTemplateParameterSpecifiersRef emptyParameters(new FogTemplateParameterSpecifiers,
//  		FogTemplateParameterSpecifiersRef::ADOPT);
//  	return *emptyParameters;
//  }

const FogTemplateParameterSpecifier *FogTemplateParameterSpecifiers::find(FogScopeContext& scopeContext,
 const PrimId& anId) const
{
 for (FogTemplateParameterSpecifierConstListOfRefToConstIterator p(_parameters); p; ++p)
  if (p->find(scopeContext, anId))
   return p.pointer();
 return 0;      //   Mult-level search orchestrated by caller.
//  	return _outer_parameters ? _outer_parameters->find(scopeContext, anId) : 0;
}

const FogTemplateParameterSpecifier *FogTemplateParameterSpecifiers::find(
    const FogTemplateParameterSpecifierId& anId, bool deepFind) const
{
 for (FogTemplateParameterSpecifierConstListOfRefToConstIterator p(_parameters); p; ++p)
  if (p->find(anId))
   return p.pointer();
 return (deepFind && _outer_parameters) ? _outer_parameters->find(anId) : 0;
}

//  
//  	Return the highest number not in use, noting that 0 is always in use for the empty list.
//  
size_t FogTemplateParameterSpecifiers::first_template_parameter_number() const
{
 if (is_null())
  return 0;
 else
  return _outer_parameters->last_template_parameter_number();
}

const PrimId& FogTemplateParameterSpecifiers::id() const
{
 if (!_id)
 {
  PrimOstrstream s;
  print_suffix(s, 0);
  mutate()._id = s.id();
 }
 return *_id;
}

void FogTemplateParameterSpecifiers::install(FogInstallContext& installContext) const
{
 Super::install(installContext);
 for (FogTemplateParameterSpecifierConstListOfRefToConstIterator p(_parameters); p; ++p)
  p->install(installContext);
}

bool FogTemplateParameterSpecifiers::is_actual(const FogScopeContext& scopeContext) const
{
 for (FogTemplateParameterSpecifierConstListOfRefToConstIterator p(_parameters); p; ++p)
  if (!p->is_actual(scopeContext))
   return false;
 return Super::is_actual(scopeContext);
}

//  
//  	Return the highest number in use, noting that 0 is always in use for the empty list.
//  
size_t FogTemplateParameterSpecifiers::last_template_parameter_number() const
{
 if (is_null())
  return 0;
 else
  return _last_outer_parameter_number + _parameters.tally();
}

void FogTemplateParameterSpecifiers::make_actual_from(FogMakeActualContext& makeActualContext)
{
 Super::make_actual_from(makeActualContext);
 for (FogTemplateParameterSpecifierListOfRefToConstIterator p(_parameters); p; ++p)
  p->make_actual(p.ref(), makeActualContext);
}

void FogTemplateParameterSpecifiers::merge_from(FogMergeContext& mergeContext, const This& templatePars)
{
 Super::merge_from(mergeContext, templatePars);
//  	Super::merge_long_into(mergeContext, _exprs, templatePars._exprs);
 FogTemplateParameterSpecifier::merge_long_into(mergeContext, _parameters, templatePars._parameters);
}

const FogMerge& FogTemplateParameterSpecifiers::needs_merge_from(FogMergeContext& mergeContext,
 const This& templatePars) const
{
 if (this == &templatePars)
  return FogMerge::both_valid();
 if (is_null() != templatePars.is_null())
  return FogMerge::set_incompatible(FogMerge::incompatible_templating());
 FogTemplateParameterSpecifierConstListOfRefToConstIterator p1(_parameters);
 FogTemplateParameterSpecifierConstListOfRefToConstIterator p2(templatePars._parameters);
 if (p1.to_go() != p2.to_go())
  return FogMerge::set_incompatible(FogMerge::incompatible_template_rank());
 FogMergeHandle needsMerge;
 for ( ; p1 && !needsMerge->is_incompatible(); ++p1, ++p2)
  needsMerge |= p1->needs_merge(mergeContext, *p2);
 return needsMerge;
}

char FogTemplateParameterSpecifiers::print_named(std::ostream& s, const PrimId *scopeId, char tailChar) const
{
 if (_is_export)
  tailChar = FogStream::space_and_emit(s, tailChar, "export ");
 if (_is_using)
  tailChar = FogStream::space_and_emit(s, tailChar, "using ");
 tailChar = FogStream::space_and_emit(s, tailChar, "template <");
 for (FogTemplateParameterSpecifierConstListOfRefToConstIterator p(_parameters); p; ++p)
 {
  tailChar = p->print_named(s, 0,  tailChar);
  if (p.to_go() > 1)
   tailChar = FogStream::space_and_emit(s, tailChar, ",");
 }
 tailChar = FogStream::space_and_emit(s, tailChar, ">");
 return tailChar;
}

char FogTemplateParameterSpecifiers::print_suffix(std::ostream& s, char tailChar) const
{
 tailChar = FogStream::space_and_emit(s, tailChar, "<");
 for (FogTemplateParameterSpecifierConstListOfRefToConstIterator p(_parameters); p; ++p)
 {
  tailChar = p->print_suffix(s, tailChar);
  if (p.to_go() > 1)
   tailChar = FogStream::space_and_emit(s, tailChar, ",");
 }
 tailChar = FogStream::space_and_emit(s, tailChar, ">");
 return tailChar;
}

std::ostream& FogTemplateParameterSpecifiers::print_viz(std::ostream& s) const
{
 s << "\"template-parameter-specifiers: ";
 print_named(s, 0, ' ');
 return s << '\"';
}

//  
//    Return the template<> nesting depth, 1 at outer level.
//  
size_t FogTemplateParameterSpecifiers::template_parameter_depth() const
{
 if (is_null())
  return 0;
 else
  return _outer_parameters->template_parameter_depth() + 1;
}

const FogTemplateParameterSpecifier& FogTemplateParameterSpecifiers::template_parameter_number(size_t i) const
{
 if (is_null())
  return FogTemplateParameterSpecifier::immutable_null();
 else if (i <= _last_outer_parameter_number)
  return _outer_parameters->template_parameter_number(i);
 else if (i <= _last_outer_parameter_number + _parameters.tally())
  return *_parameters[i - _last_outer_parameter_number - 1];
 else
  return FogTemplateParameterSpecifier::immutable_null();
}
