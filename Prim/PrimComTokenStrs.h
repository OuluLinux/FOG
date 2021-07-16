
#ifndef PRIMCOMTOKENSTRS_HXX
#define PRIMCOMTOKENSTRS_HXX


//  
//  	A PrimComTokenStrs implements an abstract parser for multiple strings from a Unix command line.
//  
class PrimComTokenStrs : public PrimComToken
{
 TYPEDECL_SINGLE(PrimComTokenStrs, PrimComToken)
 typedef PrimComToken Super;
private:
 PrimText& _value;         //   Parsed value for token.

protected:
//  
//  	Construct an aName token for a set of strings in theValue as part of aParser, using
//  	someOptions for parsing, and aUsage as a description of the token.
//  
 PrimComTokenStrs(PrimComParse& aParser, const char *aName, const char *aUsage,
  const TokenOptions& someOptions, PrimText& theValue)
  : Super(aParser, aName, aUsage, someOptions), _value(theValue) {}
 virtual ~PrimComTokenStrs();
 virtual void initialise_parse(PrimComParse& aParser);
 virtual const char *parse_text(PrimComParse& aParser, const char *someText);

public:
 virtual bool is_multiple_token() const;
#ifndef NO_PRIM_STREAMS
 virtual std::ostream& print_this(std::ostream& s) const;
#endif
//  
//  	Report the command line values.
//  
 const PrimText& value() const { return _value; }
};
#endif
 
