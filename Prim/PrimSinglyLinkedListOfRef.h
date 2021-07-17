

#ifndef PRIMSINGLYLINKEDLISTOFREF_H
#define PRIMSINGLYLINKEDLISTOFREF_H






#define PRIMSINGLYLINKEDLISTOFREFTOCONST_CLASS(E) typedef PrimSinglyLinkedListOfRefToConst<E> name2(E,SinglyLinkedListOfRefToConst);
#define PRIMSINGLYLINKEDLISTOFREF_CLASS(E) typedef PrimSinglyLinkedListOfRef<E> name2(E,SinglyLinkedListOfRef);
#define PRIMSINGLYLINKEDLISTOFREFTOCONSTITERATOR_CLASS(E) \
	typedef PrimSinglyLinkedListOfRefToConstIterator<E> name2(E,SinglyLinkedListOfRefToConstIterator);
#define PRIMCONSTSINGLYLINKEDLISTOFREFTOCONSTITERATOR_CLASS(E) \
	typedef PrimConstSinglyLinkedListOfRefToConstIterator<E> name2(E,ConstSinglyLinkedListOfRefToConstIterator);
#define PRIMSINGLYLINKEDLISTOFREFITERATOR_CLASS(E) \
	typedef PrimSinglyLinkedListOfRefIterator<E> name2(E,SinglyLinkedListOfRefIterator);
#define PRIMCONSTSINGLYLINKEDLISTOFREFITERATOR_CLASS(E) \
	typedef PrimConstSinglyLinkedListOfRefIterator<E> name2(E,ConstSinglyLinkedListOfRefIterator);

#define PRIMSINGLYLINKEDLISTOFREFS_CODE(E) PRIMSINGLYLINKEDLISTOFREFTOCONST_CODE(E) PRIMSINGLYLINKEDLISTOFREF_CODE(E)
#define PRIMSINGLYLINKEDLISTOFREFS_DATA(E) PRIMSINGLYLINKEDLISTOFREFTOCONST_DATA(E) PRIMSINGLYLINKEDLISTOFREF_DATA(E)

#define PRIMSINGLYLINKEDLISTOFREFTOCONST_IMPL(E) PRIMSINGLYLINKEDLISTOFREFTOCONST_DATA(E) PRIMSINGLYLINKEDLISTOFREFTOCONST_CODE(E)
#define PRIMSINGLYLINKEDLISTOFREF_IMPL(E) PRIMSINGLYLINKEDLISTOFREF_DATA(E) PRIMSINGLYLINKEDLISTOFREF_CODE(E)
#define PRIMSINGLYLINKEDLISTOFREFS_IMPL(E) PRIMSINGLYLINKEDLISTOFREFTOCONST_IMPL(E) PRIMSINGLYLINKEDLISTOFREF_IMPL(E)

//
//  	A PrimSinglyLinkedListOfRefElement is the private list element used by PrimSinglyLinkedListOfRefImp to
//  	organise a non-intrusive singly linked list of shared objects. The class is private to
//  	PrimSinglyLinkedListOfRefImp.
//
class PrimSinglyLinkedListOfRefElement {
	friend class PrimSinglyLinkedListOfRefImp;
public:
	const PrimRefValue* _element;     //   The element shared as a PrimRefToConst<E>, never 0.
	PrimSinglyLinkedListOfRefElement* _next;    //   Successor, 0 at list end.
private:
	PrimSinglyLinkedListOfRefElement(const PrimSinglyLinkedListOfRefElement& aRef);    //   Not implemented
	PrimSinglyLinkedListOfRefElement& operator=(const PrimSinglyLinkedListOfRefElement& aRef); //   Not implemented
private:
	PrimSinglyLinkedListOfRefElement(const PrimRefFuncs& supportFunctions, const PrimRefValue& anElement);
	~PrimSinglyLinkedListOfRefElement();
	const PrimRefValue& element() const { return *_element; }
};

