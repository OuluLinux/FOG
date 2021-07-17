#include <Prim/PrimIncludeAll.h>
















PRIMREFTOCONST_IMPL(PrimId)
PRIMLISTOFREFTOCONST_IMPL(PrimId)
PRIMMAPOFREFTOCONST_IMPL(PrimId)
PRIMSHAREDMAPOFREFTOCONST_IMPL(PrimId)

const PrimId** PrimId::_contents = 0;  //   Hashed table of pointers to entries.
size_t PrimId::_tally = 0;     //   Number of entries in table.
size_t PrimId::_capacity = 0;    //   Maximum number of entries in table.
char PrimId::_null[sizeof(PrimId)] = { 0 }; //   The null instance.

//
//  	PrimIdCyclicIterator iterates in a cyclic fashion over the static id array. Use as:
//
//  	for (PrimIdCyclicIterator p(aList); p; p++)
//  		if (*p)
//  			p->do_something();
//
class PrimIdCyclicIterator {
private:
	const PrimId** _current;
	const PrimId** _max;
	size_t _to_go;
public:
	PrimIdCyclicIterator(unsigned long anIndex)
		:
		_current(PrimId::_contents + anIndex),
		_max(PrimId::_contents + PrimId::_capacity),
		_to_go(PrimId::_capacity)
	{}
	PrimIdCyclicIterator(const PrimIdCyclicIterator& aCyclicIterator)
		: _current(aCyclicIterator._current), _max(aCyclicIterator._max), _to_go(aCyclicIterator._to_go) {}
	PrimIdCyclicIterator& operator= (const PrimIdCyclicIterator& aCyclicIterator) {
		_current = aCyclicIterator._current; _max = aCyclicIterator._max;
		_to_go = aCyclicIterator._to_go; return *this;
	}
	const PrimId*& operator *() const { return (*_current); }
	void operator++() { _current = ++_current >= _max ? PrimId::_contents : _current; }
	operator const void* () const { return (_to_go ? this : 0); }
	bool operator!() const { return (_to_go ? false : true); }
};

//
//  	Return the hashed index for hashCode.
//
inline unsigned long PrimId::hash_index(unsigned long hashCode) {
	return hashCode & (_capacity - 1);
}

//
//  	Return the hash-code resulting from updating hashCode with hashInput.
//
inline unsigned long PrimId::hash_update(unsigned long hashCode, unsigned long hashInput) {
	return (hashCode << 5) ^ hashCode ^ hashInput;
}

//
//  	Construct a new representation for aBuffer[aLength], which have aHashCode.
//
inline PrimId::PrimId(const char* aBuffer, unsigned short aLength, unsigned long aHashCode)
	:
	_hash(aHashCode),
	_length(aLength),
	_text(0) {
	if (!aBuffer)
		ERRMSG("BUG - should not construct a null PrimId.");
		
	char* mutableText = (char*)&_text;
	memcpy(mutableText, aBuffer, aLength);
	mutableText[aLength] = 0;
}

//
//  	Add an additional object to the map.
//
void PrimId::add(const PrimId& anId) {
	if (_tally >= _capacity / 2)    //   Expand map if would be over half full.
		set_capacity(std::max(_capacity * (size_t)2, (size_t)256));
		
	for (PrimIdCyclicIterator p(hash_index(anId.hash())); p; ++p)
		if (!*p) {
			*p = &anId;
			_tally++;
			break;
		}
		else if (*p == &anId) { //   Attempt to create duplicate entry ?
			GLOBAL_ERRMSG("PrimId cannot create duplicate entry for " << anId << '.');
			break;
		}
		
	check_map();
}

//
//  	Unshare the string representation.
//
void PrimId::annul() const {
	if (!_shares.annul_inline() && !is_null()) {
		PrimId* mutableThis = (PrimId*)this;
		mutableThis->PrimId::~PrimId();
		delete[] (char*)mutableThis;
	}
}

