#include <Prim/PrimIncludeAll.h>




#ifndef NO_DATA
TYPEINFO_SINGLE(PrimComToken_Bool, PrimComToken);
#endif

#ifndef NO_CODE
//  
//  	Construct an aName token for a boolean value in theValue as part of aParser, using
//  	someOptions for parsing, and aUsage as a description of the token.
//  
PrimComToken_Bool::PrimComToken_Bool(PrimComParse& aParser, bool &theValue, const char *aName,
 const char *aUsage, const TokenOptions& someOptions)
:
 Super(aParser, aName, aUsage, someOptions),
 _default_value(false),
 _value(theValue)
{}

//  
//  	Construct an aName token for a boolean value defaulting to defaultValue in theValue
//  	as part of aParser, using someOptions for parsing, and aUsage as a description of the token.
//  
PrimComToken_Bool::PrimComToken_Bool(PrimComParse& aParser, bool &theValue, const char *aName,
 const char *aUsage, const bool& defaultValue, const TokenOptions& someOptions)
:
 Super(aParser, aName, aUsage, someOptions),
 _default_value(defaultValue),
 _value(theValue)
{}

//  
//  	The default destructor does nothing.
//  
PrimComToken_Bool::~PrimComToken_Bool() {}

//  
//  	Initialise for a new parsing pass.
//  
void PrimComToken_Bool::initialise_parse(PrimComParse& aParser)
{
 _value = _default_value;
 Super::initialise_parse(aParser);
}

//  
//  	Report that this token does use a minus prefix.
//  
bool PrimComToken_Bool::is_minus_token() const { return true; }

//  
//  	Attempt to parse someText, returning non-null if accepted.
//  
const char *PrimComToken_Bool::parse_text(PrimComParse& aParser, const char *someText)
{
 const char *const p = Super::parse_text(aParser, someText);
 if (!p || (*p != EOS))
  return 0;
 _value = true;
 set_is_present();
 return p;
}

#ifndef NO_PRIM_STREAMS
//  
//  	Add an indication of token value to an in-line print-out.
//  
std::ostream& PrimComToken_Bool::print_this(std::ostream& s) const
{
 Super::print_this(s);
 s << ", " << (_value ? "true" : "false");
 return s;
}
#endif
#endif
