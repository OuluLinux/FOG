#include <Fog/FogIncludeAll.h>







TYPEINFO_SINGLE(FogMacro, Super)
PRIMREF_NULL_CLASS_AND_IMPL(FogMacro)
PRIMSHARES_IMPL(FogMacro)
PRIMREFS_IMPL(FogMacro)
PRIMMAPOFREFS_IMPL(FogMacro)
TMPL_HACK_FIX_DO(FogMacro)

FogMacro::FogMacro()
:
 _function_like(false),
 _is_bad(false)
{}

FogMacro::FogMacro(const PrimId& anId, const PrimIdList *formalList, FogTokens& someTokens)
:
 _id(anId),
 _tokens(someTokens),
 _function_like(formalList != 0),
 _is_bad(false)
{
 if (formalList)
  for (PrimIdConstListIterator p(*formalList); p; ++p)
  {
   if (_formal_map.find(*p))
    ERRMSG("Duplicate parameter \"" << *p << "\" ignored in " << viz(*this));
   else
    _formal_map.add(*p);
   _formal_list.add(*p);
  }
}

FogMacro::~FogMacro() {}

int FogMacro::find(const PrimId& anId) const
{           //  .bugbug make _formal_map a set of int.
 int i = 0;
 for (PrimIdConstListIterator p(_formal_list); p; ++p, ++i)
  if (*p == anId)
   return i;
 return -1;
}

const PrimId& FogMacro::formal(size_t anIndex) const
{
 if (anIndex < _formal_list.tally())
  return *_formal_list[anIndex];
 ERRMSG("There is no formal parameter number " << (anIndex+1) << " for " << viz(*this));
 return PrimId::null();
}

//  
//  	Return true if this macro is compatible with a macro that would result from combining aMacro and someText.
//  	This is intended to perform the valid replacement test to satisfy 3.8.3 of the ANSI C standard.
//  	The test is eased by the whitespace normalisation of someText that occurs if the lexer.
//  
bool FogMacro::is_compatible_with(const PrimIdList *formalList, const FogTokens& someTokens) const
{
 if (formalList)
 {
  if (!_function_like)
   return false;
  if (formalList->tally() != _formal_list.tally())
   return false;
  PrimIdConstListIterator p1(_formal_list);
  PrimIdConstListIterator p2(*formalList);
  for (; p1; ++p1, ++p2)
   if (*p1 != *p2)
    return false;
 }
 else
 {
  if (_function_like)
   return false;
 }
//  	return *_tokens == someTokens;
 const FogTokenListOfRefToConst& firstTokens = _tokens->tokens();
 const FogTokenListOfRefToConst& secondTokens = someTokens.tokens();
 if (firstTokens.tally() != secondTokens.tally())
  return false;
 FogTokenConstListOfRefToConstIterator p1(firstTokens);
 FogTokenConstListOfRefToConstIterator p2(secondTokens);
 for ( ; p1 && p2; ++p1, ++p2)
  if (p1.pointer() != p2.pointer())
   return false; 
 return true;
}

std::ostream& FogMacro::print_depth(std::ostream& s, int aDepth) const
{
 Super::print_depth(s, aDepth);
 if (_formal_list.tally())
 {
  s << indent(aDepth) << "formals\n";
  _formal_list.print_depth(s, aDepth+1);
 }
 _tokens->print_depth(s, aDepth);
 return s;
}

std::ostream& FogMacro::print_members(std::ostream& s, int aDepth) const
{
 Super::print_members(s, aDepth);
 if (_formal_list.tally())
 {
  s << indent(aDepth) << "formals\n";
  _formal_list.print_members(s, aDepth+1);
 }
 _tokens->print_members(s, aDepth);
 return s;
}

std::ostream& FogMacro::print_this(std::ostream& s) const
{
 s << id();
 return s;
}

std::ostream& FogMacro::print_viz(std::ostream& s) const
{
 s << "\"macro: " << id() << '\"';
 return s;
}
