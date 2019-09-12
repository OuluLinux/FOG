
#ifndef PRIMMAPOFREF_H
#define PRIMMAPOFREF_H





#define PRIMMAPOFREFTOCONST_CLASS(E) typedef PrimMapOfRefToConst< E > name2(E,MapOfRefToConst);
#define PRIMMAPOFREF_CLASS(E) typedef PrimMapOfRef< E > name2(E,MapOfRef);
#define PRIMMAPOFREFTOCONSTITERATOR_CLASS(E) \
 typedef PrimMapOfRefToConstIterator< E > name2(E,MapOfRefToConstIterator);
#define PRIMMAPOFREFITERATOR_CLASS(E) \
 typedef PrimMapOfRefIterator< E > name2(E,MapOfRefIterator);
#define PRIMMAPOFREFS_CODE(E) PRIMMAPOFREFTOCONST_CODE(E) PRIMMAPOFREF_CODE(E)
#define PRIMMAPOFREFS_DATA(E) PRIMMAPOFREFTOCONST_DATA(E) PRIMMAPOFREF_DATA(E)

#define PRIMMAPOFREFTOCONST_IMPL(E) PRIMMAPOFREFTOCONST_CODE(E) PRIMMAPOFREFTOCONST_DATA(E)
#define PRIMMAPOFREF_IMPL(E) PRIMMAPOFREF_CODE(E) PRIMMAPOFREF_DATA(E)
#define PRIMMAPOFREFS_IMPL(E) PRIMMAPOFREFTOCONST_IMPL(E) PRIMMAPOFREF_IMPL(E)

#define PRIMSHAREDMAPOFREFTOCONST_IMPL(E) PRIMSHAREDMAPOFREFTOCONST_CODE(E) PRIMSHAREDMAPOFREFTOCONST_DATA(E)
#define PRIMSHAREDMAPOFREF_IMPL(E) PRIMSHAREDMAPOFREF_CODE(E) PRIMSHAREDMAPOFREF_DATA(E)
#define PRIMSHAREDMAPOFREFS_IMPL(E) PRIMSHAREDMAPOFREFTOCONST_IMPL(E) PRIMSHAREDMAPOFREF_IMPL(E)

#define PRIMCONSTMAP_IMPL(E) PRIMCONSTMAP_CODE(E) PRIMCONSTMAP_DATA(E)
#define PRIMMAP_IMPL(E) PRIMMAP_CODE(E) PRIMMAP_DATA(E)
#define PRIMMAPS_IMPL(E) PRIMCONSTMAP_IMPL(E) PRIMMAP_IMPL(E)

//
//  	The PrimMapOfRefFuncs class defines the maintenance functions used by PrimMapOfRefImp.
//  	The functions are declared as statics to cast-down objects to avoid problems and costs
//  	associated with instantiated template virtual function tables.
//
//  .limits
//  	Use of a POD is efficient and the easiest way to avoid construction order conflicts.
//

struct PrimMapOfRefFuncs {

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
//  	Derived classes must report the id of anObject.
//
	const PrimId& (*id)(const PrimRefValue& anObject);
//
//  	Print anElement to a stream with aDepth of indentation.
//
	std::ostream& (*print_depth)(const PrimRefValue& anElement, std::ostream& s, int aDepth);
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
//  	The PrimMapOfRefToConstFunctions template defines the support functions required to permit
//  	a PrimMapOfRefImp to invoke the correct E::methods. This table an implementation of the
//  	virtual function table for a PrimRefToConst< E > but which avoids requiring extra object in the
//  	pointer class.
//
//  .limits
//  	Use of a POD is efficient and the easiest way to avoid construction order conflicts.
//
template <class E>

struct PrimMapOfRefToConstFunctions {

public:
	PrimMapOfRefFuncs _functions;
	
public:
	static void annul(const E& anElement);
	static bool check(const E& anElement, bool fullCheck);
	static const PrimId& id(const E& anElement);
	static std::ostream& print_depth(const E& anElement, std::ostream& s, int aDepth);
	static std::ostream& print_instance(const E& anElement, std::ostream& s);
	static void share(const E& anElement);
	static const E& shared_null();
	
public:
//
//  	Access returns the table of functions.
//
	operator const PrimMapOfRefFuncs& () const {
		return _functions;
	}
	
	static const PrimId& get_id(const E& anElement) {
		return anElement.id();
	}
};

//
//  	The PrimMapOfRefFunctions template defines the support functions required to permit
//  	a PrimMapOfRefImp to invoke the correct E::methods. This table an implementation of the
//  	virtual function table for a PrimRef< E > but which avoids requiring extra object in the
//  	pointer class.
//
//  .limits
//  	Use of a POD is efficient and the easiest way to avoid construction order conflicts.
//
template <class E>

struct PrimMapOfRefFunctions {

public:
	PrimMapOfRefFuncs _functions;
	
public:
	static E& shared_null();
	
public:
//
//  	Access returns the table of functions.
//
	operator const PrimMapOfRefFuncs& () const {
		return _functions;
	}
	
	static const PrimId& get_id(const E& anElement) {
		return PrimMapOfRefToConstFunctions<E>::get_id(anElement);
	}
};

//
//  	PrimMapOfRefImp defines the abstract behaviour of a map of objects. Derived classes
//  	implement the ref_xxx methods so that those operations that are dependent on the type of
//  	the map element are performed appropriately.
//
//  	A share of each contained object is maintained by the map so mapped types must support
//  	the share() and annul() protocol.
//
//  	The mapped typed are self named and so the mapped types must support the id() method.
//
//  	The foregoing restrictions can be relaxed if derived classes are hand crafted rather than using
//  	the PRIMMAPOFREFTOCONST_xxx and PRIMMAPOFREF_xxx macros for definition.
//

class PrimMapOfRefImp {

