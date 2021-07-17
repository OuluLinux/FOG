// **************************** RACAL Confidential ****************************
//
// 	  Title:			Adopted pointer to dimensioned array of heap memory class interface
//
// 	  File Name:		PrimCountedArray.H
//
// 	  Author:			E.D.Willink
//
// 	  SCCS:				@(#)PrimCountedArray.H	6.1 04/22/97
//
// 	  Latest Modification:
//  EDW	1.1		Date:	29-Oct-1996		Original.
// END
//

#ifndef PRIMCOUNTEDARRAY_H
#define PRIMCOUNTEDARRAY_H

#ifndef PRIM_H
	#include <Prim/Prim.h>
#endif

//
// 	A PrimCountedArray<T> looks after a T[N] return from new T[N], taking responsibility for deletion
// 	of the object when destructed. Ownership may be transferred to another PrimCountedArray<T> by
// 	copy or assign. The existence of an adopted object may be tested as either if (p) or if (!!p).
//
// 	This class is intended to be similar to the upcoming auto_ptr<T> class.
//
template <class T>
class PrimCountedArray {
protected:
	size_t _capacity;     //  Allocated size of _rep in units of T.
	T* _rep;       //  The adopted object.
private:
	//
	// 	The Locked nested class defines a useless type that provides a safe pointer return value.
	//
	class Locked {};
public:
	//
	// 	Construct a default instance without contents.
	//
	PrimCountedArray() : _capacity(0), _rep(0)  {}
	//
	// 	Construct an instance that adopts aT[aCapacity].
	//
	explicit PrimCountedArray(T* aT, size_t aCapacity) : _capacity(aCapacity), _rep(aT) {}
	//
	// 	Construction of a copy transfers the adopted object from the source.
	//
	PrimCountedArray(PrimCountedArray& adoptedT) : _capacity(adoptedT.capacity()), _rep(adoptedT.release()) {}
	//
	// 	Destruction deletes the content if non-0.
	//
	~PrimCountedArray() { if (_rep) delete[] _rep; }
	//
	// 	Assignment transfers the adopted object from the source.
	//
	PrimCountedArray& operator=(PrimCountedArray& anAdopted) {
		if (this != &anAdopted) {
			T* const oldRep = _rep;    //  anAdopted might be a sub-object of _rep.
			_capacity = anAdopted.capacity(); //   so retrieve the copied
			_rep = anAdopted.release();   //    object before deletion
			
			if (oldRep)
				delete[] oldRep;
		}
		
		return *this;
	}
	//
	// 	The existence of an adopted object may be determined by a pointer test.
	//
	operator const Locked* () const { return (Locked*)_rep; }
	//
	// 	The non-existence of an adopted object may be determined by a complemented test.
	//
	bool operator!() const { return _rep == 0; }
	//
	// 	Return the configured capacity.
	//
	size_t capacity() const { return _rep ? _capacity : 0; }
	//
	// 	Return a pointer to the adopted object. The caller should not delete the returned object. Use
	// 	release to acquire the object with delete responsibility.
	//
	T* get() const { return _rep; }
	//
	// 	Releasing without an argument returns the current object and adopts nothing in its stead. The caller
	// 	assumes responsibility for deletion of the return value.
	//
	T* release() { T* oldT = _rep; _rep = 0; _capacity = 0; return oldT; }
	//
	// 	Releasing with an argument returns the current object adopting aT[aCapacity] in its stead. The caller
	// 	assumes responsibility for deletion of the return value. This object assumes responsibility for
	// 	deletion of aT.
	//
	T* release(T* aT, size_t aCapacity) { T* oldT = _rep; _capacity = aCapacity; _rep = aT; return oldT; }
	//
	// 	Resetting without an argument deletes any existing adopted object and then adopts nothing.
	//
	void reset() { if (_rep) delete[] _rep; _capacity = 0; _rep = 0; }
	//
	// 	Resetting with an argument deletes any existing adopted object and then adopts aT[aCapacity].
	//
	void reset(T* aT, size_t aCapacity) { if (_rep) delete[] _rep; _capacity = aCapacity; _rep = aT; }
};
#endif
