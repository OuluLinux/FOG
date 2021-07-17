
#ifndef PRIMTYPES_HXX
#define PRIMTYPES_HXX



//
//  		A Double is a double precision floating point number.
//  .item Rn -1.67e+308...1.67e+308
//  .item Dl 4.19e-307
//  .item Dg 15 digits
//
typedef double Double;

//
//  		A Float is a single precision floating point number.
//  .item Rn -3.37e+38...3.37e+38
//  .item Dl 8.43e-37
//  .item Dg 6 digits
//
typedef float Float;

//
//  		A ULong is an unsigned integer data type capable of handling large numbers.
//  .item Rn 0...4294967295
//  .item Dl 1
//  .item Dg 32 bits
//
typedef unsigned long ULong;

//
//  		A Long is a signed integer data type capable of handling large numbers.
//  .item Rn -2147483647...2147483647
//  .item Dl 1
//  .item Dg 32 bits
//
typedef /*   signed */ long Long;

//
//  		A UInt is an unsigned integer data type optimised for machine speed.
//  .item Rn 0...65535
//  .item Dl 1
//  .item Dg 16/32 bits
//
typedef unsigned int UInt;

//
//  		An Int is a signed integer data type optimised for machine speed.
//  .item Rn -32767...32767
//  .item Dl 1
//  .item Dg 16/32 bits
//
typedef /*   signed */ int Int;

//
//  		A UShort is an unsigned integer data type optimised for memory space.
//  .item Rn 0...65535
//  .item Dl 1
//  .item Dg 16 bits
//
typedef unsigned short UShort;

//
//  		A Short is a signed integer data type optimised for memory space.
//  .item Rn -32767...32767
//  .item Dl 1
//  .item Dg 16 bits
//
typedef /*   signed */ short Short;

//
//  		A UTiny is a small unsigned integer data type.
//  .item Rn 0...255
//  .item Dl 1
//  .item Dg 8 bits
//
typedef unsigned char UTiny;

//
//  		A Tiny is a small signed integer data type.
//  .item Rn -127...127
//  .item Dl 1
//  .item Dg 8 bits
//
typedef /*   signed */ char Tiny;

//
//  		A Char is a text character data type, treatment as signed or unsigned is left
//  		to the discretion of the compiler.
//  .item Rn -128...127 or 0...255
//  .item Dl 1
//  .item Dg 8 bits
//
typedef char Char;

#define BOOL_FALSE false
#define BOOL_TRUE true

class Bool;

class TBool;

bool to_bool(int aValue);    /*   Doesn't inline with cfront 2.1 */
//
//  		Convert a pointer value into its boolean representation.
//
inline bool to_bool(const void* aPointer) { return aPointer != 0 ? true : false; }

#ifndef PRIMENUM_HXX
	#include <Prim/PrimEnum.h>
#endif

//
//  		A Bool class data type supports the boolean values true and false in the
//  		most speed-efficient representation for the target architecture.
//  .item Rn true, false
//  .item Dl -
//  .item Dg 16/32 bits
//  .note limits
//  		See the prim-types category overview for a discussion of why this class should not be used.
//
class Bool {
	static const PrimEnum _values;    //   Descriptor for this Enum.
	bool _value;        //   The boolean value.
public:
	//
	//  		Construct a Bool with a default value.
	//
	Bool() : _value(false) {}
	//
	//  		Construct a Bool from aBool.
	//
	Bool(const Bool& aBool) : _value(aBool._value) {}
	//
	//  		Construct a Bool from the non-zero-ness of an integer.
	//
	/*  	Bool(int aValue) : _value(to_bool(aValue)) {} */
	//
	//  		Construct a Bool from the non-0-ness of a const void *.
	//
	Bool(const void* aPointer) : _value(to_bool(aPointer)) {}
	Bool(const TBool& aTBool);
	Bool(bool aBool);       /*   Not inlined by cfront 2.1 */
	//
	//  		Assign a boolean value to this object.
	//
	Bool& operator=(bool aValue) { _value = aValue; return (*this); }
	//
	//  		Construct and return a Bool containing the complement of this value.
	//
	Bool operator!() const { return _value ? false : true; }
	/*  		This method is not provided because it is semantically different to int && int;
			int a = 0, b; if (a && b) does not evaluate b
			Bool a = 0, b; if (a && b) would evaluate b if b is non-trivial
		Bool operator&&(const Bool& aBool) const { return (to_bool(_value && aBool._value)); } */
	/*  		This method is not provided because it is semantically different to int && int;
			int a = 1, b; if (a || b) does not evaluate b
			Bool a = 1, b; if (a || b) would evaluate b if b is non-trivial
		Bool operator||(const Bool& aBool) const { return (to_bool(_value || aBool._value)); } */
	//
	//  		Report whether this value is identical to aValue.
	//
	Bool operator==(bool aValue) const { return (_value == aValue); }
	//
	//  		Report whether this value is identical to aBool.
	//
	Bool operator==(const Bool& aBool) const { return (_value == aBool._value); }
	//
	//  		Report whether this value is different to aValue.
	//
	Bool operator!=(bool aValue) const { return (_value != aValue); }
	//
	//  		Report whether this value is different to aBool.
	//
	Bool operator!=(const Bool& aBool) const { return (_value != aBool._value); }
	operator bool() const;      /*   Doesn't inline with cfront 2.1 */
#ifndef NO_PRIM_STREAMS
	friend std::ostream& operator<<(std::ostream& s, const Bool& aBool);
	friend std::istream& operator>>(std::istream& s, Bool& aBool);
#endif
};

//
//  		A TBool is a data type supporting the boolean values true and false in the
//  		most space-efficient representation for the target architecture.
//  .item Rn true, false
//  .item Dl -
//  .item Dg 8 bits
//  .note limits
//  		See the prim-types category overview for a discussion of why this class should not be used.
//
class TBool {
	friend class Bool;
	char _value;       //   The boolean value.
public:
	//
	//  		Construct a TBool with a default value.
	//
	TBool() : _value(false) {}
	//
	//  		Construct a TBool from a TBool.
	//
	TBool(const TBool& aValue) : _value(aValue._value) {}
	//
	//  		Construct a TBool from a bool.
	//
	TBool(bool aValue) : _value(aValue) {}
	//
	//  		Assign a boolean value to this object.
	//
	TBool& operator=(bool aValue) { _value = aValue; return (*this); }
	//
	//  		Report the value as a bool enumeration.
	//
	operator bool() const { return _value ? true : false; }
#ifndef NO_PRIM_STREAMS
	friend std::ostream& operator<<(std::ostream& s, const TBool& aTBool);
	friend std::istream& operator>>(std::istream& s, TBool& aBool);
#endif
};

//
//  		Construct a Bool from a TBool.
//
inline Bool::Bool(const TBool& aTBool) : _value(to_bool(aTBool._value)) {}

#endif