//
//  	A PrimSinglyLinkedListOfRefImp maintains a singly linked list of references in an abstract form
//  	so that derived classes may refine the list maintenance to suit the particular type of contained element.
//
//  	The contents are maintained as a non-intrusive single linked list of PrimSinglyLinkedListOfRefElement.
//  	In order to avoid virtual inheritance of reference classes, derived list of reference
//  	classes must implement certain pointer manipulations using the appropriate types.
//
//  	Each singly linked element maintains a share of the contained object.
//
class PrimSinglyLinkedListOfRefImp {
private:
	PrimSinglyLinkedListOfRefElement* _head;    //   Start of list.
	PrimSinglyLinkedListOfRefElement* _tail;    //   End of list.
	size_t _tally;           //   Number of elements in list.
	
private:
	//
	//  	Construction of a copy can only be performed by derived classes.
	//
	PrimSinglyLinkedListOfRefImp(const PrimSinglyLinkedListOfRefImp& aRef);
	PrimSinglyLinkedListOfRefElement** find(size_t anIndex);
	PrimSinglyLinkedListOfRefElement** find(const PrimRefValue& anElement);
	const PrimSinglyLinkedListOfRefElement** find(const PrimRefValue& anElement) const
	{ return (const PrimSinglyLinkedListOfRefElement**)((PrimSinglyLinkedListOfRefImp*)this)->find(anElement); }
	bool insert_at(const PrimRefFuncs& supportFunctions, const PrimRefValue& anElement,
	               PrimSinglyLinkedListOfRefElement** pp);
	bool remove_at(const PrimRefFuncs& supportFunctions, PrimSinglyLinkedListOfRefElement** pp);
	
public:
	typedef PrimRefValue* (*CloneFunction)(const PrimRefValue&);
	//
	//  	Create an empty list.
	//
	PrimSinglyLinkedListOfRefImp() : _head(0), _tail(0), _tally(0) {}
	~PrimSinglyLinkedListOfRefImp();
	//
	//  	Return a reference to the anIndex'th pointer.
	//
	const PrimRefValue* operator[](size_t anIndex) const;
	PrimRefValue* operator[](size_t anIndex)
	{ return (PrimRefValue*)((const PrimSinglyLinkedListOfRefImp*)this)->operator[](anIndex); }
	//
	//  	Add anElement to the list tail.
	//
	bool add_tail(const PrimRefFuncs& supportFunctions, const PrimRefValue& anElement)
	{ return insert_at(supportFunctions, anElement, 0); }
	bool adopt_tail(const PrimRefFuncs& supportFunctions, const PrimRefValue& anObject);
	void assign(const PrimRefFuncs& supportFunctions, const PrimSinglyLinkedListOfRefImp& aList);
	void assign(const PrimRefFuncs& supportFunctions, const PrimListOfRefImp& aList);
	void assign(const PrimRefFuncs& supportFunctions, const PrimMapOfRefImp& aMap);
	bool check(const PrimRefFuncs& supportFunctions, bool fullCheck = false) const;
	void clone(const PrimRefFuncs& supportFunctions, const PrimSinglyLinkedListOfRefElement& somePointers,
	           CloneFunction cloneFunction);
	const PrimRefValue* find(const PrimRefFuncs& supportFunctions, const PrimRefValue& anElement) const;
	bool insert_before(const PrimRefFuncs& supportFunctions, const PrimRefValue& anElement, size_t anIndex = 0);
	bool is_equal(const PrimRefFuncs& supportFunctions, const PrimSinglyLinkedListOfRefElement& aList) const;
	std::ostream& print_deep(const PrimRefFuncs& supportFunctions, std::ostream& s, int aDepth) const;
	std::ostream& print_depth(const PrimRefFuncs& supportFunctions, std::ostream& s, int aDepth) const;
	std::ostream& print_instance(const PrimRefFuncs& supportFunctions, std::ostream& s) const;
	std::ostream& print_members(const PrimRefFuncs& supportFunctions, std::ostream& s, int aDepth) const;
	std::ostream& print_on(const PrimRefFuncs& supportFunctions, std::ostream& s, int aDepth) const;
	std::ostream& print_this(const PrimRefFuncs& supportFunctions, std::ostream& s) const;
	bool remove(const PrimRefFuncs& supportFunctions, const PrimRefValue& anElement);
	bool remove(const PrimRefFuncs& supportFunctions, size_t anIndex);
	void sort(int (*compareRoutine)(const void* v1, const void* v2));
	//
	//  	Report the number of pointers.
	//
	size_t tally() const { return _tally; }
	void vacate(const PrimRefFuncs& supportFunctions);
	friend std::ostream& operator<<(std::ostream& s, const PrimSinglyLinkedListOfRefImp& aList);
};

