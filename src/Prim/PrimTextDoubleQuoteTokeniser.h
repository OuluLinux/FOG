
#ifndef PRIMTEXTDOUBLEQUOTETOKENISER_HXX
#define PRIMTEXTDOUBLEQUOTETOKENISER_HXX


//  
//  	A PrimTextDoubleQuoteTokeniser defines a tokenising strategy object that tokenises according to
//  	white-space that is not enclosed in double quotes.
//  
class PrimTextDoubleQuoteTokeniser : public PrimTextTokeniser
{
 TYPEDECL_SINGLE(PrimTextDoubleQuoteTokeniser, PrimTextTokeniser)
public:
//  
//  	Construct an un-double-quoted white-space tokeniser for s.
//  
 PrimTextDoubleQuoteTokeniser(std::istream& s) : PrimTextTokeniser(s) {}
 virtual PrimText tokenise();
};
#endif
 
