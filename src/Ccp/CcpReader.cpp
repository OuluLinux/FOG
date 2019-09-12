// **************************** RACAL Confidential ****************************
// 
// 	  Title:			C++ preprocessor reader class implementation
// 
// 	  File Name:		CcpReader.cxx
// 
// 	  Author:			E.D.Willink
// 
// 	  SCCS:				%W% %G%
// 
// 	  Latest Modification:
//  EDW	1.1		Date:	 8-Oct-1996		Original
//  EDW	1.2		Date:	 4-Sep-1997		Add test harness support
//  EDW	1.3		Date:	28-Apr-1998		Adjust to PrimStringHandle, use std streams
// END
// 

#include <Ccp/CcpIncludeAll.h>

TYPEINFO_SINGLE(CcpReader, PrimPrint)

bool CcpReader::_quiet = false;      //  True to suppress diagnostics.

// 
// 	Construct a reader to read from aFileName, or standard input if 0.
// 
CcpReader::CcpReader(const PrimString& aFileName, const char *interfaceFileName,
  const char *implementationFileName, const char *testHarnessFileName, std::ostream *errorStream)
:
 _file(aFileName ? new std::ifstream(aFileName.str()) : 0),
 _name(aFileName ? (_file ? aFileName.str() : 0) : "<standard-input>"),
 _s(*(aFileName ? (std::istream *)_file.get() : (std::istream *)&std::cin)),
 _implementation_file(implementationFileName),
 _interface_file(interfaceFileName),
 _test_file(testHarnessFileName)
{
 PrimError savedErrNo;
 if (aFileName && !_file)
  ERRMSG("Failed to allocate memory for " << c_string(aFileName.str()) << ", " << savedErrNo);
 else if (!_s)
  ERRMSG("Failed to open " << c_string(aFileName.str()) << ", " << savedErrNo);
 else
 {
  CcpParser aParser(&_s, *_name, errorStream);
   aParser.yyparse();
  if (_interface_file && aParser.has_interface())
  {
   CONDMSG(!quiet(), "Writing interface to \"" << _interface_file << "\"");
   if (!aParser.interface().write_to_file(_interface_file.str()))
   {
    PrimError savedErrNo;
    ERRMSG("Failed to write to " << c_string(_interface_file.str()) << ", " << savedErrNo);
   }
  }
  if (_implementation_file && aParser.has_implementation())
  {
   CONDMSG(!quiet(), "Writing implementation to \"" << _implementation_file << "\"");
   if (!aParser.implementation().write_to_file(_implementation_file.str()))
   {
    PrimError savedErrNo;
    ERRMSG("Failed to write to " << c_string(_implementation_file.str()) << ", " << savedErrNo);
   }
  }
  if (_test_file && aParser.has_test())
  {
   CONDMSG(!quiet(), "Writing test harness to \"" << _test_file << "\"");
   if (!aParser.test().write_to_file(_test_file.str()))
   {
    PrimError savedErrNo;
    ERRMSG("Failed to write to " << c_string(_test_file.str()) << ", " << savedErrNo);
   }
  }
 }
}

// 
// 	Report the lex debug control variable.
// 
bool& CcpReader::debug_lex()
{
 static PrimGetEnv<bool> debugLex("CcpReader::debug_lex", false);
 return debugLex;
}

// 
// 	Report the parser debug control variable.
// 
bool& CcpReader::debug_yacc()
{
 static PrimGetEnv<bool> debugYacc("CcpReader::debug_yacc", false);
 return debugYacc;
}

// 
// 	Return the list of include file paths.
// 
PrimText& CcpReader::include_paths()
{
 static PrimText includePaths;
 return includePaths;
}

// 
// 	Display the file name as part of an inline diagnostic.
// 
std::ostream& CcpReader::print_this(std::ostream& s) const
{
 return s << _name;
}
