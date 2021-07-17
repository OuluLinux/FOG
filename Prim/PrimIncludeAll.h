#pragma once


#include <string.h>
#include <iostream>
#include <istream>
#include <fstream>
#include <limits.h>
#include <iomanip>
#include <float.h>
#include <algorithm>

#ifdef flagMSC
	#include <windows.h>
#endif

#undef NO_DATA

#define PRIM_TEST_MODULE
//#define SLOW_CONSISTENCY_CHECK


// Very hackish duplicate macro to avoid compiler "template<>" error

#define TMPL_HACK_FIX(E,N,D)\
	const E& E::immutable_null() \
	{ \
		static bool initialised = false; \
		static char buf[sizeof(N)]; \
		if (!initialised) \
		{ \
			initialised = true; \
			static D aDestructor(*(N *)buf); \
		} \
		return *(const N *)buf; \
	} \
	__C__((Report the mutable null E.)) \
	E& E::mutable_null() \
	{ \
		static bool initialised = false; \
		static char buf[sizeof(N)]; \
		if (!initialised) \
		{ \
			initialised = true; \
			static D aDestructor(*(N *)buf); \
		} \
		return *(N *)buf; \
	}

#define TMPL_HACK_FIX_DO(E) \
	TMPL_HACK_FIX(E,name2(E,Null),name2(E,Destructor))





#define TMPL_HACK_FIX_TEMPLATE_EXPLICIT(E)\
	template<> const E& E::immutable_null(); \
	template<> E& E::mutable_null();




#define TMPL_HACK_FIX_TEMPLATE(E,N,D)\
	template<> const E& E::immutable_null() \
	{ \
		static bool initialised = false; \
		static char buf[sizeof(N)]; \
		if (!initialised) \
		{ \
			initialised = true; \
			static D aDestructor(*(N *)buf); \
		} \
		return *(const N *)buf; \
	} \
	__C__((Report the mutable null E.)) \
	template<> E& E::mutable_null() \
	{ \
		static bool initialised = false; \
		static char buf[sizeof(N)]; \
		if (!initialised) \
		{ \
			initialised = true; \
			static D aDestructor(*(N *)buf); \
		} \
		return *(N *)buf; \
	}

#define TMPL_HACK_FIX_DO_TEMPLATE(E) \
	PRIMREF_NULL_CLASS(E)  TMPL_HACK_FIX_TEMPLATE(E,name2(E,Null),name2(E,Destructor))





#include "Prim.h"
#include "PrimAdopted.h"
#include "PrimAdoptedArray.h"
#include "PrimBits.h"

#include "PrimBufRep.h"

#include "PrimBuf.h"
#include "PrimBufStatic.h"

#include "PrimPrint.h"
#include "PrimShares.h"
#include "PrimNotifier.h"
#include "PrimMonitor.h"
#include "PrimRef.h"
#include "PrimString.h"
#include "PrimText.h"
#include "PrimSize.h"
#include "PrimListOfRef.h"
#include "PrimNamed.h"
#include "PrimParserMonitor.h"
#include "PrimManip.h"
#include "PrimTest.h"

#include "PrimBufStatic.h"
#include "PrimCStringMonitor.h"
#include "PrimCheckLock.h"
#include "PrimComParse.h"
#include "PrimComToken.h"
#include "PrimComTokenStr.h"
#include "PrimComTokenStrs.h"
#include "PrimComToken_Bool.h"
#include "PrimComToken_Double.h"
#include "PrimComToken_String.h"
#include "PrimComToken_Enum.h"
#include "PrimComToken_File.h"
#include "PrimComToken_Files.h"
#include "PrimComToken_Int.h"
#include "PrimComToken_Long.h"
#include "PrimComToken_Strings.h"
#include "PrimCountedArray.h"
#include "PrimDirectoryIterator.h"
#include "PrimEnum.h"
#include "PrimErrNo.h"
#include "PrimGetEnv.h"
#include "PrimId.h"
#include "PrimIded.h"
#include "PrimIfstreamMonitor.h"
#include "PrimLexerContext.h"
#include "PrimLineContext.h"
#include "PrimListOfRef.h"
#include "PrimMapOfRef.h"
#include "PrimMath.h"
#include "PrimMonitor.h"
#include "PrimNamed.h"
#include "PrimNotice.h"
#include "PrimNotice_Constructor.h"
#include "PrimNotice_Destructor.h"
#include "PrimNotice_Text.h"
#include "PrimNotice_Diagnostic.h"
#include "PrimNotice_Error.h"
#include "PrimNotice_Warning.h"
#include "PrimNotices.h"
#include "PrimNotifier.h"
#include "PrimNumber.h"
#include "PrimNumberWords.h"
#include "PrimOfstreamMonitor.h"
#include "PrimOstrstream.h"
#include "PrimPrint.h"
#include "PrimRef.h"
#include "PrimSetElement.h"
#include "PrimShares.h"
#include "PrimSinglyLinkedListOfRef.h"
#include "PrimSize.h"
#include "PrimStaticDestructor.h"
#include "PrimString.h"
#include "PrimStringStatic.h"
#include "PrimStrings.h"
#include "PrimText.h"
#include "PrimTextTokeniser.h"
#include "PrimTextDoubleQuoteTokeniser.h"
#include "PrimTextMonitor.h"
#include "PrimTextWhiteSpaceTokeniser.h"
#include "PrimTime.h"
#include "PrimTypeInfo.h"
#include "PrimTypeId.h"
#include "PrimTypes.h"
#include "PrimViz.h"

#ifdef NO_DATA
	#error "NO_DATA shouldn't be defined"
#endif
