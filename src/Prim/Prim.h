// **************************** RACAL Confidential ****************************
// 
// 	  Title:			Minimal include file for Prim classes
// 
// 	  File Name:		Prim.h
// 
// 	  Author:			E.D.Willink
// 
// 	  Description:
// 
// 		This include file should be the first in all compilation modules. It provides
// 		the standard system include files, implementation constants, and miscellaneous
// 		prototypes and variable references.
// 
// 	  SCCS:				%W% %G%
// 
// 	  Latest Modification:
//  EDW	1.1		Date:	12-Nov-1992		Original renamed from prim.h
//  EDW	1.2		Date:	18-Nov-1992		Provide PrimTypeInfo definitions to resolve PrimPrint/PrimTypeInfo
//  EDW	1.3		Date:	 2-Dec-1992		Add __C__() to support macro comments
//  EDW	2.1		Date:	 9-Jan-1993		Eliminate #include comments to placate ptlink()
//  EDW	2.2		Date:	19-Feb-1993		Add PrimComParse, PrimTypId etc
//  EDW	2.3		Date:	 9-Apr-1993		Define volatile if not __STDC__
//  EDW	2.4		Date:	15-Apr-1993		Keep volatile if SDD_GNU
//  EDW	3.1		Date:	 7-Jul-1993		STATIC_CONST defined to sort out MRI
//  EDW	3.2		Date:	19-Jul-1993		Add PrimStringStatic, PrimStringText, condition stream fref's
//  EDW	3.3		Date:	 2-Sep-1993		Promote STREAM_DEBUG to PRIM_STREAM etc, define NULL
//  EDW	4.1		Date:	20-Dec-1993		Fix documentation
//  EDW	5.0		Date:	 4-Mar-1994		Change philosopy to NO_PRIM_xxx
//  EDW	5.1		Date:	21-Jul-1994		Add #define using - till ANSI comes along.
//  EDW	5.2		Date:	20-Sep-1994		Add prim_operator_new, prim_operator_delete, self
//  EDW	5.3		Date:	30-Sep-1994		prim_operator_new, prim_operator_delete declared inline
//  EDW	5.4		Date:	 6-May-1995		Add true, false, explicit, USE_xxx define conditionalisation
//  EDW	5.5		Date:	14-Aug-1995		Add PrimId, PrimRef, change to typedef'd bool
//  EDW	6.1		Date:	 7-Mar-1997		Undefine any #def'd EXIT_FAILURE/EXIT_SUCCESS
//  EDW	6.2		Date:	29-Apr-1997		Add Singly/Doubly Linked lists, rationalise PrimId
//  EDW	6.3		Date:	16-Jun-1997		Drop self
//  EDW	6.4		Date:	18-Jun-1997		Drop PrimThat from PrimId, move PrimId types to PrimId
//  EDW	6.5		Date:	 2-Jul-1997		Add PrimOstrstream, PRIM_WITH_LIST_MAP etc macros
//  EDW	6.6		Date:	30-Jul-1997		Add PrimNumber
//  EDW	6.7		Date:	30-Sep-1997		Eliminate PrimThat
//  EDW	7.1		Date:	18-Aug-1998		Require NEEDS_BOOL etc to define old bool
//  EDW	7.2		Date:	 9-Jun-2000		Add NEEDS_TYPENAME, conditinalise for egcs
// END
// 
#ifndef PRIM_H
#define PRIM_H

#ifdef WIN32
#pragma warning(disable:4355)    //  this in base initialiser.
#pragma warning(disable:4390)    //  empty controlled statement as in UNUSED.
#endif

#ifndef PRIM_TEMPLATES
#define PRIM_TEMPLATES
#endif

#ifndef NO_PRIM_CHECKS
#define PRIM_CHECKS
#endif
#ifndef NO_PRIM_FSTREAM
#define PRIM_FSTREAM
#endif
#ifndef NO_PRIM_STREAMS
#define PRIM_STREAMS
#endif
#ifndef NO_PRIM_STATISTICS
#define PRIM_STATISTICS
#endif
#ifndef NO_PRIM_TYPEINFO
#define PRIM_TYPEINFO
#endif
#ifndef NO_PRIM_UTILITIES
#define PRIM_UTILITIES
#endif
#ifndef NO_DBX_SUPPORT
#define DBX_SUPPORT
#endif
#ifndef NO_FSTREAM_SUPPORT
#define FSTREAM_SUPPORT
#endif

