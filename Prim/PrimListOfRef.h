
#ifndef PRIMLISTOFREF_H
#define PRIMLISTOFREF_H






#define PRIMLISTOFREFTOCONST_CLASS(E) typedef PrimListOfRefToConst< E > name2(E,ListOfRefToConst);
#define PRIMLISTOFREF_CLASS(E) typedef PrimListOfRef< E > name2(E,ListOfRef);
#define PRIMLISTOFREFTOCONSTITERATOR_CLASS(E) \
	typedef PrimListOfRefToConstIterator< E > name2(E,ListOfRefToConstIterator);
#define PRIMCONSTLISTOFREFTOCONSTITERATOR_CLASS(E) \
	typedef PrimConstListOfRefToConstIterator< E > name2(E,ConstListOfRefToConstIterator);
#define PRIMLISTOFREFITERATOR_CLASS(E) \
	typedef PrimListOfRefIterator< E > name2(E,ListOfRefIterator);
#define PRIMCONSTLISTOFREFITERATOR_CLASS(E) \
	typedef PrimConstListOfRefIterator< E > name2(E,ConstListOfRefIterator);

#define PRIMLISTOFREFS_CODE(E) PRIMLISTOFREFTOCONST_CODE(E) PRIMLISTOFREF_CODE(E)
#define PRIMLISTOFREFS_DATA(E) PRIMLISTOFREFTOCONST_DATA(E) PRIMLISTOFREF_DATA(E)
#define PRIMLISTS_CODE(E) PRIMCONSTLIST_CODE(E) PRIMLIST_CODE(E)
#define PRIMLISTS_DATA(E) PRIMCONSTLIST_DATA(E) PRIMLIST_DATA(E)

#define PRIMLISTOFREFTOCONST_IMPL(E) PRIMLISTOFREFTOCONST_DATA(E) PRIMLISTOFREFTOCONST_CODE(E)
#define PRIMLISTOFREF_IMPL(E) PRIMLISTOFREF_DATA(E) PRIMLISTOFREF_CODE(E)
#define PRIMLISTOFREFS_IMPL(E) PRIMLISTOFREFTOCONST_IMPL(E) PRIMLISTOFREF_IMPL(E)
#define PRIMCONSTLIST_IMPL(E) PRIMCONSTLIST_DATA(E) PRIMCONSTLIST_CODE(E)
#define PRIMLIST_IMPL(E) PRIMLIST_DATA(E) PRIMLIST_CODE(E)
#define PRIMLISTS_IMPL(E) PRIMCONSTLIST_IMPL(E) PRIMLIST_IMPL(E)

