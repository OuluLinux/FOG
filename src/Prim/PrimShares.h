
#ifndef PRIMSHARES_HXX
#define PRIMSHARES_HXX




//  
//  	A PrimShares maintains a share count, enforcing default or copy construction with one share,
//  	invariance under assign, and diagnosing inconsistent counts on destruction. The internal increment
//  	and decrement operations may (and must) be redefined to support atimic increment and decrement in
//  	a multi-threaded environment.
//  
class PrimShares
{
private:
 mutable int _shares;       //   Number of sharers of this object.

private:
//  
//  	An attempt to create an instance of a PrimShares from the heap is illegal.
//  
 void *operator new(size_t aSize) { return ((void *)(0 & aSize)); }
//  
//  	An attempt to delete an instance of a PrimShares off the heap is illegal.
//  
 void operator delete(void *) {}
 PrimShares& mutate() const { return *(PrimShares *)this; }

public:
//  
//  	Construct a shared counter with one initial share.
//  
 PrimShares() : _shares(1) {}
//  
//  	Construct a copy of a shared counter. The copied counter is unaffected. The constructed object has one share.
//  
 PrimShares(const PrimShares&) : _shares(1) {}
//  
//  	Destruction verifies consistent share count maintenance.
//  
 ~PrimShares();
//  
//  	Assign someShares to this object. This shares and the assigned shares are unaffected. 
//  
 PrimShares& operator=(const PrimShares&) { return *this; }
 bool annul() const;
//  
//  	Annul one share, returning true if successful. Returns false if there was only one share in which case the
//  	caller must destruct in order to annul the last share.
//  
 inline bool annul_inline() const { return _shares == 1 ? false : (mutate()._shares--, true); }
//  
//  	Register an additional shared copy.
//  
 void share() const { mutate()._shares++; }
//  
//  	Report the number of shares.
//  
 int shares() const { return _shares; }
 friend std::ostream& operator<<(std::ostream& s, const PrimShares& someShares);
};

//  
//  	Provide the standard declarations for a share count within the class declaration for an E.
//  
#define PRIMSHARES_DECLS(E) \
private:\
 PrimShares _shares; __C__((Share count.))\
protected:\
 bool annul_this() const { return !_shares.annul(); }\
public:\
 void annul() const;\
 void share() const { _shares.share(); }\
 int shares() const { return _shares.shares(); } \
 friend inline void annul(const E *anObject) { if (anObject) anObject->annul(); }\
private:

//  
//  	Provide the standard declarations for a share count within the class declaration for an E.
//  	All E methods are inline to avoid need for any isolated template methods.
//  
#define PRIMSHARES_DECLS_INLINE(E) \
private:\
 PrimShares _shares; __C__((Share count.))\
protected:\
 bool annul_this() const { return !_shares.annul(); }\
public:\
 void annul() const { if (annul_this()) delete (E *)this; }\
 void share() const { _shares.share(); }\
 int shares() const { return _shares.shares(); } \
 friend inline void annul(const E *anObject) { if (anObject) anObject->annul(); }\
private:

//  
//  	Provide the standard declarations for a share count within a derived class declaration for an E.
//    (Only needed when the lack of a virtual destructor invalidates the inherited version.)
//  
#define PRIMSHARES_DERIVED_DECLS(E) \
public:\
 void annul() const;\
private:

//  
//  	Provide the standard declarations for a share count within a derived class declaration for an E.
//    (Only needed when the lack of a virtual destructor invalidates the inherited version.)
//  
#define PRIMSHARES_DERIVED_DECLS_INLINE(E) \
public:\
 void annul() const { if (annul_this()) delete (E *)this; }\
private:

//  
//  	Provide the standard implementations for a share count within the class declaration for an E.
//  
#define PRIMSHARES_IMPL(E) \
 __C__((Annul one share of this object, deleting it once redundant.))\
 void E::annul() const { if (annul_this()) delete (E *)this; }\

#endif
 