template <class E>
class PrimSinglyLinkedListOfRefToConst {
	friend class PrimConstSinglyLinkedListOfRefToConstIterator<E>;
	friend class PrimSinglyLinkedListOfRefToConstIterator<E>;
	friend class PrimSinglyLinkedListOfRef<E>;
private:
	PrimSinglyLinkedListOfRefImp _list;         //   The list using cast-down types.
private:
	static const PrimRefToConstFunctions<E>& functions() { return PrimRefToConst<E>::_functions; }
public:
	PrimSinglyLinkedListOfRefToConst() {}
	~PrimSinglyLinkedListOfRefToConst() { _list.vacate(functions()); }
	PrimSinglyLinkedListOfRefToConst<E>& operator=(const PrimSinglyLinkedListOfRefToConst<E>& aList)
	{ _list.assign(functions(), aList._list); return *this; }
	PrimSinglyLinkedListOfRefToConst<E>& operator=(const PrimMapOfRefToConst<E>& aMap)
	{ _list.assign(functions(), aMap._map); return *this; }
	const E& operator[](size_t anIndex) const {
		const PrimRefValue* p = _list[anIndex];
		return p ? *(const E*)p : E::immutable_null();
	}
	bool add(const E& anElement) { return _list.add_tail(functions(), (const PrimRefValue&)anElement); }
	bool add(const PrimSinglyLinkedListOfRefToConst<E>& aList) { return _list.add_tail(functions(), aList._list); }
	bool adopt(const E& anObject) { return _list.adopt_tail(functions(), (const PrimRefValue&)anObject); }
	bool adopt(typename E::ConstThat* anObject) { return anObject ? adopt_tail(*anObject) : false; }
	//
	//  	Perform a consistency check on the list, enthusiastically if fullCheck.
	//
	bool check(bool fullCheck = false) const { return _list.check(functions(), fullCheck); }
	const E* find(const E& anElement) const
	{ return (const E*)_list.find(functions(), (const PrimRefValue&)anElement); }
	bool insert_before(const E& anElement, size_t anIndex = 0)
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
	void sort(int (*compareRoutine)(const PrimRefToConst<E>* v1, const PrimRefToConst<E>* v2))
	{ _list.sort((int (*)(const void* v1, const void* v2))compareRoutine); }
	//
	//  	Report the number of pointers.
	//
	size_t tally() const { return _list.tally(); }
	const PrimSinglyLinkedListOfRefToConst<E>& to_const() const { return *this; }
	//
	//  	Empty the list.
	//
	void vacate() { _list.vacate(functions()); }
	friend bool operator==(const PrimSinglyLinkedListOfRefToConst<E>& firstSinglyLinkedList,
	                       const PrimSinglyLinkedListOfRefToConst<E>& secondSinglyLinkedList)
	{ return firstSinglyLinkedList._list.is_equal(PrimSinglyLinkedListOfRefToConst<E>::functions(), secondSinglyLinkedList._list); }
	friend bool operator!=(const PrimSinglyLinkedListOfRefToConst<E>& firstSinglyLinkedList,
	                       const PrimSinglyLinkedListOfRefToConst<E>& secondSinglyLinkedList)
	{ return !operator==(firstSinglyLinkedList, secondSinglyLinkedList); }
	friend std::ostream& operator<<(std::ostream& s, const PrimSinglyLinkedListOfRefToConst<E>& anObject)
	{ return anObject.print_instance(s); }
};

#define PRIMSINGLYLINKEDLISTOFREFTOCONST_CODE(E)
#define PRIMSINGLYLINKEDLISTOFREFTOCONST_DATA(E)

