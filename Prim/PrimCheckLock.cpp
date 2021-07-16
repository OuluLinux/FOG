#include <Prim/PrimIncludeAll.h>





#ifndef NO_PRIM_CHECKS
int PrimCheckLock::_check_locks = 0;    //   Number of locks in force.
#endif

//  
//  	Return the debug checking control variable.
//  
bool& PrimCheckLock::debug_check()
{
 static PrimGetEnv<bool> debugCheck("PrimCheckLock::debug_check", debug_full_check());
 return debugCheck;
}

//  
//  	Return the debug full check control variable.
//  
bool& PrimCheckLock::debug_full_check()
{
 static PrimGetEnv<bool> debugFullCheck("PrimCheckLock::debug_full_check", false);
 return debugFullCheck;
}
