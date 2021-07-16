
#ifndef PRIMTEXTTOKENISER_HXX
#define PRIMTEXTTOKENISER_HXX



//  
//  	A PrimTextTokeniser defines the abstract capabilities of a tokenising strategy object.
//  
class PrimTextTokeniser : public PrimPrint
{
 TYPEDECL_SINGLE(PrimTextTokeniser, PrimPrint)
private:
 std::istream& the_s;       //   Stream to be tokenised.

protected:
//  
//  	Construct a default abstract tokeniser for s.
//  
 PrimTextTokeniser(std::istream& s) : the_s(s) {}
//  
//  	Report the stream to be tokenised.
//  
 std::istream& s() { return (the_s); }

public:
 operator void *() const;
 bool operator!() const;
//  
//  	Derived classes must implement tokenisation.
//  
 virtual PrimText tokenise() = 0;
};
#endif
 
