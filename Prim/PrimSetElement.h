
#ifndef PRIMSETELEMENT_H
#define PRIMSETELEMENT_H







//
//  	A PrimSetElement<T> defines a pair of a PrimId and a T suitable for use as an element of a PrimMapOfRef,
//  	thereby enabling sets rather than maps of T to be created.
//
template <class T>
class PrimSetElem {
	PRIMSHARES_DECLS_INLINE(PrimSetElem<T>)
	PRIMREF_DERIVED_DECLS(PrimSetElem<T>)
private:
	const PrimIdHandle _id;  //   The set identification key.
	T _value;     //   The value.
private:
	//
	//  	Construction of a copy is not implemented.
	//
	PrimSetElem(const PrimSetElem<T>& aSetElement);
	//
	//  	Assignment is not implemented.
	//
	PrimSetElem& operator=(const PrimSetElem<T>& aSetElement);
public:
	//
	//  	Construct a null set element with null id and null value.
	//
	PrimSetElem() {}
	//
	//  	Construct a set element with anId and default value.
	//
	PrimSetElem(const char* anId) : _id(anId), _value() {}
	PrimSetElem(const PrimId& anId) : _id(anId), _value() {}
	//
	//  	Construct a set element with anId identifying aValue.
	//
	PrimSetElem(const char* anId, const T& aValue) : _id(anId), _value(aValue) {}
	PrimSetElem(const PrimId& anId, const T& aValue) : _id(anId), _value(aValue) {}
	bool check(bool fullCheck) const { return true; }
	//
	//  	Return the identification field.
	//
	const PrimId& id() const { return *_id; }
	//
	//  	Return true to signal that this is a null set element if the id is null.
	//
	bool is_null() const { return _id.is_null(); }
	std::ostream& print_deep(std::ostream& s, int aDepth) const { return s; }
	std::ostream& print_depth(std::ostream& s, int aDepth) const { return s; }
	std::ostream& print_instance(std::ostream& s) const { return  s << '(' << _id << ')'; }
	//
	//  	Change the value foield to aValue;
	//
	void set_value(T& aValue) { _value = aValue; }
	//
	//  	Return the value field.
	//
	const T& value() const { return _value; }
	T& value() { return _value; }
	//
	//  	Incorporate aRef as part of an inline diagnostic.
	//
	friend std::ostream& operator<<(std::ostream& s, const PrimSetElem<T>& setElement)
	{ return setElement.print_instance(s); }
};

//
//  	A PrimSetElement<T> defines a pair of a PrimId and a T suitable for use as an element of a PrimMapOfRef,
//  	thereby enabling sets rather than maps of T to be created.
//
template <class T>
class PrimSetElement {
	PRIMSHARES_DECLS_INLINE(PrimSetElement<T>)
	PRIMREF_DERIVED_DECLS(PrimSetElement<T>)
private:
	const PrimIdHandle _id;  //   The set identification key.
	T _value;     //   The value.
private:
	//
	//  	Construction of a copy is not implemented.
	//
	PrimSetElement(const PrimSetElement<T>& aSetElement);
	//
	//  	Assignment is not implemented.
	//
	PrimSetElement& operator=(const PrimSetElement<T>& aSetElement);
public:
	//
	//  	Construct a null set element with null id and null value.
	//
	PrimSetElement() {}
	//
	//  	Construct a set element with anId identifying aValue.
	//
	PrimSetElement(const char* anId, const T& aValue) : _id(anId), _value(aValue) {}
	PrimSetElement(const PrimId& anId, const T& aValue) : _id(anId), _value(aValue) {}
	bool check(bool fullCheck) const { return _value->check(fullCheck); }
	//
	//  	Return the identification field.
	//
	const PrimId& id() const { return *_id; }
	//
	//  	Return true to signal that this is a null set element if the id is null.
	//
	bool is_null() const { return _id.is_null(); }
	std::ostream& print_deep(std::ostream& s, int aDepth) const { return _value->print_deep(s, aDepth); }
	std::ostream& print_depth(std::ostream& s, int aDepth) const { return _value->print_depth(s, aDepth); }
	std::ostream& print_instance(std::ostream& s) const
	{ s << "(" << _id << ", "; _value->print_instance(s); return  s << ")"; }
	//
	//  	Change the value foield to aValue;
	//
	void set_value(const T& aValue) { _value = aValue; }
	//
	//  	Return the value field.
	//
	const T& value() const { return _value; }
	T& value() { return _value; }
	//
	//  	Incorporate aRef as part of an inline diagnostic.
	//
	friend std::ostream& operator<<(std::ostream& s, const PrimSetElement<T>& setElement)
	{ return setElement.print_instance(s); }
};

#define PRIMSETELEM_NULLS(E,N,D) \
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
		p->_null.N::~N(); \
		return true; \
	} /*\
    template <> const E& E::immutable_null() \
    { \
    static bool initialised = false; \
    static char buf[sizeof(N)]; \
    if (!initialised) \
    { \
    initialised = true; \
    static D aDestructor(*(N *)buf); \
    } \
    return (*(const N *)buf); \
    } \
    __C__((Report the mutable null E.)) \
    template <> E& E::mutable_null() \
    { \
    static bool initialised = false; \
    static char buf[sizeof(N)]; \
    if (!initialised) \
    { \
    initialised = true; \
    static D aDestructor(*(N *)buf); \
    } \
    return (*(N *)buf); \
}*/

#define PRIMSETELEM_IMPL(E) PRIMSETELEM_NULLS(E,E,name2(E,Destructor))
#define PRIMSETELEMENT_IMPL(E) PRIMSETELEM_IMPL(E)

#endif
