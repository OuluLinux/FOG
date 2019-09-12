
#ifndef PRIMID_HXX
#define PRIMID_HXX




//  .bugbug This is a C-style extending class, and as such is only valid C++ if it is a POD, which
//   it isn't although it could be by flattening the share count macro to eliminate internal access
//   specifiers, and moving all constructors, operators, destructors, assignments to the handle.
//   Note that the null id is zero valued and so requires no initialisation.
class PrimId
{
 friend class PrimIdHandle;
 friend class PrimIdCyclicIterator;
public:
 enum Locked {};

private:
 static const PrimId **_contents;     //   Hashed table of pointers to entries.
 static size_t _tally;        //   Number of entries in table.
 static size_t _capacity;       //   Maximum number of entries in table.
 static char _null[];        //   The null instance.
private:
 static void add(const PrimId& anId);
 static const PrimId *create(const char *aBuffer, unsigned short aLength, unsigned long hashCode);
 static const PrimId *find(const char *aBuffer, unsigned short aLength, unsigned long hashCode);
 static const PrimId **find_slot(const PrimId& anId);
 static inline unsigned long hash_index(unsigned long hashCode);
 static inline unsigned long hash_update(unsigned long hashCode, unsigned long hashInput);
 static void remove(PrimId& anId);
public:
 static bool check_map(bool fullCheck = false);
//  
//  	Report the class name for debug purposes.
//  
 static const char *class_name() { return "PrimId"; }
 static const PrimId *create(const char *someText);
 static const PrimId *create(const char *aBuffer, size_t aLength);
 static const PrimId *find(const char *someText);
 static const PrimId *find(const char *aBuffer, size_t aLength);
//  
//  	Return the 0 instance.
//  
 static const PrimId& null() { return (*(const PrimId *)_null); }
 static std::ostream& print_map_on(std::ostream& s, int indentLevel = 0);
 static std::ostream& print_usage(std::ostream& s);
 static bool set_capacity(size_t aCapacity = 1024);
 static int sort_compare(const PrimRefToConst<PrimId> *s1, const PrimRefToConst<PrimId> *s2);
//  
//  	Report whether firstId is the same as secondId.
//  
 friend bool operator==(const PrimId& firstId, const PrimId& secondId)
  { return &firstId == &secondId; }
//  
//  	Report whether firstId is different to secondId.
//  
 friend bool operator!=(const PrimId& firstId, const PrimId& secondId)
  { return &firstId != &secondId; }
 friend std::ostream& operator<<(std::ostream& s, const PrimId& anId);

private:
 PRIMSHARES_DECLS(PrimId)
 PRIMREF_DERIVED_DECLS(PrimId)      //   *DERIVED* to exclude virtual is_null()
 unsigned long _hash;        //   Hash code of the representation.
 unsigned short _length;        //   Number of characters (excluding trailing 0).
 mutable char _text /*  [1]*/;       //   First byte of extendable text area.
private:
 PrimId(const PrimId& anId);       //   Copy by creation is forbidden.
 PrimId& operator=(const PrimId& anId);    //   Copy by assignment is forbidden.
private:
 inline PrimId(const char *aBuffer, unsigned short aLength, unsigned long aHashCode);
//  
//  	The destructor eliminates the map entry.	
//  
 ~PrimId() { remove(*this); }

public:
//  
//  	Return non-0 if a non-null id.
//  
 operator const Locked *() const { return (const Locked *)((const char *)this != _null ? this : 0); }
//  
//  	Return true if the null id.
//  
 bool operator!() const { return (const char *)this == _null; }
 bool check(bool fullCheck = false) const;
 int compare(const PrimId& anId) const;
//  
//  	Report the hash code.
//  
 unsigned long hash() const { return _hash; }
//  
//  	Report an id for the id, so that PrimId may be used in a PrimMapOfRef.
//  
 const PrimId& id() const { return *this; }
//  
//  	Report whether this is the null id.
//  
 bool is_null() const { return (const char *)this == _null; }
//  
//  	Report the number of hashed characters.
//  
 unsigned short length() const { return _length; }
//  
//  	There is no additional information to form part of a print-out.
//  
 std::ostream& print_deep(std::ostream& s, int aDepth = 0) const { UNUSED(aDepth); return s; }
 std::ostream& print_depth(std::ostream& s, int indentLevel = 0) const;
//  
//  	Display the text as part of an inline print-out.
//  
 std::ostream& print_instance(std::ostream& s) const { return s << *this; }
//  
//  	Report the text of the representation (never 0).
//  
 const char *str() const { return &_text; }
};

inline const PrimId& PrimId::immutable_null() { return null(); }

//  
//  	PrimIdHandle provides a reference counted handle for a PrimId. The class offers all the facilities
//  	of its public base class PrimRefToConst<PrimId> together with a friendlier name and
//  	some useful construction methods.
//  
//  .note The plethora of operator==, operator!= are necessary to defeat attempts by PrimMath.h to do
//  	inherited pointer comparison in conjuction with PrimIdHandle(const PrimId& anId).
class PrimIdHandle : public PrimRefToConst<PrimId>
{
 typedef PrimRefToConst<PrimId> Super;
public:
 PrimIdHandle() {}
 PrimIdHandle(const char *aName) : Super(PrimId::create(aName), ADOPT) {}
 PrimIdHandle(const char *aBuffer, size_t aLength) : Super(PrimId::create(aBuffer, aLength), ADOPT) {}
 PrimIdHandle(const PrimId& anId) : Super(anId) {}
 const char *str() const { return element().str(); }
 friend std::ostream& operator<<(std::ostream& s, const PrimIdHandle& idHandle)
  { return operator<<(s, idHandle.element()); }
 friend std::istream& operator>>(std::istream& s, PrimIdHandle& idHandle);
//  
//  	Report whether firstId is the same as secondId.
//  
 friend bool operator==(const PrimIdHandle& firstId, const PrimIdHandle& secondId)
  { return *firstId == *secondId; }
 friend bool operator==(const PrimIdHandle& firstId, const PrimId& secondId)
  { return *firstId == secondId; }
 friend bool operator==(const PrimId& firstId, const PrimIdHandle& secondId)
  { return firstId == *secondId; }
//  
//  	Report whether firstId is different to secondId.
//  
 friend bool operator!=(const PrimIdHandle& firstId, const PrimIdHandle& secondId)
  { return *firstId != *secondId; }
 friend bool operator!=(const PrimIdHandle& firstId, const PrimId& secondId)
  { return *firstId != secondId; }
 friend bool operator!=(const PrimId& firstId, const PrimIdHandle& secondId)
  { return firstId != *secondId; }
};
#endif
 
