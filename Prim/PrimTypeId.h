
#ifndef PRIMTYPEID_HXX
#define PRIMTYPEID_HXX



#ifndef NO_PRIM_TYPEINFO

//
//  		A PrimTypeId class encapsulates the pointer for a PrimTypeInfo. Its existence as a distinct
//  		type allows PrimTypeId instances to be created for both built-in and class types using
//  		the same syntax.
//
//  		A PrimTypeId can be created for any variable by:
//
//  		const PrimTypeId& aTypeId = PrimTypeId(aVariable);
//
//  		In the case of a built-in type, the creation of one of a fixed set of static types
//  		is managed by the appropriate PrimTypeId constructor. For class types the creation
//  		must be provided by a user defined conversion in the class.
//
class PrimTypeId {
private:
	const PrimTypeInfo& _type_info;      //   The type information.
	static const PrimTypeInfo _char_type_info;   //   Type info for a char.
	static const PrimTypeInfo _short_type_info;   //   Type info for a short.
	static const PrimTypeInfo _int_type_info;   //   Type info for an int.
	static const PrimTypeInfo _long_type_info;   //   Type info for a long.
	static const PrimTypeInfo _uchar_type_info;   //   Type info for an unsigned char.
	static const PrimTypeInfo _ushort_type_info;  //   Type info for an unsigned short.
	static const PrimTypeInfo _uint_type_info;   //   Type info for an unsigned int.
	static const PrimTypeInfo _ulong_type_info;   //   Type info for an unsigned long.
	static const PrimTypeInfo _float_type_info;   //   Type info for a float.
	static const PrimTypeInfo _double_type_info;  //   Type info for a double.
	static const PrimTypeInfo _void_star_type_info;  //   Type info for a void *.
	static const PrimTypeInfo _char_star_type_info;  //   Type info for a char *.
	
public:
	//
	//  		Create a PrimTypeId from aPrimPrint.
	//
	PrimTypeId(const PrimPrint& aPrimPrint) : _type_info(aPrimPrint.dynamic_type_info()) {}
	//
	//  		Create a PrimTypeId from another PrimTypeId.
	//
	PrimTypeId(const PrimTypeId& aTypeId) : _type_info(aTypeId._type_info) {}
	//
	//  		Create a PrimTypeId from the specified PrimTypeInfo.
	//
	PrimTypeId(const PrimTypeInfo& aTypeInfo) : _type_info(aTypeInfo) {}
	//
	//  		Create a PrimTypeId for a char.
	//
	PrimTypeId(const char aChar) : _type_info(_char_type_info) { UNUSED(aChar); }
	//
	//  		Create a PrimTypeId for a short.
	//
	PrimTypeId(const short aShort) : _type_info(_short_type_info) { UNUSED(aShort); }
	//
	//  		Create a PrimTypeId for an int.
	//
	PrimTypeId(const int anInt) : _type_info(_int_type_info) { UNUSED(anInt); }
	//
	//  		Create a PrimTypeId for a long.
	//
	PrimTypeId(const long aLong) : _type_info(_long_type_info) { UNUSED(aLong); }
	//
	//  		Create a PrimTypeId for an unsigned char.
	//
	PrimTypeId(const unsigned char anUChar) : _type_info(_uchar_type_info) { UNUSED(anUChar); }
	//
	//  		Create a PrimTypeId for an unsigned short.
	//
	PrimTypeId(const unsigned short anUShort) : _type_info(_ushort_type_info) { UNUSED(anUShort); }
	//
	//  		Create a PrimTypeId for an unsigned int.
	//
	PrimTypeId(const unsigned int anUInt) : _type_info(_uint_type_info) { UNUSED(anUInt); }
	//
	//  		Create a PrimTypeId for an unsigned long.
	//
	PrimTypeId(const unsigned long anULong) : _type_info(_ulong_type_info) { UNUSED(anULong); }
	//
	//  		Create a PrimTypeId for a float.
	//
	PrimTypeId(const float aFloat) : _type_info(_float_type_info) { UNUSED(aFloat); }
	//
	//  		Create a PrimTypeId for a double.
	//
	PrimTypeId(const double aDouble) : _type_info(_double_type_info) { UNUSED(aDouble); }
	//
	//  		Create a PrimTypeId for a void *.
	//
	PrimTypeId(const void* aPointer) : _type_info(_void_star_type_info) { UNUSED(aPointer); }
	//
	//  		Create a PrimTypeId for a char *.
	//
	PrimTypeId(const char* aPointer) : _type_info(_char_star_type_info) { UNUSED(aPointer); }
	//
	//  		Indirection on a PrimTypeId yields an operation on the PrimTypeInfo.
	//
	const PrimTypeInfo* operator->() const { return (&_type_info); }
	//
	//  		Report the class name of the described type.
	//
	const char* class_name() const { return (_type_info.name()); }
	//
	//  		Report the encapsulated type information.
	//
	const PrimTypeInfo& type_info() const { return (_type_info); }
};
#endif
#endif

