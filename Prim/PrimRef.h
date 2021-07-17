
#ifndef PRIMREF_H
#define PRIMREF_H





#define PRIMREFTOCONST_CLASS(E) typedef PrimRefToConst< E > name2(E,RefToConst) ;
#define PRIMREFTOCONST_INLINES(E)
#define PRIMREFTOCONST_EQUALS(E)
#define PRIMREF_CLASS(E) typedef PrimRef< E > name2(E,Ref) ;
#define PRIMREF_INLINES(E)

#define PRIMREFS_DATA(E) PRIMREFTOCONST_DATA(E) PRIMREF_DATA(E)
#define PRIMREFS_CODE(E) PRIMREFTOCONST_CODE(E) PRIMREF_CODE(E)

#define PRIMREFTOCONST_IMPL(E) PRIMREFTOCONST_CODE(E) PRIMREFTOCONST_DATA(E)
#define PRIMREF_IMPL(E) PRIMREF_CODE(E) PRIMREF_DATA(E)
#define PRIMREFS_IMPL(E) PRIMREFTOCONST_IMPL(E) PRIMREF_IMPL(E)

//
//  	PrimRefValue defines the indeterminate type used to represent the cast down target of a PrimRef.
//
class PrimRefValue {};

//
//  	PrimRefBase defines the memory layout of an abstract reference so that address only operations
//  	of cast pointer class may share common code.
//
class PrimRefBase {
private:
	const PrimRefValue* _elem;      //   The element - never 0 for valid objects.
public:
	enum Enums { ADOPT };
	//
	//  	Constuct an unconfigured reference.
	//
	PrimRefBase() {}
	//
	//  	Assign aValue as the element.
	//
	PrimRefBase& operator=(const PrimRefValue& anElement) { _elem = &anElement; return *this; }
	//
	//  	The reference may be converted to the element.
	//
	//  	const PrimRefValue& element() const { return *_elem; }
	//
	//  	The reference may be converted to the element.
	//
	operator const PrimRefValue* () const { return _elem; }
	//
	//  	Indirection yields the element.
	//
	const PrimRefValue& operator*() const { return *_elem; }
	//
	//  	Report whether this is a reference to anElement.
	//
	bool operator==(const PrimRefValue& anElement) const { return _elem == &anElement; }
};

//
//  	The PrimRefFuncs class defines the maintenance functions used by collection classes.
//  	The functions are declared as statics to cast-down objects to avoid problems and costs
//  	associated with instantiated template virtual function tables.
//
//  .limits
//  	Use of a POD is efficient and the easiest way to avoid construction order conflicts.
//
struct PrimRefFuncs {
public:
	//
	//  	Annul anElement to release any share count.
	//
	void (*annul)(const PrimRefValue& anElement);
	//
	//  	Perform a consistency check on anElement.
	//
	bool (*check)(const PrimRefValue& anElement, bool fullCheck);
	//
	//  	Print anElement to a stream with aDepth of indentation.
	//
	std::ostream& (*print_deep)(const PrimRefValue& anElement, std::ostream& s, int aDepth);
	//
	//  	Print anElement to a stream as part of an inline diagnostic.
	//
	std::ostream& (*print_instance)(const PrimRefValue& anElement, std::ostream& s);
	//
	//  	Create a share of anElement to increase the share count.
	//
	void (*share)(const PrimRefValue& anElement);
	//
	//  	Return a reference to a shared null element.
	//
	const PrimRefValue& (*shared_null)();
};

//
//  	The PrimRefToConstFunctions template defines the support functions required to permit
//  	a PrimListOfRefImp to invoke the correct E::methods. This table an implementation of the
//  	virtual function table for a PrimRefToConst< E > but which avoids requiring extra object in the
//  	pointer class.
//
//  .limits
//  	Use of a POD is efficient and the easiest way to avoid construction order conflicts.
//
template <class E>
struct PrimRefToConstFunctions {
public:
	PrimRefFuncs _functions;
public:
	static void annul(const E& anElement);
	static bool check(const E& anElement, bool fullCheck);
	static std::ostream& print_deep(const E& anElement, std::ostream& s, int aDepth);
	static std::ostream& print_instance(const E& anElement, std::ostream& s);
	static void share(const E& anElement);
	static const E& shared_null();
public:
	//
	//  	Access returns the table of functions.
	//
	operator const PrimRefFuncs& () const { return _functions; }
};

