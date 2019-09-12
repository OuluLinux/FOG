#include <Fog/FogIncludeAll.h>








void FogScope_Names::add_name(const PrimId& anId, FogEntity& anEntity)
{
 FogEntitySetOfListOfRefElement *theNames = _names.find(anId);
 if (theNames)
 {
  FogEntityListOfRef& theEntities = theNames->value();
  FogEntity *firstName = theEntities.tally() ? theEntities[0].pointer() : 0;
  if (firstName && ((anEntity.is_function() != 0) != (firstName->is_function() != 0)))
   ERRMSG("Should not add name \"" << anId << "\" for " << viz(anEntity)
    << "\n\tin conflict with " << viz(*firstName));
  if (theEntities.find(anEntity))  //   Suppress duplicates which may arise from multiple
   return;       //    potential declarations
 }
 else
 {
  theNames = new FogEntitySetOfListOfRefElement(anId);
  if (theNames)
   _names.adopt(*theNames);
 }
 if (theNames)
  theNames->value().add(anEntity);
}

//  
//  	Add the objects (variable/function/enumerator) identified by makeContext to theFinding.
//  	Returns the number of names found.
//  
size_t FogScope_Names::find_local_name(FogEntityFinder& theFinder)
{
 FogEntitySetOfListOfRefElement *theNames = _names.find(theFinder.id());
 if (!theNames)
  return 0;
 size_t foundNameCount = 0;
 FogEntityListOfRef& nameList = theNames->value();
 for (FogEntityConstListOfRefIterator p(nameList); p; ++p)
  if (p->is_active())
  {
   theFinder.add_find(*p);
   foundNameCount++;
  }
 return foundNameCount;
}

const FogEntityListOfRefToConst *FogScope_Names::find_local_names(const PrimId& anId) const
{
 const FogEntitySetOfListOfRefElement *elementSet = _names.find(anId);
 return elementSet ? &elementSet->value().to_const() : 0;
}

std::ostream& FogScope_Names::print_depth(std::ostream& s, int aDepth) const
{
 if (_names.tally())
 {
  s << indent(aDepth) << "names\n";
  for (FogEntityConstSetOfListOfRefToConstIterator p1(_names); p1; ++p1)
  {
   s << indent(aDepth+1) << *p1 << '\n';
   for (FogEntityConstListOfRefToConstIterator p2(p1->value()); p2; ++p2)
    s << indent(aDepth+2) << viz(*p2) << '\n';
  }
 }
 return s;
}

std::ostream& FogScope_Names::print_members(std::ostream& s, int aDepth) const
{
 if (_names.tally())
 {
  s << indent(aDepth) << "names\n";
  for (FogEntityConstSetOfListOfRefToConstIterator p1(_names); p1; ++p1)
  {
   s << indent(aDepth+1) << *p1 << '\n';
   for (FogEntityConstListOfRefToConstIterator p2(p1->value()); p2; ++p2)
    s << indent(aDepth+2) << viz(*p2) << '\n';
  }
 }
 return s;
}
