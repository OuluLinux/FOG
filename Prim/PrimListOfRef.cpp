#include <Prim/PrimIncludeAll.h>










//
//  	The destructor releases any memory.
//
PrimListOfRefImp::~PrimListOfRefImp() {
	if (_contents) {
		if (_tally)
			ERRMSG(*this << " has not been vacated by derived destructor.");
			
		delete[] _contents;
		_contents = 0;
	}
}

//
//  	Add anElement to the list.
//
bool PrimListOfRefImp::add(const PrimRefFuncs& supportFunctions, const PrimRefValue& anElement) {
	if (!set_capacity(supportFunctions, tally() + 1))
		return false;
		
	PrimRefBase* q = _contents + _tally;
	supportFunctions.share(*(*q = anElement));
	_tally++;
	return true;
}

//
//  	Add aList to this list.
//
bool PrimListOfRefImp::add(const PrimRefFuncs& supportFunctions, const PrimListOfRefImp& aList) {
	if (!set_capacity(supportFunctions, tally() + aList.tally()))
		return false;
		
	const PrimRefBase* p = aList._contents;
	PrimRefBase* q = _contents + _tally;
	
	for (size_t i = aList.tally(); i-- > 0; _tally++)
		supportFunctions.share(*(*q++ = *p++));
		
	return true;
}

//
//  	Add anObject to the list, relieving the caller of the responsibility for deleting anObject.
//  	It is more efficient for the caller to resolve the existence of anObject
//  	and invoke add() as appropriate, since the abstract behaviour has to invoke a virtual method
//  	to ensure that sharing is maintained correctly. However the convenience of just invoking
//  	aList.adopt(new T) is more important.
//
bool PrimListOfRefImp::adopt(const PrimRefFuncs& supportFunctions, const PrimRefValue& anObject) {
	bool returnStatus = add(supportFunctions, anObject);
	supportFunctions.annul(anObject);
	return (returnStatus);
}

//
//  	Assign somePointers as a replacement content of this list.
//
void PrimListOfRefImp::assign(const PrimRefFuncs& supportFunctions,
                              const PrimListOfRefImp& somePointers) {
	if ((this != &somePointers)
	        && (vacate(supportFunctions), set_capacity(supportFunctions, somePointers.tally()))) {
		const PrimRefBase* p = somePointers._contents;
		PrimRefBase* q = _contents;
		
		if (p && q)
			for ( ; _tally < somePointers.tally(); _tally++)
				supportFunctions.share(*(*q++ = *p++));
	}
}

//
//  	Assign the elements of aMap as a replacement content of this list.
//
void PrimListOfRefImp::assign(const PrimRefFuncs& supportFunctions, const PrimMapOfRefImp& aMap) {
	if (vacate(supportFunctions), set_capacity(supportFunctions, aMap.tally())) {
		const PrimRefValue** p = aMap._contents;
		PrimRefBase* q = _contents;
		
		if (p && q)
			for (size_t i = aMap.capacity(); i-- > 0; p++)
				if (*p) {
					supportFunctions.share(*(*q++ = **p));
					_tally++;
				}
	}
}

//
//  	Perform a consistency check returning false after generating any error diagnostic.
//
bool PrimListOfRefImp::check(const PrimRefFuncs& supportFunctions, bool fullCheck) const {
	if (!PrimCheckLock::checking_enabled())
		return false;
		
	if (_capacity && !_contents) {
		ERRMSG(*this << " has capacity but no contents.");
		return false;
	}
	
	if (!_capacity && _contents) {
		ERRMSG(*this << " has contents but no capacity.");
		return false;
	}
	
	if (_tally > _capacity) {
		ERRMSG(*this << " has inconsistent tally.");
		return false;
	}
	
	if (fullCheck) {
		const PrimRefBase* p = contents();
		
		for (size_t i = tally(); i-- > 0; p++)
			if (!supportFunctions.check(**p, fullCheck))
				return false;
	}
	
	return true;
}

