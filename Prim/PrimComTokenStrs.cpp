#include <Prim/PrimIncludeAll.h>




#ifndef NO_DATA
	TYPEINFO_SINGLE(PrimComTokenStrs, PrimComToken);
#endif

#ifndef NO_CODE
//
//  	The default destructor does nothing.
//
PrimComTokenStrs::~PrimComTokenStrs() {}

//
//  	Initialise for a new parsing pass.
//
void PrimComTokenStrs::initialise_parse(PrimComParse& aParser) {
	_value.erase();
	Super::initialise_parse(aParser);
}

//
//  	Report that this token does support multiple values.
//
bool PrimComTokenStrs::is_multiple_token() const { return true; }

//
//  	Attempt to parse someText, returning non-null if accepted.
//
const char* PrimComTokenStrs::parse_text(PrimComParse& aParser, const char* someText) {
	const char* p = Super::parse_text(aParser, someText);
	
	if (!p || ((*p == EOS) && ((p = aParser.next_arg()) == 0)))
		return (0);
		
	_value.append(p);
	set_is_present();
	return someText + strlen(someText);
}

#ifndef NO_PRIM_STREAMS
//
//  	Add an indication of token value to an in-line print-out.
//
std::ostream& PrimComTokenStrs::print_this(std::ostream& s) const {
	Super::print_this(s);
	s << ", \"" << _value << "\"";
	return s;
}
#endif
#endif
