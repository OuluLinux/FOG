
#ifndef PRIMCOMTOKEN_FILE_HXX
#define PRIMCOMTOKEN_FILE_HXX


//  
//  	A PrimComToken_File implements a parser for a Unix command line file.
//  
class PrimComToken_File : public PrimComTokenStr
{
 TYPEDECL_SINGLE(PrimComToken_File, PrimComTokenStr)
 typedef PrimComTokenStr Super;
public:
 PrimComToken_File(PrimComParse& aParser, PrimStringHandle& theValue, const char *aName, const char *aUsage,
  const TokenOptions& someOptions = default_options());
 PrimComToken_File(PrimComParse& aParser, PrimStringHandle& theValue, const char *aName, const char *aUsage,
  const PrimString& defaultValue, const TokenOptions& someOptions = default_options());
 virtual ~PrimComToken_File();
 virtual bool is_file_token() const;
#ifndef NO_PRIM_STREAMS
 virtual std::ostream& print_token_name(std::ostream& s) const;
#endif
};
#endif
 
