#include <Prim/PrimIncludeAll.h>




#ifndef NO_DATA
TYPEINFO_SINGLE(PrimTextTokeniser, PrimPrint);
#endif

#ifndef NO_CODE
//  
//  	Return the stream state as the tokeniser state.
//  
PrimTextTokeniser::operator void *() const { return (&the_s); } // Hack

//  
//  	Return the stream state as the tokeniser state.
//  
bool PrimTextTokeniser::operator!() const { return (!the_s); }
#endif