//
//  	Performa consistency check, validating the hash code.
//
bool PrimId::check(bool fullCheck) const {
	if (!fullCheck)
		return true;
		
	unsigned long hashCode = 0;
	const char* p = str();
	
	for (size_t i = length(); i-- > 0; p++)
		hashCode = hash_update(hashCode, *p);
		
	if (hashCode != hash()) {
		ERRMSG(*this << " has hash code " << hash() << " rather than " << hashCode);
		return false;
	}
	
	if (find(str(), length(), hash()) != this) {
		ERRMSG(*this << " could not be found");
		return false;
	}
	
	return true;
}

//
//  	Perform a self-consistency check.
//
bool PrimId::check_map(bool fullCheck) {
	UNUSED(fullCheck);
	
	if (!PrimCheckLock::checking_enabled())
		return true;
		
	size_t checkTally = 0;
	const PrimId* const* p = _contents;
	
	for (size_t j = _capacity; j-- > 0; p++) {
		if (*p) {
			checkTally++;
			
			if (find_slot(**p) != p) {
				GLOBAL_ERRMSG("PrimId has " << **p << " mis-hashed.");
				return false;
			}
		}
	}
	
	if (_tally != checkTally) {
		GLOBAL_ERRMSG("PrimId has << " << checkTally << " actual contents when " << _tally << " tallied.");
		return false;
	}
	
	return true;
}

//
//  	Compare this string with anId for sort comparison purposes, returns -1
//  	if "this" < anId, 0 if == anId, +1 if "this" > anId. null text sorts after non-null text.
//
int PrimId::compare(const PrimId& anId) const {
	if (this == &anId)      //   Catch the trivial case and the both null case
		return 0;
		
	int anOrder = memcmp(str(), anId.str(), std::min(length(), anId.length()));
	
	if (anOrder != 0)
		return anOrder;
		
	anOrder = length() - anId.length();
	
	if (anOrder != 0)
		return anOrder;
		
	return !is_null();
}

//
//  	Construct an id representation for some null terminated text. In the event of
//  	an allocation failure the 0 is returned. The caller is responsible for elimination of the shared return.
//
//  	PrimId::create(0) returns the null-id which is PrimId::null().
//  	PrimId::create("") returns the empty-id which is distinct from PrimId::null().
//
const PrimId* PrimId::create(const char* someText) {
	if (!someText)
		return &null();
		
	unsigned long hashCode = 0;
	const char* p = someText;
	
	for (size_t i = USHRT_MAX; (i-- > 0) && *p; p++)
		hashCode = hash_update(hashCode, *p);
		
	if (*p)
		ERRMSG("Identifier truncated to " << USHRT_MAX << " characters.");
		
	return create(someText, (unsigned short)(p - someText), hashCode);
}

//
//  	Construct an id representation for aBuffer[aLength]. In the event of an allocation failure
//  	0 is returned. The caller is responsible for elimination of the shared return.
//
//  	PrimId::create(0, anyLength) returns the null-id which is PrimId::null().
//  	PrimId::create(nonZeroBuffer, 0) returns the empty-id which is distinct from PrimId::null().
//
const PrimId* PrimId::create(const char* aBuffer, size_t aLength) {
	if (!aBuffer)
		return &null();
		
	if (aLength > USHRT_MAX) {
		ERRMSG("Identifier truncated from " << aLength << " to " << USHRT_MAX << " characters.");
		aLength = USHRT_MAX;
	}
	
	unsigned long hashCode = 0;
	const char* p = aBuffer;
	
	for (size_t i = aLength; i-- > 0; p++)
		hashCode = hash_update(hashCode, *p);
		
	return create(aBuffer, (unsigned short)(aLength), hashCode);
}

