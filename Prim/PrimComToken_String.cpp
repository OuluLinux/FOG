#include <Prim/PrimIncludeAll.h>




#ifndef NO_DATA
TYPEINFO_SINGLE(PrimComToken_String, PrimComTokenStr);
#endif

#ifndef NO_CODE
//  
//  	Construct an aName token for a string value in theValue as part of aParser, using
//  	someOptions for parsing, and aUsage as a description of the token.
//  
PrimComToken_String::PrimComToken_String(PrimComParse& aParser, PrimStringHandle& theValue, const char *aName,
  const char *aUsage, const TokenOptions& someOptions)
:
 Super(aParser, aName, aUsage, someOptions, theValue)
{}

//  
//  	Construct an aName token for a string value defaulting to defaultValue in theValue
//  	as part of aParser, using someOptions for parsing, and aUsage as a description of the token.
//  
PrimComToken_String::PrimComToken_String(PrimComParse& aParser, PrimStringHandle& theValue, const char *aName,
  const char *aUsage, const PrimString& defaultValue, const TokenOptions& someOptions)
:
 Super(aParser, aName, aUsage, someOptions, theValue, defaultValue)
{}

//  
//  	Construct an aName token for a string value defaulting to defaultValue in theValue
//  	as part of aParser, using someOptions for parsing, and aUsage as a description of the token.
//  
PrimComToken_String::PrimComToken_String(PrimComParse& aParser, PrimStringHandle& theValue, const char *aName,
  const char *aUsage, const char *defaultValue, const TokenOptions& someOptions)
:
 Super(aParser, aName, aUsage, someOptions, theValue, *PrimStringHandle(defaultValue))
{}

//  
//  	The default destructor does nothing.
//  
PrimComToken_String::~PrimComToken_String() {}

//  
//  	Report that this token does use a minus prefix.
//  
bool PrimComToken_String::is_minus_token() const { return true; }

#ifndef NO_PRIM_STREAMS
//  
//  	Print out a text string to describe the token argument as "*".
//  
std::ostream& PrimComToken_String::print_token_arg_text(std::ostream& s) const
{
 return s << "*";
}
#endif
#endif
