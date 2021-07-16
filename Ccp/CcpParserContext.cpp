// **************************** RACAL Confidential ****************************
// 
// 	  Title:			Parser context class implementation
// 
// 	  File Name:		CcpParserContext.cxx
// 
// 	  Author:			E.D.Willink
// 
// 	  SCCS:				%W% %G%
// 
// 	  Latest Modification:
//  EDW	1.1		Date:	 8-Oct-1996		Original
//  EDW	1.2		Date:	 4-Sep-1997		Add test harness
//  EDW	1.3		Date:	28-Apr-1998		Adjust to PrimStringHandle, use std streams
// END
// 

#include <Ccp/CcpIncludeAll.h>

TYPEINFO_SINGLE(CcpParserContext, PrimPrint)

// 
// 	Construct a parser context reading from inputStream, which originates from fileName and generating
// 	error messages to outputStream.
// 
CcpParserContext::CcpParserContext(std::istream *inputStream, const PrimString& fileName,
  std::ostream *outputStream)
:
 _lexer(inputStream, fileName, outputStream),
 _in_implementation(false),
 _in_interface(false),
 _in_test(false),
 _has_implementation(false),
 _has_interface(false),
 _has_test(false)
{}

// 
// 	Add a guarded include of the file named aString.
// 
void CcpParserContext::add_import(int lineNumber, const PrimString& includeFile, const PrimString& guardName)
{
 std::ostrstream s1;
 s1 << "#ifndef ";
 if (!guardName.is_null())
  s1 << guardName;
 else if (strchr(includeFile.str(), '.') != 0)
  for (const char *p = includeFile.str(); *p; p++)
   if (isdigit(*p))
    s1 << *p;
   else if (islower(*p))
    s1 << char(toupper(*p));
   else if (isupper(*p))
    s1 << *p;
   else
    s1 << '_';
 else         //  Plauger policy for std library files
 {
  s1 << '_';
  for (const char *p = includeFile.str(); *p; p++)
   if (isdigit(*p))
    s1 << *p;
   else if (islower(*p))
    s1 << char(toupper(*p));
   else if (isupper(*p))
    s1 << *p;
   else
    s1 << '_';
  s1 << '_';
 }
 s1 << std::ends;
 add_line(s1.str());
 delete s1.str();
 std::ostrstream s2;
 add_line_number(lineNumber);
 s2 << "#include <" << includeFile << ">" << std::ends;
 add_line(s2.str());
 delete s2.str();
 static const PrimStringHandle endifLine("#endif");
 if (!guardName.is_null())
 {
  std::ostrstream s3;
  s3 << "#ifndef " << guardName << std::ends;
  add_line(s3.str());
  delete s3.str();
  std::ostrstream s4;
  s4 << "#define " << guardName << std::ends;
  add_line(s4.str());
  delete s4.str();
  add_line(*endifLine);
 }
 add_line(*endifLine);
 add_line_number(lineNumber+1);
}

// 
// 	Add aLine to the current define, implementation or interface.
// 
void CcpParserContext::add_line(const PrimString& aLine)
{
 if (!_define->is_null())
  _define->add(aLine);
 else
 {
  if (_in_implementation)
   _implementation.append(aLine);
  if (_in_interface)
   _interface.append(aLine);
  if (_in_test)
   _test.append(aLine);
 }
}

// 
// 	Write the current line number to the output files.
// 
void CcpParserContext::add_line_number(int lineNumber)
{
 std::ostrstream s;
 s << "#line " << lineNumber << " \"" << _lexer.file() << "\"" << std::ends;
 add_line(s.str());
 delete s.str();
}

// 
// 	Establish context to define aDefine.
// 
void CcpParserContext::create_define(const PrimString& aDefine)
{
 _define.adopt(new CcpDefine(aDefine.str()));
}

// 
// 	Install the define in the lexer and resume normal parsing.
// 
void CcpParserContext::install_define()
{
 _lexer.add(*_define);
 _define = (CcpDefine *)0;
}

// 
// 	Configure for addition of lines to the implementation.
// 
void CcpParserContext::set_implementation(int lineNumber)
{
 _has_implementation = true;
 _in_implementation = true;
 _in_interface = false;
 _in_test = false;
 add_line_number(lineNumber+1);
}

// 
// 	Configure for addition of lines to the interface.
// 
void CcpParserContext::set_interface(int lineNumber)
{
 _has_interface = true;
 _in_interface = true;
 _in_implementation = false;
 _in_test = false;
 add_line_number(lineNumber+1);
}

// 
// 	Configure for addition of lines to the test harness.
// 
void CcpParserContext::set_test(int lineNumber)
{
 _has_test = true;
 _in_interface = false;
 _in_implementation = false;
 _in_test = true;
 add_line_number(lineNumber+1);
}
