#include <Prim/PrimIncludeAll.h>




#ifndef NO_DATA
	TYPEINFO_SINGLE(PrimTextDoubleQuoteTokeniser, PrimTextTokeniser);
#endif

#ifndef NO_CODE


//
//  	Create an array of text strings by breaking the inherited stream at white space provided the white space
//  	is not enclosed in double quotes.
//
PrimText PrimTextDoubleQuoteTokeniser::tokenise() {
	PrimText tokens;
	bool in_quotes = false;
	char c;
	std::strstream* token_buf = 0;
	
	while (s().get(c).good() && c) {
		if (c == '\"') {
			in_quotes = !in_quotes;
			
			if (!token_buf)
				token_buf = new std::strstream;
		}
		else if (in_quotes || !isspace(c)) {
			if (!token_buf)
				token_buf = new std::strstream;
				
			*token_buf << c;
		}
		else if (token_buf) {
			*token_buf << std::ends;
			char* p1 = token_buf->str();
			tokens.append(p1);
			delete[] p1;
			delete token_buf;
			token_buf = 0;
		}
	}
	
	if (token_buf) {
		*token_buf << std::ends;
		char* p2 = token_buf->str();
		tokens.append(p2);
		delete[] p2;
		delete token_buf;
	}
	
	return (tokens);
}
#endif
