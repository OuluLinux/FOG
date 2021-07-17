#include <Prim/PrimIncludeAll.h>






PRIMREFTOCONST_IMPL(PrimString)

char PrimString::_null[sizeof(PrimString)] = { 0 };         //   The null string.
const PrimString* PrimStringHandle::_null = (const PrimString*)PrimString::_null;  //   The null handle.

//
//  	Construct a new representation for aBuffer[aLength].
//
inline PrimString::PrimString(const char* aBuffer, size_t aLength)
	:
	_text(0) {
	if (!aBuffer)
		ERRMSG("BUG - should not construct a null PrimString.");
		
	char* mutableText = (char*)&_text;
	memcpy(mutableText, aBuffer, aLength);
	mutableText[aLength] = 0;
}

//
//  	Unshare the string representation.
//
void PrimString::annul() const {
	if (!_shares.annul_inline() && !is_null()) {
		PrimString* mutableThis = (PrimString*)this;
		mutableThis->PrimString::~PrimString();
		delete[](char*)mutableThis;
	}
}

//
//  	Compare this string with aStringHandle for sort comparison purposes, returns -1
//  	if "this" < aString, 0 if == aString, +1 if "this" > aString. 0 text sorts after non-0 text.
//
int PrimString::compare(const PrimString& aString) const {
	if (this == &aString)      //   Catch the trivial case and the both null case
		return 0;
	else if (is_null())
		return 1;
	else if (aString.is_null())
		return -1;
	else
		return strcmp(str(), aString.str());
}

//
//  	Construct a string representation for some null terminated text. In the event of
//  	an allocation failure the null representation is returned.
//
const PrimString* PrimString::create(const char* someText) {
	if (!someText) {
		null().share();
		return &null();
	}
	
	size_t aLength = strlen(someText);
	char* aString = new char[sizeof(PrimString) + aLength];
	
	if (!aString) {
		GLOBAL_ERRMSG("PrimString failed to allocate memory for a " << aLength << " byte PrimString.");
		null().share();
		return &null();
	}
	
	return new (aString) PrimString(someText, aLength);
}

//
//  	Construct a string representation for some optionally null terminated text. In the event of
//  	an allocation failure the null representation is returned. An EOS is appended following the text.
//
const PrimString* PrimString::create(const char* aBuffer, size_t aLength) {
	if (!aBuffer) {
		null().share();
		return &null();
	}
	
	char* aString = new char[sizeof(PrimString) + aLength];
	
	if (!aString) {
		GLOBAL_ERRMSG("PrimString failed to allocate memory for a " << aLength << " byte PrimString.");
		null().share();
		return &null();
	}
	
	return new (aString) PrimString(aBuffer, aLength);
}

#if 0
//
//  	Construct a string representation for the null terminated concatentation of firstText and secondText.
//  	If either text is 0 the contribution is ignored. If both texts are 0 or in the
//  	event of a memory allocation failure, a representation of the null string is created.
//
const PrimStringHandleRep& PrimStringHandle::construct(const char* firstText, const char* secondText) {
	const unsigned int firstLength = firstText ? strlen(firstText) : 0;
	const unsigned int secondLength = secondText ? strlen(secondText) : 0;
	const unsigned int netLength = firstLength + secondLength;
	
	if (netLength == 0)
		return PrimStringHandleNullRep::_null_rep;
		
	char* myText = new char[netLength + 1];
	
	if (!myText)
		return PrimStringHandleNullRep::_null_rep;
		
	if (firstLength)
		strcpy(myText, firstText);
		
	if (secondLength)
		strcpy(myText + firstLength, secondText);
		
	const PrimStringHandleRep* aRep = new PrimStringHandleRep(myText);
	
	if (aRep)
		return *aRep;
		
	delete[] myText;
	return PrimStringHandleNullRep::_null_rep;
}

#endif

//
//  	Convert the string representation into a hash code suitable for use in a dictionary.
//
unsigned long PrimString::hash() const {
	unsigned long hashCode = 0;
	
	for (const char* p = &_text; *p != EOS; p++)
		hashCode = (hashCode << 5) ^ hashCode ^ *p;
		
	return hashCode;
}

//
//  	Compare the representation of this string to aStringHandle. Returns true if the strings exactly match
//  	or are both the 0 string. The 0 string does not equal the empty string.
//
bool PrimString::is_equal(const PrimString& aString) const {
	if (this == &aString)      //   Catch the trivial case and the both null case
		return true;
		
	if (is_null())        //   Avoid unnecessary strcmp
		return false;
		
	return strcmp(str(), aString.str()) == 0;
}

//
//  	Compare the representation of this string to someText. someText may be 0 in which
//  	case it compares equal to the representation of the null string, but not the empty string.
//
bool PrimString::is_equal(const char* someText) const {
	if (is_null())
		return someText == 0;
		
	if (someText == 0)
		return false;
		
	return strcmp(str(), someText) == 0;
}

#if 0
#ifndef NO_PRIM_STREAMS
//
//  	Append the class name to the supplied output stream.
//
//  	Returns the updated stream.
//
std::ostream& PrimStringHandle::print_class_name(std::ostream& s) const {
	return s << class_name();
}

//
//  	Append the string to the supplied output stream applying a class name prefix as for PrimPrint.
//
//  	Returns the updated stream.
//
std::ostream& PrimStringHandle::print_instance(std::ostream& s) const {
	s << class_name();
	PrimPrint::print_address_of(s, this);
	s << "(\"" << _rep->str() << "\")";
	return s;
}

#endif
#endif

//
//  	Return an ordering between s1 and s2, with a calling signature compatible with PrimText::sort().
//
int PrimString::sort_compare(const PrimStringHandle* s1, const PrimStringHandle* s2) {
	return (*s1)->compare(**s2);
}

#ifndef NO_PRIM_STREAMS
//
//  	Append the string to the supplied output stream.
//
//  	Returns the updated stream.
//
std::ostream& operator<<(std::ostream& s, const PrimString& aString) {
	return s << aString.str();
}

//
//  	Read a word (1024 characters maximum) from the input stream and store it in aStringHandle.
//
//  	Returns the updated stream.
//
std::istream& operator>>(std::istream& s, PrimStringHandle& aStringHandle) {
	char aWord[1025];
	s >> aWord;
	aStringHandle = (s ? aWord : 0);
	return s;
}

#endif

#ifndef NO_DBX_SUPPORT
//
//  	Debug routine for use with dbxtool.
//
const char* dbx_string(const PrimString* aString) {
	if (!aString)
		return "(nil)";
	else
		return aString->str();
}
#endif
