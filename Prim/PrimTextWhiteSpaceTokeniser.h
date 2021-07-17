
#ifndef PRIMTEXTWHITESPACETOKENISER_HXX
#define PRIMTEXTWHITESPACETOKENISER_HXX


//
//  	A PrimTextWhiteSpaceTokeniser defines a tokenising strategy object that tokenises according to white-space.
//
class PrimTextWhiteSpaceTokeniser : public PrimTextTokeniser {
	TYPEDECL_SINGLE(PrimTextWhiteSpaceTokeniser, PrimTextTokeniser)
public:
	//
	//  	Construct a white-space tokeniser for s.
	//
	PrimTextWhiteSpaceTokeniser(std::istream& s) : PrimTextTokeniser(s) {}
	virtual PrimText tokenise();
};
#endif