template <class E>
class PrimSinglyLinkedListOfRef {
	friend class PrimConstSinglyLinkedListOfRefToConstIterator<E>;
	friend class PrimConstSinglyLinkedListOfRefIterator<E>;
	friend class PrimSinglyLinkedListOfRefToConstIterator<E>;
	friend class PrimSinglyLinkedListOfRefIterator<E>;
private:
	PrimSinglyLinkedListOfRefImp _list;       //   The list using cast-down types.
private:
	static const PrimRefFunctions<E>& functions() { return PrimRef<E>::_functions; }
public:
	typedef E* (*CloneFunction)(const E&, PrimPrint&);
	PrimSinglyLinkedListOfRef() {}
	PrimSinglyLinkedListOfRef(const PrimSinglyLinkedListOfRef<E>& aList)
	{ _list.assign(functions(), aList._list); }
	PrimSinglyLinkedListOfRef(const PrimSinglyLinkedListOfRefToConst<E>& aList, CloneFunction cloneFunction)
	{ _list.clone(functions(), aList._list, (PrimSinglyLinkedListOfRefImp::CloneFunction)cloneFunction); }
	~PrimSinglyLinkedListOfRef() { _list.vacate(functions()); }
	PrimSinglyLinkedListOfRef<E>& operator=(const PrimSinglyLinkedListOfRef<E>& aList)
	{ _list.assign(functions(), aList._list); return *this; }
	E& operator[](size_t anIndex) { PrimRefValue* p = _list[anIndex]; return p ? *(E*)p : E::mutable_null(); }
	const PrimRef<E>& operator[](size_t anIndex) const {
		const PrimRefValue* p = find_at(functions(), anIndex);
		return p ? *(const PrimRef<E>*)p : PrimRef<E>::null();
	}
	bool add(E& anElement) { return _list.add_tail(functions(), (const PrimRefValue&)anElement); }
	bool add(const PrimSinglyLinkedListOfRef<E>& aList) { return _list.add_tail(functions(), aList._list); }
	bool adopt(E& anObject) { return _list.adopt_tail(functions(), (const PrimRefValue&)anObject); }
	bool adopt(typename E::That* anObject) { return anObject ? adopt_tail(*anObject) : false; }
	operator const PrimSinglyLinkedListOfRefToConst<E>& () const
	{ return *(const PrimSinglyLinkedListOfRefToConst<E>*)this; }
	//
	//  	Perform a consistency check on the list, enthusiastically if fullCheck.
	//
	bool check(bool fullCheck = false) const { return _list.check(functions(), fullCheck); }
	E* find(const E& anElement) const { return (E*)_list.find(functions(), (const PrimRefValue&)anElement); }
	bool insert_before(E& anElement, size_t anIndex = 0)
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
	void sort(int (*compareRoutine)(const PrimRefToConst<E>* v1, const PrimRefToConst<E>* v2))
	{ _list.sort((int (*)(const void* v1, const void* v2))compareRoutine); }
	//
	//  	Report the number of pointers.
	//
	size_t tally() const { return _list.tally(); }
	const PrimSinglyLinkedListOfRefToConst<E>& to_const() const
	{ return *(const PrimSinglyLinkedListOfRefToConst<E>*)this; }
	//
	//  	Empty the list.
	//
	void vacate() { _list.vacate(functions()); }
	friend bool operator==(const PrimSinglyLinkedListOfRef<E>& firstSinglyLinkedList,
	                       const PrimSinglyLinkedListOfRef<E>& secondSinglyLinkedList) {
		return firstSinglyLinkedList._list.is_equal(PrimSinglyLinkedListOfRef<E>::functions(),
		        secondSinglyLinkedList._list);
	}
	friend bool operator!=(const PrimSinglyLinkedListOfRef<E>& firstSinglyLinkedList,
	                       const PrimSinglyLinkedListOfRef<E>& secondSinglyLinkedList)
	{ return !operator==(firstSinglyLinkedList, secondSinglyLinkedList); }
	friend std::ostream& operator<<(std::ostream& s, const PrimSinglyLinkedListOfRef<E>& anObject)
	{ return anObject.print_instance(s); }
	
};