//
//  	A PrimListOfRefImp maintains a list of references in an abstract form so that derived classes may
//  	refine the list maintenance to suit the particular type of pointed at element.
//
//  	The contents are maintained as an array of PrimRefBase which clients may pun to create their own
//  	reference classes. In order to avoid virtual inheritance of reference classes, derived list of reference
//  	classes must implement certain pointer manipulations using the appropriate types.
//
//  	Within the array, indexes from 0 to _tally (exclusive) always reference an object of the correct type
//  	and possess a share of that object. Indexes from _tally (inclusive) to _capacity (exclusive) have
//  	memory reserved for use of a pointer, but have undefined value, but no share of any object.
//
class PrimListOfRefImp {
	friend class PrimMapOfRefImp;
private:
	PrimRefBase* _contents;      //   The pointers.
	size_t _capacity;       //   Allocated dimension of _contents.
	size_t _tally;        //   Maximum valid array index.
	
private:
	//
	//  	Construction of a copy can only be performed by derived classes.
	//
	explicit PrimListOfRefImp(const PrimListOfRefImp& aRef);
	
public:
	typedef PrimRefValue* (*CloneFunction)(const PrimRefValue&);
	enum PrimListOfRefImpEnums { INLINE };
	//
	//  	Create an empty list.
	//
	PrimListOfRefImp() : _contents(0), _capacity(0), _tally(0) {}
	//
	//  	Create an empty list.
	//
	explicit PrimListOfRefImp(PrimListOfRefImpEnums anEnum) : _contents(0), _capacity(0), _tally(0)
	{ UNUSED(anEnum); }
	~PrimListOfRefImp();
	//
	//  	Return a reference to the anIndex'th pointer.
	//
	const PrimRefBase& operator[](size_t anIndex) const { return _contents[anIndex]; }
	PrimRefBase& operator[](size_t anIndex) { return _contents[anIndex]; }
	bool add(const PrimRefFuncs& supportFunctions, const PrimRefValue& anElement);
	bool add(const PrimRefFuncs& supportFunctions, const PrimListOfRefImp& aList);
	bool adopt(const PrimRefFuncs& supportFunctions, const PrimRefValue& anObject);
	void assign(const PrimRefFuncs& supportFunctions, const PrimListOfRefImp& aList);
	void assign(const PrimRefFuncs& supportFunctions, const PrimMapOfRefImp& aMap);
	//
	//  	Report the allocated number of design pointers.
	//
	size_t capacity() const { return _capacity; }
	bool check(const PrimRefFuncs& supportFunctions, bool fullCheck = false) const;
	void clone(const PrimRefFuncs& supportFunctions, const PrimListOfRefImp& somePointers,
	           CloneFunction cloneFunction);
	//
	//  	Report the array used for the contents, for strictly short term usage.
	//
	PrimRefBase* contents() { return _contents; }
	//
	//  	Report the array used for the contents, for strictly short term usage.
	//
	const PrimRefBase* contents() const { return _contents; }
	const PrimRefValue* find(const PrimRefFuncs& supportFunctions, const PrimRefValue& anElement) const;
	bool insert_before(const PrimRefFuncs& supportFunctions, const PrimRefValue& anElement,
	                   unsigned int anIndex = 0);
	bool is_equal(const PrimRefFuncs& supportFunctions, const PrimListOfRefImp& aList) const;
	std::ostream& print_deep(const PrimRefFuncs& supportFunctions, std::ostream& s, int aDepth) const;
	std::ostream& print_depth(const PrimRefFuncs& supportFunctions, std::ostream& s, int aDepth) const;
	std::ostream& print_instance(const PrimRefFuncs& supportFunctions, std::ostream& s) const;
	std::ostream& print_members(const PrimRefFuncs& supportFunctions, std::ostream& s, int aDepth) const;
	std::ostream& print_on(const PrimRefFuncs& supportFunctions, std::ostream& s, int aDepth) const;
	std::ostream& print_this(const PrimRefFuncs& supportFunctions, std::ostream& s) const;
	bool remove(const PrimRefFuncs& supportFunctions, const PrimRefValue& anElement);
	bool remove(const PrimRefFuncs& supportFunctions, size_t anIndex);
	//
	//  	Increase the capacity to aCapacity, returning true if successful. The tally and contents are
	//  	preserved.
	//
	bool set_capacity(const PrimRefFuncs& supportFunctions, size_t aCapacity)
	{ return set_sizes(supportFunctions, aCapacity, _tally); }
	bool set_sizes(const PrimRefFuncs& supportFunctions, size_t aCapacity, size_t aTally);
	//
	//  	Change the tally to aTally, returning true if successful. The list is expanded if necessary,
	//  	redundant contents are eliminated, residual contents are untouched, new contents are zero pointers.
	//
	bool set_tally(const PrimRefFuncs& supportFunctions, size_t aTally)
	{ return set_sizes(supportFunctions, 0, aTally); }
	void sort(int (*compareRoutine)(const void* v1, const void* v2), size_t startIndex, size_t numIndexes);
	//
	//  	Report the number of pointers.
	//
	size_t tally() const { return _tally; }
	void vacate(const PrimRefFuncs& supportFunctions);
	//  	friend bool operator==(const PrimListOfRefImp& firstList, const PrimListOfRefImp& secondList);
	friend std::ostream& operator<<(std::ostream& s, const PrimListOfRefImp& aList);
};

