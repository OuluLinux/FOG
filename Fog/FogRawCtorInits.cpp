#include <Fog/FogIncludeAll.h>








TYPEINFO_SINGLE(FogRawCtorInits, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogRawCtorInits)
PRIMREFS_IMPL(FogRawCtorInits)
FOGTOKEN_UNIQUE_IMPL(FogRawCtorInits)
TMPL_HACK_FIX_DO(FogRawCtorInits)

FogRawCtorInits::FogRawCtorInits() {}
FogRawCtorInits::~FogRawCtorInits() {}

void FogRawCtorInits::add(FogRawCtorInit& ctorInit)
{
//  	ctorInit.compile();
 _inits.add(ctorInit);
}

std::ostream& FogRawCtorInits::print_depth(std::ostream& s, int aDepth) const
{
 Super::print_depth(s, aDepth);
 if (_inits.tally())
  _inits.print_depth(s, aDepth);
 return s;
}

std::ostream& FogRawCtorInits::print_members(std::ostream& s, int aDepth) const
{
 Super::print_members(s, aDepth);
 if (_inits.tally())
  _inits.print_members(s, aDepth);
 return s;
}

std::ostream& FogRawCtorInits::print_this(std::ostream& s) const { return s; }

std::ostream& FogRawCtorInits::print_viz(std::ostream& s) const
{
 s << "\"ctor-inits: ";
 print_this(s);
 return s << '\"';
}
