#include <Prim/PrimIncludeAll.h>




#ifdef ECHO
#undef ECHO
#endif







TYPEINFO_SINGLE(PrimLexerContext, Super)

//  
//  	Construct a buffer context to read aFile.
//  
PrimLexerContext::PrimLexerContext(const PrimString& aFile)
:
 _file(aFile),
 _line(0),
 _source_col(0),
 _detabbed_col(0),
 _buffer(0),
 _context(current_line_context()),
 _current_index(-1),
 _next_index(-1),
 _at_eof(false)
{
 adopt_current_line_context(new PrimLineContext(file(), _line));
}

//  
//  	Construct a buffer context to read aFile.
//  
PrimLexerContext::PrimLexerContext(Install doInstall, const PrimString& aFile)
:
 Super(NO_INSTALL),
 _file(aFile),
 _line(0),
 _source_col(0),
 _detabbed_col(0),
 _buffer(0),
 _context(current_line_context()),
 _current_index(-1),
 _next_index(-1),
 _at_eof(false)
{
 adopt_current_line_context(new PrimLineContext(file(), _line));
 if (doInstall == INSTALL)
  install();
}

//  
//  	Construct a new lexing context that opens and reads from fileName.
//  	The existing nestedContext is saved for subsequent restoration.
//  
PrimLexerContext::PrimLexerContext(Install doInstall, PrimAdopted<PrimLexerContext>& nestedContext,
 const PrimString& fileName)
:
 Super(NO_INSTALL),
 _next(nestedContext),
 _file(fileName),
 _line(0),
 _source_col(0),
 _detabbed_col(0),
 _buffer(0),
 _context(current_line_context()),
 _current_index(-1),
 _next_index(-1),
 _at_eof(false),
 _stream(new std::ifstream(fileName.str()))
{
 adopt_current_line_context(new PrimLineContext(file(), _line));
 if (doInstall == INSTALL)
  install();
 if (_next && !_next->buffer())
  ERRMSG("BUG -- No buffer saved for nested context " << *_next);
}

//  
//  	Construct a new lexing context that reads aString identifying its source as sourceId.
//  	The existing nestedContext is saved for subsequent restoration.
//  
PrimLexerContext::PrimLexerContext(Install doInstall, PrimAdopted<PrimLexerContext>& nestedContext,
  const PrimString& sourceId, const PrimString& aString)
:
 Super(NO_INSTALL),
 _next(nestedContext),
 _file(sourceId),
 _line(0),
 _source_col(0),
 _detabbed_col(0),
 _buffer(0),
 _context(current_line_context()),
 _current_index(-1),
 _next_index(-1),
 _at_eof(false),
 _string(aString),
 _stream(new std::istrstream(aString.str()))
{
 adopt_current_line_context(new PrimLineContext(file(), _line));
 if (doInstall == INSTALL)
  install();
 if (_next && !_next->buffer())
  ERRMSG("BUG -- No buffer saved for nested context " << *_next);
}

//  
//  	Construct a new lexing context that reads aStream identifying its source as sourceId.
//  	The existing nestedContext is saved for subsequent restoration.
//  
PrimLexerContext::PrimLexerContext(Install doInstall, PrimAdopted<PrimLexerContext>& nestedContext,
  const PrimString& sourceId, PrimAdopted<std::istream>& aStream)
:
 Super(NO_INSTALL),
 _next(nestedContext),
 _file(sourceId),
 _line(0),
 _source_col(0),
 _detabbed_col(0),
 _buffer(0),
 _context(current_line_context()),
 _current_index(-1),
 _next_index(-1),
 _at_eof(false),
 _stream(aStream)
{
 adopt_current_line_context(new PrimLineContext(file(), _line));
 if (doInstall == INSTALL)
  install();
 if (_next && !_next->buffer())
  ERRMSG("BUG -- No buffer saved for nested context " << *_next);
}