//
//  	PRIMREFTOCONST_CLASS defines a pointer class PrimRefToConst< E > to an immutable element class E,
//  	registering itself as a shared owner of the element.
//
template <class E>
class PrimRefToConst {
public:
	enum Enums { ADOPT = PrimRefBase::ADOPT };     //   Constructor argument.
	enum Locked {};           //   Useless type for pointer return.
private:
	const E* _elem;           //   The element.
public:
	static const PrimRefToConstFunctions< E > _functions;  //   The virtual function table.
private:
	//
	//  	Release any sharing of the representation.
	//
	void annul() { _elem->annul(); }
	//
	//  	Report whether this is a pointer to a null element.
	//
public:
	bool is_null() const { return _elem->is_null(); }
	
private:
	//
	//  	Establish any required sharing of the representation.
	//
	void share() { _elem->share(); }
public:
	void adopt(const E& anElem);
	void adopt(const E* anElem);
	void assign(const E& anElem);
	void assign(const E* anElem);
	//
	//  	Identify the element.
	//
	const E& element() const { return *_elem; }
	//
	//  	Initialise this pointer after default cast construction to reference null.
	//
	void init() { _elem = &null_elem(); share(); }
	//
	//  	Report whether this is a reference to anElem.
	//
	bool is(const E& anElem) const { return _elem == &anElem; }
	//
	//  	Report whether this is a reference to the same object as aRef.
	//
	bool is(const PrimRefToConst< E >& aRef) const { return _elem == aRef.pointer(); }
	//
	//  	Configure this pointer to reference nothing prior to default cast destruction.
	//
	void kill() { annul(); _elem = &null_elem(); }
	//
	//  	Report the null element.
	//
	const E& null_elem() const { return E::immutable_null(); }
	//
	//  	Identify the element.
	//
	const E* pointer() const { return _elem; }
	//
	//  	Reset to point at null.
	//
	void reset() { assign(0); }
	//
	//  	Return this pointer as an equivalent pointer to const element.
	//
	const PrimRefToConst< E >& to_const() const { return *this; }
	PrimRefToConst< E >& to_const() { return *this; }
public:
	//
	//  	Construct a non-pointing pointer.
	//
	PrimRefToConst() : _elem(&null_elem()) { share(); }
	//
	//  	Construct a pointer to anElem.
	//
	PrimRefToConst(const E& anElem) : _elem(&anElem) { share(); }
	//
	//  	Construct a pointer to anElem, which may be 0.
	//
	PrimRefToConst(const E* anElem) : _elem(anElem ? anElem : & null_elem()) { share(); }
	//
	//  	Construct a pointer to anElem, which may be 0. Ownership is transferred to the pointer.
	//
	PrimRefToConst(const E* anElem, Enums) : _elem(anElem ? anElem : & null_elem()) { if (!anElem) share(); }
	//
	//  	Construct a copy of aRef.
	//
	PrimRefToConst(const PrimRefToConst< E >& aRef) : _elem(aRef._elem) { share(); }
	//
	//  	The destructor releases the shared count.
	//
	~PrimRefToConst() { annul(); }
	//
	//  	Assign anElem to this pointer.
	//
	PrimRefToConst< E >& operator=(const E& anElem) { assign(anElem); return *this; }
	//
	//  	Assign anElem, which may be 0, to this pointer.
	//
	PrimRefToConst< E >& operator=(const E* anElem) { assign(anElem); return *this; }
	//
	//  	Assign aRef to this design pointer.
	//
	PrimRefToConst< E >& operator=(const PrimRefToConst< E >& aRef) { assign(*aRef._elem); return *this; }
	//
	//  	Indirection of the pointer indirects to the far end of of a pointer chain.
	//
	const E& operator*() const { return *_elem; }
	//
	//  	Indirection through the pointer operates on the far end of of a pointer chain.
	//
	const E* operator->() const { return _elem; }
	//
	//  	Return an indication as to whether this is a valid pointer.
	//
	operator const Locked* () const { return (const Locked*)(is_null() ? 0 : this); }
	//
	//  	Testing the complement of the pointer returns a 0 value if pointing at non-0.
	//
	bool operator!() const { return is_null(); }
	//
	//  	Report whether firstRef is the same as secondRef.
	//
	friend bool operator==(const PrimRefToConst< E >& firstRef, const PrimRefToConst< E >& secondRef)
	{ return firstRef._elem == secondRef._elem; }
	//
	//  	Report whether firstRef is the different secondRef.
	//
	friend bool operator!=(const PrimRefToConst< E >& firstRef, const PrimRefToConst< E >& secondRef)
	{ return firstRef._elem != secondRef._elem; }
	//
	//  	Incorporate aRef as part of an inline diagnostic.
	//
	/*   withdrawn:
		if operator<<(std::ostream& s, const E& aRef) is undefined a PrimRefToConst< E > creation loop results */
	//  	friend std::ostream& operator<<(std::ostream& s, const PrimRefToConst< E >& aRef)
	//  		{ return s << aRef.element(); }
};