template <class E>
class PrimListOfRefToConst {
	friend class PrimConstListOfRefToConstIterator< E >;
	friend class PrimListOfRefToConstIterator< E >;
	friend class PrimListOfRef< E >;
	friend class PrimMapOfRefToConst< E >;
private:
	PrimListOfRefImp _list;         //   The list using cast-down types.
private:
	static const PrimRefToConstFunctions< E >& functions() { return PrimRefToConst< E >::_functions; }
protected:
	PrimRefToConst< E >* contents() { return (PrimRefToConst< E >*)_list.contents(); }
	const PrimRefToConst< E >* contents() const { return (const PrimRefToConst< E >*)_list.contents(); }
public:
	PrimListOfRefToConst() {}
	explicit PrimListOfRefToConst(const PrimSize& initialCapacity)
	{ _list.set_capacity(functions(), initialCapacity.size()); }
	PrimListOfRefToConst(const PrimSize& initialCapacity, const PrimSize& initialTally)
	{ _list.set_sizes(functions(), initialCapacity.size(), initialTally.size()); }
	explicit PrimListOfRefToConst(const PrimListOfRefToConst< E >& aList)
	{ _list.assign(functions(), aList._list); }
	explicit PrimListOfRefToConst(const PrimMapOfRefToConst< E >& aMap)
	{ _list.assign(functions(), (const PrimMapOfRefImp&)aMap); }
	~PrimListOfRefToConst() { _list.vacate(functions()); }
	PrimListOfRefToConst< E >& operator=(const PrimListOfRefToConst< E >& aList)
	{ _list.assign(functions(), aList._list); return *this; }
	PrimListOfRefToConst< E >& operator=(const PrimMapOfRefToConst< E >& aMap)
	{ _list.assign(functions(), aMap._map); return *this; }
	PrimRefToConst< E >& operator[](size_t anIndex) { return contents()[anIndex]; }
	const PrimRefToConst< E >& operator[](size_t anIndex) const { return contents()[anIndex]; }
	PrimListOfRefToConst< E >& operator+=(const E& anObject)
	{ _list.add(functions(), (const PrimRefValue&)anObject); return *this; }
	PrimListOfRefToConst< E >& operator+=(const PrimListOfRefToConst< E >& aList)
	{ _list.add(functions(), aList._list); return *this; }
	bool add(const E& anElement) { return _list.add(functions(), (const PrimRefValue&)anElement); }
	bool add(const PrimListOfRefToConst< E >& aList) { return _list.add(functions(), aList._list); }
	bool adopt(const E& anObject) { return _list.adopt(functions(), (const PrimRefValue&)anObject); }
	bool adopt(const E* anObject) { return anObject ? adopt(*anObject) : false; }
	//
	//  	Report the allocated number of list elements.
	//
	size_t capacity() const { return _list.capacity(); }
	//
	//  	Perform a consistency check on the list, enthusiastically if fullCheck.
	//
	bool check(bool fullCheck = false) const { return _list.check(functions(), fullCheck); }
	const E* find(const E& anElement) const
	{ return (const E*)_list.find(functions(), (const PrimRefValue&)anElement); }
	bool insert_before(const E& anElement, unsigned int anIndex = 0)
	{ return _list.insert_before(functions(), (const PrimRefValue&)anElement, anIndex); }
	std::ostream& print_deep(std::ostream& s, int aDepth = 0) const
	{ return _list.print_deep(functions(), s, aDepth); }
	std::ostream& print_depth(std::ostream& s, int aDepth = 0) const
	{ return _list.print_depth(functions(), s, aDepth); }
	std::ostream& print_instance(std::ostream& s) const { return _list.print_instance(functions(), s); }
	std::ostream& print_members(std::ostream& s, int aDepth = 0) const
	{ return _list.print_members(functions(), s, aDepth); }
	std::ostream& print_on(std::ostream& s, int aDepth = 0) const
	{ return _list.print_on(functions(), s, aDepth); }
	std::ostream& print_this(std::ostream& s) const { return _list.print_this(functions(), s); }
	bool remove(const E& anElement) { return _list.remove(functions(), (const PrimRefValue&)anElement); }
	bool remove(size_t anIndex) { return _list.remove(functions(), anIndex); }
	//
	//  	Increase the capacity to aCapacity, returning true if successful. The tally and contents are
	//  	preserved.
	//
	bool set_capacity(size_t aCapacity) { return _list.set_capacity(functions(), aCapacity);  }
	//
	//  	Change the tally to aTally, returning true if successful. The list is expanded if necessary,
	//  	redundant contents are eliminated, residual contents are untouched, new contents are zero pointers.
	//
	bool set_tally(size_t aTally) { return _list.set_tally(functions(), aTally);  }
	void sort(int (*compareRoutine)(const PrimRefToConst< E >* v1, const PrimRefToConst< E >* v2))
	{ _list.sort((int (*)(const void* v1, const void* v2))compareRoutine, 0, tally()); }
	void sort(int (*compareRoutine)(const PrimRefToConst< E >* v1, const PrimRefToConst< E >* v2),
	          size_t startIndex, size_t numIndexes)
	{ _list.sort((int (*)(const void* v1, const void* v2))compareRoutine, startIndex, numIndexes); }
	//
	//  	Report the number of pointers.
	//
	size_t tally() const { return _list.tally(); }
	const PrimListOfRefToConst< E >& to_const() const { return *this; }
	//
	//  	Empty the list.
	//
	void vacate() { _list.vacate(functions()); }
	friend bool operator==(const PrimListOfRefToConst< E >& firstList, const PrimListOfRefToConst< E >& secondList)
	{ return firstList._list.is_equal(PrimListOfRefToConst< E >::functions(), secondList._list); }
	friend bool operator!=(const PrimListOfRefToConst< E >& firstList,
	                       const PrimListOfRefToConst< E >& secondList)
	{ return !operator==(firstList, secondList); }
	friend std::ostream& operator<<(std::ostream& s, const PrimListOfRefToConst< E >& anObject)
	{ return anObject.print_instance(s); }
	friend inline void annul(PrimListOfRefToConst< E >* anObject) { if (anObject) delete anObject; }
};