//
//  	Clone somePointers as a replacement content of this list invoking cloneFunction(oldObject)
//  	to generate each clone. The return from cloneFunction should be the unadopted return from new.
//
void PrimListOfRefImp::clone(const PrimRefFuncs& supportFunctions, const PrimListOfRefImp& somePointers,
                             CloneFunction cloneFunction) {
	if ((this != &somePointers)
	        && (vacate(supportFunctions), set_capacity(supportFunctions, somePointers.tally()))) {
		const PrimRefBase* p = somePointers._contents;
		PrimRefBase* q = _contents;
		
		if (p && q)
			for ( ; _tally < somePointers.tally(); _tally++)
				*q++ = *cloneFunction(**p++);
	}
}

//
//  	Return a non-null pointer if anElement is non-null and matches one of the list pointers.
//
const PrimRefValue* PrimListOfRefImp::find(const PrimRefFuncs& /*supportFunctions*/, const PrimRefValue& anElement) const {
	/*UNUSED(&supportFunctions);*/
	const PrimRefBase* p = contents();
	
	for (size_t i = tally(); i-- > 0; p++)
		if (*p == anElement)
			return (&anElement);
			
	return 0;
}

//
//  	Insert anElement in front of the element at anIndex.
//
bool PrimListOfRefImp::insert_before(const PrimRefFuncs& supportFunctions, const PrimRefValue& anElement,
                                     unsigned int anIndex) {
	if (anIndex >= tally())
		anIndex = tally();
		
	if (!set_capacity(supportFunctions, tally() + 1))
		return false;
		
	PrimRefBase* q = _contents + _tally;
	
	for (size_t i = tally(); i > anIndex; q--, i--)
		*q = *(q - 1);
		
	supportFunctions.share(*(*q = anElement));
	_tally++;
	check(supportFunctions);
	return true;
}

//
//  	Display the full details of the list as part of a deep diagnostic print-out.
//
std::ostream& PrimListOfRefImp::print_deep(const PrimRefFuncs& supportFunctions,
        std::ostream& s, int aDepth) const {
	s << indent(aDepth);
	print_instance(supportFunctions, s);
	s << '\n';
	print_depth(supportFunctions, s, aDepth + 1);
	return s;
}

//
//  	Display the full details of the list as part of a deep diagnostic print-out.
//
std::ostream& PrimListOfRefImp::print_depth(const PrimRefFuncs& supportFunctions,
        std::ostream& s, int aDepth) const {
	const PrimRefBase* p = contents();
	
	for (size_t i = tally(); i-- > 0; p++)
		supportFunctions.print_deep(**p, s, aDepth);
		
	return s;
}

