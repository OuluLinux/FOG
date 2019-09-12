
#ifndef PRIMCOMTOKEN_DOUBLE_HXX
#define PRIMCOMTOKEN_DOUBLE_HXX


//  
//  	A PrimComToken_Double implements a parser for a floating point Unix command line token.
//  
class PrimComToken_Double : public PrimComToken
{
 TYPEDECL_SINGLE(PrimComToken_Double, PrimComToken)
 typedef PrimComToken Super;
private:
 const double _default_value;     //   Default value for token.
 double& _value;         //   Parsed value for token.
/*  */
//  	Construction of parser's for float values is expressly prohibited to generate a compiler
//  	diagnostic to catch a very simple but otherwise hard to diagnose programming error.
//  
 PrimComToken_Double(PrimComParse& aParser, float &theValue, const char *aName,
  const char *aUsage, const TokenOptions& someOptions = default_options());
 PrimComToken_Double(PrimComParse& aParser, float &theValue, const char *aName,
  const char *aUsage, float defaultValue, const TokenOptions& someOptions = default_options());
protected:
 virtual void initialise_parse(PrimComParse& aParser);
 virtual const char *parse_text(PrimComParse& aParser, const char *someText);
public:
 PrimComToken_Double(PrimComParse& aParser, double &theValue, const char *aName,
  const char *aUsage, const TokenOptions& someOptions = default_options());
 PrimComToken_Double(PrimComParse& aParser, double &theValue, const char *aName,
  const char *aUsage, double defaultValue, const TokenOptions& someOptions = default_options());
 virtual ~PrimComToken_Double();
 virtual bool is_minus_token() const;
#ifndef NO_PRIM_STREAMS
 virtual std::ostream& print_this(std::ostream& s) const;
 virtual std::ostream& print_token_arg_text(std::ostream& s) const;
#endif
//  
//  	Report the command line value.
//  
 double value() const { return (_value); }
};
#endif
 