	friend class PrimMapOfRefImpIterator;  //   To access _contents.
	
	friend class PrimListOfRefImp;    //   To access _contents.
	
protected:
	enum AddMode {
		DEFAULT,        //   Error message on duplicate
		REPLACE,        //   Replace duplicate with new offering
		FILTER,         //   Discard duplicate new offering
		REHASH         //   Rehashing
	};
	
private:
	const PrimRefValue **_contents;    //   Hashed table of pointers to entries.
	size_t _tally;        //   Number of entries in table.
	size_t _capacity;       //   Maximum number of entries in table.
	
private:
//
//  	Construction of a copy must be performed by derived classes.
//
	explicit PrimMapOfRefImp(const PrimMapOfRefImp& aMap);
//
//  	Assignment must be performed by derived classes.
//
	PrimMapOfRefImp& operator=(const PrimMapOfRefImp& aMap);
	bool add(const PrimMapOfRefFuncs& supportFunctions, const PrimRefValue& anObject, AddMode addMode);
	const PrimRefValue *const *find_slot(const PrimMapOfRefFuncs& supportFunctions, const PrimId& anId) const;
//
//  	Locate the slot comprising the object for anId, returning 0 if not found.
//
	const PrimRefValue **find_slot(const PrimMapOfRefFuncs& supportFunctions, const PrimId& anId) {
		return (const PrimRefValue **)((const PrimMapOfRefImp *)this)->find_slot(supportFunctions, anId);
	}
	
	unsigned long hash(const PrimId& anId) const;
	
public:
	typedef PrimRefValue *(*CloneFunction)(const PrimRefValue&);
	enum PrimMapOfRefImpEnums { INLINE };
//
//  	Construct an empty map.
//
	PrimMapOfRefImp() : _contents(0), _tally(0), _capacity(0) {}
	
//
//  	Construct an empty map.
//
	explicit PrimMapOfRefImp(PrimMapOfRefImpEnums anEnum) : _contents(0), _tally(0), _capacity(0) {
		UNUSED(anEnum);
	}
	
	~PrimMapOfRefImp();
//
//  	Add anObject to the map.
//
	bool add(const PrimMapOfRefFuncs& supportFunctions, const PrimRefValue& anObject) {
		return add(supportFunctions, anObject, DEFAULT);
	}
	
	bool add(const PrimMapOfRefFuncs& supportFunctions, const PrimMapOfRefImp& aMap);
	bool add_filtered(const PrimMapOfRefFuncs& supportFunctions, const PrimRefValue& anObject) {
		return add(supportFunctions, anObject, FILTER);
	}
	
	bool add_or_replace(const PrimMapOfRefFuncs& supportFunctions, const PrimRefValue& anObject);
	bool adopt(const PrimMapOfRefFuncs& supportFunctions, const PrimRefValue& anObject);
	void assign(const PrimMapOfRefFuncs& supportFunctions, const PrimMapOfRefImp& aMap);
	void assign(const PrimMapOfRefFuncs& supportFunctions, const PrimListOfRefImp& aList);
	void assign_difference(const PrimMapOfRefFuncs& supportFunctions, const PrimMapOfRefImp& aMap);
	void assign_intersection(const PrimMapOfRefFuncs& supportFunctions, const PrimMapOfRefImp& aMap);
	void assign_union(const PrimMapOfRefFuncs& supportFunctions, const PrimMapOfRefImp& aMap);
	void assign_union(const PrimMapOfRefFuncs& supportFunctions, const PrimListOfRefImp& aList);
//
//  	Report the capacity of the map.
//
	size_t capacity() const {
		return _capacity;
	}
	
	bool check(const PrimMapOfRefFuncs& supportFunctions, bool fullCheck = false) const;
	void clone(const PrimMapOfRefFuncs& supportFunctions, const PrimMapOfRefImp& somePointers,
			   CloneFunction cloneFunction);
//
//  	Identify the contents array.
//
	const PrimRefValue *const *contents() const {
		return (const PrimRefValue *const *)_contents;
	}
	
	void deep_copy(const PrimMapOfRefFuncs& supportFunctions, const PrimMapOfRefImp& aMap);
	const PrimRefValue *find(const PrimMapOfRefFuncs& supportFunctions, const PrimId& anId) const;
	const PrimRefValue *find(const PrimMapOfRefFuncs& supportFunctions, const char *aName) const;
	static bool increment(const PrimRefValue *const *& aPointer, size_t& toGo);
	const PrimRefValue& index(const PrimMapOfRefFuncs& supportFunctions, const PrimId& anId) const;
	const PrimRefValue& index(const PrimMapOfRefFuncs& supportFunctions, const char *aName) const;
	bool is_equal(const PrimMapOfRefFuncs& supportFunctions, const PrimMapOfRefImp& aMap) const;
	std::ostream& print_deep(const PrimMapOfRefFuncs& supportFunctions, std::ostream& s,
			int indentLevel = 0) const;
	std::ostream& print_depth(const PrimMapOfRefFuncs& supportFunctions, std::ostream& s,
			int indentLevel = 0) const;
	std::ostream& print_instance(const PrimMapOfRefFuncs& supportFunctions, std::ostream& s) const;
	std::ostream& print_members(const PrimMapOfRefFuncs& supportFunctions, std::ostream& s,
			int indentLevel = 0) const;
	std::ostream& print_on(const PrimMapOfRefFuncs& supportFunctions, std::ostream& s,
			int indentLevel = 0) const;
	std::ostream& print_this(const PrimMapOfRefFuncs& supportFunctions, std::ostream& s) const;
	bool remove(const PrimMapOfRefFuncs& supportFunctions, const PrimId& anId);
	bool replace(const PrimMapOfRefFuncs& supportFunctions, const PrimRefValue& anObject);
	bool set_capacity(const PrimMapOfRefFuncs& supportFunctions, size_t aCapacity);
	bool set_capacity(const PrimMapOfRefFuncs& supportFunctions, const PrimSize& aCapacity) {
		return set_capacity(supportFunctions, aCapacity.size());
	}
	
//
//  	Report the number of elements in the map.
//
	size_t tally() const {
		return _tally;
	}
	