#define PRIMLISTOFREFTOCONST_CODE(E)
#define PRIMLISTOFREFTOCONST_DATA(E)

template <class E>
class PrimListOfRef {
	friend class PrimConstListOfRefToConstIterator< E >;
	friend class PrimConstListOfRefIterator< E >;
	friend class PrimListOfRefToConstIterator< E >;
	friend class PrimListOfRefIterator< E >;
	friend class PrimMapOfRef< E >;
private:
	PrimListOfRefImp _list;         //   The list using cast-down types.
private:
	static const PrimRefFunctions< E >& functions() { return PrimRef< E >::_functions; }
protected:
	PrimRef< E >* contents() { return (PrimRef< E >*)_list.contents(); }
	const PrimRef< E >* contents() const { return (const PrimRef< E >*)_list.contents(); }
public:
	typedef E* (*CloneFunction)(const E&, PrimPrint&);
	PrimListOfRef() {}
	explicit PrimListOfRef(const PrimSize& initialCapacity) { _list.set_capacity(functions(), initialCapacity.size()); }
	PrimListOfRef(const PrimSize& initialCapacity, const PrimSize& initialTally)
	{ _list.set_sizes(functions(), initialCapacity.size(), initialTally.size()); }
	explicit PrimListOfRef(const PrimListOfRef< E >& aList) { _list.assign(functions(), aList._list); }
	PrimListOfRef(const PrimListOfRefToConst< E >& aList, CloneFunction cloneFunction)
	{ _list.clone(functions(), aList._list, (PrimListOfRefImp::CloneFunction)cloneFunction); }
	explicit PrimListOfRef(const PrimMapOfRef< E >& aMap)
	{ _list.assign(functions(), (const PrimMapOfRefImp&)aMap); }
	~PrimListOfRef() { _list.vacate(functions()); }
	PrimListOfRef< E >& operator=(const PrimListOfRef< E >& aList)
	{ _list.assign(functions(), aList._list); return *this; }
	PrimListOfRef< E >& operator=(const PrimMapOfRef< E >& aMap)
	{ _list.assign(functions(), aMap._map); return *this; }
	PrimRef< E >& operator[](size_t anIndex) { return contents()[anIndex]; }
	const PrimRef< E >& operator[](size_t anIndex) const { return contents()[anIndex]; }
	PrimListOfRef< E >& operator+=(E& anObject)
	{ _list.add(functions(), (const PrimRefValue&)anObject); return *this; }
	PrimListOfRef< E >& operator+=(const PrimListOfRef< E >& aList)
	{ _list.add(functions(), aList._list); return *this; }
	bool add(E& anElement) { return _list.add(functions(), (const PrimRefValue&)anElement); }
	bool add(const PrimListOfRef< E >& aList) { return _list.add(functions(), aList._list); }
	bool adopt(E& anObject) { return _list.adopt(functions(), (const PrimRefValue&)anObject); }
	bool adopt(E* anObject) { return anObject ? adopt(*anObject) : false; }
	operator PrimListOfRefToConst< E >& () { return *(PrimListOfRefToConst< E >*)this; }
	operator const PrimListOfRefToConst< E >& () const { return *(const PrimListOfRefToConst< E >*)this; }
	//
	//  	Report the allocated number of list elements.
	//
	size_t capacity() const { return _list.capacity(); }
	//
	//  	Perform a consistency check on the list, enthusiastically if fullCheck.
	//
	bool check(bool fullCheck = false) const { return _list.check(functions(), fullCheck); }
	E* find(const E& anElement) const { return (E*)_list.find(functions(), (const PrimRefValue&)anElement); }
	bool insert_before(E& anElement, unsigned int anIndex = 0)
	{ return _list.insert_before(functions(), (const PrimRefValue&)anElement, anIndex); }
	std::ostream& print_deep(std::ostream& s, int aDepth = 0) const
	{ return _list.print_deep(functions(), s, aDepth); }
	std::ostream& print_depth(std::ostream& s, int aDepth = 0) const
	{ return _list.print_depth(functions(), s, aDepth); }
	std::ostream& print_instance(std::ostream& s) const { return _list.print_instance(functions(), s); }
	std::ostream& print_members(std::ostream& s, int aDepth = 0) const
	{ return _list.print_members(functions(), s, aDepth); }
	std::ostream& print_on(std::ostream& s, int aDepth = 0) const
	{ return _list.print_on(functions(), s, aDepth); }
	std::ostream& print_this(std::ostream& s) const { return _list.print_this(functions(), s); }
	bool remove(const E& anElement) { return _list.remove(functions(), (const PrimRefValue&)anElement); }
	bool remove(size_t anIndex) { return _list.remove(functions(), anIndex); }
	//
	//  	Increase the capacity to aCapacity, returning true if successful. The tally and contents are
	//  	preserved.
	//
	bool set_capacity(size_t aCapacity) { return _list.set_capacity(functions(), aCapacity);  }
	//
	//  	Change the tally to aTally, returning true if successful. The list is expanded if necessary,
	//  	redundant contents are eliminated, residual contents are untouched, new contents are zero pointers.
	//
	bool set_tally(size_t aTally) { return _list.set_tally(functions(), aTally);  }
	void sort(int (*compareRoutine)(const PrimRefToConst< E >* v1, const PrimRefToConst< E >* v2))
	{ _list.sort((int (*)(const void* v1, const void* v2))compareRoutine, 0, tally()); }
	void sort(int (*compareRoutine)(const PrimRefToConst< E >* v1, const PrimRefToConst< E >* v2),
	          size_t startIndex, size_t numIndexes)
	{ _list.sort((int (*)(const void* v1, const void* v2))compareRoutine, startIndex, numIndexes); }
	//
	//  	Report the number of pointers.
	//
	size_t tally() const { return _list.tally(); }
	const PrimListOfRefToConst< E >& to_const() const { return *(const PrimListOfRefToConst< E >*)this; }
	//
	//  	Empty the list.
	//
	void vacate() { _list.vacate(functions()); }
	friend bool operator==(const PrimListOfRef< E >& firstList, const PrimListOfRef< E >& secondList)
	{ return firstList._list.is_equal(PrimListOfRef< E >::functions(), secondList._list); }
	friend bool operator!=(const PrimListOfRef< E >& firstList, const PrimListOfRef< E >& secondList)
	{ return !operator==(firstList, secondList); }
	friend std::ostream& operator<<(std::ostream& s, const PrimListOfRef< E >& anObject)
	{ return anObject.print_instance(s); }
	friend inline void annul(PrimListOfRef< E >* anObject) { if (anObject) delete anObject; }
};

