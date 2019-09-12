#include <Prim/PrimIncludeAll.h>












//  
//  	Create a list element ifdentifying anElement.
//  
PrimSinglyLinkedListOfRefElement::PrimSinglyLinkedListOfRefElement(const PrimRefFuncs& supportFunctions,
  const PrimRefValue& anElement)
:
 _element(&anElement),
 _next(0)
{
 supportFunctions.share(anElement);
}

//  
//  	Create a list element ifdentifying anElement.
//  
PrimSinglyLinkedListOfRefElement::~PrimSinglyLinkedListOfRefElement()
{
  if (_element)
  ERRMSG("PrimSinglyLinkedListOfRefElement has not released its element.");
}

//  
//  	The destructor releases any memory.
//  
PrimSinglyLinkedListOfRefImp::~PrimSinglyLinkedListOfRefImp()
{
 if (_head || _tail)
  ERRMSG(*this << " has not been vacated by derived destructor.");
}

//  
//  	Return the the pointer to the anIndex element if anIndex valid, or 0 otherwise.
//  
const PrimRefValue *PrimSinglyLinkedListOfRefImp::operator[](size_t anIndex) const
{
 size_t i = 0;
 for (PrimSinglyLinkedListOfRefElement *p = _head; p; p = p->_next, i++)
  if (i >= anIndex)
   return p->_element;
 return 0;
}

//  
//  	Add anObject to the list, relieving the caller of the responsibility for deleting anObject.
//  	It is more efficient for the caller to resolve the existence of anObject
//  	and invoke add() as appropriate, since the abstract behaviour has to invoke a virtual method
//  	to ensure that sharing is maintained correctly. However the convenience of just invoking
//  	aList.adopt(new T) is more important.
//  
bool PrimSinglyLinkedListOfRefImp::adopt_tail(const PrimRefFuncs& supportFunctions, const PrimRefValue& anObject)
{
 bool returnStatus = add_tail(supportFunctions, anObject);
 supportFunctions.annul(anObject);
 return returnStatus;
}

//  
//  	Assign aList as a replacement content of this list.
//  
void PrimSinglyLinkedListOfRefImp::assign(const PrimRefFuncs& supportFunctions, const PrimSinglyLinkedListOfRefImp& aList)
{
 vacate(supportFunctions);
 for (PrimSinglyLinkedListOfRefElement *p = aList._head; p; p = p->_next)
  add_tail(supportFunctions, p->element());
}

//  
//  	Assign aList as a replacement content of this list.
//  
void PrimSinglyLinkedListOfRefImp::assign(const PrimRefFuncs& supportFunctions, const PrimListOfRefImp& aList)
{
 vacate(supportFunctions);
 const PrimRefBase *p = aList.contents();
 if (p)
  for (int i = aList.tally(); i-- > 0; p++)
   add_tail(supportFunctions, **p);
}

//  
//  	Assign the elements of aMap as a replacement content of this list.
//  
void PrimSinglyLinkedListOfRefImp::assign(const PrimRefFuncs& supportFunctions, const PrimMapOfRefImp& aMap)
{
 vacate(supportFunctions);
 const PrimRefValue *const *p = aMap.contents();
 if (p)
  for (size_t i = aMap.capacity(); i-- > 0; p++)
   if (*p)
    add_tail(supportFunctions, **p);
}

//  
//  	Perform a consistency check returning false after generating any error diagnostic.
//  
bool PrimSinglyLinkedListOfRefImp::check(const PrimRefFuncs& supportFunctions, bool fullCheck) const
{
 if (!PrimCheckLock::checking_enabled())
  return false;
 if (_head && !_tail)
 {
  ERRMSG(*this << " has head but no tial.");
  return false;
 }
 if (!_head && _tail)
 {
  ERRMSG(*this << " has tail but no head.");
  return false;
 }
 size_t aTally = 0;
 for (const PrimSinglyLinkedListOfRefElement *p = _head; p; p = p->_next, aTally++)
 {
  if (fullCheck && !supportFunctions.check(p->element(), fullCheck))
   return false;
  if (!p->_next && (p != _tail))
  {
   ERRMSG(*this << " linkage does not lead to tail.");  
   return false;
  }
 }
 if (aTally != _tally)
 {
  ERRMSG(*this << " should have " << aTally << " tallied entries.");
  return false;
 }
 return true;
}