	void vacate(const PrimMapOfRefFuncs& supportFunctions);
	static bool validate(const PrimRefValue *const *& aPointer, size_t& toGo);
//  	friend bool operator==(const PrimMapOfRefImp& firstMap, const PrimMapOfRefImp& secondMap);
	friend std::ostream& operator<<(std::ostream& s, const PrimMapOfRefImp& aMap);
};

template <class E>

class PrimMapOfRefToConst {

	friend class PrimConstMapOfRefToConstIterator< E >;
	
	friend class PrimListOfRefToConst< E >;
	
	friend class PrimMapOfRefToConstIterator< E >;
	
	friend class PrimMapOfRef< E >;
	
private:
	PrimMapOfRefImp _map;          //   The map using cast-down types.
	static const PrimMapOfRefToConstFunctions< E > _functions; //   The virtual function table.
	
protected:
	const E *const *contents() const {
		return (const E *const *)_map.contents();
	}
	
public:
	enum Enums { INLINE };
	
public:
	PrimMapOfRefToConst() {}
	
	explicit PrimMapOfRefToConst(const PrimSize& initialCapacity) {
		_map.set_capacity(_functions, initialCapacity);
	}
	
	explicit PrimMapOfRefToConst(const PrimMapOfRefToConst< E >& aMap) {
		_map.assign(_functions, aMap._map);
	}
	
	explicit PrimMapOfRefToConst(const PrimListOfRefToConst< E >& aList) {
		_map.assign(_functions, aList._list);
	}
	
	~PrimMapOfRefToConst() {
		_map.vacate(_functions);
	}
	
	PrimMapOfRefToConst< E >& operator=(const PrimMapOfRefToConst< E >& aMap) {
		_map.assign(_functions, aMap._map);
		return *this;
	}
	
	PrimMapOfRefToConst< E >& operator=(const PrimListOfRefToConst< E >& aList) {
		_map.assign(_functions, aList._list);
		return *this;
	}
	
	const E& operator[](const PrimId& anId) const {
		return (const E&)_map.index(_functions, anId);
	}
	
	const E& operator[](const char *aName) const {
		return (const E&)_map.index(_functions, aName);
	}
	
	PrimMapOfRefToConst< E >& operator+=(const E& anObject) {
		_map.add(_functions, (const PrimRefValue&)anObject);
		return *this;
	}
	
	PrimMapOfRefToConst< E >& operator+=(const PrimMapOfRefToConst< E >& aMap) {
		_map.add(_functions, aMap._map);
		return *this;
	}
	
	PrimMapOfRefToConst< E >& operator-=(const E& anObject) {
		_map.remove(_functions, _functions.get_id(anObject));
		return *this;
	}
	
	PrimMapOfRefToConst< E >& operator-=(const PrimMapOfRefToConst< E >& aMap) {
		_map.assign_difference(_functions, aMap._map);
		return *this;
	}
	
	PrimMapOfRefToConst< E >& operator&=(const PrimMapOfRefToConst< E >& aMap) {
		_map.assign_intersection(_functions, aMap._map);
		return *this;
	}
	
	PrimMapOfRefToConst< E >& operator|=(const E& anObject) {
		_map.add_filtered(_functions, (const PrimRefValue&)anObject);
		return *this;
	}
	
	PrimMapOfRefToConst< E >& operator|=(const PrimMapOfRefToConst< E >& aMap) {
		_map.assign_union(_functions, aMap._map);
		return *this;
	}
	
	PrimMapOfRefToConst< E >& operator|=(const PrimListOfRefToConst< E >& aList) {
		_map.assign_union(_functions, aList._list);
		return *this;
	}
	
	bool add(const E& anObject) {
		return _map.add(_functions, (const PrimRefValue&)anObject);
	}
	
	bool add_filtered(const E& anElement) {
		return _map.add_filtered(_functions, (const PrimRefValue&)anElement);
	}
	
	bool add_or_replace(const E& anObject) {
		return _map.add_or_replace(_functions, (const PrimRefValue&)anObject);
	}
	
	bool add(const PrimMapOfRefToConst< E >& aMap) {
		return _map.add(_functions, aMap._map);
	}
	
	bool adopt(const E& anObject) {
		return _map.adopt(_functions, (const PrimRefValue&)anObject);
	}
	
	bool adopt(const E *anObject) {
		return anObject ? adopt(*anObject) : false;
	}
	
//
//  	Report the allocated number of pointers in the map.
//
	size_t capacity() const {
		return _map.capacity();
	}
	
//
//  	Perform a consistency check on the list, enthusiastically if fullCheck.
//
	bool check(bool fullCheck = false) const {
		return _map.check(_functions, fullCheck);
	}
	
//
//  	Assign a deep copy of aMap as the replacement contents of this map.
//
	void deep_copy(const PrimMapOfRefToConst< E >& aMap) {
		_map.deep_copy(_functions, aMap._map);
	}
	
//
//  	Find the element corresponding to anId, or 0 if no such element.
//
	const E *find(const PrimId& anId) const {
		return (const E *)_map.find(_functions, anId);
	}
	
//
//  	Find the element corresponding to aName, or 0 if no such element.
//
	const E *find(const char *aName) const {
		return (const E *)_map.find(_functions, aName);
	}
	
