
#ifndef PRIMSTRINGSTATIC_HXX
#define PRIMSTRINGSTATIC_HXX


//
//  		A PrimStringStatic implements a statically constructed PrimString using the customers text buffer. //		//		This differs from the normal PrimString behaviour that creates a dynamic copy of the text buffer
//  		and a dynamic internal object for the representation. This class is intended for definition of fixed
//  		strings used by static objects, avoiding the need to invoke constructors at start up (and diagnostics
//  		resulting from the failure to destruct at shut down).
//
//  class PrimStringStatic : public PrimString
//  {
//  private:
//  	PrimStringRep _static_rep;					// The static representation.
//  public:
//  	PrimStringStatic(const char *someText);
//  };
#endif

