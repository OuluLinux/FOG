
#ifndef PRIMCOMTOKEN_LONG_HXX
#define PRIMCOMTOKEN_LONG_HXX


//
//  	A PrimComToken_Long implements a parser for an integer Unix command line token.
//
class PrimComToken_Long : public PrimComToken {
	TYPEDECL_SINGLE(PrimComToken_Long, PrimComToken)
	typedef PrimComToken Super;
private:
	const long _default_value;      //   Default value for token.
	long& _value;         //   Parsed value for token.
	const int& _number_base;      //   Parsing base for strtol().
	/*  */
	//  	Construction of parser's for int values is expressly prohibited to generate a compiler
	//  	diagnostic to catch a very simple but otherwise hard to diagnose programming error.
	//
	PrimComToken_Long(PrimComParse& aParser, int& theValue, const char* aName,
	                  const char* aUsage, const TokenOptions& someOptions = default_options());
	//
	//  	Construction of parser's for int values is expressly prohibited to generate a compiler
	//  	diagnostic to catch a very simple but otherwise hard to diagnose programming error.
	//
	PrimComToken_Long(PrimComParse& aParser, int& theValue, const char* aName,
	                  const char* aUsage, long defaultValue, const int& _numberBase = default_number_base(),
	                  const TokenOptions& someOptions = default_options());
protected:
	virtual void initialise_parse(PrimComParse& aParser);
	virtual const char* parse_text(PrimComParse& aParser, const char* someText);
	
public:
	PrimComToken_Long(PrimComParse& aParser, long& theValue, const char* aName,
	                  const char* aUsage, const TokenOptions& someOptions = default_options());
	PrimComToken_Long(PrimComParse& aParser, long& theValue, const char* aName,
	                  const char* aUsage, long defaultValue, const int& numberBase = default_number_base(),
	                  const TokenOptions& someOptions = default_options());
	virtual ~PrimComToken_Long();
	virtual bool is_minus_token() const;
#ifndef NO_PRIM_STREAMS
	virtual std::ostream& print_this(std::ostream& s) const;
	virtual std::ostream& print_token_arg_text(std::ostream& s) const;
#endif
	//
	//  	Report the command line value.
	//
	long value() const { return (_value); }
};
#endif