	std::ostream& print_deep(std::ostream& s, int aDepth = 0) const {
		return _map.print_deep(_functions, s, aDepth);
	}
	
	std::ostream& print_depth(std::ostream& s, int aDepth = 0) const {
		return _map.print_depth(_functions, s, aDepth);
	}
	
	std::ostream& print_instance(std::ostream& s) const {
		return _map.print_instance(_functions, s);
	}
	
	std::ostream& print_members(std::ostream& s, int aDepth = 0) const {
		return _map.print_members(_functions, s, aDepth);
	}
	
	std::ostream& print_on(std::ostream& s, int aDepth = 0) const {
		return _map.print_on(_functions, s, aDepth);
	}
	
	std::ostream& print_this(std::ostream& s) const {
		return _map.print_this(_functions, s);
	}
	
	bool remove(const PrimId& anId) {
		return _map.remove(_functions, anId);
	}
	
	bool replace(const E& anObject) {
		return _map.replace(_functions, (const PrimRefValue&)anObject);
	}
	
//
//  	Configure the map to use an array of aCapacity pointers. (Must be a power of two).
//
	bool set_capacity(size_t aCapacity) {
		return _map.set_capacity(_functions, aCapacity);
	}
	
//
//  	Report the number of elements in the map.
//
	size_t tally() const {
		return _map.tally();
	}
	
	const PrimMapOfRefToConst< E >& to_const() const {
		return *this;
	}
	
	void vacate() {
		_map.vacate(_functions);
	}
	
	friend bool operator==(const PrimMapOfRefToConst< E >& firstMap, const PrimMapOfRefToConst< E >& secondMap) {
		return firstMap._map.is_equal(PrimMapOfRefToConst< E >::_functions, secondMap._map);
	}
	
	friend bool operator!=(const PrimMapOfRefToConst< E >& firstMap, const PrimMapOfRefToConst< E >& secondMap) {
		return !operator==(firstMap, secondMap);
	}
	
	friend std::ostream& operator<<(std::ostream& s, const PrimMapOfRefToConst< E >& anObject) {
		return anObject.print_instance(s);
	}
};

#define PRIMMAPOFREFTOCONST_CODE(E) \
template<> void PrimMapOfRefToConstFunctions< E >::annul(const E& anElement) { anElement.annul(); } \
template<> bool PrimMapOfRefToConstFunctions< E >::check(const E& anElement, bool fullCheck) \
 { return anElement.check(fullCheck); } \
template<> const PrimId& PrimMapOfRefToConstFunctions< E >::id(const E& anElement) \
 { return anElement.id(); } \
template<> std::ostream& PrimMapOfRefToConstFunctions< E >::print_depth(const E& anElement, std::ostream& s, int aDepth) \
 { return anElement.print_depth(s, aDepth); } \
template<> std::ostream& PrimMapOfRefToConstFunctions< E >::print_instance(const E& anElement, std::ostream& s) \
 { return anElement.print_instance(s); } \
template<> void PrimMapOfRefToConstFunctions< E >::share(const E& anElement) { anElement.share(); } \
template<> const E& PrimMapOfRefToConstFunctions< E >::shared_null() \
 { E::immutable_null().share(); return E::immutable_null(); }

#define PRIMMAPOFREFTOCONST_DATA(E) \
template<> const PrimMapOfRefToConstFunctions< E > PrimMapOfRefToConst< E >::_functions = { \
  (void (*)(const PrimRefValue&))&PrimMapOfRefToConstFunctions< E >::annul, \
  (bool (*)(const PrimRefValue&, bool))&PrimMapOfRefToConstFunctions< E >::check, \
  (const PrimId& (*)(const PrimRefValue&))&PrimMapOfRefToConstFunctions< E >::id, \
  (std::ostream& (*)(const PrimRefValue&, std::ostream&, int))&PrimMapOfRefToConstFunctions< E >::print_depth, \
  (std::ostream& (*)(const PrimRefValue&, std::ostream&))&PrimMapOfRefToConstFunctions< E >::print_instance, \
  (void (*)(const PrimRefValue&))&PrimMapOfRefToConstFunctions< E >::share, \
  (const PrimRefValue& (*)())&PrimMapOfRefToConstFunctions< E >::shared_null };

template <class E>

class PrimMapOfRef {

	friend class PrimConstMapOfRefIterator< E >;
	
	friend class PrimConstMapOfRefToConstIterator< E >;
	
	friend class PrimListOfRef< E >;
	
	friend class PrimMapOfRefIterator< E >;
	
	friend class PrimMapOfRefToConstIterator< E >;
	
private:
	PrimMapOfRefImp _map;         //   The map using cast-down types.
	static const PrimMapOfRefFunctions< E > _functions;  //   The virtual function table.
	
protected:
	E *const *contents() const {
		return (E *const *)_map.contents();
	}
	
public:
	typedef E *(*CloneFunction)(const E&, PrimPrint&);
	enum Enums { INLINE };
#ifdef PRIM_DEEP_COPY
	
public:
#else

private:
#endif
	PrimMapOfRef(const PrimMapOfRefToConst< E >& aMap) {
		_map.deep_copy(_functions, aMap._map);
	}
	
public:
	PrimMapOfRef() {}
	
