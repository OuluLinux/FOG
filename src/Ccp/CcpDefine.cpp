// **************************** RACAL Confidential ****************************
// 
// 	  Title:			Defined macro class implementation
// 
// 	  File Name:		CcpDefine.cxx
// 
// 	  Author:			E.D.Willink
// 
// 	  SCCS:				%W% %G%
// 
// 	  Latest Modification:
//  EDW	1.1		Date:	 8-Oct-1996		Original
//  EDW	1.2		Date:	 2-Sep-1998		Use std streams
// END
// 

#include <Ccp/CcpIncludeAll.h>

TYPEINFO_SINGLE(CcpDefine, PrimPrint)
PRIMSHARES_IMPL(CcpDefine)
PRIMREF_NULL_CLASS_AND_IMPL(CcpDefine)
PRIMREFS_IMPL(CcpDefine)
PRIMMAPOFREFS_IMPL(CcpDefine)

TMPL_HACK_FIX_DO(CcpDefine)

// 
// 	Provide a comprehensive print-out of the object.
// 
std::ostream& CcpDefine::print_depth(std::ostream& s, int aDepth) const
{
 _text.print_on(s, aDepth);
 return s;
}

// 
// 	Contribute the identifier to an inline print-out.
// 
std::ostream& CcpDefine::print_this(std::ostream& s) const
{
 return s << _id;
}
