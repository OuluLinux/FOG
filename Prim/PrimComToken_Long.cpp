#include <Prim/PrimIncludeAll.h>




#ifndef NO_DATA
	TYPEINFO_SINGLE(PrimComToken_Long, PrimComToken);
#endif

#ifndef NO_CODE
//
//  	Construct an aName token for an integer value in theValue as part of aParser, using
//  	someOptions for parsing, and aUsage as a description of the token. Parsing of input values
//  	uses strtol() base 0, which is decimal by default but allowing C-style overrides.
//
PrimComToken_Long::PrimComToken_Long(PrimComParse& aParser, long& theValue, const char* aName,
                                     const char* aUsage, const TokenOptions& someOptions)
	:
	Super(aParser, aName, aUsage, someOptions),
	_default_value(0),
	_value(theValue),
	_number_base(default_number_base())
{}

//
//  	Construct an aName token for an integer value defaulting to defaultValue in theValue
//  	as part of aParser, using someOptions for parsing, and aUsage as a description of the token.
//  	numberBase may be specified to define a memory location containing the base in which numbers
//  	are parsed internally by strtol().
//
PrimComToken_Long::PrimComToken_Long(PrimComParse& aParser, long& theValue, const char* aName,
                                     const char* aUsage, long defaultValue, const int& numberBase, const TokenOptions& someOptions)
	:
	Super(aParser, aName, aUsage, someOptions),
	_default_value(defaultValue),
	_value(theValue),
	_number_base(numberBase)
{}

//
//  	The default destructor does nothing.
//
PrimComToken_Long::~PrimComToken_Long() {}

//
//  	Initialise for a new parsing pass.
//
void PrimComToken_Long::initialise_parse(PrimComParse& aParser) {
	_value = _default_value;
	Super::initialise_parse(aParser);
}

//
//  	Report that this token does use a minus prefix.
//
bool PrimComToken_Long::is_minus_token() const { return true; }

//
//  	Attempt to parse someText, returning non-null if accepted.
//
const char* PrimComToken_Long::parse_text(PrimComParse& aParser, const char* someText) {
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
		
	int numberBase = _number_base;
	
	if ((*p == '0') && ((*(p + 1) == 'x') || (*(p + 1) == 'X')))
		numberBase = 16;
		
	char* pEnd;
	long aValue = strtol(p, &pEnd, numberBase);
	
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
std::ostream& PrimComToken_Long::print_this(std::ostream& s) const {
	Super::print_this(s);
	s << ", " << _value << ", base " << _number_base;
	return s;
}

//
//  	Print out a text string to describe the token argument as "#".
//
std::ostream& PrimComToken_Long::print_token_arg_text(std::ostream& s) const {
	return s << "#";
}
#endif
#endif
