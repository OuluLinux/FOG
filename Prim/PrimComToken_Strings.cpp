#include <Prim/PrimIncludeAll.h>




#ifndef NO_DATA
	TYPEINFO_SINGLE(PrimComToken_Strings, PrimComTokenStrs);
#endif

#ifndef NO_CODE
//
//  	Construct an aName token for a set of string values in theValue as part of aParser, using
//  	someOptions for parsing, and aUsage as a description of the token.
//
PrimComToken_Strings::PrimComToken_Strings(PrimComParse& aParser, PrimText& theValue, const char* aName,
        const char* aUsage, const TokenOptions& someOptions)
	:
	Super(aParser, aName, aUsage, someOptions, theValue)
{}

//
//  	The default destructor does nothing.
//
PrimComToken_Strings::~PrimComToken_Strings() {}

//
//  	Report that this token does use a minus prefix.
//
bool PrimComToken_Strings::is_minus_token() const { return true; }

#ifndef NO_PRIM_STREAMS
//
//  	Print out a text string to describe the token argument as "*".
//
std::ostream& PrimComToken_Strings::print_token_arg_text(std::ostream& s) const {
	return s << "*";
}
#endif
#endif
