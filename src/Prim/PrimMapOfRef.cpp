#include <Prim/PrimIncludeAll.h>











//  
//  	Calculate the hash index for a particular id by masking off its least significant bits.
//  
inline unsigned long PrimMapOfRefImp::hash(const PrimId& idRep) const
{
 return (_capacity-1) & idRep.hash();
}

//  
//  	Clean up on destruction, releasing the entire contents.
//  
PrimMapOfRefImp::~PrimMapOfRefImp()
{
 if (_contents)
 {
  if (_tally)
   ERRMSG(*this << " has not been vacated by derived destructor.");
  delete[] _contents;
  _contents = 0;
 }
}

//  
//  	Add anObject to the map. addMode selects four alternate behaviours.
//  
//  	REHASH: Addition is part of a rehash, so no share count changes occur.
//  	No error message.
//  	Returns false if anObject not added.
//  
//  	REPLACE: anObject is added if absent, or replaces an existing content.
//  	No error message.
//  	Returns false if anObject not added (excludes collision case).
//  
//  	FILTER: anObject is added if absent.
//  	Error message if collision is for distinct objects.
//  	Returns false if anObject not added (includes collision case).
//  
//  	DEFAULT: anObject is added if absent.
//  	Error message for any collision.
//  	Returns false if anObject not added (includes collision case).
//  
//  	All behaviours generate an error message for an allocation failure.
//  
bool PrimMapOfRefImp::add(const PrimMapOfRefFuncs& supportFunctions, const PrimRefValue& anObject,
 AddMode addMode)
{
 if ((_tally >= _capacity/2) && !set_capacity(supportFunctions, 2 * _tally + 1)) //   Expand map if would be over half full.
  return false;
 const PrimId& anId = supportFunctions.id(anObject);
 const PrimRefValue **p = _contents + hash(anId);
 const PrimRefValue *const *const pMax = _contents + _capacity;
 for (size_t n = _capacity; n > 0; n--, p = ++p >= pMax ? _contents : p)
 {
  if (!*p)
  {
   *p = &anObject;
   _tally++;
   if (addMode == REHASH)
    return true;
   supportFunctions.share(**p);
   check(supportFunctions);
   return true;
  }
  else if (supportFunctions.id(**p) == anId)  //   Collision ?
  {
   if (addMode == REPLACE)
   {
    if (*p != &anObject)
    {
     supportFunctions.share(anObject);
     supportFunctions.annul(**p);
     *p = &anObject;
     return true;
    }
   }
   else if (addMode == FILTER)
   {
    if (*p != &anObject)
    {
     PrimOstrstream ss;
     ss << *this << " ignored ";
     supportFunctions.print_instance(anObject, ss);
     ss << " in conflict with ";
     supportFunctions.print_instance(**p, ss);
     ss << "." << std::ends;
     ERRMSGZ(ss.str());
    }
    
   }
   else
   {
    PrimOstrstream ss;
    ss << *this << " ignored duplicate ";
    supportFunctions.print_instance(anObject, ss);
    ss << "." << std::ends;
    ERRMSGZ(ss.str());
   }
   return false;
  }
 }
 PrimOstrstream ss;
 ss << *this << " failed to find space for ";
 supportFunctions.print_instance(anObject, ss);
 ss << "." << std::ends;
 ERRMSGZ(ss.str());
 return false;
}

//  
//  	Add each of the members of aMap to this map.
//  
bool PrimMapOfRefImp::add(const PrimMapOfRefFuncs& supportFunctions, const PrimMapOfRefImp& aMap)
{
 size_t newTally = _tally + aMap.tally();
 if ((newTally >= _capacity/2)
  && !set_capacity(supportFunctions, 2 * newTally + 1))  //   Expand map if would be over half full.
  return false;
 bool returnStatus = true;
 const PrimRefValue *const *p = aMap.contents();
 for (size_t i = aMap.capacity(); i-- > 0; p++)
  if (*p)
   if (!add(supportFunctions, **p))
    returnStatus = false;
 return returnStatus;
}

