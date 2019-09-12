
#ifndef PRIMPRINT_HXX
#define PRIMPRINT_HXX



#ifndef NO_PRIM_TYPEINFO
//
//  	A PrimTypeInfo_Base class identifies the properties of a single base, providing a pointer
//  	to the base type information, and an offset from the start of this object to this base.
//

struct PrimTypeInfo_Base {

public:
	const PrimTypeInfo *type;   //   Pointer to the type information of this base, 0 at end.
	long offset;       //   Offset to base from start of object.
};

#define TYPEINFO_DECLARATION(T) \
public: \
virtual const PrimTypeInfo& dynamic_type_info() const; \
static const PrimTypeInfo& static_type_info(); \
private:
#define TYPEDECL_MULTIPLE(B) public: \
__C__((Return a reference to the first base class of this object.))\
inline const PrimPrint& report_this_inline() const { return (((const B *)this)->report_this_inline()); }\
virtual PrimPrint *report_this();

#define TYPEDECL_ROOT(T) TYPEINFO_DECLARATION(T) private:
#define TYPEDECL_SINGLE(T, B) TYPEINFO_DECLARATION(T) private:
#define TYPEDECL_DOUBLE(T, B1, B2) TYPEINFO_DECLARATION(T) TYPEDECL_MULTIPLE(B1) private:
#define TYPEDECL_TRIPLE(T, B1, B2, B3) TYPEINFO_DECLARATION(T) TYPEDECL_MULTIPLE(B1) private:
#define TYPEDECL_QUADRUPLE(T, B1, B2, B3, B4) TYPEINFO_DECLARATION(T) TYPEDECL_MULTIPLE(B1) private:
#define TYPEDECL_QUINTUPLE(T, B1, B2, B3, B4, B5) TYPEINFO_DECLARATION(T) TYPEDECL_MULTIPLE(B1) private:
#define TYPEDECL_SEXTUPLE(T, B1, B2, B3, B4, B5, B6) TYPEINFO_DECLARATION(T) TYPEDECL_MULTIPLE(B1) private:

#define TYPEINFO_IMPL_BEGIN(T) \
__C__((Report the type information describing this object. This is a virtual method and so))\
__C__((may identify the type of a derived class.))\
const PrimTypeInfo& T::dynamic_type_info() const { return static_type_info(); } \
__C__((Report the run-time type information. This is a static method and so always identifies))\
__C__((the type of this class as T.))\
const PrimTypeInfo& T::static_type_info() {
/*
 *		The __STDC__ conditionalisation ensures that the stringising functionality
 *		is provided correctly whether a K&R or ANSI C preprocessor is used.
 */
#if defined(__STDC__) || defined(CENTERLINE_CLPP) || defined(_MSC_VER)
#define TYPEINFO_IMPL_END(T, BASES) \
 static const PrimTypeInfo typeInfo(#T, BASES); return typeInfo; }
#else
#define TYPEINFO_IMPL_END(T, BASES) \
 static const PrimTypeInfo typeInfo("T", BASES); return typeInfo; }
#endif
#define ANY_NON_NULL_ADDRESS (char *)0x1000
#define TYPEINFO_BASE_OF(baseT, parentT) \
 { &baseT::static_type_info(), (char *)&(baseT&)(parentT&)*ANY_NON_NULL_ADDRESS - ANY_NON_NULL_ADDRESS }
#define TYPEINFO_MULTIPLE(T) \
__C__((Return a pointer to the first base class of this object.))\
PrimPrint *T::report_this() { return ((PrimPrint *)&report_this_inline()); }

#define TYPEINFO_ROOT(T) \
 TYPEINFO_IMPL_BEGIN(T) \
 TYPEINFO_IMPL_END(T, 0)
#define TYPEINFO_SINGLE(T, B) \
 TYPEINFO_IMPL_BEGIN(T) \
 static const PrimTypeInfo_Base baseList[2] = { TYPEINFO_BASE_OF(B, T), { 0, 0 }}; \
 TYPEINFO_IMPL_END(T, baseList)
#define TYPEINFO_DOUBLE(T, B1, B2) \
 TYPEINFO_IMPL_BEGIN(T) \
 static const PrimTypeInfo_Base baseList[3] = { TYPEINFO_BASE_OF(B1, T), TYPEINFO_BASE_OF(B2, T), { 0, 0 }}; \
 TYPEINFO_IMPL_END(T, baseList) \
 TYPEINFO_MULTIPLE(T)
#define TYPEINFO_TRIPLE(T, B1, B2, B3) \
 TYPEINFO_IMPL_BEGIN(T) \
 static const PrimTypeInfo_Base baseList[4] = { TYPEINFO_BASE_OF(B1, T), TYPEINFO_BASE_OF(B2, T), \
  TYPEINFO_BASE_OF(B3, T), { 0, 0 }}; \
 TYPEINFO_IMPL_END(T, baseList) \
 TYPEINFO_MULTIPLE(T)
#define TYPEINFO_QUADRUPLE(T, B1, B2, B3, B4) \
 TYPEINFO_IMPL_BEGIN(T) \
 static const PrimTypeInfo_Base baseList[5] = { TYPEINFO_BASE_OF(B1, T), TYPEINFO_BASE_OF(B2, T), \
  TYPEINFO_BASE_OF(B3, T), TYPEINFO_BASE_OF(B4, T), { 0, 0} }; \
 TYPEINFO_IMPL_END(T, baseList) \
 TYPEINFO_MULTIPLE(T)
#define TYPEINFO_QUINTUPLE(T, B1, B2, B3, B4, B5) \
 TYPEINFO_IMPL_BEGIN(T) \
 static const PrimTypeInfo_Base baseList[6] = { TYPEINFO_BASE_OF(B1, T), TYPEINFO_BASE_OF(B2, T), \
  TYPEINFO_BASE_OF(B3, T),TYPEINFO_BASE_OF(B4, T), TYPEINFO_BASE_OF(B5, T), { 0, 0} }; \
 TYPEINFO_IMPL_END(T, baseList) \
 TYPEINFO_MULTIPLE(T)
#define TYPEINFO_SEXTUPLE(T, B1, B2, B3, B4, B5, B6) \
 TYPEINFO_IMPL_BEGIN(T) \
 static const PrimTypeInfo_Base baseList[7] = { TYPEINFO_BASE_OF(B1, T), TYPEINFO_BASE_OF(B2, T), \
 TYPEINFO_BASE_OF(B3, T), TYPEINFO_BASE_OF(B4, T), TYPEINFO_BASE_OF(B5, T), TYPEINFO_BASE_OF(B6, T), { 0, 0} }; \
 TYPEINFO_IMPL_END(T, baseList) \
 TYPEINFO_MULTIPLE(T)

#else

#error "Should include these"

#define TYPEDECL_ROOT(T)
#define TYPEDECL_SINGLE(T, B)
#define TYPEDECL_DOUBLE(T, B1, B2)
#define TYPEDECL_TRIPLE(T, B1, B2, B3)
#define TYPEDECL_QUADRUPLE(T, B1, B2, B3, B4)
#define TYPEDECL_QUINTUPLE(T, B1, B2, B3, B4, B5)
#define TYPEDECL_SEXTUPLE(T, B1, B2, B3, B4, B5, B6)
#define TYPEINFO_DECLARATION(T)
#define TYPEINFO_ROOT(T)
#define TYPEINFO_SINGLE(T, B)
#define TYPEINFO_DOUBLE(T, B1, B2)
#define TYPEINFO_TRIPLE(T, B1, B2, B3)
#define TYPEINFO_QUADRUPLE(T, B1, B2, B3, B4)
#define TYPEINFO_QUINTUPLE(T, B1, B2, B3, B4, B5)
#define TYPEINFO_SEXTUPLE(T, B1, B2, B3, B4, B5, B6)
#endif

//
//  	The PrimPrint class provides standardised run-time type information and printing capabilities
//  	for its derived classes.
//  .note tutorial
//  	@file(PrimPrint.tutorial)
//

class PrimPrint {

private:
	TYPEDECL_ROOT(PrimPrint)
	
public:
//
//  	Construct a PrimPrint object.
//
	PrimPrint() {}
	
//
//  	The destructor does nothing.
//
	virtual ~PrimPrint() {}
	
#ifndef NO_PRIM_CHECKS
	virtual bool check(bool fullCheck = false) const;
#else
	static void check(bool fullCheck = false) {
		UNUSED(fullCheck);
	}
	
#endif
#ifndef NO_PRIM_STREAMS
	static bool& print_address();
	static std::ostream& print_address_of(std::ostream& s, const volatile void *anObject);
	virtual std::ostream& print_class_name(std::ostream& s) const;
	virtual std::ostream& print_class_suffix(std::ostream& s) const;
	virtual std::ostream& print_deep(std::ostream& s, int anIndentLevel = 0) const;
	virtual std::ostream& print_depth(std::ostream& s, int anIndentLevel = 0) const;
	virtual std::ostream& print_instance(std::ostream& s) const;
	virtual std::ostream& print_members(std::ostream& s, int anIndentLevel = 0) const;
	virtual std::ostream& print_on(std::ostream& s, int anIndentLevel = 0) const;
	virtual std::ostream& print_this(std::ostream& s) const;
#endif
#ifndef NO_PRIM_TYPEINFO
	const char *class_name() const;
#endif
	virtual PrimPrint *report_this();
//
//  	Identify "this" for the benefit of derived multiply inheriting classes implementing report_this().
//
	inline const PrimPrint& report_this_inline() const {
		return (*this);
	}
	
#ifndef NO_PRIM_STREAMS
	friend std::ostream& operator<<(std::ostream& s, const PrimPrint& aPrint);
#endif
};

//
//  	Return 0 as "this" in global scope to signify a non-PrimPrint context.
//
inline PrimPrint *report_this() {
	return (0);
}


#endif