#include <cstddef>

#ifdef NULL
#undef NULL
#endif
#define NULL 0

#if !defined(__STDC__) && !defined(volatile) && !defined(SDD_GNU) && !defined(USE_VOLATILE)
#define volatile
#endif

#ifdef NEEDS_EXPLICIT
#define explicit
#endif
#ifdef NEEDS_MUTABLE
#define mutable
#endif
#ifdef NEEDS_TYPENAME
#define typename
#endif
#ifdef NEEDS_USING
#define using
#endif

#ifdef __GNUC__
class PrimFriendOfGnuC {};
#define NEEDS_A_FRIEND_FOR_GNUC friend class PrimFriendOfGnuC;
#else
#define NEEDS_A_FRIEND_FOR_GNUC
#endif

#if !defined(CPP_EXPANSION) && !defined(SDD_CPP_EXPANSION)
// 		Undefined for normal usage (comments in macros discarded).
#define __C__(a)
#else
// 		Defined to leave macro comments in a form that a simple filter can rescue.
#define __C__(a) /a/
#endif

#ifndef NO_PRIM_STREAMS
#include <iosfwd>
#endif

// 
// 		A bool is data type supporting the boolean values TRUE and FALSE in the
// 		most speed efficient representation for the target architecture.
// 
#ifdef NEEDS_BOOL
#ifdef FALSE
#undef FALSE
#endif
#ifdef TRUE
#undef TRUE
#endif
typedef int bool;
const bool false = 0;
const bool true = 1;
// const bool FALSE = false;
// const bool TRUE = true;
#endif

#define UNUSED(a) if (0 && a)       //  Used to suppress compiler warnings

// 
// 		An ASCII mnemonic for the null end of string character.
// 
const char EOS = '\0';

#ifdef EXIT_FAILURE
#undef EXIT_FAILURE
#endif
#ifdef EXIT_SUCCESS
#undef EXIT_SUCCESS
#endif
const int EXIT_FAILURE = 1;
const int EXIT_SUCCESS = 0;

#define PRIM_ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))

class PrimBuf;
class PrimCStringMonitor;
class PrimCheckLock;
class PrimComParse;
class PrimComToken;
class PrimComToken_Bool;
class PrimComToken_Double;
class PrimComToken_Enum;
class PrimComToken_File;
class PrimComToken_Files;
class PrimComToken_Int;
class PrimComToken_Long;
class PrimComToken_String;
class PrimComToken_Strings;
class PrimCoord;
class PrimCString;
class PrimDebug;
class PrimDouble;
class PrimEnum;
class PrimId;
class PrimIdHandle;
class PrimIded;
class PrimIfstreamMonitor;
class PrimInteger;
class PrimLexerContext;
class PrimLineContext;
class PrimListOfRefImp;
class PrimMapOfRefImp;
class PrimMonitor;
class PrimNotifier;
class PrimNotice;
class PrimNotice_Constructor;
class PrimNotice_Destructor;
class PrimNotice_Diagnostic;
class PrimNotice_Error;
class PrimNotice_Warning;
class PrimNumber;
class PrimOfstreamMonitor;
class PrimOstrstream;
class PrimParserMonitor;
class PrimPrint;
class PrimQuad;
class PrimRefBase;
class PrimShares;
class PrimSize;
class PrimString;
class PrimStringHandle;
class PrimStringStatic;
class PrimText;
class PrimTextMonitor;
class PrimTextTokeniser;
class PrimTime;
class PrimTypeId;
class PrimTypeInfo;
struct PrimTypeInfo_Base;

