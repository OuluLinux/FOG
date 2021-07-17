// **************************** RACAL Confidential ****************************
//
// 	  Title:			Common objects compilation unit
//
// 	  File Name:		PrimObjects.cc
//
// 	  Author:			E.D.Willink
//
// 	  SCCS:				%W% %G%
//
// 	  Latest Modification:
//  EDW	6.0		Date:	 1-Jun-1996		Original
//  EDW	6.2		Date:	14-Nov-1997		Add PrimTime
//  EDW	7.1		Date:	18-Aug-1998		Add Imp to PrimGetEnv, support std:: steams emulation
// END
//















#ifdef NEEDS_STD
istream_withassign& std::cin = ::cin;
ostream_withassign& std::cout = ::cout;
ostream_withassign& std::cerr = ::cerr;
ostream_withassign& std::clog = ::clog;

smanip_int std::setfill(int _n) { return ::setfill(_n); }
smanip_int std::setw(int _n) { return ::setw(_n); }
#endif