//  
//  	Add anObject to the map, or replace it if already present. Returns false if anObject not replaced.
//  
bool PrimMapOfRefImp::add_or_replace(const PrimMapOfRefFuncs& supportFunctions, const PrimRefValue& anObject)
{
 return replace(supportFunctions, anObject) || add(supportFunctions, anObject);
}

//  
//  	Add anObject to the map, relieving the caller of the responsibility for deleting anObject.
//  	It is more efficient for the caller to resolve the existence of anObject
//  	and invoke add() as appropriate, since the abstract behaviour has to invoke a virtual method
//  	to ensure that sharing is maintained correctly. However the convenience of just invoking
//  	aMap.adopt(new T) is more important.
//  
bool PrimMapOfRefImp::adopt(const PrimMapOfRefFuncs& supportFunctions, const PrimRefValue& anObject)
{
 bool returnStatus = add(supportFunctions, anObject);
 supportFunctions.annul(anObject);
 return returnStatus;
}

//  
//  	Assign aMap to this map.
//  
void PrimMapOfRefImp::assign(const PrimMapOfRefFuncs& supportFunctions, const PrimMapOfRefImp& aMap)
{
 if (this != &aMap)
 {
  vacate(supportFunctions);
  if (set_capacity(supportFunctions, 2 * aMap._tally))
  {
   const PrimRefValue *const *const pMax = aMap._contents + aMap._capacity;
   for (const PrimRefValue *const *p = aMap._contents; p < pMax; p++)
    if (*p)
     add(supportFunctions, **p);
  }
 }
}

//  
//  	Assign aList to this map.
//  
void PrimMapOfRefImp::assign(const PrimMapOfRefFuncs& supportFunctions, const PrimListOfRefImp& aList)
{
 vacate(supportFunctions);
 const PrimRefBase *const pMax = aList._contents + aList._tally;
 for (const PrimRefBase *p = aList._contents; p < pMax; p++)
  if (*p)
   add(supportFunctions, **p, FILTER);
}

//  
//  	Remove the intersection of this map and aMap from this map.
//  
void PrimMapOfRefImp::assign_difference(const PrimMapOfRefFuncs& supportFunctions, const PrimMapOfRefImp& aMap)
{
 if (this != &aMap)
 {
  const PrimRefValue *const *const pMax = aMap._contents + aMap._capacity;
  for (const PrimRefValue *const *p = aMap._contents; p < pMax; )
   if (*p && find(supportFunctions, supportFunctions.id(**p)))
    remove(supportFunctions, supportFunctions.id(**p));
   else
    p++;
 }
 else
  vacate(supportFunctions);
}

//  
//  	Assign the intersection of this map and aMap to this map.
//  
void PrimMapOfRefImp::assign_intersection(const PrimMapOfRefFuncs& supportFunctions, const PrimMapOfRefImp& aMap)
{
 if (this != &aMap)
 {
  const PrimRefValue *const *const pMax = _contents + _capacity;
  for (const PrimRefValue *const *p = _contents; p < pMax; )
   if (*p && !aMap.find(supportFunctions, supportFunctions.id(**p)))
    remove(supportFunctions, supportFunctions.id(**p));
   else
    p++;
 }
}

//  
//  	Assign the union of this map and aMap to this map.
//  
void PrimMapOfRefImp::assign_union(const PrimMapOfRefFuncs& supportFunctions, const PrimMapOfRefImp& aMap)
{
 if ((this != &aMap) && set_capacity(supportFunctions, std::max(_tally, aMap._tally)))
 {
  const PrimRefValue *const *const pMax = aMap._contents + aMap._capacity;
  for (const PrimRefValue *const *p = aMap._contents; p < pMax; p++)
   if (*p)
    add(supportFunctions, **p, FILTER);
//  			{
//  				const PrimRefValue *q = find(supportFunctions, supportFunctions.id(**p));
//  				if (!q)
//  					add(supportFunctions, **p);
//  				else if (*p != q)
//  				{
//  					PrimOstrstream ss;
//  					ss << *this << " cannot unify inconsistent ";
//  					supportFunctions.print_instance(**p, ss);
//  					ss << " and ";
//  					supportFunctions.print_instance(*q, ss);
//  					ss << ".";
//  				PrimNotice_Error aNotice(ss.str());
//  						aNotice.post_on(prim_notifier());
//  				}
//  			}
 }
}