#define PRIMSINGLYLINKEDLISTOFREF_CODE(E)
#define PRIMSINGLYLINKEDLISTOFREF_DATA(E)

//
//  	Define an iterator PrimSinglyLinkedListOfRefToConstIterator<E> over a PrimSinglyLinkedListOfRefToConst<E>.
//  	The iterator largely
//  	hides the indirection implicit in the list implementation, such that *p, and p-> access the list content.
//  	Test of the iterator tests the iteration bounds. The iterator may only be used when valid.
//  	Use as:
//
//  	for (PrimSinglyLinkedListOfRefToConstIterator<E> p(aList); p; p++)
//  		p->do_something_const();
//
//  	The ref() method permits the embedded list pointer to be accessed.
//
template <class E>
class PrimSinglyLinkedListOfRefToConstIterator {
private:
	PrimSinglyLinkedListOfRefElement* _current;
	class Locked {};
public:
	PrimSinglyLinkedListOfRefToConstIterator() : _current(0) {}
	PrimSinglyLinkedListOfRefToConstIterator(PrimSinglyLinkedListOfRefToConst<E>& aList)
		: _current(aList._head) {}
	PrimSinglyLinkedListOfRefToConstIterator(PrimSinglyLinkedListOfRef<E>& aList) : _current(aList._head) {}
	PrimSinglyLinkedListOfRefToConstIterator& operator=(PrimSinglyLinkedListOfRefToConst<E>& aList)
	{ _current = aList._head; return *this; }
	PrimSinglyLinkedListOfRefToConstIterator& operator=(PrimSinglyLinkedListOfRef<E>& aList)
	{ _current = aList._head; return *this; }
	typename E::ConstThat& operator*() const { return ((PrimRefToConst<E>*)_current->_element)->that(); }
	typename E::ConstThat* operator->() const { return &((PrimRefToConst<E>*)_current->_element)->that(); }
	void operator++() { if (_current) _current = _current->_next; }
	void operator++(int) { if (_current) _current = _current->_next; }
	operator const Locked* () const { return (const Locked*)_current; }
	bool operator!() const { return _current ? false : true; }
	const E& element() const { return ((PrimRefToConst<E>*)_current->_element)->element(); }
	const E* pointer() const { return ((PrimRefToConst<E>*)_current->_element)->pointer(); }
	PrimRefToConst<E>& ref() { return *((PrimRefToConst<E>*)_current->_element); }
};

//
//  	Define an iterator PrimConstSinglyLinkedListOfRefToConstIterator<E> over a
//  	PrimSinglyLinkedListOfRefToConst<E>. The iterator
//  	hides the indirection implicit in the list implementation, such that *p, and p-> access the list content.
//  	Test of the iterator tests the iteration bounds. The iterator may only be used when valid.
//  	Use as:
//
//  	for (PrimConstSinglyLinkedListOfRefToConstIterator<E> p(aList); p; p++)
//  		p->do_something_const();
//
template <class E>
class PrimConstSinglyLinkedListOfRefToConstIterator {
private:
	const PrimSinglyLinkedListOfRefElement* _current;
	class Locked {};
public:
	PrimConstSinglyLinkedListOfRefToConstIterator() : _current(0) {}
	PrimConstSinglyLinkedListOfRefToConstIterator(const PrimSinglyLinkedListOfRefToConst<E>& aList)
		: _current(aList._head) {}
	PrimConstSinglyLinkedListOfRefToConstIterator(const PrimSinglyLinkedListOfRef<E>& aList)
		: _current(aList._head) {}
	PrimConstSinglyLinkedListOfRefToConstIterator& operator=(const PrimSinglyLinkedListOfRefToConst<E>& aList)
	{ _current = aList._head; return *this; }
	PrimConstSinglyLinkedListOfRefToConstIterator& operator=(const PrimSinglyLinkedListOfRef<E>& aList)
	{ _current = aList._head; return *this; }
	typename E::ConstThat& operator*() const { return ((PrimRefToConst<E>*)_current->_element)->that(); }
	typename E::ConstThat* operator->() const { return &((PrimRefToConst<E>*)_current->_element)->that(); }
	void operator++() { if (_current) _current = _current->_next; }
	void operator++(int) { if (_current) _current = _current->_next; }
	operator const Locked* () const { return (const Locked*)_current; }
	bool operator!() const { return _current ? false : true; }
	const E& element() const { return ((PrimRefToConst<E>*)_current->_element)->element(); }
	const E* pointer() const { return ((PrimRefToConst<E>*)_current->_element)->pointer(); }
};

