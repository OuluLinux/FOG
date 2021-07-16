


/*   The include file guard follows the previous include to prevent an adverse ordering while PrimComToken
 * includes PrimComs.h to maintain backward compatability. */
#ifndef PRIMCOMTOKENSTR_HXX
#define PRIMCOMTOKENSTR_HXX

//  
//  	A PrimComTokenStr implements an abstract parser for a Unix command line string.
//  
class PrimComTokenStr : public PrimComToken
{
 TYPEDECL_SINGLE(PrimComTokenStr, PrimComToken)
 typedef PrimComToken Super;
private:
 const PrimStringHandle _default_value;    //   Default value for token.
 PrimStringHandle& _value;       //   Parsed value for token.

protected:
//  
//  	Construct an aName token for a string value in theValue as part of aParser, using
//  	someOptions for parsing, and aUsage as a description of the token.
//  
 PrimComTokenStr(PrimComParse& aParser, const char *aName, const char *aUsage,
  const TokenOptions& someOptions, PrimStringHandle& theValue)
  : Super(aParser, aName, aUsage, someOptions), _value(theValue) {}
//  
//  	Construct an aName token for a string value defaulting to defaultValue in theValue
//  	as part of aParser, using someOptions for parsing, and aUsage as a description of the token.
//  
 PrimComTokenStr(PrimComParse& aParser, const char *aName, const char *aUsage,
  const TokenOptions& someOptions, PrimStringHandle& theValue, const PrimString& defaultValue)
  : Super(aParser, aName, aUsage, someOptions), _default_value(defaultValue), _value(theValue) {}
 virtual ~PrimComTokenStr();
 virtual void initialise_parse(PrimComParse& aParser);
 virtual const char *parse_text(PrimComParse& aParser, const char *someText);

public:
#ifndef NO_PRIM_STREAMS
 virtual std::ostream& print_this(std::ostream& s) const;
#endif
//  
//  	Report the command line value.
//  
 const PrimString& value() const { return *_value; }
};
#endif
 