	explicit PrimMapOfRef(const PrimSize& initialCapacity) {
		_map.set_capacity(_functions, initialCapacity);
	}
	
	explicit PrimMapOfRef(const PrimMapOfRef< E >& aMap) {
		_map.assign(_functions, aMap._map);
	}
	
	explicit PrimMapOfRef(const PrimMapOfRefToConst< E >& aMap, CloneFunction cloneFunction) {
		_map.clone(_functions, aMap._map, (PrimMapOfRefImp::CloneFunction)cloneFunction);
	}
	
	explicit PrimMapOfRef(const PrimListOfRef< E >& aList) {
		_map.assign(_functions, aList._list);
	}
	
	~PrimMapOfRef() {
		_map.vacate(_functions);
	}
	
	PrimMapOfRef< E >& operator=(const PrimMapOfRef< E >& aMap) {
		_map.assign(_functions, aMap._map);
		return *this;
	}
	
	PrimMapOfRef< E >& operator=(const PrimListOfRef< E >& aList) {
		_map.assign(_functions, aList._list);
		return *this;
	}
	
	operator PrimMapOfRefToConst< E >& () {
		return *(PrimMapOfRefToConst< E > *)this;
	}
	
	operator const PrimMapOfRefToConst< E >& () const {
		return *(const PrimMapOfRefToConst< E > *)this;
	}
	
	E& operator[](const PrimId& anId) const {
		return (E&)_map.index(_functions, anId);
	}
	
	E& operator[](const char *aName) const {
		return (E&)_map.index(_functions, aName);
	}
	
	PrimMapOfRef< E >& operator+=(E& anObject) {
		_map.add(_functions, (const PrimRefValue&)anObject);
		return *this;
	}
	
	PrimMapOfRef< E >& operator+=(const PrimMapOfRef< E >& aMap) {
		_map.add(_functions, aMap._map);
		return *this;
	}
	
	PrimMapOfRef< E >& operator-=(E& anObject) {
		_map.remove(_functions, _functions.get_id(anObject));
		return *this;
	}
	
	PrimMapOfRef< E >& operator-=(const PrimMapOfRef< E >& aMap) {
		_map.assign_difference(_functions, aMap._map);
		return *this;
	}
	
	PrimMapOfRef< E >& operator&=(const PrimMapOfRef< E >& aMap) {
		_map.assign_intersection(_functions, aMap._map);
		return *this;
	}
	
	PrimMapOfRef< E >& operator|=(E& anObject) {
		_map.add_filtered(_functions, (const PrimRefValue&)anObject);
		return *this;
	}
	
	PrimMapOfRef< E >& operator|=(const PrimMapOfRef< E >& aMap) {
		_map.assign_union(_functions, aMap._map);
		return *this;
	}
	
	PrimMapOfRef< E >& operator|=(const PrimListOfRef< E >& aList) {
		_map.assign_union(_functions, aList._list);
		return *this;
	}
	
	bool add(E& anElement) {
		return _map.add(_functions, (const PrimRefValue&)anElement);
	}
	
	bool add_filtered(E& anElement) {
		return _map.add_filtered(_functions, (const PrimRefValue&)anElement);
	}
	
	bool add_or_replace(E& anElement) {
		return _map.add_or_replace(_functions, (const PrimRefValue&)anElement);
	}
	
	bool add(const PrimMapOfRef< E >& aMap) {
		return _map.add(_functions, aMap._map);
	}
	
	bool adopt(E& anObject) {
		return _map.adopt(_functions, (const PrimRefValue&)anObject);
	}
	
	bool adopt(E *anObject) {
		return anObject ? adopt(*anObject) : false;
	}
	
//
//  	Report the allocated number of pointers in the map.
//
	size_t capacity() const {
		return _map.capacity();
	}
	
//
//  	Perform a consistency check on the list, enthusiastically if fullCheck.
//
	bool check(bool fullCheck = false) const {
		return _map.check(_functions, fullCheck);
	}
	
//
//  	Assign a deep copy of aMap as the replacement contents of this map.
//
	void deep_copy(const PrimMapOfRefToConst< E >& aMap) {
		_map.deep_copy(_functions, aMap);
	}
	
//
//  	Find the element corresponding to anId, or 0 if no such element.
//
	E *find(const PrimId& anId) const {
		return (E *)_map.find(_functions, anId);
	}
	
//
//  	Find the element corresponding to aName, or 0 if no such element.
//
	E *find(const char *aName) const {
		return (E *)_map.find(_functions, aName);
	}
	
	std::ostream& print_deep(std::ostream& s, int aDepth = 0) const {
		return _map.print_deep(_functions, s, aDepth);
	}
	
	std::ostream& print_depth(std::ostream& s, int aDepth = 0) const {
		return _map.print_depth(_functions, s, aDepth);
	}
	
	std::ostream& print_instance(std::ostream& s) const {
		return _map.print_instance(_functions, s);
	}
	
	std::ostream& print_members(std::ostream& s, int aDepth = 0) const {
		return _map.print_members(_functions, s, aDepth);
	}
	
	std::ostream& print_on(std::ostream& s, int aDepth = 0) const {
		return _map.print_on(_functions, s, aDepth);
	}
	
	std::ostream& print_this(std::ostream& s) const {
		return _map.print_this(_functions, s);
	}
	
	bool remove(const PrimId& anId) {
		return _map.remove(_functions, anId);
	}
	
