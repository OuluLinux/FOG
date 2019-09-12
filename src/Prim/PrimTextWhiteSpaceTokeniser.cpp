#include <Prim/PrimIncludeAll.h>




#ifndef NO_DATA
TYPEINFO_SINGLE(PrimTextWhiteSpaceTokeniser, PrimTextTokeniser);
#endif

#ifndef NO_CODE
//  
//  	Create an array of text strings by breaking the inherited stream at white space.
//  
PrimText PrimTextWhiteSpaceTokeniser::tokenise()
{
 PrimText tokens;
 PrimStringHandle string;
 while ((s() >> string).good())
  tokens.append(*string);
 return tokens;
}
#endif
