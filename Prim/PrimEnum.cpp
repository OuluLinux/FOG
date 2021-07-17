#include <Prim/PrimIncludeAll.h>








//
//  	Report the textual interpretation of a given enum value. Generates an error message and
//  	returns the enum class name if invalid.
//
const char* PrimEnum::operator[](size_t anIndex) const {
	if (anIndex < _max_index)
		return (_texts[anIndex]);
		
	PRIMERR(BAD_ENUM, "cannot operator[" << anIndex << "] an " << _name);
	return _name;
}

//
//  	Return the index of the enumeration that matches someText in a case independent fashion.
//  	Any spaces, tabs, non-printables etc must match exactly. If no case matches 0 is returned
//  	without any error diagnostic.
//
size_t PrimEnum::operator[](const char* someText) const {
	if (someText != 0)
		for (size_t i = 0; i < _max_index; i++)
			if (strcasecmp(someText, _texts[i]) == 0)
				return i;
				
	return 0;
}

#ifndef NO_PRIM_STREAMS
//
//  	Extract the longest (up to 1024 characters) sequence of alphanumeric or underscore
//  	characters from the input stream, and return the index of the case independent match
//  	against the array of permitted values. The character (or end of file) following the sequence
//  	is read and restored to the input stream. If no match is
//  	found zero is returned and the input stream is set failed. If the stream goes bad while
//  	building the sequence, 0 is returned and the stream retains its badness.
//
size_t PrimEnum::operator[](std::istream& s) const {
	if ((s.flags() & std::ios::skipws) != 0)
		s >> std::ws;
		
	char aBuf[1025];
	size_t i;
	
	for (i = 0; s && (i < sizeof(aBuf)); i++) {
		if (s.peek() == EOF)
			break;
			
		s.get(aBuf[i]);
		
		if (!s)
			return (0);
			
		if (!isalnum(aBuf[i]) && (aBuf[i] != '_')) {
			s.putback(aBuf[i]);
			break;
		}
	}
	
	if ((0 < i) && (i < sizeof(aBuf))) {
		aBuf[i] = EOS;
		
		for (i = 0; i < _max_index; i++)
			if (strcasecmp(aBuf, _texts[i]) == 0)
				return (i);
	}
	
	s.clear(std::ios::failbit | s.rdstate());
	return 0;
}
#endif