	bool replace(E& anObject) {
		return _map.replace(_functions, (const PrimRefValue&)anObject);
	}
	
//
//  	Configure the map to use an array of aCapacity pointers. (Must be a power of two).
//
	bool set_capacity(size_t aCapacity) {
		return _map.set_capacity(_functions, aCapacity);
	}
	
//
//  	Report the number of elements in the map.
//
	size_t tally() const {
		return _map.tally();
	}
	
	const PrimMapOfRefToConst< E >& to_const() const {
		return *(const PrimMapOfRefToConst< E > *)this;
	}
	
	void vacate() {
		_map.vacate(_functions);
	}
	
	friend bool operator==(const PrimMapOfRef< E >& firstMap, const PrimMapOfRef< E >& secondMap) {
		return firstMap._map.is_equal(PrimMapOfRef< E >::_functions, secondMap._map);
	}
	
	friend bool operator!=(const PrimMapOfRef< E >& firstMap, const PrimMapOfRef< E >& secondMap) {
		return !operator==(firstMap, secondMap);
	}
	
	friend std::ostream& operator<<(std::ostream& s, const PrimMapOfRef< E >& anObject) {
		return anObject.print_instance(s);
	}
};

#define PRIMMAPOFREF_CODE(E) \
template <> E& PrimMapOfRefFunctions< E >::shared_null() { E::mutable_null().share(); return E::mutable_null(); }

#define PRIMMAPOFREF_DATA(E) \
template <> const PrimMapOfRefFunctions< E > PrimMapOfRef< E >::_functions = { \
  (void (*)(const PrimRefValue&))&PrimMapOfRefToConstFunctions< E >::annul, \
  (bool (*)(const PrimRefValue&, bool))&PrimMapOfRefToConstFunctions< E >::check, \
  (const PrimId& (*)(const PrimRefValue&))&PrimMapOfRefToConstFunctions< E >::id, \
  (std::ostream& (*)(const PrimRefValue&, std::ostream&, int))&PrimMapOfRefToConstFunctions< E >::print_depth, \
  (std::ostream& (*)(const PrimRefValue&, std::ostream&))&PrimMapOfRefToConstFunctions< E >::print_instance, \
  (void (*)(const PrimRefValue&))&PrimMapOfRefToConstFunctions< E >::share, \
  (const PrimRefValue& (*)())&PrimMapOfRefFunctions< E >::shared_null };

template <class E>

class PrimSharedMapOfRefToConst : public PrimMapOfRefToConst< E > {
	PRIMSHARES_DECLS_INLINE(PrimSharedMapOfRefToConst< E >)
	PRIMREF_DERIVED_DECLS(PrimSharedMapOfRefToConst< E >)
	
public:
	bool is_null() const;
};

#define PRIMSHAREDMAPOFREFTOCONST_CODE(E) \
PRIMREF_DERIVED_NULLS(PrimSharedMapOfRefToConst< E >,PrimSharedMapOfRefToConst< E >,name2(PrimSharedMapOfRefToConst_,E))

#define PRIMSHAREDMAPOFREFTOCONST_DATA(E)

template <class E>

class PrimConstMap : public PrimPrint {
	TYPEDECL_SINGLE(PrimConstMap< E >, PrimPrint)
	PRIMSHARES_DECLS(PrimConstMap< E >)
	PRIMREF_DECLS(PrimConstMap< E >)
	
private:
	PrimMapOfRefToConst< E > _map;        //   The shared map.
	
private:
	PrimConstMap(const PrimConstMap< E >& aMap);     //   Not yet implemented.
	PrimConstMap& operator=(const PrimConstMap< E >& aMap);  //   Not yet implemented.
	
public:
//
//  	Construct an empty shared map.
//
	PrimConstMap() {}
	
//
//  	Construct an empty map with initialCapacity.
//
	explicit PrimConstMap(const PrimSize& initialCapacity) : _map(initialCapacity) {}
	
//
//  	Construct a shared map of initially containing anElement.
//
	explicit PrimConstMap(const E& anElement) {
		_map.add(anElement);
	}
	
//
//  	The destructor adds no functionality.
//
	virtual ~PrimConstMap() {}
	
//  	PrimConstMap< E >& add(const E& anElement) { _map.add(anElement); }
	void add(const E& anElement) {
		_map.add(anElement);
	}
	
//
//  	Return the current allocated capacity.
//
	size_t capacity() const {
		return _map.capacity();
	}
	
	virtual bool check(bool fullCheck = false) const;
//
//  	Report the map of expressions.
//
	const PrimMapOfRefToConst< E >& map() const {
		return _map;
	}
	
	virtual std::ostream& print_depth(std::ostream& s, int aDepth = 0) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth = 0) const;
	virtual std::ostream& print_this(std::ostream& s) const;
//
//  	Return the number of tallied entries.
//
	size_t tally() const {
		return _map.tally();
	}
	
//
//  	Dispose of the entire contents.
//
	void vacate() {
		_map.vacate();
	}
};

#define PRIMCONSTMAP_CODE(E) \
bool PrimConstMap< E >::check(bool fullCheck) const \
{ if (!PrimPrint::check(fullCheck)) return false; return _map.check(fullCheck); } \
std::ostream& PrimConstMap< E >::print_depth(std::ostream& s, int aDepth) const \
{ return _map.print_deep(s, aDepth); } \
std::ostream& PrimConstMap< E >::print_members(std::ostream& s, int aDepth) const \
{ return _map.print_on(s, aDepth); } \
std::ostream& PrimConstMap< E >::print_this(std::ostream& s) const \
{ return _map.print_instance(s); }

#define PRIMCONSTMAP_DATA(E) \
TYPEINFO_SINGLE(PrimConstMap< E >, PrimPrint)

