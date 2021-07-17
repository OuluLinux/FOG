
#ifndef PRIMTYPEINFO_HXX
#define PRIMTYPEINFO_HXX


#ifndef NO_PRIM_TYPEINFO

#define ptr_cast(T, p) (((p) == 0) ? (T*)0 : (T*)((p)->dynamic_type_info().locate_type(T::static_type_info(), \
                        ((T *)p)->report_this(), (p))))

#ifndef NO_PRIM_CHECKS
	#define CHECKED_CAST(T, p, where) (T *)PrimTypeInfo::check_cast((T *)(p), ptr_cast(T, (p)), where)
#else
	#define CHECKED_CAST(T, p, where) (T *)(p)
#endif

//
//  		A PrimTypeInfo holds information about the type of a class. It is closely modelled
//  		after pp 442-451 of Stroustrup II. The implementation is simplified: No iteration
//  		over bases is supported. Multiple type infos per class are not supported.
//  .note tutorial
//  		@file(PrimTypeInfo.tutorial)
//
class PrimTypeInfo : public PrimPrint {
	TYPEDECL_SINGLE(PrimTypeInfo, PrimPrint)
private:
	const char* _name;         //   Name of the class.
	const PrimTypeInfo_Base* const _bases;    //   0 terminated list of base classes,
	//   0 if no bases.
private:
	//
	//  	Creation of a copy is forbidden.
	//
	PrimTypeInfo(const PrimTypeInfo& aTypeInfo);
	//
	//  	Assignment is forbidden.
	//
	PrimTypeInfo& operator=(const PrimTypeInfo& aTypeInfo);
	
public:
	//
	//  		Create a new type descriptor named aName with someBases identifying the base classes
	//
	PrimTypeInfo(const char* aName, const PrimTypeInfo_Base* someBases)
		: _name(aName), _bases(someBases) {}
	//
	//  		The destructor does nothing.
	//
	virtual ~PrimTypeInfo() {}
	static void* check_cast(void* compileTimeCast, void* runTimeCast, const char* someText);
	//
	//  		The default initialise class implementation does nothing, returning true if the class is
	//  		fully initialised, false if initialisation has yet to occur. Derived template classes reimplement
	//  		to establish composite type names.
	//
	virtual bool initialise_class() const;
	void* locate_base(const PrimTypeInfo& aBase, const void* ofObject, bool isDirect = false) const;
	void* locate_type(const PrimTypeInfo& aBase, const void* ofObject, const void* ofSubObject,
	                  bool isDirect = false) const;
	//
	//  		Report the name of the class controlled by this PrimTypeInfo.
	//
	virtual const char* name() const { return _name; }
#ifndef NO_PRIM_STREAMS
	virtual std::ostream& print_this(std::ostream& s) const;
#endif
	//
	//  		Report whether this type is the same as aTypeInfo.
	//
	int operator==(const PrimTypeInfo& aTypeInfo) const { return this == &aTypeInfo; }
	//
	//  		Report whether this type is different to aTypeInfo.
	//
	int operator!=(const PrimTypeInfo& aTypeInfo) const { return this != &aTypeInfo; }
};

//
//  	A PrimTypeInfo1 extends PrimTypeInfo to support type descriptions for classes with a
//  	single template parameter.
//
//  class PrimTypeInfo1 : public PrimTypeInfo
//  {
//  private:
//  	PrimString _full_name;								// Full name of class including template suffixes.
//  	const PrimTypeInfo& _type_info1;					// Type information for first parameter.
//
//  public:
//  	PrimTypeInfo1(const char *aName, const PrimTypeId& t1, const PrimTypeInfo_Base *someBases);
//
//  		The destructor does nothing.
//
//  	virtual ~PrimTypeInfo1() {}
//  	virtual bool initialise_class() const;
//  	virtual const char *name() const;
//  };

#else

#define CHECKED_CAST(T, p, where) (T *)(p)

#endif
#endif