//  
//  	Assign the union of this map and aList to this map.
//  
void PrimMapOfRefImp::assign_union(const PrimMapOfRefFuncs& supportFunctions, const PrimListOfRefImp& aList)
{
 const PrimRefBase *const pMax = aList._contents + aList._tally;
 for (const PrimRefBase *p = aList._contents; p < pMax; p++)
  if (*p)
   add(supportFunctions, **p, FILTER);
}

//  
//  	Perform a self-consistency check.
//  
bool PrimMapOfRefImp::check(const PrimMapOfRefFuncs& supportFunctions, bool fullCheck) const
{
 if (!PrimCheckLock::checking_enabled())
  return true;
 if ((_capacity & (_capacity-1)) != 0)
 {
  ERRMSG(*this << " map size is not a power of two.");
  return false;
 }
 size_t checkTally = 0;
 const PrimRefValue *const *p = _contents;
 size_t j = _capacity;
 for ( ; j-- > 0; p++)
  if (*p)
  {
   checkTally++;
   if (find(supportFunctions, supportFunctions.id(**p)) != *p)
   {
    PrimOstrstream ss;
    ss << *this << " has ";
    supportFunctions.print_instance(**p, ss);
    ss << " mis-hashed." << std::ends;
    ERRMSGZ(ss.str());
    return false;
   }
  }
 if (_tally != checkTally)
 {
  ERRMSG(*this << " has << " << checkTally << " actual contents when " << _tally << " tallied.");
  return false;
 }
 if (fullCheck)
  for (p = _contents, j = _capacity; j-- > 0; p++)
   if (*p && !supportFunctions.check(**p, fullCheck))
    return false;
 return true;
}

//  
//  	Clone somePointers as a replacement content of this list invoking cloneFunction(oldObject)
//  	to generate each clone. The return from cloneFunction should be the unadopted return from new.
//  
void PrimMapOfRefImp::clone(const PrimMapOfRefFuncs& supportFunctions, const PrimMapOfRefImp& somePointers,
  CloneFunction cloneFunction)
{
 if ((this != &somePointers)
  && (vacate(supportFunctions), set_capacity(supportFunctions, somePointers.tally())))
 {
  const PrimRefValue *const *p = somePointers._contents;
  if (p)
  {
   const PrimRefValue *const *const pMax = p + somePointers._capacity;
   for ( ; p < pMax; p++)
    if (*p)
    {
     const PrimRefValue *clonedObject = cloneFunction(**p);
     add(supportFunctions, *clonedObject);
    }
  }
 }
}

#if 0
//  
//  	Construct a copy of amap without sharing any non-const objects.
//  
void PrimMapOfRefImp::deep_copy(const PrimMapOfRefFuncs& supportFunctions, const PrimMapOfRefImp& aMap)
{
 if (this != &aMap)
 {
  vacate();
  if (set_capacity(aMap._tally))
  {
   const PrimRefValue *const *const pMax = aMap._contents + aMap._capacity;
   for (const PrimRefValue *const *p = aMap._contents; p < pMax; p++)
    if (*p)
    {
     PrimRefValue *clonedObject = ptr_clone(**p);
     add(supportFunctions, *clonedObject);
    }
  }
 }
}
#endif

//  
//  	Locate the object for anId, returning 0 if not found.
//  
const PrimRefValue *PrimMapOfRefImp::find(const PrimMapOfRefFuncs& supportFunctions, const PrimId& anId) const
{
 if (!_tally)        //   No point searching if there
  return 0;        //    is nothing that could be found.
 const PrimRefValue *const *p = find_slot(supportFunctions, anId);
 if (!p)
  return 0;
 else
  return *p;
}