template <class E>

class PrimSharedMapOfRef : public PrimMapOfRef< E > {
	PRIMSHARES_DECLS_INLINE(PrimSharedMapOfRef< E >)
	PRIMREF_DERIVED_DECLS(PrimSharedMapOfRef< E >)
	
public:
	bool is_null() const;
};

#define PRIMSHAREDMAPOFREF_CODE(E) \
PRIMREF_DERIVED_NULLS(PrimSharedMapOfRef< E >,PrimSharedMapOfRef< E >,name2(PrimSharedMapOfRef_,E))

#define PRIMSHAREDMAPOFREF_DATA(E)

template <class E>

class PrimMap : public PrimPrint {
	TYPEDECL_SINGLE(PrimMap< E >, PrimPrint)
	PRIMSHARES_DECLS(PrimMap< E >)
	PRIMREF_DECLS(PrimMap< E >)
	
private:
	PrimMapOfRef< E > _map;        //   The shared map.
	
private:
	PrimMap(const PrimMap< E >& aMap);     //   Not yet implemented.
	PrimMap& operator=(const PrimMap< E >& aMap);   //   Not yet implemented.
	
public:
//
//  	Construct an empty shared map.
//
	PrimMap() {}
	
//
//  	Construct an empty map with initialCapacity.
//
	explicit PrimMap(const PrimSize& initialCapacity) : _map(initialCapacity) {}
	
//
//  	Construct a shared map of initially containing anElement.
//
	explicit PrimMap(E& anElement) {
		_map.add(anElement);
	}
	
//
//  	The destructor adds no functionality.
//
	virtual ~PrimMap() {}
	
	void add(E& anElement) {
		_map.add(anElement);
	}
	
//
//  	Return the current allocated capacity.
//
	size_t capacity() const {
		return _map.capacity();
	}
	
	virtual bool check(bool fullCheck = false) const;
//
//  	Report the map of expressions.
//
	const PrimMapOfRefToConst< E >& map() const {
		return _map.to_const();
	}
	
	PrimMapOfRef< E >& map() {
		return _map;
	}
	
	virtual std::ostream& print_depth(std::ostream& s, int aDepth = 0) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth = 0) const;
	virtual std::ostream& print_this(std::ostream& s) const;
//
//  	Return the number of tallied entries.
//
	size_t tally() const {
		return _map.tally();
	}
	
//
//  	Dispose of the entire contents.
//
	void vacate() {
		_map.vacate();
	}
};

#define PRIMMAP_CODE(E) \
bool PrimMap< E >::check(bool fullCheck) const \
{ if (!PrimPrint::check(fullCheck)) return false; return _map.check(fullCheck); } \
std::ostream& PrimMap< E >::print_depth(std::ostream& s, int aDepth) const \
{ return _map.print_deep(s, aDepth); } \
std::ostream& PrimMap< E >::print_members(std::ostream& s, int aDepth) const \
{ return _map.print_on(s, aDepth); } \
std::ostream& PrimMap< E >::print_this(std::ostream& s) const \
{ return _map.print_instance(s); }

#define PRIMMAP_DATA(E) \
TYPEINFO_SINGLE(PrimMap< E >, PrimPrint)

//
//  	Define an iterator PrimMapOfRefToConstIterator< E > over a PrimMapOfRefToConst< E >. The iterator hides
//  	the indirection implicit in the map implementation, such that *p, and p-> access the map content.
//  	Test of the iterator tests the iteration bounds. Use as:
//
//  	for (PrimMapOfRefToConstIterator< E > p(aMap); p; p++)
//  		p->do_something_const();
//
template <class E>

class PrimMapOfRefToConstIterator {

private:
	const E *const *_p;
	size_t _to_go;
	
private:
	void increment() {
		PrimMapOfRefImp::increment((const PrimRefValue *const *&)_p, _to_go);
	}
	
	void validate() {
		PrimMapOfRefImp::validate((const PrimRefValue *const *&)_p, _to_go);
	}
	
public:
	PrimMapOfRefToConstIterator() : _to_go(0) {}
	
	explicit PrimMapOfRefToConstIterator(PrimMapOfRef< E >& aMap)
			: _p(aMap.contents()), _to_go(aMap.capacity()) {
		validate();
	}
	
	explicit PrimMapOfRefToConstIterator(PrimMapOfRefToConst< E >& aMap)
			: _p(aMap.contents()), _to_go(aMap.capacity()) {
		validate();
	}
	
	PrimMapOfRefToConstIterator< E >& operator=(PrimMapOfRef< E >& aMap) {
		_to_go = aMap.capacity();
		_p = aMap.contents();
		validate();
		return *this;
	}
	
	PrimMapOfRefToConstIterator< E >& operator=(PrimMapOfRefToConst< E >& aMap) {
		_to_go = aMap.capacity();
		_p = aMap.contents();
		validate();
		return *this;
	}
	
	const E& operator *() const {
		return **_p;
	}
	
	const E *operator->() const {
		return *_p;
	}
	
	void operator++() {
		increment();
	}
	
	void operator++(int) {
		increment();
	}
	
	operator size_t() const {
		return _to_go;
	}
	
	bool operator!() const {
		return _to_go ? false : true;
	}
	
	const E& element() const {
		return **_p;
	}
	
	const E *pointer() const {
		return *_p;
	}
	
	size_t to_go() const {
		return _to_go;
	}
};

