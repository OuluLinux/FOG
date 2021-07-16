
#ifndef PRIMCOMTOKEN_FILES_HXX
#define PRIMCOMTOKEN_FILES_HXX


//  
//  	A PrimComToken_Files implements a parser for multiple string Unix command line file argument.
//  
class PrimComToken_Files : public PrimComTokenStrs
{
 TYPEDECL_SINGLE(PrimComToken_Files, PrimComTokenStrs)
 typedef PrimComTokenStrs Super;
public:
//  
//  	Construct an aName token for a set of files in theValue as part of aParser, using
//  	someOptions for parsing, and aUsage as a description of the token. aName is
//  	ignored for file names, but is present for consistency with other tokens.
//  
 PrimComToken_Files(PrimComParse& aParser, PrimText& theValue, const char *aName, const char *aUsage,
  const TokenOptions& someOptions = default_options())
 : Super(aParser, aName, aUsage, someOptions, theValue) {}
 virtual ~PrimComToken_Files();
 virtual bool is_file_token() const;
#ifndef NO_PRIM_STREAMS
 virtual std::ostream& print_token_name(std::ostream& s) const;
#endif
};
#endif
 
