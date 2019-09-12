// **************************** RACAL Confidential ****************************
// 
// 	  Title:			C++ preprocessor reader class interface
// 
// 	  File Name:		CcpReader.hxx
// 
// 	  Author:			E.D.Willink
// 
// 	  SCCS:				%W% %G%
// 
// 	  Latest Modification:
//  EDW	1.1		Date:	 8-Oct-1996		Original.
//  EDW	1.2		Date:	 4-Sep-1997		Add test harness file.
//  EDW	1.3		Date:	28-Apr-1998		Adjust to PrimStringHandle, Use std streams
// END
// 

#ifndef CCPREADER_HXX
#define CCPREADER_HXX


// 
// 	A CcpReader supervises the reading, parsing and post-parsing of an Fcode input file.
// 
class CcpReader : public PrimPrint
{
 TYPEDECL_SINGLE(CcpReader, PrimPrint)
private:
 class Locked {};

private:
 PrimAdopted<std::ifstream> _file;  //  Input file stream object, 0 if none needed.
 PrimStringHandle _name;     //  The input file name.
 std::istream& _s;      //  Stream from which input is read.
 PrimStringHandle _implementation_file; //  File to receive the implementation lines.
 PrimStringHandle _interface_file;  //  File to receive the interface lines.
 PrimStringHandle _test_file;   //  File to receive the test harness lines.
 static bool _quiet;      //  True to suppress diagnostics.

private:
 CcpReader(const CcpReader& aReader);    //  Not implemented.
 CcpReader& operator=(const CcpReader& aReader);  //  Not implemented.

public:
 CcpReader(const PrimString& aFileName, const char *interfaceFileName,
  const char *implementationFileName, const char *testHarnessFileName,
  std::ostream *errorStream = 0);
// 
// 	Report whether the reader is in a fit state.
// 
 operator const Locked *() const { return (const Locked *)(_file.get() && _s ? this : 0); }
// 
// 	Report whether the reader is in an unfit state.
// 
 bool operator!() const { return !operator const Locked *(); }
 static bool& debug_lex();
 static bool& debug_yacc();
 static PrimText& include_paths();
 virtual std::ostream& print_this(std::ostream& s) const;
// 
// 	Return the diagnostic suippression variable.
// 
 static bool& quiet() { return _quiet; }
};

#endif
