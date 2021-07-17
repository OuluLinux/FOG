
#ifndef PRIMCOMTOKEN_STRING_HXX
#define PRIMCOMTOKEN_STRING_HXX


//
//  	A PrimComToken_String implements a parser for a string Unix command line token.
//
class PrimComToken_String : public PrimComTokenStr {
	TYPEDECL_SINGLE(PrimComToken_String, PrimComTokenStr)
	typedef PrimComTokenStr Super;
public:
	PrimComToken_String(PrimComParse& aParser, PrimStringHandle& theValue, const char* aName, const char* aUsage,
	                    const TokenOptions& someOptions = default_options());
	PrimComToken_String(PrimComParse& aParser, PrimStringHandle& theValue, const char* aName, const char* aUsage,
	                    const PrimString& defaultValue, const TokenOptions& someOptions = default_options());
	PrimComToken_String(PrimComParse& aParser, PrimStringHandle& theValue, const char* aName, const char* aUsage,
	                    const char* defaultValue, const TokenOptions& someOptions = default_options());
	virtual ~PrimComToken_String();
	virtual bool is_minus_token() const;
#ifndef NO_PRIM_STREAMS
	virtual std::ostream& print_token_arg_text(std::ostream& s) const;
#endif
};
#endif

