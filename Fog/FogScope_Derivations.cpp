#include <Fog/FogIncludeAll.h>





void FogScope_Derivations::changed_base_class(FogScope& thisScope, FogBaseClass& baseClass)
{
 for (FogScopeConstMapOfRefIterator p(_derivations); p; ++p)
  p->changed_base_class(baseClass);
}

void FogScope_Derivations::derive_from(FogScope& derivedScope, FogBaseClass& baseClass)
{
 for (FogScopeConstMapOfRefIterator p(_derivations); p; ++p)
  p->derive_from(baseClass);
}

void FogScope_Derivations::derive_from(FogFunction& baseFunction, FogPotentialDeclaration *potentialDeclaration)
{
 for (FogScopeConstMapOfRefIterator p(_derivations); p; ++p)
  p->derive_from(baseFunction, potentialDeclaration);
}

void FogScope_Derivations::derive_from(FogTypedef& baseTypedef, FogPotentialDeclaration *potentialDeclaration)
{
 for (FogScopeConstMapOfRefIterator p(_derivations); p; ++p)
  p->derive_from(baseTypedef, potentialDeclaration);
}

void FogScope_Derivations::derive_from(FogVariable& baseVariable, FogPotentialDeclaration *potentialDeclaration)
{
 for (FogScopeConstMapOfRefIterator p(_derivations); p; ++p)
  p->derive_from(baseVariable, potentialDeclaration);
}

void FogScope_Derivations::destroy()
{
 _derivations.vacate();
}

std::ostream& FogScope_Derivations::print_depth(std::ostream& s, int aDepth) const
{
 if (_derivations.tally())
 {
  s << indent(aDepth) << "derivations\n";
  _derivations.print_members(s, aDepth);
 }
 return s;
}

std::ostream& FogScope_Derivations::print_members(std::ostream& s, int aDepth) const
{
 return s;
}