//
//  	Define an iterator PrimSinglyLinkedListOfRefIterator<E> over a PrimSinglyLinkedListOfRef<E>. The iterator largely hides the
//  	indirection implicit in the list implementation, such that *p, and p-> access the list content.
//  	Test of the iterator tests the iteration bounds. The iterator may only be used when valid.
//  	Use as:
//
//  	for (PrimSinglyLinkedListOfRefIterator<E> p(aList); p; p++)
//  		p->do_something();
//
//  	The ref() method permits the embedded list pointer to be accessed.
//
template <class E>
class PrimSinglyLinkedListOfRefIterator {
private:
	PrimSinglyLinkedListOfRefElement* _current;
	class Locked {};
public:
	PrimSinglyLinkedListOfRefIterator() : _current(0) {}
	PrimSinglyLinkedListOfRefIterator(PrimSinglyLinkedListOfRef<E>& aList) : _current(aList._head) {}
	PrimSinglyLinkedListOfRefIterator& operator=(PrimSinglyLinkedListOfRef<E>& aList)
	{ _current = aList._head; return *this; }
	typename E::That& operator*() const { return ((PrimRef<E>*)_current->_element)->that(); }
	typename E::That* operator->() const { return &((PrimRef<E>*)_current->_element)->that(); }
	void operator++() { if (_current) _current = _current->_next; }
	void operator++(int) { if (_current) _current = _current->_next; }
	operator const Locked* () const { return (const Locked*)_current; }
	bool operator!() const { return _current ? false : true; }
	E& element() const { return ((PrimRef<E>*)_current->_element)->element(); }
	E* pointer() const { return ((PrimRef<E>*)_current->_element)->pointer(); }
	PrimRef<E>& ref() { return *((PrimRef<E>*)_current->_element); }
};

//
//  	Define an iterator PrimConstSinglyLinkedListOfRefIterator<E> over a PrimSinglyLinkedListOfRef<E>. The iterator hides the
//  	indirection implicit in the list implementation, such that *p, and p-> access the list content.
//  	Test of the iterator tests the iteration bounds. The iterator may only be used when valid.
//  	Use as:
//
//  	for (PrimConstSinglyLinkedListOfRefIterator<E> p(aList); p; p++)
//  		p->do_something();
//
template <class E>
class PrimConstSinglyLinkedListOfRefIterator {
private:
	const PrimSinglyLinkedListOfRefElement* _current;
	class Locked {};
public:
	PrimConstSinglyLinkedListOfRefIterator() : _current(0) {}
	PrimConstSinglyLinkedListOfRefIterator(const PrimSinglyLinkedListOfRef<E>& aList)
		: _current(aList._head) {}
	PrimConstSinglyLinkedListOfRefIterator& operator=(const PrimSinglyLinkedListOfRef<E>& aList)
	{ _current = aList._head; return *this; }
	typename E::That& operator*() const { return ((PrimRef<E>*)_current->_element)->that(); }
	typename E::That* operator->() const { return &((PrimRef<E>*)_current->_element)->that(); }
	void operator++() { if (_current) _current = _current->_next; }
	void operator++(int) { if (_current) _current = _current->_next; }
	operator const Locked* () const { return (const Locked*)_current; }
	bool operator!() const { return _current ? false : true; }
	E& element() const { return ((PrimRef<E>*)_current->_element)->element(); }
	E* pointer() const { return ((PrimRef<E>*)_current->_element)->pointer(); }
};

#endif