//  
//  	Clone somePointers as a replacement content of this list invoking cloneFunction(oldObject)
//  	to generate each clone. The return from cloneFunction should be the unadopted return from new.
//  
void PrimSinglyLinkedListOfRefImp::clone(const PrimRefFuncs& supportFunctions,
  const PrimSinglyLinkedListOfRefElement& aList, CloneFunction cloneFunction)
{
 vacate(supportFunctions);
 for (const PrimSinglyLinkedListOfRefElement *p = _head; p; p = p->_next)
  adopt_tail(supportFunctions, *cloneFunction(p->element()));
}

//  
//  	Return the address of the pointer to the anIndex element.
//  
PrimSinglyLinkedListOfRefElement **PrimSinglyLinkedListOfRefImp::find(size_t anIndex)
{
 PrimSinglyLinkedListOfRefElement **pp = &_head;
 size_t i = 0;
 for (PrimSinglyLinkedListOfRefElement *p = *pp; p; pp = &p->_next, p = *pp, i++)
  if (i >= anIndex)
   return pp;
 return 0;
}

//  
//  	Return the address of the pointer to the first element matching anElement if there is one, or
//  	0 if not.
//  
PrimSinglyLinkedListOfRefElement **PrimSinglyLinkedListOfRefImp::find(const PrimRefValue& anElement)
{
 PrimSinglyLinkedListOfRefElement **pp = &_head;
 for (PrimSinglyLinkedListOfRefElement *p = *pp; p; pp = &p->_next, p = *pp)
  if (p->_element == &anElement)
   return pp;
 return 0;
}

//  
//  	Return a non-null pointer if anElement is non-null and matches one of the list pointers.
//  
const PrimRefValue *PrimSinglyLinkedListOfRefImp::find(const PrimRefFuncs& supportFunctions,
  const PrimRefValue& anElement) const
{
 UNUSED(&supportFunctions);
 return find(anElement) ? &anElement : 0;
}

//  
//  	Insert anElement before the element at (*pp). If pp is null insertion is after the tail.
//  
bool PrimSinglyLinkedListOfRefImp::insert_at(const PrimRefFuncs& supportFunctions, const PrimRefValue& anElement,
   PrimSinglyLinkedListOfRefElement **pp)
{
 PrimSinglyLinkedListOfRefElement *p = new PrimSinglyLinkedListOfRefElement(supportFunctions, anElement);
 if (!p)
 {
  ERRMSG("Failed to allocated memory for singly linked list element.");
  return false;
 }
 if (!pp)          //   At tail.
 {
  _tail ? _tail->_next : _head = p;
  _tail = p;
 }
 else if (!*pp)         //   Into empty list.
 {
  _head = p;
  _tail = p;
 }
 else           //   Not at tail of non-empty list.
 {
  p->_next = *pp;
  *pp = p;
 }
 _tally++;
 check(supportFunctions);
 return true;
}

//  
//  	Insert anElement in front of the element at anIndex.
//  
bool PrimSinglyLinkedListOfRefImp::insert_before(const PrimRefFuncs& supportFunctions,
  const PrimRefValue& anElement, size_t anIndex)
{
 return insert_at(supportFunctions, anElement, find(anIndex));
}

//  
//  	Display the full details of the list as part of a deep diagnostic print-out.
//  
std::ostream& PrimSinglyLinkedListOfRefImp::print_deep(const PrimRefFuncs& supportFunctions,
  std::ostream& s, int aDepth) const
{
 s << indent(aDepth);
 print_instance(supportFunctions, s);
 s << '\n';
 print_depth(supportFunctions, s, aDepth+1);
 return s;
}

//  
//  	Display the full details of the list as part of a deep diagnostic print-out.
//  
std::ostream& PrimSinglyLinkedListOfRefImp::print_depth(const PrimRefFuncs& supportFunctions,
  std::ostream& s, int aDepth) const
{
 for (const PrimSinglyLinkedListOfRefElement *p = _head; p; p = p->_next)
  supportFunctions.print_deep(p->element(), s, aDepth);
 return s;
}

//  
//  	Display the list dimensions as part of an inline diagnostic.
//  
std::ostream& PrimSinglyLinkedListOfRefImp::print_instance(const PrimRefFuncs& supportFunctions,
  std::ostream& s) const
{
 s << "PrimSinglyLinkedListOfRefImp";
 if (PrimPrint::print_address())
  PrimPrint::print_address_of(s, this);
 s << "(";
 print_this(supportFunctions, s);
 s << ")";
 return s;
}

