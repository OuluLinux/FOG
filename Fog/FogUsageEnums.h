
#ifndef FOGUSAGEENUMS_HXX
#define FOGUSAGEENUMS_HXX



class FogUsageEnums {

public:
	enum TypeEnum {      //   Ordering reflects preferred emission order.
		INTERFACE_FINISH,      //   Should be next
		NAME,
		FRIEND,
		PUBLIC_TYPEDEF,
		PROTECTED_TYPEDEF,
		PRIVATE_TYPEDEF,
		PUBLIC_ENUM,
		PROTECTED_ENUM,
		PRIVATE_ENUM,
		PUBLIC_VARIABLE,
		PROTECTED_VARIABLE,
		PRIVATE_VARIABLE,
		PUBLIC_FUNCTION,
		PROTECTED_FUNCTION,
		PRIVATE_FUNCTION,
		STATIC_PUBLIC_VARIABLE,
		STATIC_PROTECTED_VARIABLE,
		STATIC_PRIVATE_VARIABLE,
		STATIC_PUBLIC_FUNCTION,
		STATIC_PROTECTED_FUNCTION,
		STATIC_PRIVATE_FUNCTION,
		INTERFACE_START,      //   Must be last interface
		FILE,
		INLINE,
		IMPLEMENTATION,       //   Last to get all interfaces done first
		INDEXES, INVALID = INDEXES    //   Needs array[INDEXES] to do values.
	};
	
public:
	static const PrimEnum& values();
	
	friend std::ostream& operator<<(std::ostream& s, const TypeEnum& aUsage);
	friend std::istream& operator>>(std::istream& s, TypeEnum& aUsage);
};

#endif

