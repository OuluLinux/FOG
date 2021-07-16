#include <Fog/FogIncludeAll.h>





//  
//  	Initialisers are in FogLexer.l, where magic int numbers for states are known.
//  

std::ostream& operator<<(std::ostream& s, const FogLexerState& aState) { return s << aState._name; }
