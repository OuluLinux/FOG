#include <Prim/PrimIncludeAll.h>




#ifndef NO_DATA
	TYPEINFO_SINGLE(PrimComToken_File, PrimComTokenStr);
#endif

#ifndef NO_CODE
//
//  	Construct an aName token for a string value in theValue as part of aParser, using
//  	someOptions for parsing, and aUsage as a description of the token. aName is
//  	ignored for file names, but is present for consistency with other tokens.
//
PrimComToken_File::PrimComToken_File(PrimComParse& aParser, PrimStringHandle& theValue, const char* aName,
                                     const char* aUsage, const TokenOptions& someOptions)
	:
	Super(aParser, aName, aUsage, someOptions, theValue)
{}

//
//  	Construct an aName token for a string value defaulting to defaultValue in theValue
//  	as part of aParser, using someOptions for parsing, and aUsage as a description of the token. aName is
//  	ignored for file names, but is present for consistency with other tokens.
//
PrimComToken_File::PrimComToken_File(PrimComParse& aParser, PrimStringHandle& theValue, const char* aName,
                                     const char* aUsage, const PrimString& defaultValue, const TokenOptions& someOptions)
	:
	Super(aParser, aName, aUsage, someOptions, theValue, defaultValue)
{}

//
//  	The default destructor does nothing.
//
PrimComToken_File::~PrimComToken_File() {}

//
//  	Report that this token parses file names.
//
bool PrimComToken_File::is_file_token() const { return true; }

#ifndef NO_PRIM_STREAMS
//
//  	Print out the name of this command line token which is <file>.
//
std::ostream& PrimComToken_File::print_token_name(std::ostream& s) const {
	s << "<file>";
	return s;
}
#endif
#endif