template <class T> class PrimAdopted;
template <class T> class PrimAdoptedArray;
template <class T> class PrimConstDoublyLinkedListOfRefIterator;
template <class T> class PrimConstDoublyLinkedListOfRefToConstIterator;
template <class E> class PrimConstList;
template <class E> class PrimConstListOfRefIterator;
template <class E> class PrimConstListOfRefToConstIterator;
template <class E> class PrimConstMap;
template <class E> class PrimConstMapOfRefIterator;
template <class E> class PrimConstMapOfRefToConstIterator;
template <class T> class PrimConstSinglyLinkedListOfRefIterator;
template <class T> class PrimConstSinglyLinkedListOfRefToConstIterator;
template <class T> class PrimDoublyLinkedListOfRef;
template <class T> class PrimDoublyLinkedListOfRefIterator;
template <class T> class PrimDoublyLinkedListOfRefToConst;
template <class T> class PrimDoublyLinkedListOfRefToConstIterator;
template <class T> class PrimGetEnv;
template <class E> class PrimList;
template <class E> class PrimListOfRef;
template <class E> class PrimListOfRefIterator;
template <class E> class PrimListOfRefToConst;
template <class E> class PrimListOfRefToConstIterator;
template <class E> class PrimMap;
template <class E> class PrimMapOfRef;
template <class E> class PrimMapOfRefIterator;
template <class E> class PrimMapOfRefToConst;
template <class E> class PrimMapOfRefToConstIterator;
template <class E> class PrimRef;
template <class E> class PrimRefToConst;
template <class T> class PrimSetElem;
template <class T> class PrimSetElement;
template <class T> class PrimSharedListOfRef;
template <class T> class PrimSharedListOfRefToConst;
template <class T> class PrimSharedMapOfRef;
template <class T> class PrimSharedMapOfRefToConst;
template <class T> class PrimSinglyLinkedListOfRef;
template <class T> class PrimSinglyLinkedListOfRefIterator;
template <class T> class PrimSinglyLinkedListOfRefToConst;
template <class T> class PrimSinglyLinkedListOfRefToConstIterator;
template <class T> class PrimStaticDestructor;

#include <Ccp/generic.h>

#define PRIM_AND_LIST(T) \
typedef PrimListOfRefToConst<T> name2(T,ListOfRefToConst); \
typedef PrimListOfRef<T> name2(T,ListOfRef); \
typedef PrimListOfRefToConstIterator<T> name2(T,ListOfRefToConstIterator); \
typedef PrimListOfRefIterator<T> name2(T,ListOfRefIterator); \
typedef PrimConstListOfRefToConstIterator<T> name2(T,ConstListOfRefToConstIterator); \
typedef PrimConstListOfRefIterator<T> name2(T,ConstListOfRefIterator);
#define PRIM_AND_MAP(T) \
typedef PrimMapOfRefToConst<T> name2(T,MapOfRefToConst); \
typedef PrimMapOfRef<T> name2(T,MapOfRef); \
typedef PrimMapOfRefToConstIterator<T> name2(T,MapOfRefToConstIterator); \
typedef PrimMapOfRefIterator<T> name2(T,MapOfRefIterator); \
typedef PrimConstMapOfRefToConstIterator<T> name2(T,ConstMapOfRefToConstIterator); \
typedef PrimConstMapOfRefIterator<T> name2(T,ConstMapOfRefIterator);
#define PRIM_AND_REF(T) \
typedef PrimRefToConst<T> name2(T,RefToConst); \
typedef PrimRef<T> name2(T,Ref);
#define PRIM_AND_SHARED_LIST(T) \
PRIM_AND_LIST(T) \
typedef PrimConstList<T> name2(T,ConstList); \
typedef PrimList<T> name2(T,List); \
PRIM_AND_REF(name2(T,ConstList)) \
PRIM_AND_REF(name2(T,List))
#define PRIM_AND_SHARED_MAP(T) \
PRIM_AND_MAP(T) \
typedef PrimConstMap<T> name2(T,ConstMap); \
typedef PrimMap<T> name2(T,Map); \
PRIM_AND_REF(name2(T,ConstMap)) \
PRIM_AND_REF(name2(T,Map))