//
//  	Construct an id representation with hashCode for aBuffer[aLength]. In the event of an allocation failure
//  	0 is returned. The caller is responsible for elimination of the shared return. This method does the
//  	actual work for the other creation methods.
//
const PrimId* PrimId::create(const char* aBuffer, unsigned short aLength, unsigned long hashCode) {
	if (!aBuffer)
		return &null();
		
	if (_tally >= _capacity / 2)      //   Expand map if would be over half full.
		set_capacity(std::max((int)_capacity * 2ul, 256ul));
		
	PrimIdCyclicIterator q(hash_index(hashCode));
	
	for ( ; q && *q; ++q) {
		if ((hashCode == (*q)->_hash) && (aLength == (*q)->_length)
		        && (memcmp(aBuffer, &(*q)->_text, aLength) == 0)) {
			(*q)->share();
			return *q;
		}
	}
	
	if (*q) {
		GLOBAL_ERRMSG("PrimId failed to locate empty slot.");
		return 0;
	}
	
	char* aProtoRep = new char[sizeof(PrimId) + aLength];
	
	if (!aProtoRep) {
		GLOBAL_ERRMSG("PrimId failed to allocate memory for a " << aLength << " byte PrimId.");
		return 0;
	}
	
	*q = new (aProtoRep) PrimId(aBuffer, aLength, hashCode);
	_tally++;
	check_map();
	return *q;
}

//
//  	Locate the shared representation for some null terminated text, returning 0 if not found.
//
//  	PrimId::find(0) always returns the null-id which is PrimId::null().
//  	PrimId::find("") may return the empty-id which is distinct from PrimId::null().
//
const PrimId* PrimId::find(const char* someText) {
	if (!someText)
		return &null();
		
	unsigned long hashCode = 0;
	const char* p = someText;
	
	for (size_t i = USHRT_MAX; (i-- > 0) && *p; p++)
		hashCode = hash_update(hashCode, *p);
		
	if (*p)
		ERRMSG("Identifier truncated to " << USHRT_MAX << " characters.");
		
	return find(someText, (unsigned short)(p - someText), hashCode);
}

//
//  	Locate the shared representation for aBuffer[aLength], returning 0 if not found.
//
//  	PrimId::find(0, anyLength) always returns the null-id which is PrimId::null().
//  	PrimId::find(nonZeroBuffer, 0) may return the empty-id which is distinct from PrimId::null().
//
const PrimId* PrimId::find(const char* aBuffer, size_t aLength) {
	if (!aBuffer)
		return &null();
		
	if (aLength > USHRT_MAX) {
		ERRMSG("Identifier truncated from " << aLength << " to " << USHRT_MAX << " characters.");
		aLength = USHRT_MAX;
	}
	
	unsigned long hashCode = 0;
	const char* p = aBuffer;
	
	for (size_t i = aLength; i-- > 0; p++)
		hashCode = hash_update(hashCode, *p);
		
	return find(aBuffer, (unsigned short)(aLength), hashCode);
}

//
//  	Locate the shared representation for aBuffer[aLength] at hashCode, returning 0 if not found.
//
const PrimId* PrimId::find(const char* aBuffer, unsigned short aLength, unsigned long hashCode) {
	if (!aBuffer)
		return &null();
		
	for (PrimIdCyclicIterator q(hash_index(hashCode)); q && *q; ++q)
		if ((aLength == (*q)->_length) && (memcmp(aBuffer, &(*q)->_text, aLength) == 0))
			return *q;
			
	return 0;
}

//
//  	Locate the slot for anId, returning 0 if not found.
//
const PrimId** PrimId::find_slot(const PrimId& anId) {
	for (PrimIdCyclicIterator p(hash_index(anId.hash())); p; ++p)
		if (!*p || (*p == &anId))
			return &*p;
			
	return 0;
}

//
//  	Provide a full diagnostic print out of the contents of this id.
//
std::ostream& PrimId::print_depth(std::ostream& s, int indentLevel) const {
	return s;
}

//
//  	Provide a full diagnostic print out of the contents of this id-rep map.
//
std::ostream& PrimId::print_map_on(std::ostream& s, int indentLevel) {
	const PrimId* const* p = _contents;
	
	for (size_t i = 0; i < _capacity; i++, p++)
		if (*p)
			s << indent(indentLevel) << i << " -> " << **p << '\n';
			
	return s;
}

//
//  	Contribute a tally/capacity usage summary to a diagnostic.
//
std::ostream& PrimId::print_usage(std::ostream& s) {
	return s << _tally << '/' << _capacity;
}