//  
//  	Construct a copy of aContext, with the copy acquiring the adopted resources.
//  
PrimLexerContext::PrimLexerContext(PrimLexerContext& aContext)
:
 Super(NO_INSTALL),
 _next(aContext._next),
 _file(aContext._file),
 _line(aContext._line),
 _source_col(aContext._source_col),
 _detabbed_col(aContext._detabbed_col),
 _buffer(aContext._buffer),
 _context(current_line_context()),
 _current_index(aContext._current_index),
 _next_index(aContext._next_index),
 _at_eof(aContext._at_eof)
{
 _lines[0] = aContext._lines[0];
 _lines[1] = aContext._lines[1];
}

//  
//  	The destructor restores the earlier line context.
//  
PrimLexerContext::~PrimLexerContext()
{
 set_current_line_context(_context->is_null() ? 0 : &*_context);
}

//  
//  	Make the next line the current line.
//  
void PrimLexerContext::advance_line()
{
 _line++;
 _source_col = 0;
 _detabbed_col = 0;
 if (_next_index < 0)
 {
  if (_current_index >= 0)
  {
   _current_index = -1;
   _at_eof = true;
   adopt_current_line_context(new PrimLineContext(file(), 0));
  }
 }
 else
 {
  _current_index = _next_index;
  _next_index = -1; 
  if (_lines[_current_index]._where.get())
  {
   _file = _lines[_current_index]._where->file();
   _line = _lines[_current_index]._where->line();
  }
  adopt_current_line_context(new PrimLineContext(file(), _line));
 }
}

//  
//  	Buffer the incoming line from lineText[lineLength]. (A negative lineLength is permitted).
//  	If autoAdvance, the buffered text is immediately visible for error reporting as the current line.
//  	If not, visibility is delayed until advance_line(). The very first line is autoAdvance'd anyway.
//  
void PrimLexerContext::buffer_line(const unsigned char *lineText, size_t lineLength, AutoAdvance autoAdvance)
{
 clear_done();
 if (_next_index >= 0)
 {
  if ((_current_index >= 0) && !_lines[_current_index]._auto_advance)  //   If no pre-notification given.
   ERRMSG("BUG - PrimLexerContext::advance_line missed prior to PrimLexerContext::buffer_line.");
  advance_line();
 }
 if (_current_index >= 0)
  _next_index = ~_current_index & 1;
 else
  _next_index = 0;
 LineContext *const pLine = &_lines[_next_index];
 pLine->_auto_advance = false;
 pLine->_current_line.reset();
 pLine->_where.reset();
 if (lineLength >= pLine->_text.capacity())
 {
  size_t newCapacity = 2 * pLine->_text.capacity();
  if (newCapacity < 256)
   newCapacity = 256;
  if (newCapacity <= lineLength)
   newCapacity = lineLength+1;
  pLine->_text.reset(new unsigned char[newCapacity], newCapacity);
  if (!pLine->_text.get())
   ERRMSG(*this << " failed to allocate memory for " << newCapacity << " byte line buffer.");
 }
 if (pLine->_text.get())
 {
  if (lineText)
  {
   memcpy(pLine->_text.get(), lineText, lineLength);
   pLine->_text.get()[lineLength] = 0;
  }
  else
   memset(pLine->_text.get(), 0, lineLength+1);
  CONDMSG(debug_input(), std::setw(4) << (_line+1) << ": " <<  (char *)pLine->_text.get()); 
 }
 if (autoAdvance || (_current_index < 0))
  advance_line();
}

//  
//  	Return a clone of this context as part of push. Derived classes must reimplement.
//  
PrimLexerContext *PrimLexerContext::clone()
{
 return new PrimLexerContext(*this);
}

//  
//  	Return a sharable string containing the current line, caching it ijn the current line context so
//  	that the same version gets shared if required again and saving the need to find an owner.
//  
const PrimString& PrimLexerContext::current_line() const
{
 if (_at_eof || (_current_index < 0))
  return PrimString::null();
 LineContext& lineContext = mutate()._lines[_current_index];
 if (!lineContext._current_line && lineContext._text.get())
  lineContext._current_line = (char *)lineContext._text.get();
 return *lineContext._current_line;
}

bool& PrimLexerContext::debug_input()
{
 static PrimGetEnv<bool> debugInput("Prim::debug_input", false);
 return debugInput;
}

bool& PrimLexerContext::debug_lex()
{
 static PrimGetEnv<bool> debugLex("Prim::debug_lex", false);
 return debugLex;
}

