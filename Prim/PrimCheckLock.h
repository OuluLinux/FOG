
#ifndef PRIMCHECKLOCK_HXX
#define PRIMCHECKLOCK_HXX


//  
//  		A PrimCheckLock establishes a lock preventing checks occurring while an invariant property
//  		is changed, and arranges for a check after the change.
//  .note cscrelations
//  		PrimCheckLock inhibits the checking performed by PrimPrint::check().
//  .note csusum
//  		The lock is created by construction, and released automatically on destruction.
//  .note limits
//  		The lock is maintained by a single global variable and so no checks are performed by
//  		nested locks.
//  
class PrimCheckLock
{
private:
#ifndef NO_PRIM_CHECKS
 const PrimPrint& _inhibitor;    //   Object inhibiting checks.
 bool _full_check;       //   true for full check on destruction.
 static int _check_locks;     //   Number of locks in force.
#endif

private:
//  
//  	Creation of a copy is forbidden.
//  
 PrimCheckLock(const PrimCheckLock& aCheckLock);    
//  
//  	Assignment is forbidden.
//  
 PrimCheckLock& operator=(const PrimCheckLock& aCheckLock);

private:
//  
//  		An attempt to create an instance of a PrimCheckLock from the heap results a zero return.
//  
 void *operator new(size_t aSize) { return ((void *)(0 & aSize)); }
//  
//  		An attempt to delete an instance of a PrimCheckLock off the heap is ignored.
//  
 void operator delete(void *p) { UNUSED(p); }

public:
//  
//  		Construct a checking lock enforced by anInhibitor. On destruction a check(fullCheck)
//  		is performed on anInhibitor.
//  .details
//  		No check is made on creation of the lock since in many circumstances such as dispatch
//  		from a queue the invariant may already have started to change, and the lock is imposed
//  		while the invariant is restored.
//  
 PrimCheckLock(const PrimPrint& anInhibitor, bool fullCheck = false)
#ifndef NO_PRIM_CHECKS
  : _inhibitor(anInhibitor), _full_check(fullCheck) { _check_locks++; }
#else
  { UNUSED(&anInhibitor); UNUSED(fullCheck); }
#endif
//  
//  		Destructing the final check lock activates a check.
//  
#ifndef NO_PRIM_CHECKS
 ~PrimCheckLock() { if (--_check_locks == 0) _inhibitor.check(_full_check); }
#else
 ~PrimCheckLock() {}
#endif
//  
//  		Report whether checking may be performed.
//  
#ifndef NO_PRIM_CHECKS
 static bool checking_enabled() { return (debug_check() || debug_full_check()) && (_check_locks == 0); }
#else
 static bool checking_enabled() { return false; }
#endif
 static bool& debug_check();
 static bool& debug_full_check();
//  
//  		Report whether checks are locked.
//  
#ifndef NO_PRIM_CHECKS
 static bool is_locked() { return _check_locks > 0; }
#else
 static bool is_locked() { return false; }
#endif
};
#endif
 