#define PRIM_WITH_REF(T) \
class T; \
PRIM_AND_REF(T)
#define PRIM_WITH_LIST(T) \
class T; \
PRIM_AND_REF(T) \
PRIM_AND_LIST(T)
#define PRIM_WITH_SHARED_LIST(T) \
class T; \
PRIM_AND_REF(T) \
PRIM_AND_SHARED_LIST(T)
#define PRIM_WITH_MAP(T) \
class T; \
PRIM_AND_REF(T) \
PRIM_AND_MAP(T)
#define PRIM_WITH_SHARED_MAP(T) \
class T; \
PRIM_AND_REF(T) \
PRIM_AND_SHARED_MAP(T)
#define PRIM_WITH_LIST_MAP(T) \
class T; \
PRIM_AND_REF(T) \
PRIM_AND_LIST(T) \
PRIM_AND_MAP(T)
#define PRIM_WITH_SHARED_LIST_MAP(T) \
class T; \
PRIM_AND_REF(T) \
PRIM_AND_SHARED_LIST(T) \
PRIM_AND_SHARED_MAP(T)

#define PRIM_THAT_AND_LIST(T) PRIM_AND_LIST(T)
#define PRIM_THAT_AND_1LIST(T) PRIM_AND_1LIST(T)
// typedef PrimSinglyLinkedListOfRefToConst<name2(T,That)> name2(T,1ListOfRefToConst); \
// typedef PrimSinglyLinkedListOfRef<name2(T,That)> name2(T,1ListOfRef); \
// typedef PrimSinglyLinkedListOfRefToConstIterator<name2(T,That)> name2(T,1ListOfRefToConstIterator); \
// typedef PrimSinglyLinkedListOfRefIterator<name2(T,That)> name2(T,1ListOfRefIterator); \
// typedef PrimConstSinglyLinkedListOfRefToConstIterator<name2(T,That)> name2(T,Const1ListOfRefToConstIterator); \
// typedef PrimConstSinglyLinkedListOfRefIterator<name2(T,That)> name2(T,1LinkedListOfRefIterator);
#define PRIM_THAT_AND_MAP(T) PRIM_AND_MAP(T)
#define PRIM_THAT_AND_REF(T) PRIM_AND_REF(T)
#define PRIM_THAT_WITH_REF(T) PRIM_WITH_REF(T)
#define PRIM_THAT_WITH_LIST(T) PRIM_WITH_LIST(T)
#define PRIM_THAT_WITH_MAP(T) PRIM_WITH_MAP(T)
#define PRIM_THAT_WITH_LIST_MAP(T) PRIM_WITH_LIST_MAP(T)

PRIM_WITH_REF(PrimLineContext)
PRIM_WITH_LIST_MAP(PrimNumber)

typedef PrimListOfRefToConst<PrimId> PrimIdList;
typedef PrimMapOfRefToConst<PrimId> PrimIdMap;
typedef PrimSharedListOfRefToConst<PrimId> PrimIdSharedList;
typedef PrimSharedMapOfRefToConst<PrimId> PrimIdSharedMap;
typedef PrimConstListOfRefToConstIterator<PrimId> PrimIdConstListIterator;
typedef PrimConstMapOfRefToConstIterator<PrimId> PrimIdConstMapIterator;
typedef PrimListOfRefToConstIterator<PrimId> PrimIdListIterator;
typedef PrimMapOfRefToConstIterator<PrimId> PrimIdMapIterator;
typedef PrimRef<PrimIdSharedList> PrimIdSharedListRef;
typedef PrimRefToConst<PrimIdSharedList> PrimIdSharedListRefToConst;
typedef PrimRef<PrimIdSharedMap> PrimIdSharedMapRef;
typedef PrimRefToConst<PrimIdSharedMap> PrimIdSharedMapRefToConst;

#ifndef NO_PRIM_STREAMS
void *prim_operator_new(size_t aSize);
void prim_operator_delete(void *p);
#else
// 
// 	The non-debug implementation of an unwanted new() just returns 0.
// 
inline void *prim_operator_new(size_t aSize) { UNUSED(aSize); return (0); }
// 
// 	The non-debug implementation of an unwanted delete() does nothing.
// 
inline void prim_operator_delete(void *p) { UNUSED(p); }
#endif

#endif
