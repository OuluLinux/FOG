#include <Prim/PrimIncludeAll.h>




#ifndef NO_DATA
	TYPEINFO_SINGLE(PrimComToken_Double, PrimComToken);
#endif

#ifndef NO_CODE
//
//  	Construct an aName token for a floating point value in theValue as part of aParser, using
//  	someOptions for parsing, and aUsage as a description of the token.
//
PrimComToken_Double::PrimComToken_Double(PrimComParse& aParser, double& theValue, const char* aName,
        const char* aUsage, const TokenOptions& someOptions)
	:
	Super(aParser, aName, aUsage, someOptions),
	_default_value(0.0),
	_value(theValue)
{}

//
//  	Construct an aName token for a floating point value defaulting to defaultValue in theValue
//  	as part of aParser, using someOptions for parsing, and aUsage as a description of the token.
//
PrimComToken_Double::PrimComToken_Double(PrimComParse& aParser, double& theValue, const char* aName,
        const char* aUsage, double defaultValue, const TokenOptions& someOptions)
	:
	Super(aParser, aName, aUsage, someOptions),
	_default_value(defaultValue),
	_value(theValue)
{}

//
//  	The default destructor does nothing.
//
PrimComToken_Double::~PrimComToken_Double() {}

//
//  	Initialise for a new parsing pass.
//
void PrimComToken_Double::initialise_parse(PrimComParse& aParser) {
	_value = _default_value;
	Super::initialise_parse(aParser);
}

//
//  	Report that this token does use a minus prefix.
//
bool PrimComToken_Double::is_minus_token() const { return true; }

//
//  	Attempt to parse someText, returning non-null if accepted.
//
const char* PrimComToken_Double::parse_text(PrimComParse& aParser, const char* someText) {
	const char* p = Super::parse_text(aParser, someText);
	
	if (!p)            //   If bad prefix
		return 0;          //  	parsing fails
		
	if (*p != EOS)          //   If more
		;            //  	must parse it
	else if (is_optional()) {      //   If no more and optional
		//  	all done
		set_is_present();
		return p;
	}
	else if ((p = aParser.next_arg()) == 0)    //   If no additional argument
		return 0;          //  	parsing fails
		
	char* pEnd;
	double aValue = strtod(p, &pEnd);
	
	if ((!pEnd || (*pEnd != EOS)) && aParser.diagnose_warnings()) {
		PRIMWRN(BAD_PARSE, *this << " failed to resolve \"" << p << "\"");
		return 0;
	}
	
	_value = aValue;
	set_is_present();
	return pEnd;
}

#ifndef NO_PRIM_STREAMS
//
//  	Add an indication of token value to an in-line print-out.
//
std::ostream& PrimComToken_Double::print_this(std::ostream& s) const {
	Super::print_this(s);
	s << ", " << _value;
	return s;
}

//
//  	Print out a text string to describe the token argument as "#.#".
//
std::ostream& PrimComToken_Double::print_token_arg_text(std::ostream& s) const {
	return s << "#.#";
}
#endif
#endif
