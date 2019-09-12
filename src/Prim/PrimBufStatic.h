
#ifndef PRIMBUFSTATIC_HXX
#define PRIMBUFSTATIC_HXX



//  
//   A PrimBufStatic implements a statically constructed PrimBuf using the customers text buffer.
//   This differs from the normal PrimBuf behaviour that creates a dynamic copy of the text buffer
//   and a dynamic internal object for the representation. This class is intended for definition of fixed
//   strings used by static objects, avoiding the need to invoke constructors at start up (and diagnostics
//   resulting from the failure to destruct at shut down).
//  .note cscrelations
//   PrimBufStatic is derived from @x(PrimBuf).
//  .note csusum
//   PrimBufStatic implements a specialised construction method to use the customers text buffer.
//  
class PrimBufStatic : public PrimBuf
{
private:
 PrimBufRep _static_rep;      //   The representation in staically allocated memory.

public:
 PrimBufStatic(const char *aBuf);
//  
//  	Construct a new buffer using the supplied buffer.
//  
 PrimBufStatic(const char *aBuf, size_t aLength)
  : PrimBuf(_static_rep), _static_rep(aBuf, aLength, 0) {}
};
#endif
 
