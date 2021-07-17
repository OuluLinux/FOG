#include <Prim/PrimIncludeAll.h>




#ifndef NO_DATA
	TYPEINFO_SINGLE(PrimIded, PrimPrint);
#endif

#ifndef NO_CODE
//
//  	The destructor adds no functionality.
//
PrimIded::~PrimIded() {}

#ifndef NO_PRIM_STREAMS
//
//  	Provide the id as part of an in-line diagnostic print out.
//
std::ostream& PrimIded::print_this(std::ostream& s) const { return (s << _id); }
#endif
#endif
