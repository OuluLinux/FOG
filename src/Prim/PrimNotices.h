// **************************** RACAL Confidential ****************************
// 
// 	  Title:			Notice include file
// 
// 	  File Name:		PrimNotices.h
// 
// 	  Author:			E.D.Willink
// 
// 	  SCCS:				%W% %G%
// 
// 	  Latest Modification:
//  EDW	6.1		Date:	28-Jan-1997		Original split off from PrimNotice.xx
//  EDW	6.2		Date:	26-Nov-1997		Change buf to support use with derived stream
//  EDW	7.1		Date:	21-Aug-1998		use std:: streams
// END
// 

#ifndef PRIMNOTICES_H
#define PRIMNOTICES_H

#ifndef NO_PRIM_STREAMS
#ifndef PRIMNOTICE_DIAGNOSTIC_HXX
#include <Prim/PrimNotice_Diagnostic.h>
#endif
#ifndef PRIMNOTICE_ERROR_HXX
#include <Prim/PrimNotice_Error.h>
#endif
#ifndef PRIMNOTICE_WARNING_HXX
#include <Prim/PrimNotice_Warning.h>
#endif
#ifndef _STRSTREAM_
#include <strstream>
#endif

#define CONDMSG(cond, a) do { if (cond) { char b_u_f[512] = { 0 }; std::ostrstream oss(b_u_f, sizeof(b_u_f)-1); \
std::ostream &oss_ref = oss; oss_ref << a << std::ends; \
PrimNotice_Diagnostic aNotice(b_u_f, sizeof(b_u_f)); aNotice.post_on(prim_notifier()); } } while (false)
#define DIAMSG(a) do { char b_u_f[512] = { 0 }; std::ostrstream oss(b_u_f, sizeof(b_u_f)-1); std::ostream &oss_ref = oss; oss_ref << a << std::ends; \
PrimNotice_Diagnostic aNotice(b_u_f, sizeof(b_u_f)); aNotice.post_on(prim_notifier()); } while (false)
#define ERRMSG(a) do { char b_u_f[512] = { 0 }; std::ostrstream oss(b_u_f, sizeof(b_u_f)-1); std::ostream &oss_ref = oss; oss_ref << a << std::ends; \
PrimNotice_Error aNotice(b_u_f, sizeof(b_u_f)); aNotice.post_on(prim_notifier()); } while (false)
#define WRNMSG(a) do { char b_u_f[512] = { 0 }; std::ostrstream oss(b_u_f, sizeof(b_u_f)-1); std::ostream &oss_ref = oss; oss_ref << a << std::ends; \
PrimNotice_Warning aNotice(b_u_f, sizeof(b_u_f)); aNotice.post_on(prim_notifier()); } while (false)
#define GLOBAL_CONDMSG(cond, a) do { if (cond) { char b_u_f[512] = { 0 }; std::ostrstream oss(b_u_f, sizeof(b_u_f)-1); \
std::ostream &oss_ref = oss; oss_ref << a << std::ends; \
PrimNotice_Diagnostic aNotice(b_u_f, sizeof(b_u_f)); aNotice.post_on(0); } } while (false)
#define GLOBAL_DIAMSG(a) do { char b_u_f[512] = { 0 }; std::ostrstream oss(b_u_f, sizeof(b_u_f)-1); \
std::ostream &oss_ref = oss; oss_ref << a << std::ends; \
PrimNotice_Diagnostic aNotice(b_u_f, sizeof(b_u_f)); aNotice.post_on(0); } while (false)
#define GLOBAL_ERRMSG(a) do { char b_u_f[512] = { 0 }; std::ostrstream oss(b_u_f, sizeof(b_u_f)-1); \
std::ostream &oss_ref = oss; oss_ref << a << std::ends; \
PrimNotice_Error aNotice(b_u_f, sizeof(b_u_f)); aNotice.post_on(0); } while (false)
#define GLOBAL_WRNMSG(a) do { char b_u_f[512] = { 0 }; std::ostrstream oss(b_u_f, sizeof(b_u_f)-1); \
std::ostream &oss_ref = oss; oss_ref << a << std::ends; \
PrimNotice_Warning aNotice(b_u_f, sizeof(b_u_f)); aNotice.post_on(0); } while (false)
#define CONDMSGZ(cond, s) do { if (cond) { \
PrimNotice_Diagnostic aNotice(s); aNotice.post_on(prim_notifier()); } } while (false)
#define DIAMSGZ(s) do { PrimNotice_Diagnostic aNotice(s); aNotice.post_on(prim_notifier()); } while (false)
#define ERRMSGZ(s) PrimNotice_Error(s).post_on(prim_notifier())
#define WRNMSGZ(s) do { PrimNotice_Warning aNotice(s); aNotice.post_on(prim_notifier()); } while (false)
#else
#define CONDMSG(a, b)
#define DIAMSG(a)
#define ERRMSG(a)
#define WRNMSG(a)
#define GLOBAL_CONDMSG(a, b)
#define GLOBAL_DIAMSG(a)
#define GLOBAL_ERRMSG(a)
#define GLOBAL_WRNMSG(a)
#endif

// 
// 	Report the notifier on which an error or other message should be posted. This implementation
// 	in the global name space returns 0 to cause the notice to be posted on the global notice board.
// 	Implementations within a class scope may cause the ERRMSG macros that use prim_notifier() to redirect
// 	to some local context.
// 
inline PrimNotifier *prim_notifier() { return (0); }
#endif