//  
//  	Locate the object for aName, returning 0 if not found.
//  
const PrimRefValue *PrimMapOfRefImp::find(const PrimMapOfRefFuncs& supportFunctions, const char *aName) const
{
 if (!_tally)        //   No point searching if there
  return 0;        //    is nothing that could be found.
 const PrimId *anId = PrimId::find(aName);
 if (!anId)         //   No point searching if there
  return 0;        //    is no matching Id.
 const PrimRefValue *const *p = find_slot(supportFunctions, *anId);
 if (!p)
  return 0;
 else
  return *p;
}

//  
//  	Locate the slot comprising the object for anId, returning 0 if not found.
//  
const PrimRefValue *const *PrimMapOfRefImp::find_slot(const PrimMapOfRefFuncs& supportFunctions,
                    const PrimId& anId) const
{
 const PrimRefValue *const *p = _contents + hash(anId);
 const PrimRefValue *const *const pMax = _contents + _capacity;
 for (size_t n = _capacity; n-- > 0; p = ++p >= pMax ? _contents : p)
 {
  if (!*p)
   return p;
  const PrimId& slotId = supportFunctions.id(**p);  //   Temporary needed for SunC++ 4.1
  if (slotId == anId)
   return p;
 }
 return 0;
}

//  
//  	Increment aPointer to a map content within the constraint that there are toGo remaining locations
//  	and that the result of the increment is to be the next occupied entry. Returns false without
//  	updating aPointer, if toGo reaches zero.
//  
bool PrimMapOfRefImp::increment(const PrimRefValue *const *& aPointer, size_t& toGo)
{
 size_t n = toGo;
 if (!n)
  return false;
 const PrimRefValue *const *p = aPointer;
 do
 {
  p++, n--;
 } while (n && !*p);
 toGo = n;
 if (!n)
  return false;
 aPointer = p;
 return true;
}

//  
//  	Return a reference to the element at anId, or a reference to the null element if no such element.
//  
const PrimRefValue& PrimMapOfRefImp::index(const PrimMapOfRefFuncs& supportFunctions, const PrimId& anId) const
{
 const PrimRefValue *anObject = find(supportFunctions, anId);
 return anObject ? *anObject : supportFunctions.shared_null();
}

//  
//  	Return a reference to the element at aName, or a reference to the null element if no such element.
//  
const PrimRefValue& PrimMapOfRefImp::index(const PrimMapOfRefFuncs& supportFunctions, const char *aName) const
{
 const PrimRefValue *anObject = find(supportFunctions, aName);
 return anObject ? *anObject : supportFunctions.shared_null();
}

#if 0
//  
//  	Return true if each map has entities with corresponding identities, each of which respond
//  	successfully to operator== -- which is not available via supportFunctions.
//  
bool PrimMapOfRefImp::is_equal(const PrimMapOfRefFuncs& supportFunctions, const PrimMapOfRefImp& aMap) const
{
 if (_tally != aMap._tally)
  return false;
 const PrimRefValue *const *p = _contents;
 for (size_t i = 0; i < _capacity; i++, p++)
  if (*p)
  {
   const PrimRefValue *const *q = aMap.find(supportFunctions, supportFunctions.id(**p));
   if (!q)
    return false;
   //   ... more code needed
  }
 return true;
}
#endif

//  
//  	Display the full details of the list as part of a deep diagnostic print-out.
//  
std::ostream& PrimMapOfRefImp::print_deep(const PrimMapOfRefFuncs& supportFunctions,
  std::ostream& s, int aDepth) const
{
 s << indent(aDepth);
 print_instance(supportFunctions, s);
 s << '\n';
 print_depth(supportFunctions, s, aDepth+1);
 return s;
}