#define PRIMREFTOCONST_CODE(E) \
	__C__((Adopt anElem as owned by this pointer.))\
	template <> void PrimRefToConst< E >::adopt(const E& anElem) \
	{ \
		const E *oldElem = _elem; \
		_elem = &anElem; \
		oldElem->annul(); \
	}\
	__C__((Adopt anElem, which may be 0, as owned by this pointer.))\
	template <> void PrimRefToConst< E >::adopt(const E *anElem) \
	{ \
		const E *oldElem = _elem; \
		if (anElem) \
			_elem = anElem; \
		else \
		{ \
			_elem = &null_elem(); \
			share(); \
		} \
		oldElem->annul(); \
	}\
	__C__((Assign anElem to this pointer.))\
	template <> void PrimRefToConst< E >::assign(const E& anElem) \
	{ \
		if (_elem != &anElem) \
		{ \
			const E *oldElem = _elem; \
			_elem = &anElem; \
			share(); \
			oldElem->annul(); \
		} \
	}\
	__C__((Assign anElem, which may be 0, to this pointer.))\
	template <> void PrimRefToConst< E >::assign(const E *anElem) \
	{ \
		const E& newElem = anElem ? *anElem : null_elem(); \
		if (_elem != &newElem) \
		{ \
			const E *oldElem = _elem; \
			_elem = &newElem; \
			share(); \
			oldElem->annul(); \
		} \
	} \
	template <> void PrimRefToConstFunctions< E >::annul(const E& anElement) { anElement.annul(); } \
	template <> bool PrimRefToConstFunctions< E >::check(const E& anElement, bool fullCheck) \
	{ return anElement.check(fullCheck); } \
	template <> std::ostream& PrimRefToConstFunctions< E >::print_deep(const E& anElement, std::ostream& s, int aDepth) \
	{ return anElement.print_deep(s, aDepth); } \
	template <> std::ostream& PrimRefToConstFunctions< E >::print_instance(const E& anElement, std::ostream& s) \
	{ return anElement.print_instance(s); } \
	template <> void PrimRefToConstFunctions< E >::share(const E& anElement) { anElement.share(); } \
	template <> const E& PrimRefToConstFunctions< E >::shared_null() \
	{ E::immutable_null().share(); return E::immutable_null(); }

#define PRIMREFTOCONST_DATA(E) \
	template <> const PrimRefToConstFunctions< E > PrimRefToConst< E >::_functions = { \
	                                                                                   (void (*)(const PrimRefValue&))& PrimRefToConstFunctions< E >::annul, \
	                                                                                   (bool (*)(const PrimRefValue&, bool))&PrimRefToConstFunctions< E >::check, \
	                                                                                   (std::ostream& (*)(const PrimRefValue&, std::ostream&, int))&PrimRefToConstFunctions< E >::print_deep, \
	                                                                                   (std::ostream& (*)(const PrimRefValue&, std::ostream&))&PrimRefToConstFunctions< E >::print_instance, \
	                                                                                   (void (*)(const PrimRefValue&))&PrimRefToConstFunctions< E >::share, \
	                                                                                   (const PrimRefValue& (*)())&PrimRefToConstFunctions< E >::shared_null };

//
//  	The PrimRefFunctions template defines the support functions required to permit
//  	a PrimListOfRefImp to invoke the correct E::methods. This table an implementation of the
//  	virtual function table for a PrimRef< E > but which avoids requiring extra object in the
//  	pointer class.
//
//  .limits
//  	Use of a POD is efficient and the easiest way to avoid construction order conflicts.
//
template <class E>
struct PrimRefFunctions {
public:
	PrimRefFuncs _functions;
public:
	static E& shared_null();
public:
	//
	//  	Access returns the table of functions.
	//
	operator const PrimRefFuncs& () const { return _functions; }
};

