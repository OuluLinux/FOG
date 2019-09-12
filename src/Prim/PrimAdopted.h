// **************************** RACAL Confidential ****************************
// 
// 	  Title:			Adopted pointer to heap memory class interface
// 
// 	  File Name:		PrimAdopted.H
// 
// 	  Author:			E.D.Willink
// 
// 	  SCCS:				@(#)PrimAdopted.H	6.1 04/22/97
// 
// 	  Latest Modification:
//  EDW	1.1		Date:	23-Apr-1996		Original.
// END
// 

#ifndef PRIMADOPTED_H
#define PRIMADOPTED_H

#ifndef PRIM_H
#include <Prim/Prim.h>
#endif

// 
// 	A PrimAdopted<T> looks after a T * return from new T, taking responsibility for deletion
// 	of the object when destructed. Ownership may be transferred to another PrimAdopted<T> by
// 	copy or assign. Ownership may be acquired by release. The existence of an adopted object
// 	may be tested as either if (p) or if (!!p).
// 
// 	This class is intended to be as similar as possible to the upcoming auto_ptr<T> class.
// 
template <class T>
class PrimAdoptedBuiltIn
{
protected:
 T *_rep;       //  The adopted object.
private:
// 
// 	The Locked nested class defines a useless type that provides a safe pointer return value.
// 
 class Locked {};
public:
// 
// 	Construct an instance that adopts aT, which may be 0.
// 
 explicit PrimAdoptedBuiltIn(T *aT = 0) : _rep(aT) {}
// 
// 	Construction of a copy transfers the adopted object from the source.
// 
 PrimAdoptedBuiltIn(PrimAdoptedBuiltIn& adoptedT) : _rep(adoptedT.release(0)) {}
// 
// 	Destruction deletes the content if non-0.
// 
 ~PrimAdoptedBuiltIn() { if (_rep) delete _rep; }
// 
// 	Assignment transfers the adopted object from the source.
// 
 PrimAdoptedBuiltIn& operator=(PrimAdoptedBuiltIn& anAdopted)
 {
  if (this != &anAdopted)
  {
   T *const oldRep = _rep;   //  anAdopted might be a sub-object of _rep.
   _rep = anAdopted.release();  //   so retrieve the copied object before deletion
   if (oldRep)
    delete oldRep;
  }
  return *this;
 }
// 
// 	The existence of an adopted object may be determined by a pointer test.
// 
 operator const Locked *() const { return (Locked *)_rep; }
// 
// 	The non-existence of an adopted object may be determined by a complemented test.
// 
 bool operator!() const { return _rep == 0; }
// 
// 	Return a pointer to the adopted object. The caller should not delete the returned object. Use
// 	release to acquire the object with delete responsibility. 
// 
 T *get() const { return _rep; }
// 
// 	Releasing returns the current object adopting aT which may be 0 in its stead. The caller
// 	assumes responsibility for deletion of the return value. This object assumes responsibility for
// 	deletion of aT.
// 	
 T *release(T* aT = 0) { T *oldT = _rep; _rep = aT; return oldT; }
// 
// 	Resetting deletes any existing adopted object and then adopts aT which may be 0.
// 	
 void reset(T* aT = 0) { if (_rep) delete _rep; _rep = aT; }
};

// 
// 	A PrimAdopted<T> looks after a T * return from new T, taking responsibility for deletion
// 	of the object when destructed. Ownership may be transferred to another PrimAdopted<T> by
// 	copy or assign. Ownership may be acquired by release. The existence of an adopted object
// 	may be tested as either if (p) or if (!!p). When there is an adopted object operator*()
// 	and operator->() may be used for access.
// 
// 	This class is intended to be as similar as possible to the upcoming auto_ptr<T> class.
// 
template <class T>
class PrimAdopted : public PrimAdoptedBuiltIn<T>
{
public:
// 
// 	Construct an instance that adopts aT, which may be 0.
// 
 explicit PrimAdopted(T *aT = 0) : PrimAdoptedBuiltIn<T>(aT) {}
// 
// 	Indirection of the adoption object yields the adopted object. The result is ill-defined if the
// 	object is 0.
// 
 T& operator*() const { return *PrimAdoptedBuiltIn<T>::_rep; }
// 
// 	Indirection through the adoption object operatoes on the adopted object. The result is ill-defined
// 	if the object is 0.
// 
 T *operator->() const { return PrimAdoptedBuiltIn<T>::_rep; }
};
#endif
