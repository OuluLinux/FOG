// **************************** RACAL Confidential ****************************
// 
// 	  Title:			Lexer and Parser token class implementations
// 
// 	  File Name:		CcpToken.cxx
// 
// 	  Author:			E.D.Willink
// 
// 	  SCCS:				@(#)CcpToken.cxx	1.1 06/30/97
// 
// 	  Latest Modification:
//  EDW	1.1		Date:	 8-Oct-1996		Original
// END
// 


#include <Ccp/CcpIncludeAll.h>

// 
// 	Define the out of line member access functions for the parsed data value class T whose data type is
// 	D and which is accessed by methods named N.
// 
#define CCPTOKEN_MACRO_REF(T, C, D, R, N) \
name2(D,R)& CcpToken::N() \
{ \
 delete this; \
 new (*this) T; \
 return (N()); \
}
#define CCPTOKEN_MACRO_VAL(T, D, N) \
D& CcpToken::N() \
{ \
 delete this; \
 new (*this) T; \
 return (N()); \
}
CCPTOKEN_LIST_REF
CCPTOKEN_LIST_VAL
#undef CCPTOKEN_MACRO_REF
#undef CCPTOKEN_MACRO_VAL

#ifdef CCPTOKEN_READ_CHECK
#define CCPTOKEN_MACRO_REF(T, C, D, R, N) \
C D& CcpToken::N() const \
{ \
 ERRMSG("CcpToken::"#N"() invoked for a \"" << data_type() << "\" rather than a \""#N"\" data value."); \
 static name2(D,R) aD; \
 return (*aD); \
}
#define CCPTOKEN_MACRO_VAL(T, D, N) \
const D& CcpToken::N() const \
{ \
 ERRMSG("CcpToken::"#N"() invoked for a \"" << data_type() << "\" rather than a \""#N"\" data value."); \
 static D aD; \
 return (aD); \
}
CCPTOKEN_LIST_REF
CCPTOKEN_LIST_VAL
#undef CCPTOKEN_MACRO_REF
#undef CCPTOKEN_MACRO_VAL
#endif

// 
// 	Deletion of a token does nothing.
// 
// .limits
// 	This must be outlined to avoid a crash with SUN C++ 4.1.
// 
void CcpToken::operator delete(void *p, size_t aSize) { UNUSED(aSize); UNUSED(p); }

// 
// 	Create a clone of this token at the location identified by p.
// 
void CcpToken::clone_to(CcpToken& p) const { new (p) CcpToken(*this); }

// 
// 	Create a clone of this token at the location identified by p.
// 
const char *CcpToken::data_type() const { return "-root-"; }

// 
// 	Define the out of line functions for a derived token class T whose data type is defined
// 	by D and which is accessed by methods named N.
// 
#define CCPTOKEN_MACRO_REF(T, C, D, R, N) \
T::~T() {} \
void T::clone_to(CcpToken& p) const { new (p) T(*this); } \
const char *T::data_type() const { return #N; } \
name2(D,R)& T::N() { return name2(_,N); }
#define CCPTOKEN_MACRO_VAL(T, D, N) \
T::~T() {} \
void T::clone_to(CcpToken& p) const { new (p) T(*this); } \
const char *T::data_type() const { return #N; } \
D& T::N() { return name2(_,N); }
CCPTOKEN_LIST_REF
CCPTOKEN_LIST_VAL
#undef CCPTOKEN_MACRO_REF
#undef CCPTOKEN_MACRO_VAL

#ifdef CCPTOKEN_READ_CHECK
#define CCPTOKEN_MACRO_REF(T, C, D, R, N) \
C D& T::N() const { return *name2(_,N); }
#define CCPTOKEN_MACRO_VAL(T, D, N) \
const D& T::N() const { return name2(_,N); }
CCPTOKEN_LIST_REF
CCPTOKEN_LIST_VAL
#undef CCPTOKEN_MACRO_REF
#undef CCPTOKEN_MACRO_VAL
#endif
