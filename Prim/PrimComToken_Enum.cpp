#include <Prim/PrimIncludeAll.h>




#ifndef NO_DATA
TYPEINFO_SINGLE(PrimComToken_Enum, PrimComToken_String);
#endif

#ifndef NO_CODE
//  
//  	Construct an aName token for a string value in theValue as part of aParser, using
//  	someOptions for parsing, and aUsage as a description of the token.
//  
PrimComToken_Enum::PrimComToken_Enum(PrimComParse& aParser, const PrimEnum& anEnum, int& theValue,
 const char *aName, const char *aUsage, const TokenOptions& someOptions)
:
 Super(aParser, _buffer, aName, aUsage, someOptions),
 _enum(anEnum),
 _value(theValue),
 _default_value(0)
{}

//  
//  	Construct an aName token for a string value defaulting to defaultValue in theValue
//  	as part of aParser, using someOptions for parsing, and aUsage as a description of the token.
//  
PrimComToken_Enum::PrimComToken_Enum(PrimComParse& aParser, const PrimEnum& anEnum, int& theValue,
 const char *aName, const char *aUsage, int defaultValue, const TokenOptions& someOptions)
:
 Super(aParser, _buffer, aName, aUsage, *PrimStringHandle(anEnum[defaultValue]), someOptions),
 _enum(anEnum),
 _value(theValue),
 _default_value(defaultValue)
{}

//  
//  	The default destructor does nothing.
//  
PrimComToken_Enum::~PrimComToken_Enum() {}

//  
//  	Initialise for a new parsing pass.
//  
void PrimComToken_Enum::initialise_parse(PrimComParse& aParser)
{
 _value = _default_value;
 Super::initialise_parse(aParser);
}

//  
//  	Attempt to parse someText, returning true if accepted.
//  
const char *PrimComToken_Enum::parse_text(PrimComParse& aParser, const char *someText)
{
 const char *p = Super::parse_text(aParser, someText);
 if (p == 0)          //   If bad text
  return 0;         //  	parsing fails
 std::istrstream s((char *)_buffer.str());
 size_t aValue = _enum[s];
 if (!s && aParser.diagnose_warnings())
 {
  PRIMWRN(BAD_PARSE, *this << " failed to resolve \"" << _buffer << "\"");
  return 0;
 }
 _value = aValue;
 return p;
}
#endif