#define PRIMLISTOFREF_CODE(E)
#define PRIMLISTOFREF_DATA(E)

template <class E>
class PrimConstList : public PrimPrint {
	TYPEDECL_SINGLE(PrimConstList< E >, PrimPrint)
	PRIMSHARES_DECLS(PrimConstList< E >)
	PRIMREF_DECLS(PrimConstList< E >)
private:
	PrimListOfRefToConst< E > _list;        //   The shared list.
	
private:
	PrimConstList(const PrimConstList< E >& aList);    //   Not yet implemented.
	PrimConstList& operator=(const PrimConstList< E >& aList); //   Not yet implemented.
	
public:
	//
	//  	Construct an empty shared list.
	//
	PrimConstList() {}
	//
	//  	Construct a shared list with initialCapacity.
	//
	explicit PrimConstList(const PrimSize& initialCapacity) : _list(initialCapacity) {}
	//
	//  	Construct a shared list of initially containing anElement.
	//
	explicit PrimConstList(const E& anElement) { _list.add(anElement); }
	//
	//  	The destructor adds no functionality.
	//
	virtual ~PrimConstList() {}
	void add(const E& anElement) { _list.add(anElement); }
	//
	//  	Return the current allocated capacity.
	//
	size_t capacity() const { return _list.capacity(); }
	virtual bool check(bool fullCheck = false) const;
	//
	//  	Report the list.
	//
	const PrimListOfRefToConst< E >& list() const { return _list; }
	PrimListOfRefToConst< E >& list() { return _list; }
	virtual std::ostream& print_depth(std::ostream& s, int aDepth = 0) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth = 0) const;
	virtual std::ostream& print_this(std::ostream& s) const;
	//
	//  	Change the tally to aTally, returning true if successful. The list is expanded if necessary,
	//  	redundant contents are eliminated, residual contents are untouched, new contents are zero pointers.
	//
	bool set_tally(size_t aTally) { return _list.set_tally(aTally);  }
	//
	//  	Return the number of tallied entries.
	//
	size_t tally() const { return _list.tally(); }
	//
	//  	Dispose of the entire contents.
	//
	void vacate() { _list.vacate(); }
};

