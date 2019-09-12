#include <Prim/PrimIncludeAll.h>


//  
//  		Construct a new string using the supplied buffer, which should be a quoted string or
//  		some other immutable text.
//  
//  PrimStringStatic::PrimStringStatic(const char *someText)
//  :
//  	PrimString(_static_rep),
//  	_static_rep(someText)
//  {
//  	_static_rep.share();					// Leak to prevent static destruction.
//  }
