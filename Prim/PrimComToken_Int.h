
#ifndef PRIMCOMTOKEN_INT_HXX
#define PRIMCOMTOKEN_INT_HXX


//
//  	A PrimComToken_Int implements a parser for an integer Unix command line token.
//
class PrimComToken_Int : public PrimComToken {
	TYPEDECL_SINGLE(PrimComToken_Int, PrimComToken)
	typedef PrimComToken Super;
private:
	const int _default_value;      //   Default value for token.
	int& _value;         //   Parsed value for token.
	const int& _number_base;      //   Parsing base for strtol().
	/*  */
	//  	Construction of parser's for long values is expressly prohibited to generate a compiler
	//  	diagnostic to catch a very simple but otherwise hard to diagnose programming error.
	//
	PrimComToken_Int(PrimComParse& aParser, long& theValue, const char* aName,
	                 const char* aUsage, const TokenOptions& someOptions = default_options());
	//
	//  	Construction of parser's for long values is expressly prohibited to generate a compiler
	//  	diagnostic to catch a very simple but otherwise hard to diagnose programming error.
	//
	PrimComToken_Int(PrimComParse& aParser, long& theValue, const char* aName,
	                 const char* aUsage, int defaultValue, const int& _numberBase = default_number_base(),
	                 const TokenOptions& someOptions = default_options());
	                 
protected:
	virtual void initialise_parse(PrimComParse& aParser);
	virtual const char* parse_text(PrimComParse& aParser, const char* someText);
	
public:
	PrimComToken_Int(PrimComParse& aParser, int& theValue, const char* aName,
	                 const char* aUsage, const TokenOptions& someOptions = default_options());
	PrimComToken_Int(PrimComParse& aParser, int& theValue, const char* aName,
	                 const char* aUsage, int defaultValue, const int& numberBase = default_number_base(),
	                 const TokenOptions& someOptions = default_options());
	virtual ~PrimComToken_Int();
	virtual bool is_minus_token() const;
#ifndef NO_PRIM_STREAMS
	virtual std::ostream& print_this(std::ostream& s) const;
	virtual std::ostream& print_token_arg_text(std::ostream& s) const;
#endif
	//
	//  	Report the command line value.
	//
	int value() const { return (_value); }
};
#endif