#define PRIMCONSTLIST_CODE(E) \
	bool PrimConstList< E >::check(bool fullCheck) const \
	{ if (!PrimPrint::check(fullCheck)) return false; return _list.check(fullCheck); } \
	std::ostream& PrimConstList< E >::print_depth(std::ostream& s, int aDepth) const \
	{ return _list.print_deep(s, aDepth); } \
	std::ostream& PrimConstList< E >::print_members(std::ostream& s, int aDepth) const \
	{ return _list.print_on(s, aDepth); } \
	std::ostream& PrimConstList< E >::print_this(std::ostream& s) const \
	{ return _list.print_instance(s); }

#define PRIMCONSTLIST_DATA(E) \
	TYPEINFO_SINGLE(PrimConstList< E >, PrimPrint)

template <class E>
class PrimList : public PrimPrint {
	TYPEDECL_SINGLE(PrimList< E >, PrimPrint)
	PRIMSHARES_DECLS(PrimList< E >)
	PRIMREF_DECLS(PrimList< E >)
private:
	PrimListOfRef< E > _list;        //   The shared list.
	
private:
	PrimList(const PrimList< E >& aList);     //   Not yet implemented.
	PrimList& operator=(const PrimList< E >& aList);  //   Not yet implemented.
	
public:
	//
	//  	Construct an empty shared list.
	//
	PrimList() {}
	//
	//  	Construct a shared list with initialCapacity.
	//
	explicit PrimList(const PrimSize& initialCapacity) : _list(initialCapacity) {}
	//
	//  	Construct a shared list of initially containing anElement.
	//
	explicit PrimList(E& anElement) { _list.add(anElement); }
	//
	//  	The destructor adds no functionality.
	//
	virtual ~PrimList() {}
	void add(E& anElement) { _list.add(anElement); }
	//
	//  	Return the current allocated capacity.
	//
	size_t capacity() const { return _list.capacity(); }
	virtual bool check(bool fullCheck = false) const;
	//
	//  	Report the list of expressions.
	//
	const PrimListOfRefToConst< E >& list() const { return _list.to_const(); }
	PrimListOfRef< E >& list() { return _list; }
	virtual std::ostream& print_depth(std::ostream& s, int aDepth = 0) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth = 0) const;
	virtual std::ostream& print_this(std::ostream& s) const;
	//
	//  	Change the tally to aTally, returning true if successful. The list is expanded if necessary,
	//  	redundant contents are eliminated, residual contents are untouched, new contents are zero pointers.
	//
	bool set_tally(size_t aTally) { return _list.set_tally(aTally);  }
	//
	//  	Return the number of tallied entries.
	//
	size_t tally() const { return _list.tally(); }
	//
	//  	Dispose of the entire contents.
	//
	void vacate() { _list.vacate(); }
};

