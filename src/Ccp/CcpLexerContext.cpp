// **************************** RACAL Confidential ****************************
// 
// 	  Title:			Lexer include buffer context class implementation
// 
// 	  File Name:		CcpLexerContext.cxx
// 
// 	  Author:			E.D.Willink
// 
// 	  SCCS:				%W% %G%
// 
// 	  Latest Modification:
//  EDW	1.1		Date:	 8-Oct-1996		Original
//  EDW	1.2		Date:	23-Jul-1998		Fix signed/unsigned comparison, use std streams
// END
// 

#include <Ccp/CcpIncludeAll.h>

#ifdef ECHO
#undef ECHO
#endif

TYPEINFO_SINGLE(CcpLexerContext, PrimPrint)

// 
// 	Construct a buffer context to read aFile.
// 
CcpLexerContext::CcpLexerContext(const PrimString& aFile)
:
 _next(0),
 _file(aFile),
 _line(0),
 _col(0),
 _text_line(0),
 _text_capacity(0),
 _buffer(0)
{}

// 
// 	Construct a copy of aContext, with the copy acquiring the adopted resources.
// 
CcpLexerContext::CcpLexerContext(CcpLexerContext& aContext)
:
 _next(aContext._next),
 _file(aContext._file),
 _line(aContext._line),
 _col(aContext._col),
 _text_line(aContext._text_line),
 _text_capacity(aContext._text_capacity),
 _buffer(aContext._buffer)
{}

// 
// 	Buffer the incoming line from lineText[lineLength]. (A negative lineLength is permitted).
// 	If newLine an extra line number is counted.
// 
void CcpLexerContext::buffer_line(const unsigned char *lineText, int lineLength, bool newLine)
{
 if (newLine)
  _line++;
 if ((lineLength >= 0) && (size_t(lineLength) >= _text_capacity))
 {
  int newCapacity = 2 * _text_capacity;
  if (newCapacity < 256)
   newCapacity = 256;
  if (newCapacity <= lineLength)
   newCapacity = lineLength+1;
  _text_line.reset(new unsigned char[newCapacity]);
  if (_text_line.get())
   _text_capacity = newCapacity;
  else
  {
   ERRMSG(*this << " failed to allocate memory for " << newCapacity << " byte line buffer.");
   _text_capacity = 0;
  }
 }
 if (_text_line.get())
 {
  memcpy(_text_line.get(), lineText, lineLength);
  _text_line.get()[lineLength] = 0;
 }
 _line++;
 _col = 0;
}

// 
// 	Contribute a diagnostic describing a yyLeng source segment.
// 
std::ostream& CcpLexerContext::message(std::ostream& s, const char *msg,
  const unsigned char *yyText, size_t yyLeng) const
{
 s << c_string(file()) << ", line " << _line << ", columns " << (_col - yyLeng + 1) << " to " << _col << ":";
 if (msg)
  s << " " << msg;
 s << " at: " << c_string(yyText, yyLeng, "", "", PrimCString::ECHO);
 if (_text_line.get())
  s << "\n\t" << c_string(_text_line.get(), "", "", PrimCString::ECHO);
 return s;
}

// 
// 	Pop the next context creating to the current context, returning true if successful, false if
// 	at top of stack. No error message is generated, since this is the way to test for stack empty.
// 
bool CcpLexerContext::pop()
{
 CcpLexerContext *nextBuffer = _next.get();
 if (!nextBuffer)
  return false;
 _file = nextBuffer->_file;
 _line = nextBuffer->_line;
 _col = nextBuffer->_col;
 _text_line = nextBuffer->_text_line;
 _text_capacity = nextBuffer->_text_capacity;
 _buffer = nextBuffer->_buffer;
 _next = nextBuffer->_next;     //  Last since causes destruction of nextBuffer.
 return true;
}

// 
// 	Display the context as part of an inline diagnostic.
// 
std::ostream& CcpLexerContext::print_this(std::ostream& s) const
{
 return (s << _file << ":" << _line << ":" << _col);
}

// 
// 	Push the current context with aBuffer to the next context creating a new context for aFile.
// 
void CcpLexerContext::push(const PrimString& aFile, struct yy_buffer_state *aBuffer)
{
 _buffer = aBuffer;
 _next.reset(new CcpLexerContext(*this));
 _text_capacity = 0;
 _file = aFile;
 _line = 0;
 _col = 0;
 _buffer = 0;
}
