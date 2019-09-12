#include <Prim/PrimIncludeAll.h>




TYPEINFO_SINGLE(PrimNamed, PrimPrint);

//  
//  	The default destructor does nothing.
//  
PrimNamed::~PrimNamed() {}

#ifndef NO_PRIM_STREAMS
//  
//  		Provide the name as part of an in-line diagnostic print out.
//  
std::ostream& PrimNamed::print_this(std::ostream& s) const { return s << _name; }
#endif