#define PRIMLIST_CODE(E) \
	bool PrimList< E >::check(bool fullCheck) const \
	{ if (!PrimPrint::check(fullCheck)) return false; return _list.check(fullCheck); } \
	std::ostream& PrimList< E >::print_depth(std::ostream& s, int aDepth) const \
	{ return _list.print_deep(s, aDepth); } \
	std::ostream& PrimList< E >::print_members(std::ostream& s, int aDepth) const \
	{ return _list.print_on(s, aDepth); } \
	std::ostream& PrimList< E >::print_this(std::ostream& s) const \
	{ return _list.print_instance(s); }

#define PRIMLIST_DATA(E) \
	TYPEINFO_SINGLE(PrimList< E >, PrimPrint)

//
//  	Define an iterator PrimListOfRefToConstIterator< E > over a PrimListOfRefToConst< E >. The iterator largely
//  	hides the indirection implicit in the list implementation, such that *p, and p-> access the list content.
//  	Test of the iterator tests the iteration bounds. The iterator may only be used when valid.
//  	Use as:
//
//  	for (PrimListOfRefToConstIterator< E > p(aList); p; p++)
//  		p->do_something_const();
//
//  	The ref() method permits the embedded list pointer to be accessed.
//
template <class E>
class PrimListOfRefToConstIterator {
private:
	PrimRefToConst< E >* _current;
	size_t _to_go;
public:
	PrimListOfRefToConstIterator() : _to_go(0) {}
	explicit PrimListOfRefToConstIterator(PrimRefToConst< E >& anElement)
		: _current(&anElement), _to_go(1) {}
	explicit PrimListOfRefToConstIterator(PrimListOfRefToConst< E >& aList)
		: _current(aList.contents()), _to_go(aList.tally()) {}
	explicit PrimListOfRefToConstIterator(PrimListOfRef< E >& aList)
		: _current((PrimRefToConst< E >*)aList.contents()), _to_go(aList.tally()) {}
	PrimListOfRefToConstIterator& operator=(PrimRefToConst< E >& anElement)
	{ _to_go = 1; _current = &anElement; return *this; }
	PrimListOfRefToConstIterator& operator=(PrimListOfRefToConst< E >& aList)
	{ _to_go = aList.tally(); _current = aList.contents(); return *this; }
	PrimListOfRefToConstIterator& operator=(PrimListOfRef< E >& aList)
	{ _to_go = aList.tally(); _current = (PrimRefToConst< E >*)aList.contents(); return *this; }
	const E& operator*() const { return _current->element(); }
	const E* operator->() const { return _current->pointer(); }
	void operator++() { if (_to_go > 0) { --_to_go; _current++; } }
	void operator++(int) { if (_to_go > 0) { --_to_go; _current++; } }
	operator size_t() const { return _to_go; }
	bool operator!() const { return _to_go ? false : true; }
	const E& element() const { return _current->element(); }
	const E* pointer() const { return _current->pointer(); }
	PrimRefToConst< E >& ref() { return *_current; }
	size_t to_go() const { return _to_go; }
};

//
//  	Define an iterator PrimConstListOfRefToConstIterator< E > over a PrimListOfRefToConst< E >. The iterator
//  	hides the indirection implicit in the list implementation, such that *p, and p-> access the list content.
//  	Test of the iterator tests the iteration bounds. The iterator may only be used when valid.
//  	Use as:
//
//  	for (PrimConstListOfRefToConstIterator< E > p(aList); p; p++)
//  		p->do_something_const();
//
template <class E>
class PrimConstListOfRefToConstIterator {
private:
	const PrimRefToConst< E >* _current;
	size_t _to_go;
public:
	PrimConstListOfRefToConstIterator() : _to_go(0) {}
	explicit PrimConstListOfRefToConstIterator(const PrimRefToConst< E >& anElement)
		: _current(&anElement), _to_go(1) {}
	explicit PrimConstListOfRefToConstIterator(const PrimListOfRefToConst< E >& aList)
		: _current(aList.contents()), _to_go(aList.tally()) {}
	explicit PrimConstListOfRefToConstIterator(const PrimListOfRef< E >& aList)
		: _current((const PrimRefToConst< E >*)aList.contents()), _to_go(aList.tally()) {}
	PrimConstListOfRefToConstIterator& operator=(const PrimRefToConst< E >& anElement)
	{ _to_go = 1; _current = &anElement; return *this; }
	PrimConstListOfRefToConstIterator& operator=(const PrimListOfRefToConst< E >& aList)
	{ _to_go = aList.tally(); _current = aList.contents(); return *this; }
	PrimConstListOfRefToConstIterator& operator=(const PrimListOfRef< E >& aList)
	{ _to_go = aList.tally(); _current = (const PrimRefToConst< E >*)aList.contents(); return *this; }
	const E& operator*() const { return _current->element(); }
	const E* operator->() const { return _current->pointer(); }
	void operator++() { if (_to_go > 0) { --_to_go; _current++; } }
	void operator++(int) { if (_to_go > 0) { --_to_go; _current++; } }
	operator size_t() const { return _to_go; }
	bool operator!() const { return _to_go ? false : true; }
	const E& element() const { return _current->element(); }
	const E* pointer() const { return _current->pointer(); }
	//  	const PrimRefToConst< E >& ref() { return *_current; }
	size_t to_go() const { return _to_go; }
};