//
//  	PRIMREF_CLASS defines a pointer class P to a mutable element class E, registering itself as a
//  	shared owner of the element. The pointer is derived from the const pointer class PrimRefToConst< E >.
//
template <class E>
class PrimRef {
public:
	enum Enums { ADOPT = PrimRefBase::ADOPT };     //   Constructor argument.
	enum Locked {};           //   Useless type for pointer return.
private:
	E* _elem;            //   The element.
public:
	static const PrimRefFunctions< E > _functions;   //   The virtual function table.
private:
	//
	//  	Release any sharing of the representation.
	//
	void annul() { _elem->annul(); }
	//
	//  	Return a clone of anElem if non-0, or a share of the null element otherwise.
	//
	E& clone(const E* anElem) { return anElem ? anElem->clone() : (null_elem().share(), null_elem()); }
	//
	//
	//  	Establish any required sharing of the representation.
	//
	void share() { _elem->share(); }
public:

	//  	Report whether this is a pointer to a null element.
	//
	bool is_null() const { return _elem->is_null(); }
	
	void adopt(const E& anElem);
	//
	//  	Adopt anElem as owned by this pointer.
	//
	void adopt(E& anElem) { to_const().adopt(anElem); }
	//
	//  	Adopt a mutable clone of anElem, which may be 0, as owned by this pointer.
	//
	void adopt(const E* anElem) { to_const().adopt(clone(anElem)); }
	//
	//  	Adopt anElem, which may be 0, as owned by this pointer.
	//
	void adopt(E* anElem) { to_const().adopt(*(anElem ? anElem : (null_elem().share(), &null_elem()))); }
	//
	//  	Assign a mutable clone of anElem, which may be 0, to this pointer.
	//
	void assign(const E& anElem) { to_const().adopt(anElem->clone()); }
	//
	//  	Assign anElem, which may be 0, to this pointer.
	//
	void assign(E& anElem) { to_const().assign(anElem); }
	//
	//  	Assign a mutable clone of anElem, which may be 0, to this pointer.
	//
	void assign(const E* anElem) { to_const().adopt(clone(anElem)); }
	//
	//  	Assign anElem, which may be 0, to this pointer.
	//
	void assign(E* anElem) { to_const().assign(anElem ? *anElem : E::mutable_null()); }
	//
	//  	Identify the element.
	//
	E& element() const { return *_elem; }
	//
	//  	Initialise this pointer after default cast construction to reference null.
	//
	void init() { _elem = &null_elem(); share(); }
	//
	//  	Report whether this is a reference to anElem.
	//
	bool is(const E& anElem) const { return _elem == &anElem; }
	//
	//  	Report whether this is a reference to the same object as aRef.
	//
	bool is(const PrimRef< E >& aRef) const { return _elem == aRef.pointer(); }
	//
	//  	Configure this pointer to reference nothing prior to default cast destruction.
	//
	void kill() { annul(); _elem = &null_elem(); }
	//
	//  	Report the null element.
	//
	E& null_elem() const { return E::mutable_null(); }
	//
	//  	Identify the element.
	//
	E* pointer() const { return _elem; }
	//
	//  	Reset to point at null, avoiding a const/non-const overloading ambiguity.
	//
	void reset() { to_const().assign(E::mutable_null()); }
	//
	//  	Return an equivalent pointer to const element.
	//
	const PrimRefToConst< E >& to_const() const { return *(const PrimRefToConst< E >*)this; }
	PrimRefToConst< E >& to_const() { return *(PrimRefToConst< E >*)this; }
public:
	//
	//  	Construct a non-pointing pointer.
	//
	PrimRef() : _elem(&null_elem()) { share(); }
	//
	//  	Construct a pointer to mutable clone of anElem.
	//
	PrimRef(const E& anElem) : _elem(&anElem.clone()) {}
	//
	//  	Construct a pointer to anElem.
	//
	PrimRef(E& anElem) : _elem(&anElem) { share(); }
	//
	//  	Construct a pointer to mutable clone of anElem, which may be 0.
	//
	PrimRef(const E* anElem) : _elem(anElem ? & anElem->clone() : & null_elem().share()) {}
	//
	//  	Construct a pointer to anElem, which may be 0.
	//
	PrimRef(E* anElem) : _elem(anElem ? anElem : & null_elem()) { share(); }
	//
	//  	Construct a pointer to anElem, which may be 0. Ownership is transferred to the pointer.
	//
	PrimRef(E* anElem, Enums) : _elem(anElem ? anElem : & null_elem()) { if (!anElem) share(); }
	//
	//  	Construct a pointer to mutable clone of the element identified by aRef.
	//
	PrimRef(const PrimRefToConst< E >& aRef) : _elem(&aRef->clone()) {}
	//
	//  	Construct a copy of aRef.
	//
	PrimRef(const PrimRef< E >& aRef) : _elem(aRef._elem) { share(); }
	//
	//  	The destructor releases the shared count.
	//
	~PrimRef() { annul(); }
	inline PrimRef< E >& operator=(const E& anElem);
	//
	//  	Assign anElem to this pointer.
	//
	PrimRef< E >& operator=(E& anElem) { assign(anElem); return *this; }
	PrimRef< E >& operator=(const E* anElem) { assign(anElem); return *this; }
	//
	//  	Assign anElem, which may be 0, to this pointer.
	//
	PrimRef< E >& operator=(E* anElem) { assign(anElem); return *this; }
	//
	//  	Assign aRef to this pointer.
	//
	PrimRef< E >& operator=(const PrimRef< E >& aRef) { assign(aRef.element()); return *this; }
	PrimRef< E >& operator=(const PrimRefToConst< E >& aRef) { assign(aRef.element()); return *this; }
	//
	//  	A pointer to non-const may be converted to a pointer to const.
	//
	operator const PrimRefToConst< E >& () const { return *(const PrimRefToConst< E >*)this; }
	//
	//  	Indirection of the pointer indirects to the far end of of a pointer chain.
	//
	E& operator*() const { return *_elem; }
	//
	//  	Indirection through the pointer operates on the far end of of a pointer chain.
	//
	E* operator->() const { return _elem; }
	//
	//  	Return an indication as to whether this is a valid pointer.
	//
	operator const Locked* () const { return (const Locked*)(is_null() ? 0 : this); }
	//
	//  	Testing the complement of the pointer returns a 0 value if pointing at non-0.
	//
	bool operator!() const { return is_null(); }
	//
	//  	Report whether firstRef is the same as secondRef.
	//
	friend bool operator==(const PrimRef< E >& firstRef, const PrimRef< E >& secondRef)
	{ return firstRef._elem == secondRef._elem; }
	//
	//  	Report whether firstRef is the different secondRef.
	//
	friend bool operator!=(const PrimRef< E >& firstRef, const PrimRef< E >& secondRef)
	{ return firstRef._elem != secondRef._elem; }
	//
	//  	Incorporate aRef as part of an inline diagnostic.
	//
	/*   withdrawn:
		if operator<<(std::ostream& s, const E& aRef) is undefined a PrimRef< E > creation loop results */
	//  	friend std::ostream& operator<<(std::ostream& s, const PrimRef< E >& aRef) { return s << aRef.element(); }
};

