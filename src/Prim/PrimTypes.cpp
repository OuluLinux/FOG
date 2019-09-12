#include <Prim/PrimIncludeAll.h>



#ifndef NO_PRIM_STREAMS

#endif

const char *const bool_texts[] = { "FALSE", "TRUE" };

const PrimEnum Bool::_values("Bool", bool_texts, PRIM_ARRAY_SIZE(bool_texts));

//  
//  		Create a Bool from a bool.
//  
Bool::Bool(bool aBool)
:
 _value(aBool)
{}

//  
//  		Report the value as a bool enumeration.
//  
Bool::operator bool() const { return (_value); }

//  
//  		Convert an integer value into its boolean representaion.
//  
bool to_bool(int aValue) { return (aValue != 0 ? true : false); }

#ifndef NO_PRIM_STREAMS
//  
//  		Extract a bool from the input stream returning it to theBool.
//  		The input stream is set bad if the longest sequence of alphanumeric, underscore
//  		characters does not comprise a case independent match to one of the permitted
//  		text values.
//  
std::istream& operator>>(std::istream& s, Bool& theBool)
{
 theBool = theBool._values[s] ? true : false;
 return (s);
}

//  
//  		Provide an in-line diagnostic of the value of a boolean type as true or false.
//  
std::ostream& operator<<(std::ostream& s, const Bool& aBool)
{
 s << aBool._values[aBool._value ? true : false];
 return (s);
}
#endif