//
//  	Display the list dimensions as part of an inline diagnostic.
//
std::ostream& PrimListOfRefImp::print_instance(const PrimRefFuncs& supportFunctions, std::ostream& s) const {
	s << "PrimListOfRefImp";
	
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
std::ostream& PrimListOfRefImp::print_members(const PrimRefFuncs& supportFunctions,
        std::ostream& s, int aDepth) const {
	const PrimRefBase* p = contents();
	
	for (size_t i = 0; i < tally(); i++, p++) {
		s << indent(aDepth) << std::setw(4) << i << ' ';
		supportFunctions.print_instance(**p, s);
		s << '\n';
	}
	
	return s;
}

//
//  	Display the full details of the list as part of a deep diagnostic print-out.
//
std::ostream& PrimListOfRefImp::print_on(const PrimRefFuncs& supportFunctions, std::ostream& s, int aDepth) const {
	s << indent(aDepth);
	print_instance(supportFunctions, s);
	s << '\n';
	print_members(supportFunctions, s, aDepth + 1);
	return s;
}

//
//  	Display the list dimensions as part of an inline diagnostic.
//
std::ostream& PrimListOfRefImp::print_this(const PrimRefFuncs& /*supportFunctions*/, std::ostream& s) const {
	/*UNUSED(&supportFunctions);*/
	return (s << tally() << '/' << capacity());
}

//
//  	Remove the first reference to anElement from this list of pointers, closing up the gap.
//  	Returns true if an element removed.
//
bool PrimListOfRefImp::remove(const PrimRefFuncs& supportFunctions, const PrimRefValue& anElement) {
	PrimRefBase* p = contents();
	
	for (size_t i = tally(); i-- > 0; p++)
		if (*p == anElement) {
			for ( ; i-- > 0; p++)
				*p = *(p + 1);
				
			supportFunctions.annul(anElement);
			_tally--;
			check(supportFunctions);
			return true;
		}
		
	return false;
}

//
//  	Remove the element at anIndex, closing the gap. (The return is void rather than
//  	the element to avoid the generally redundant cost of constructing a copied return value).
//  	If anIndex is out of bounds, it is silently ignored.
//  	Returns true if an element removed.
//
bool PrimListOfRefImp::remove(const PrimRefFuncs& supportFunctions, size_t anIndex) {
	if (anIndex >= tally())
		return false;
		
	PrimRefBase* p = contents() + anIndex;
	const PrimRefValue& anElement = **p;
	
	for (size_t i = tally() - anIndex - 1; i-- > 0; p++)
		*p = *(p + 1);
		
	supportFunctions.annul(anElement);
	_tally--;
	check(supportFunctions);
	return true;
}

//
//  	Increase the capacity to the largest of aCapacity, aTally, _capacity, and the tally to aTally
//  	returning true if successful. The contents shared between the old and new tally are preserved.
//  	Obsolete tallied contents are released. New tallied contents are null. The untallied contents
//  	are unshared but otherwise unpredictable.
//
bool PrimListOfRefImp::set_sizes(const PrimRefFuncs& supportFunctions, size_t aCapacity, size_t aTally) {
	if (aCapacity < aTally)
		aCapacity = aTally;
		
	if (capacity() < aCapacity) {
		const size_t newCapacity = std::max(std::max(4, (int)aCapacity), 2 * (int)capacity());
		PrimRefBase* const newContents = new PrimRefBase[(int)newCapacity];
		
		if (!newContents) {
			ERRMSG(*this << " failed to allocate memory for PrimRefBase[" << newCapacity << "].");
			return false;
		}
		
		const PrimRefBase* p = contents();
		PrimRefBase* q = newContents;
		
		for (size_t i = tally(); i-- > 0; )
			*q++ = *p++;
			
		PrimRefBase* const oldContents = contents();
		_contents = newContents;
		_capacity = newCapacity;
		
		if (oldContents)
			delete[] oldContents;
	}
	
	while (aTally < tally())      //   Reverse, non-pointer traversal to accommodate
		supportFunctions.annul(*_contents[--_tally]);    //    re-entrant removals.
		
	if (aTally > tally()) {
		PrimRefBase* q = _contents + tally();
		
		do {
			*q++ = supportFunctions.shared_null();
		} while (aTally > ++_tally);
	}
	
	return true;
}

//
//  	Sort the list from startIndex through to startIndex + numIndex -1 in accordance with compareRoutine.
//
void PrimListOfRefImp::sort(int (*compareRoutine)(const void* v1, const void* v2),
                            size_t startIndex, size_t numIndexes) {
	if (numIndexes > 1)
		qsort((char*)&_contents[startIndex], numIndexes, sizeof(*_contents), compareRoutine);
}

//
//  	Empty the list.
//
void PrimListOfRefImp::vacate(const PrimRefFuncs& supportFunctions) {
	if (_contents)
		while (_tally > 0)        //   Reverse, non-pointer traversal to accommodate
			supportFunctions.annul(*_contents[--_tally]);    //    re-entrant removals.
}

//
//  	Generate an inline diagnostic desribing the list in so far as possible without any support functions.
//
std::ostream& operator<<(std::ostream& s, const PrimListOfRefImp& aList) {
	s << "PrimListOfRefImp";
	PrimPrint::print_address_of(s, &aList);
	s << "(" << aList.tally() << "/" << aList.capacity() << ")";
	return s;
}