bool& PrimLexerContext::debug_lex2yacc()
{
 static PrimGetEnv<bool> debugLex2Yacc("Prim::debug_lex2yacc", false);
 return debugLex2Yacc;
}

bool& PrimLexerContext::debug_yacc()
{
 static PrimGetEnv<bool> debugYacc("Prim::debug_yacc", false);
 return debugYacc;
}

//  
//  	Pop the next context creating to the current context, returning true if successful, false if
//  	at top of stack. No error message is generated, since this is the way to test for stack empty.
//  
bool PrimLexerContext::pop()
{
 PrimLexerContext *nextBuffer = _next.get();
 if (!nextBuffer)
  return false;
 _file = nextBuffer->_file;
 _line = nextBuffer->_line;
 _source_col = nextBuffer->_source_col;
 _detabbed_col = nextBuffer->_detabbed_col;
 _lines[0] = nextBuffer->_lines[0];
 _lines[1] = nextBuffer->_lines[1];
 _buffer = nextBuffer->_buffer;
 _current_index = nextBuffer->_current_index;
 _next_index = nextBuffer->_next_index;
 _at_eof = nextBuffer->_at_eof;
//  	_context = ???
 _next = nextBuffer->_next;     //   Last since causes destruction of nextBuffer.
 return true;
}

std::ostream& PrimLexerContext::print_depth(std::ostream& s, int aDepth) const
{
 Super::print_depth(s, aDepth);
 if (_next)
  _next.get()->print_deep(s, aDepth);
 return s;
}

std::ostream& PrimLexerContext::print_members(std::ostream& s, int aDepth) const
{
 Super::print_members(s, aDepth);
 if (_next)
  _next.get()->print_on(s, aDepth);
 return s;
}

std::ostream& PrimLexerContext::print_message(std::ostream& s) const
{
 if (_at_eof || (_current_index < 0))
  s << "Processing " << c_string(_file.str());
 else
 {
  s << "At " << c_string(_file.str()) << "." << _line << "." << _source_col;
  if (_lines[_current_index]._text.get())
   s << ": " << c_string(_lines[_current_index]._text.get(), "", "", PrimCString::ECHO);
 }
 return s;
}

//  
//  	Display the context as part of an inline diagnostic.
//  
std::ostream& PrimLexerContext::print_this(std::ostream& s) const
{
 return (s << _file << ":" << _line << ":" << _source_col << "." << _detabbed_col);
}

//  
//  	Push the current context with aBuffer to the next context creating a new context for aFile.
//  
void PrimLexerContext::push(const PrimString& aFile, struct yy_buffer_state *aBuffer)
{
 _buffer = aBuffer;
 _next.reset(clone());
 _lines[0].reset();
 _lines[1].reset();
 _file = aFile;
 _line = 0;
 _source_col = 0;
 _detabbed_col = 0;
 _buffer = 0;
 _current_index = -1;
 _next_index = -1;
 _at_eof = false;
 adopt_current_line_context(new PrimLineContext(file(), _line));
}

//  
//  	Register the most recent input to buffer_line() to permit an auto-advance. This registration
//  	indicates that there is no distinctive termination enabling the caller to invoke advance_line().
//  	This is typically used to mark input lines from which a continuation marker has been stripped,
//  	so that the caller can perceive the per-line buffered input as a continuous character sequence.
//  
void PrimLexerContext::set_auto_advance()
{
 if (_next_index >= 0)
  _lines[_next_index]._auto_advance = true;
 else if (_current_index >= 0)
  _lines[_current_index]._auto_advance = true;
}

//  
//  	Register the most recent input to buffer_line() as occuring at lineContext rather than previous+1.
//  
void PrimLexerContext::set_current_line_context(const PrimLineContext *lineContext)
{
 if (lineContext)
 {
  if (_next_index >= 0)
   _lines[_next_index]._where.reset(new PrimLineContext(*lineContext));
  else
  {
   _file = lineContext->file();
   _line = lineContext->line();
   adopt_current_line_context(new PrimLineContext(*lineContext));
  }
 }
}

void PrimLexerContext::set_eof()
{
 _at_eof = true;
 _line = 0;
 adopt_current_line_context(new PrimLineContext(file(), _line));
}