//
//  	Define an iterator PrimListOfRefIterator< E > over a PrimListOfRef< E >. The iterator largely hides the
//  	indirection implicit in the list implementation, such that *p, and p-> access the list content.
//  	Test of the iterator tests the iteration bounds. The iterator may only be used when valid.
//  	Use as:
//
//  	for (PrimListOfRefIterator< E > p(aList); p; p++)
//  		p->do_something();
//
//  	The ref() method permits the embedded list pointer to be accessed.
//
template <class E>
class PrimListOfRefIterator {
private:
	PrimRef< E >* _current;
	size_t _to_go;
public:
	PrimListOfRefIterator() : _to_go(0) {}
	explicit PrimListOfRefIterator(PrimRef< E >& anElement)
		: _current(&anElement), _to_go(1) {}
	explicit PrimListOfRefIterator(PrimListOfRef< E >& aList)
		: _current(aList.contents()), _to_go(aList.tally()) {}
	PrimListOfRefIterator& operator=(PrimRef< E >& anElement)
	{ _to_go = 1; _current = &anElement; return *this; }
	PrimListOfRefIterator& operator=(PrimListOfRef< E >& aList)
	{ _to_go = aList.tally(); _current = aList.contents(); return *this; }
	E& operator*() const { return _current->element(); }
	E* operator->() const { return _current->pointer(); }
	void operator++() { if (_to_go > 0) { --_to_go; _current++; } }
	void operator++(int) { if (_to_go > 0) { --_to_go; _current++; } }
	operator size_t() const { return _to_go; }
	bool operator!() const { return _to_go ? false : true; }
	E& element() const { return _current->element(); }
	E* pointer() const { return _current->pointer(); }
	PrimRef< E >& ref() { return *_current; }
	size_t to_go() const { return _to_go; }
};

//
//  	Define an iterator PrimConstListOfRefIterator< E > over a PrimListOfRef< E >. The iterator hides the
//  	indirection implicit in the list implementation, such that *p, and p-> access the list content.
//  	Test of the iterator tests the iteration bounds. The iterator may only be used when valid.
//  	Use as:
//
//  	for (PrimConstListOfRefIterator< E > p(aList); p; p++)
//  		p->do_something();
//
template <class E>
class PrimConstListOfRefIterator {
private:
	const PrimRef< E >* _current;
	size_t _to_go;
public:
	PrimConstListOfRefIterator() : _to_go(0) {}
	explicit PrimConstListOfRefIterator(const PrimRef< E >& anElement)
		: _current(&anElement), _to_go(1) {}
	explicit PrimConstListOfRefIterator(const PrimListOfRef< E >& aList)
		: _current(aList.contents()), _to_go(aList.tally()) {}
	PrimConstListOfRefIterator& operator=(const PrimRef< E >& anElement)
	{ _to_go = 1; _current = &anElement; return *this; }
	PrimConstListOfRefIterator& operator=(const PrimListOfRef< E >& aList)
	{ _to_go = aList.tally(); _current = aList.contents(); return *this; }
	E& operator*() const { return _current->element(); }
	E* operator->() const { return _current->pointer(); }
	void operator++() { if (_to_go > 0) { --_to_go; _current++; } }
	void operator++(int) { if (_to_go > 0) { --_to_go; _current++; } }
	operator size_t() const { return _to_go; }
	bool operator!() const { return _to_go ? false : true; }
	E& element() const { return _current->element(); }
	E* pointer() const { return _current->pointer(); }
	//  	const PrimRef< E >& ref() { return *_current; }
	size_t to_go() const { return _to_go; }
};

#endif