//  
//  	Provide a full recursive diagnostic print out of the contents of this map.
//  
std::ostream& PrimMapOfRefImp::print_depth(const PrimMapOfRefFuncs& supportFunctions,
  std::ostream& s, int indentLevel) const
{
 const PrimRefValue *const *p = _contents;
 for (size_t i = 0; i < _capacity; i++, p++)
  if (*p)
  {
   s << indent(indentLevel+1) << std::hex << i << "[" << hash(supportFunctions.id(**p))
     << "," << supportFunctions.id(**p).hash() << "] -> " << std::dec;
   supportFunctions.print_instance(**p, s) << '\n';
   supportFunctions.print_depth(**p, s, indentLevel+2);
  }
 return s;
}

//  
//  	Display the list dimensions as part of an inline diagnostic.
//  
std::ostream& PrimMapOfRefImp::print_instance(const PrimMapOfRefFuncs& supportFunctions, std::ostream& s) const
{
 s << "PrimMapOfRefImp";
 if (PrimPrint::print_address())
  PrimPrint::print_address_of(s, this);
 s << "(";
 print_this(supportFunctions, s);
 s << ")";
 return s;
}

//  
//  	Provide a full diagnostic print out of the contents of this map.
//  
std::ostream& PrimMapOfRefImp::print_members(const PrimMapOfRefFuncs& supportFunctions,
  std::ostream& s, int indentLevel) const
{
 const PrimRefValue *const *p = _contents;
 for (size_t i = 0; i < _capacity; i++, p++)
  if (*p)
  {
   s << indent(indentLevel+1);
   supportFunctions.print_instance(**p, s) << '\n';
  }
 return s;
}

//  
//  	Display the full details of the list as part of a deep diagnostic print-out.
//  
std::ostream& PrimMapOfRefImp::print_on(const PrimMapOfRefFuncs& supportFunctions,
  std::ostream& s, int aDepth) const
{
 s << indent(aDepth);
 print_instance(supportFunctions, s);
 s << '\n';
 print_members(supportFunctions, s, aDepth+1);
 return s;
}

//  
//  	Provide the dimensions as part of a brief diagnostic.
//  
std::ostream& PrimMapOfRefImp::print_this(const PrimMapOfRefFuncs& /*supportFunctions*/, std::ostream& s) const
{
 /*UNUSED(&supportFunctions);*/
 s << _tally << '/' << _capacity;
 return s;
}

//  
//  	Remove the object identified by anId from the set repair the map, returning true if removed.
//  
bool PrimMapOfRefImp::remove(const PrimMapOfRefFuncs& supportFunctions, const PrimId& anId)
{
 if (!_tally)            //   Report instant failure so that
  return false;           //    vacate() has no transient state
 const PrimRefValue **pStart = find_slot(supportFunctions, anId);
 if (!pStart || !*pStart)
  return false;
 const PrimRefValue& anObject = **pStart;
 *pStart = 0;            //   Lose specified entry
 supportFunctions.annul(anObject);
 const PrimRefValue **p = pStart;       //   Pointer to entry under consideration
 const PrimRefValue *const *const pMax = _contents + _capacity; //   Upper limit of ident table
 size_t n = _capacity;
 while (--n > 0)            //   Potential loop of all except the trashed entry
 {
  p++;             //   Advance pointers to next entry
  if (p >= pMax)
   p = _contents;
  if (!*p)            //   If empty slot, hash table now repaired
   break;
  const PrimRefValue *const *const pSlot = _contents + hash(supportFunctions.id(**p)); //   Rehash slot
  if ((pStart < p) ? ((pSlot <= pStart) || (p < pSlot)) : ((pSlot <= pStart) && (p < pSlot)))
  {
   *pStart = *p;          //   Move entry back to the empty slot
   pStart = p;           //   Adjust the empty slot position
   *pStart = 0;          //   Declare the new empty slot
  }
 }
 _tally--;             //   One less entry in table
 check(supportFunctions);
 return true;
}

