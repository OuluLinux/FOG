
#ifndef PRIMCOMTOKEN_STRINGS_HXX
#define PRIMCOMTOKEN_STRINGS_HXX


//  
//  	A PrimComToken_Strings implements a parser for multiple string Unix command line token.
//  
class PrimComToken_Strings : public PrimComTokenStrs
{
 TYPEDECL_SINGLE(PrimComToken_Strings, PrimComTokenStrs)
 typedef PrimComTokenStrs Super;
public:
 PrimComToken_Strings(PrimComParse& aParser, PrimText& theValue, const char *aName,
  const char *aUsage, const TokenOptions& someOptions = default_options());
 virtual ~PrimComToken_Strings();
 virtual bool is_minus_token() const;
#ifndef NO_PRIM_STREAMS
 virtual std::ostream& print_token_arg_text(std::ostream& s) const;
#endif
};
#endif
 