//
//  	Remove anId and repair the map.
//
void PrimId::remove(PrimId& anId) {
	const PrimId** pStart = find_slot(anId);
	
	if (!pStart) {
		GLOBAL_ERRMSG("PrimId cannot remove non-existent " << anId << '.');
		return;
	}
	
	const PrimId** p = pStart;        //   Pointer to entry under consideration
	const PrimId* const* const pMax = _contents + _capacity; //   Upper limit of ident table
	*pStart = 0;            //   Lose specified entry
	size_t n = _capacity;
	
	while (--n > 0) {          //   Potential loop of all except the trashed entry
		p++;             //   Advance pointers to next entry
		
		if (p >= pMax)
			p = _contents;
			
		if (!*p)            //   If empty slot, hash table now repaired
			break;
			
		const PrimId* const* const pSlot = _contents + hash_index((*p)->hash()); //   Rehash the slot
		
		if ((pStart < p) ? ((pSlot <= pStart) || (p < pSlot)) : ((pSlot <= pStart) && (p < pSlot))) {
			*pStart = *p;          //   Move entry back to the empty slot
			pStart = p;           //   Adjust the empty slot position
			*pStart = 0;          //   Declare the new empty slot
		}
	}
	
	_tally--;             //   One less entry in table
	check_map();
	
	if (_tally == 0) {         //   Clean up the id_rep map if the application
		//    is tidy enough to remove all its ids.
		_capacity = 0;
		::delete[] _contents, _contents = 0;
	}
}

//
//  	Set the map capacity to at least aCapacity, returning true if successful.
//
bool PrimId::set_capacity(size_t aCapacity) {
	if ((aCapacity & (aCapacity - 1)) != 0) {
		GLOBAL_ERRMSG("PrimId map size is not a power of two.");
		return false;
	}
	
	if (aCapacity > _capacity) {
		const PrimId** const oldContents = _contents;
		const size_t oldCapacity = _capacity;
		const size_t newCapacity = std::max((int)aCapacity * 1ul, 16ul);
		PrimId** newContents = new PrimId *[newCapacity];
		
		if (!newContents) {
			GLOBAL_ERRMSG("PrimId failed to allocate memory for " << _capacity << " id slots.");
			return false;
		}
		
		for (size_t i = 0; i < newCapacity; i++)
			newContents[i] = 0;
			
		_capacity = newCapacity;
		typedef const PrimId** Fudge;    //   Sparcworks C++ 4.1 needs this to suppress bogus
		_contents = Fudge(newContents);     //   warning assigning PrimId ** to const PrimId **.
		_tally = 0;
		const PrimId** const oldEnd = oldContents + oldCapacity;
		
		for (const PrimId** p = oldContents; p < oldEnd; p++)
			if (*p)
				add(**p);
				
		::delete[] oldContents;
	}
	
	return true;
}

//
//  	Return an ordering between s1 and s2, with a calling signature compatible with PrimText::sort().
//
int PrimId::sort_compare(const PrimRefToConst<PrimId>* s1, const PrimRefToConst<PrimId>* s2) {
	return (*s1)->compare(**s2);
}

//
//  	Contribute the text representation to an inline diagnostic.
//
std::ostream& operator<<(std::ostream& s, const PrimId& anId) {
	s << c_string(anId.str(), anId.length(), 0, 0, PrimCString::ECHO);
	//  	s.write(anId.str(), anId.length());
	return s;
}

//
//  		Read a word (1024 characters maximum) from the input stream and store it in anId.
//
//  		Returns the updated stream.
//
std::istream& operator>>(std::istream& s, PrimIdHandle& idHandle) {
	char aWord[1025];
	s >> aWord;
	idHandle = s ? aWord : 0;
	return s;
}

#ifndef NO_DBX_SUPPORT
//
//  		Debug routine for use with dbxtool.
//
const char* dbx_id(const PrimId* anId) {
	if (!anId)
		return "(nil)";
	else
		return anId->str();
}
#endif