//  
//  	Display the details of the list as part of a diagnostic print-out.
//  
std::ostream& PrimSinglyLinkedListOfRefImp::print_members(const PrimRefFuncs& supportFunctions,
  std::ostream& s, int aDepth) const
{
 const PrimSinglyLinkedListOfRefElement *p = _head;
 for (int i = 0; p; p = p->_next, i++)
 {
  s << indent(aDepth) << std::setw(4) << i << ' ';
  supportFunctions.print_instance(p->element(), s);
  s << '\n';
 }
 return s;
}

//  
//  	Display the full details of the list as part of a deep diagnostic print-out.
//  
std::ostream& PrimSinglyLinkedListOfRefImp::print_on(const PrimRefFuncs& supportFunctions,
  std::ostream& s, int aDepth) const
{
 s << indent(aDepth);
 print_instance(supportFunctions, s);
 s << '\n';
 print_members(supportFunctions, s, aDepth+1);
 return s;
}

//  
//  	Display the list dimensions as part of an inline diagnostic.
//  
std::ostream& PrimSinglyLinkedListOfRefImp::print_this(const PrimRefFuncs& supportFunctions,
  std::ostream& s) const
{
 UNUSED(&supportFunctions);
 return s << tally();
}

//  
//  	Remove the first reference to anElement from this list of pointers, closing up the gap.
//  	Returns true if an element removed.
//  
bool PrimSinglyLinkedListOfRefImp::remove(const PrimRefFuncs& supportFunctions, const PrimRefValue& anElement)
{
 PrimSinglyLinkedListOfRefElement **pp = find(anElement);
 if (!pp)
  return false;
 remove_at(supportFunctions, pp);
 return false;
}

//  
//  	Remove the element at **pp, closing the gap.
//  
bool PrimSinglyLinkedListOfRefImp::remove_at(const PrimRefFuncs& supportFunctions,
  PrimSinglyLinkedListOfRefElement **pp)
{
 PrimSinglyLinkedListOfRefElement *p = *pp;
 supportFunctions.annul(p->element());
 if ((*pp = p->_next) != 0)
  ;
 else if (pp == &_head)
  _tail = 0;
 else if (pp == &_tail)
  _head = 0;
 else
  _tail = (PrimSinglyLinkedListOfRefElement *)((char *)pp
   - ((char *)&((PrimSinglyLinkedListOfRefElement *)0x1000)->_next - 0x1000));
 p->_element = 0;
 delete p;
 _tally--;
 check(supportFunctions);
 return true;
}

//  
//  	Remove the element at anIndex, closing the gap. (The return is void rather than
//  	the element to avoid the generally redundant cost of constructing a copied return value).
//  	If anIndex is out of bounds, it is silently ignored.
//  	Returns true if an element removed.
//  
bool PrimSinglyLinkedListOfRefImp::remove(const PrimRefFuncs& supportFunctions, size_t anIndex)
{
 PrimSinglyLinkedListOfRefElement **pp = find(anIndex);
 if (!pp)
  return false;
 remove_at(supportFunctions, pp);
 return true;
}

//  
//  	Sort the list in accordance with compareRoutine.
//  
void PrimSinglyLinkedListOfRefImp::sort(int (*compareRoutine)(const void *v1, const void *v2))
{
 const PrimRefValue **sortBuffer = new const PrimRefValue *[_tally];
 const PrimRefValue **q = sortBuffer;
 for (const PrimSinglyLinkedListOfRefElement *p1 = _head; p1; p1 = p1->_next)
  *q++ = p1->_element;
 qsort((char *)sortBuffer, _tally, sizeof(*sortBuffer), compareRoutine);
 q = sortBuffer;
 for (PrimSinglyLinkedListOfRefElement *p2 = _head; p2; p2 = p2->_next)
  p2->_element = *q++;
 delete[] sortBuffer;
}

//  
//  	Empty the list.
//  
void PrimSinglyLinkedListOfRefImp::vacate(const PrimRefFuncs& supportFunctions)
{
 while (_head)
 {
  PrimSinglyLinkedListOfRefElement *p = _head;
  --_tally;
  _head = p->_next;
  supportFunctions.annul(*p->_element);
  p->_element = 0;
  delete p;
 }
 _tail = 0;
}

//  
//  	Generate an inline diagnostic desribing the list in so far as possible without any support functions.
//  
std::ostream& operator<<(std::ostream& s, const PrimSinglyLinkedListOfRefImp& aList)
{
 s << "PrimSinglyLinkedListOfRefImp";
 PrimPrint::print_address_of(s, &aList);
 s << "(" << aList.tally() << ")";
 return s; 
}
