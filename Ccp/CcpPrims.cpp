// **************************** RACAL Confidential ****************************
// 
// 	  Title:			C++ preprocessor prim library classes compilation unit.
// 
// 	  File Name:		CcpPrims.cpp
// 
// 	  Author:			E.D.Willink
// 
// 	  SCCS:				%W% %G%
// 
// 	  Latest Modification:
//  EDW	1.1		Date:	16-Mar-1999		Original
//  EDW	1.2		Date:	13-May-1999		Add cerr and friends
// 
// 	This bootstrap duplicates the Prim library, but since the Prim library uses this
// 	tool to build itself, we have a chicken and egg situation, resolved by taking
// 	copies of the Prim library. At the time of the split there was no discrepancy,
// 	so regular updates from improved Prim modules should not be a problem.
// END
// 

#include <Ccp/CcpIncludeAll.h>

































#ifdef NEEDS_STD
istream_withassign& std::cin = ::cin;
ostream_withassign& std::cout = ::cout;
ostream_withassign& std::cerr = ::cerr;
ostream_withassign& std::clog = ::clog;

smanip_int std::setfill(int _n) { return ::setfill(_n); }
smanip_int std::setw(int _n) { return ::setw(_n); }
#endif