//
//  	Define an iterator PrimMapOfRefToConstIterator< E > over a PrimMapOfRefToConst< E >. The iterator hides
//  	the indirection implicit in the map implementation, such that *p, and p-> access the map content.
//  	Test of the iterator tests the iteration bounds. Use as:
//
//  	for (PrimConstMapOfRefToConstIterator< E > p(aMap); p; p++)
//  		p->do_something_const();
//
template <class E>

class PrimConstMapOfRefToConstIterator {

private:
	const E *const *_p;
	size_t _to_go;
	
private:
	void increment() {
		PrimMapOfRefImp::increment((const PrimRefValue *const *&)_p, _to_go);
	}
	
	void validate() {
		PrimMapOfRefImp::validate((const PrimRefValue *const *&)_p, _to_go);
	}
	
public:
	PrimConstMapOfRefToConstIterator() : _to_go(0) {}
	
	explicit PrimConstMapOfRefToConstIterator(const PrimMapOfRef< E >& aMap)
			: _p(aMap.contents()), _to_go(aMap.capacity()) {
		validate();
	}
	
	explicit PrimConstMapOfRefToConstIterator(const PrimMapOfRefToConst< E >& aMap)
			: _p(aMap.contents()), _to_go(aMap.capacity()) {
		validate();
	}
	
	PrimConstMapOfRefToConstIterator< E >& operator=(const PrimMapOfRef< E >& aMap) {
		_to_go = aMap.capacity();
		_p = aMap.contents();
		validate();
		return *this;
	}
	
	PrimConstMapOfRefToConstIterator< E >& operator=(const PrimMapOfRefToConst< E >& aMap) {
		_to_go = aMap.capacity();
		_p = aMap.contents();
		validate();
		return *this;
	}
	
	const E& operator *() const {
		return **_p;
	}
	
	const E *operator->() const {
		return *_p;
	}
	
	void operator++() {
		increment();
	}
	
	void operator++(int) {
		increment();
	}
	
	operator size_t() const {
		return _to_go;
	}
	
	bool operator!() const {
		return _to_go ? false : true;
	}
	
	const E& element() const {
		return **_p;
	}
	
	const E *pointer() const {
		return *_p;
	}
	
	size_t to_go() const {
		return _to_go;
	}
};

//
//  	Define an iterator PrimMapOfRefIterator< E > over a PrimMapOfRef< E >. The iterator hides
//  	the indirection implicit in the map implementation, such that *p, and p-> access the map content.
//  	Test of the iterator tests the iteration bounds. Use as:
//
//  	for (PrimMapOfRefToConstIterator< E > p(aMap); p; p++)
//  		p->do_something_const();
//
template <class E>

class PrimMapOfRefIterator {

private:
	E *const *_p;
	size_t _to_go;
	
private:
	void increment() {
		PrimMapOfRefImp::increment((const PrimRefValue *const *&)_p, _to_go);
	}
	
	void validate() {
		PrimMapOfRefImp::validate((const PrimRefValue *const *&)_p, _to_go);
	}
	
public:
	PrimMapOfRefIterator() : _to_go(0) {}
	
	explicit PrimMapOfRefIterator(PrimMapOfRef< E >& aMap)
			: _p(aMap.contents()), _to_go(aMap.capacity()) {
		validate();
	}
	
	PrimMapOfRefIterator< E >& operator=(PrimMapOfRef< E >& aMap) {
		_to_go = aMap.capacity();
		_p = aMap.contents();
		validate();
		return *this;
	}
	
	E& operator *() const {
		return **_p;
	}
	
	E *operator->() const {
		return *_p;
	}
	
	void operator++() {
		increment();
	}
	
	void operator++(int) {
		increment();
	}
	
	operator size_t() const {
		return _to_go;
	}
	
	bool operator!() const {
		return _to_go ? false : true;
	}
	
	E& element() const {
		return **_p;
	}
	
	E *pointer() const {
		return *_p;
	}
	
	size_t to_go() const {
		return _to_go;
	}
};

//
//  	Define an iterator PrimMapOfRefIterator< E > over a PrimMapOfRef< E >. The iterator hides
//  	the indirection implicit in the map implementation, such that *p, and p-> access the map content.
//  	Test of the iterator tests the iteration bounds. Use as:
//
//  	for (PrimMapOfRefToConstIterator< E > p(aMap); p; p++)
//  		p->do_something_const();
//
template <class E>

class PrimConstMapOfRefIterator {

private:
	E *const *_p;
	size_t _to_go;
	
private:
	void increment() {
		PrimMapOfRefImp::increment((const PrimRefValue *const *&)_p, _to_go);
	}
	
	void validate() {
		PrimMapOfRefImp::validate((const PrimRefValue *const *&)_p, _to_go);
	}
	
public:
	PrimConstMapOfRefIterator() : _to_go(0) {}
	
	explicit PrimConstMapOfRefIterator(const PrimMapOfRef< E >& aMap)
			: _p(aMap.contents()), _to_go(aMap.capacity()) {
		validate();
	}
	
	PrimConstMapOfRefIterator< E >& operator=(const PrimMapOfRef< E >& aMap) {
		_to_go = aMap.capacity();
		_p = aMap.contents();
		validate();
		return *this;
	}
	
	E& operator *() const {
		return **_p;
	}
	
	E *operator->() const {
		return *_p;
	}
	
	void operator++() {
		increment();
	}
	
	void operator++(int) {
		increment();
	}
	
	operator size_t() const {
		return _to_go;
	}
	
	bool operator!() const {
		return _to_go ? false : true;
	}
	
	E& element() const {
		return **_p;
	}
	
	E *pointer() const {
		return *_p;
	}
	
	size_t to_go() const {
		return _to_go;
	}
};

#endif

