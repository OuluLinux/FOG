
#ifndef PRIMENUM_HXX
#define PRIMENUM_HXX


//  
//  	A PrimEnum provides support for interpretation of a set of enum values as text.
//  .note tutorial
//  	@file(PrimEnum.tutorial)
//  
class PrimEnum
{
private:
 const char *const _name;    //   Name of this set of enums.
 const char *const *const _texts;  //   Array of permitted text values.
 size_t _max_index;      //   Dimension of texts array.

public:
//  
//  	Define the characteristics of the enum descriptor. enumName should be the class name, used as
//  	the name of all illegal enum indexes. someTexts and arraySize define an array of text string
//  	equivalents for enum indexes. The index into someTexts should be the integer value of the
//  	corresponding enumeration value.
//  
 PrimEnum(const char *enumName, const char *const *someTexts, size_t arraySize)
  : _name(enumName), _texts(someTexts), _max_index(arraySize) {}
//  
//  	Report whether a potential index would be valid.
//  
 int is_valid(size_t anIndex) const { return anIndex < _max_index; }
 const char *operator[](size_t anIndex) const;
 size_t operator[](const char *someText) const;
#ifndef NO_PRIM_STREAMS
 size_t operator[](std::istream& s) const;
#endif
};
#endif
 
