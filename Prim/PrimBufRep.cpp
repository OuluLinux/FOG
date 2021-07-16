#include <Prim/PrimIncludeAll.h>


//  
//  	Destruction frees the memory buffer.
//  
PrimBufRep::~PrimBufRep()
{
 if (this != &_null_rep)
  delete[] _bytes;
}