#define PRIMREF_CODE(E) \
	template <> E& PrimRefFunctions< E >::shared_null() { E::mutable_null().share(); return E::mutable_null(); }

#define PRIMREF_DATA(E) \
	template <> const PrimRefFunctions< E > PrimRef< E >::_functions = { \
	                                                                     (void (*)(const PrimRefValue&))&PrimRefToConstFunctions< E >::annul, \
	                                                                     (bool (*)(const PrimRefValue&, bool))&PrimRefToConstFunctions< E >::check, \
	                                                                     (std::ostream& (*)(const PrimRefValue&, std::ostream&, int))&PrimRefToConstFunctions< E >::print_deep, \
	                                                                     (std::ostream& (*)(const PrimRefValue&, std::ostream&))&PrimRefToConstFunctions< E >::print_instance, \
	                                                                     (void (*)(const PrimRefValue&))&PrimRefToConstFunctions< E >::share, \
	                                                                     (const PrimRefValue& (*)())&PrimRefFunctions< E >::shared_null };

#define PRIMREF_CLONES(E)

//
//  	Provide the standard declarations within the class declaration of an E to support references to an E.
//  	Either PRIMSHARES_DECLS(E) or PRIMSHARES_DERIVED_DECLS(E,B) must also be provided to support the
//  	share count.
//
#define PRIMREF_DECLS(E) \
	public: \
	virtual bool is_null() const; \
	PRIMREF_DERIVED_DECLS(E)

//
//  	Provide the standard declarations within the class declaration of an E to support references to an E.
//  	Either PRIMSHARES_DECLS(E) or PRIMSHARES_DERIVED_DECLS(E,B) must also be provided to support the
//  	share count.
//
#define PRIMREF_DERIVED_DECLS(E) \
	public: \
	static const E& immutable_null(); \
	static E& mutable_null(); \
	private:

