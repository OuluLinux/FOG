
#ifndef PRIMCOMTOKEN_BOOL_HXX
#define PRIMCOMTOKEN_BOOL_HXX


//
//  	A PrimComToken_Bool implements a parser for a boolean Unix command line token.
//
class PrimComToken_Bool : public PrimComToken {
	TYPEDECL_SINGLE(PrimComToken_Bool, PrimComToken)
	typedef PrimComToken Super;
private:
	const bool _default_value;     //   Default value for token.
	bool& _value;        //   Parsed value for token.
protected:
	virtual void initialise_parse(PrimComParse& aParser);
	virtual const char* parse_text(PrimComParse& aParser, const char* someText);
public:
	PrimComToken_Bool(PrimComParse& aParser, bool& theValue, const char* aName,
	                  const char* aUsage, const TokenOptions& someOptions = default_options());
	PrimComToken_Bool(PrimComParse& aParser, bool& theValue, const char* aName,
	                  const char* aUsage, const bool& defaultValue, const TokenOptions& someOptions = default_options());
	virtual ~PrimComToken_Bool();
	virtual bool is_minus_token() const;
#ifndef NO_PRIM_STREAMS
	virtual std::ostream& print_this(std::ostream& s) const;
#endif
	//
	//  	Report the command line value.
	//
	const bool& value() const { return _value; }
};
#endif