//  
//  	Replace anObject in the map. Returns false if anObject not replaced.
//  
bool PrimMapOfRefImp::replace(const PrimMapOfRefFuncs& supportFunctions, const PrimRefValue& anObject)
{
 if (!_tally)            //   Report instant failure so that
  return false;           //    vacate() has no transient state
 const PrimRefValue **p = find_slot(supportFunctions, supportFunctions.id(anObject));
 if (!p || !*p)
  return false;
 const PrimRefValue& oldObject = **p;
 *p = &anObject;
 supportFunctions.share(anObject);
 supportFunctions.annul(oldObject);
 check(supportFunctions);
 return true;
}

//  
//  	Increase the capacity to the next sensible power-of two size to accommodate the larger of aCapacity
//  	and the existing tally.
//  
bool PrimMapOfRefImp::set_capacity(const PrimMapOfRefFuncs& supportFunctions, size_t aCapacity)
{
 if (aCapacity > _capacity)
 {
  const PrimRefValue **const oldContents = _contents;
  const size_t oldCapacity = _capacity;
  size_t newCapacity = std::max(_capacity, (size_t)16);
  while (newCapacity < aCapacity)
   newCapacity <<= 1;
  const PrimRefValue **const newContents = new const PrimRefValue *[newCapacity];
  if (!newContents)
  {
   ERRMSG(*this << " failed to allocate memory for " << newCapacity << " object slots.");
   return false;
  }
  const PrimRefValue **q = newContents;
  for (size_t i = newCapacity; i-- > 0; )
   *q++ = 0;
  _capacity = newCapacity;
  _contents = newContents;
  _tally = 0;
  const PrimRefValue *const *const oldEnd = oldContents + oldCapacity;
  for (const PrimRefValue *const *p = oldContents; p < oldEnd; p++)
   if (*p)
    add(supportFunctions, **p, REHASH);
  delete[] oldContents;
 }
 return true;
}

//  
//  	Conditionally increment aPointer to a map content within the constraint that there are toGo remaining
//  	locations and that the result of any increment is to be the next occupied entry. Returns false without
//  	updating aPointer, if toGo reaches zero. No increment occurs if aPointer and toGo signify a valid entry.
//  
bool PrimMapOfRefImp::validate(const PrimRefValue *const *& aPointer, size_t& toGo)
{
 size_t n = toGo;
 const PrimRefValue *const *p = aPointer;
 while (n && !*p)
  n--, p++;
 toGo = n;
 if (!n)
  return false;
 aPointer = p;
 return true;
}

//  
//  	Remove all members from the map.
//  
void PrimMapOfRefImp::vacate(const PrimMapOfRefFuncs& supportFunctions)
{
 _tally = 0;             //   Defeat reentrant remove.
 const PrimRefValue **p = _contents;
 for (size_t i = _capacity; i-- > 0; p++)
  if (*p)
   supportFunctions.annul(**p), *p = 0;
}

#if 0
//  
//  	Report whether firstMap contains the same information as secondMap.
//  
bool operator==(const PrimMapOfRefImp& firstMap, const PrimMapOfRefImp& secondMap)
{
 if (firstMap._tally != secondMap._tally)     //   Can't be same if different tallies.
  return false;
 const PrimRefValue *const *p = firstMap._contents;
 for (size_t i = 0; i < firstMap._capacity; i++, p++)
  if (*p)
  {
   const PrimRefValue *q = secondMap.find(firstMap.supportFunctions.id(**p));
   if (!q || !firstMap.supportFunctions.is_equal(**p, *q))
    return false;
  }
 return true;
}
#endif

//  
//  	Generate an inline diagnostic desribing the map in so far as possible without any support functions.
//  
std::ostream& operator<<(std::ostream& s, const PrimMapOfRefImp& aMap)
{
 s << "PrimMapOfRefImp";
 PrimPrint::print_address_of(s, &aMap);
 s << "(" << aMap.tally() << "/" << aMap.capacity() << ")";
 return s; 
}
