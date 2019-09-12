#include <Fog/FogIncludeAll.h>









void FogScope_Variables::create_usages(FogScope& inScope)
{
 const FogTag& aTag = inScope.tag();
 bool orderedVariables = aTag.is_class_tag() || aTag.is_struct_tag();
 FogVariable *prevVariable = 0;
 for (FogVariableConstListOfRefIterator p(_distinct_variable_list); p; ++p)
 {
  FogVariable& aVariable = *p;
  if (aVariable.is_active())
  {
   aVariable.create_usages();
   if (!aVariable.decl_specifiers().is_static() && orderedVariables)
   {
    if (prevVariable)
     prevVariable->interface_usage_finish().add_use_by(aVariable.interface_usage_start());
    prevVariable = &aVariable;
   }
  }
 }
}

void FogScope_Variables::derive_from(FogScope& derivedScope, FogBaseClass& baseClass)
{
 for (FogVariableConstListOfRefIterator p(baseClass.base().distinct_variable_list()); p; ++p)
  derive_from(derivedScope, *p, 0);
}

void FogScope_Variables::derive_from(FogScope& derivedScope, FogVariable& baseVariable,
 FogPotentialDeclaration *potentialDeclaration)
{
 FogStaticScopeContext derivedScopeContext(derivedScope);
 FogDerivable *aDerivable = _distinct_variable_map.find(baseVariable.local_signature_id());
 if (!potentialDeclaration)
  aDerivable = baseVariable.make_derived(derivedScopeContext, aDerivable);
 else if (!aDerivable)
  aDerivable = baseVariable.make_derived(derivedScopeContext, aDerivable);
 else
  potentialDeclaration->make_derivation(derivedScopeContext, *aDerivable);
}

void FogScope_Variables::destroy()
{
 for (FogVariableConstListOfRefIterator p(_distinct_variable_list); p; ++p)
  p->destroy();
 _distinct_variable_map.vacate();
 _distinct_variable_list.vacate();
}

void FogScope_Variables::do_compile(FogCompileContext& inScope)
{
 for (FogVariableConstListOfRefIterator p(_distinct_variable_list); p; ++p)
 {
  FogVariable& aVariable = *p;
//  		if (aVariable.is_active())
   aVariable.do_compile(inScope);
 }
}

void FogScope_Variables::do_derive(FogDeriveContext& inScope)
{
 for (FogVariableConstListOfRefIterator p(_distinct_variable_list); p; ++p)
  p->do_derive(inScope);
}

FogVariable *FogScope_Variables::find_local_variable(const FogVariable& aVariable)
{
 const PrimId& anId = aVariable.local_signature_id();
 FogVariable *foundVariable = _distinct_variable_map.find(anId);
 return foundVariable && foundVariable->is_active() ? foundVariable : 0;
}

FogVariable *FogScope_Variables::make_variable(FogMakerContext& makerContext)
{
 const PrimIdHandle localId = makerContext.local_signature_id();
 FogVariable *aVariable = _distinct_variable_map.find(*localId);
 if (!aVariable)
 {
  FogScope& thisScope = makerContext.dynamic_scope();
  if (thisScope.is_frozen())
   ERRMSG("Frozen class structure suppresses " << viz(makerContext));
  else if ((aVariable = new FogVariable(makerContext)) != 0)
  {
   _distinct_variable_map.adopt(*aVariable);
   _distinct_variable_list.add(*aVariable);
   thisScope.add_variable(*aVariable, 0);
  }
 }
 return aVariable;
}

bool FogScope_Variables::needs_emission() const
{
 for (FogVariableConstListOfRefToConstIterator p(_distinct_variable_list); p; ++p)
  if (p->is_active())
   return true;
 return false;
}

std::ostream& FogScope_Variables::print_depth(std::ostream& s, int aDepth) const
{
 if (_distinct_variable_list.tally())
 {
  s << indent(aDepth) << "variables\n";
  _distinct_variable_list.print_depth(s, aDepth+1);
 }
 return s;
}

std::ostream& FogScope_Variables::print_members(std::ostream& s, int aDepth) const
{
 if (_distinct_variable_list.tally())
 {
  s << indent(aDepth) << "variables\n";
  _distinct_variable_list.print_members(s, aDepth+1);
 }
 return s;
}

void FogScope_Variables::validate() const
{
 for (FogVariableConstListOfRefToConstIterator p(_distinct_variable_list); p; ++p)
  if (p->is_active())
   p->validate();
}