//
//  	Provide a partial declaration for a null class to implement the null derived class of an E.
//  	Additional virtual methods may be added before the closing brace to trap undesirable behaviour
//  	of the null object.
//
#define PRIMREF_NULL_CLAS(E) \
	class name2(E,Null) : public E {\
		TYPEDECL_SINGLE(name2(E,Null), E)\
	public:\
		__C__((Construct a null E.))\
		name2(E,Null)() {} \
		~name2(E,Null)(); \
		virtual bool is_null() const;

//
//  	Provide a complete declaration for a null class to implement the null derived class of an E.
//
#define PRIMREF_NULL_CLASS(E) PRIMREF_NULL_CLAS(E) };

//
//  	Provide the standard code to support a null class, which assumes that PrimShares is in use by
//  	using if (shares()) as an object initialisation test. This assumption may be worked around
//  	by declaring a private shares() method in the null class.
//
#define PRIMREF_NULL_CODE(E) \
	__C__((Report that this is not a null E.)) \
	bool E::is_null() const { return false; } \
	PRIMREF_DERIVED_NULL_CODE(E)

//
//  	Provide the standard code to support a null derived from a class that also has a nullclass,
//  	which assumes that PrimShares is in use by
//  	using if (shares()) as an object initialisation test. This assumption may be worked around
//  	by declaring a private shares() method in the null class.
//
//  	The null objects are flagged as initialised prior to use, so that self-referencing
//  	null objects do not degenerate into an infinite loop during construction.
//
//  	The null objects are destructed only once their share counts have decayed to the value immediately
//  	following construction, and couresy of PrimStaticDestructor only once practically all static
//  	destruction has happened. The likelihood of a null object coming back to life after static
//  	destruction is therefore very low and almost certainly a bug.
//
#define PRIMREF_DERIVED_NULLS(E,N,D) \
	class D : private PrimStaticDestructor<D> \
	{ \
		N& _null; \
		int _threshhold; \
		static bool destruct(D *p); \
	public: \
		D(N& aNull); \
	}; \
	D::D(N& aNull) : PrimStaticDestructor<D>(&D::destruct, this), \
		_null(*::new (&aNull) N), _threshhold(_null.shares()) {} \
	bool D::destruct(D *p) \
	{ \
		if (p->_null.shares() != p->_threshhold) \
			return false; \
		typedef N Dtor; \
		p->_null.Dtor::~Dtor(); \
		return true; \
	} /*\
    template <> static const E& E::immutable_null() \
    { \
    static bool initialised = false; \
    static char buf[sizeof(N)]; \
    if (!initialised) \
    { \
    initialised = true; \
    static D aDestructor(*(N *)buf); \
    } \
    return *(const N *)buf; \
    } \
    __C__((Report the mutable null E.)) \
    E& E::mutable_null() \
    { \
    static bool initialised = false; \
    static char buf[sizeof(N)]; \
    if (!initialised) \
    { \
    initialised = true; \
    static D aDestructor(*(N *)buf); \
    } \
    return *(N *)buf; \
}*/

#define PRIMREF_DERIVED_NULL_CODE(E) \
	PRIMREF_DERIVED_NULLS(E,name2(E,Null),name2(E,Destructor)) \
	__C__((The destructor adds no functionality.)) \
	name2(E,Null)::~name2(E,Null)() {} \
	__C__((Report that this is a null E.)) \
	bool name2(E,Null)::is_null() const { return true; }

//
//  	Provide the data to support a derived null class of E.
//
#define PRIMREF_NULL_DATA(E) TYPEINFO_SINGLE(name2(E,Null),E)

//
//  	Provide the code and data to support a derived null class of E.
//
#define PRIMREF_NULL_IMPL(E) PRIMREF_NULL_CODE(E) PRIMREF_NULL_DATA(E)
#define PRIMREF_DERIVED_NULL_IMPL(E) PRIMREF_DERIVED_NULL_CODE(E) PRIMREF_NULL_DATA(E)

//
//  	Declare a derived null class of E together with its the code and data support.
//
#define PRIMREF_NULL_CLASS_AND_IMPL(E) PRIMREF_NULL_CLASS(E) PRIMREF_NULL_IMPL(E)
#define PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(E) PRIMREF_NULL_CLASS(E) PRIMREF_DERIVED_NULL_IMPL(E)
#endif

