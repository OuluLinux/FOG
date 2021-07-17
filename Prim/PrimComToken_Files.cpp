#include <Prim/PrimIncludeAll.h>




#ifndef NO_DATA
	TYPEINFO_SINGLE(PrimComToken_Files, PrimComTokenStrs);
#endif

#ifndef NO_CODE
//
//  	The default destructor does nothing.
//
PrimComToken_Files::~PrimComToken_Files() {}

//
//  	Report that this token parses file names.
//
bool PrimComToken_Files::is_file_token() const { return true; }

#ifndef NO_PRIM_STREAMS
//
//  	Print out the name of this command line token which is <file>.
//
std::ostream& PrimComToken_Files::print_token_name(std::ostream& s) const {
	s << "<file>";
	return s;
}
#endif
#endif
