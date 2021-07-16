
#ifndef PRIMLEXERCONTEXT_HXX
#define PRIMLEXERCONTEXT_HXX








//  
//  	A PrimLexerContext maintains the lexing input context during processing of a file, and a stack
//  	of outer contexts for restoration as include file processing unwinds.
//  	This class is intended for the exclusive use of an XxxLexer created by flex++ which is responsible
//  	for appropriate deletion of the parsing buffer by provision of a PrimAdopted<PrimLexerContext> _context.
//  
//  	New contexts are typically pushed by:
//  
//  		_context->set_buffer(YY_CURRENT_BUFFER);
//  		_context.reset(new PrimLexerContext(_context, anId.str(), *aString));
//  		yy_switch_to_buffer(yy_create_buffer(_context->stream(), YY_BUF_SIZE));
//    or
//  		_context->set_buffer(YY_CURRENT_BUFFER);
//  		_context.reset(new PrimLexerContext(_context, fileName));
//  		yy_switch_to_buffer(yy_create_buffer(_context->stream(), YY_BUF_SIZE));
//  
//  	and subsequently popped by
//  
//  	%define WRAP_NOCODE
//  	int XxxLexer::yywrap()
//  	{
//  		_context = _context->next();
//  		if (!_context)
//  			return 1;
//  		struct yy_buffer_state *deadBuffer = YY_CURRENT_BUFFER;
//  		yy_switch_to_buffer(_context->buffer());
//  		yy_delete_buffer(deadBuffer);
//  		return 0;
//  	}
//  
//  	Provision of code in XxxLexer is necessary since flex++ does not make yy or YY symbols accessible.
//  
//  	See /dsp/c++/fog/*/FogLexer.l for an example of use to resolve textual macros.
//  
class PrimLexerContext : public PrimParserMonitor
{
 typedef PrimParserMonitor Super;
 typedef PrimLexerContext This;
 TYPEDECL_SINGLE(This, Super)
public:
 enum AutoAdvance { DELAYED_ADVANCE = false, AUTO_ADVANCE = true };
private:
 struct LineContext
 {
  PrimCountedArray<unsigned char> _text; //   The line contents.
  bool _auto_advance;      //   One shot flags set true before buffer_line to enable silent 
            //    automatic invocation of advance_line, used to buffer
            //     continuation lines, since caller cannot detect wrap.
  PrimAdopted<PrimLineContext> _where; //   Non-default location of line.
  mutable PrimStringHandle _current_line; //   String version of _text iff created.
  LineContext() : _auto_advance(false) {}
  void reset() { _text.reset(); _auto_advance = false; _where.reset(); }
 };
  
private:
 PrimAdopted<PrimLexerContext> _next;  //   Next buffer in stack.
 PrimStringHandle _file;      //   The source file.
 size_t _line;        //   The line number.
 size_t _source_col;       //   The current source column.
 size_t _detabbed_col;      //   The current detabbed source column.
 LineContext _lines[2];      //   The current and next line contents.
 struct yy_buffer_state *_buffer;   //   The lexer buffer on stack, YY_CURRENT_BUFFER otherwise.
 PrimLineContextRefToConst _context;   //   Saved context of less nested line.
 int _current_index;       //   Location of current line in _lines[], -ve if none.
 int _next_index;       //   Location of next line in _lines[], -ve if none.
 bool _at_eof;        //   Set true once file read.
            //   ? redundant w.r.t. _current_index -ve ?
 PrimStringHandle _string;     //   String that can cache an istrstream input for _stream.
 PrimAdopted<std::istream> _stream;   //   Stream used to provide input from push(.., aStream, ...).
 
private:
 This& operator=(const This& aContext);    //   Not implemented.

protected:
 PrimLexerContext(This& aContext);     //   Perverse implementation to suit push.

private:
 virtual PrimLexerContext *clone();
 This& mutate() const { return *(This *)this; }
public:
 PrimLexerContext(const PrimString& aFile);
//  	PrimLexerContext(PrimAdopted<PrimLexerContext>& nestedContext, const PrimString& fileName);
//  	PrimLexerContext(PrimAdopted<PrimLexerContext>& nestedContext, const PrimString& sourceId,
//  		const PrimString& aString);
//  	PrimLexerContext(PrimAdopted<PrimLexerContext>& nestedContext, const PrimString& sourceId,
//  		PrimAdopted<std::istream>& aStream);
 PrimLexerContext(Install doInstall, const PrimString& aFile);
 PrimLexerContext(Install doInstall, PrimAdopted<PrimLexerContext>& nestedContext,
  const PrimString& fileName);
 PrimLexerContext(Install doInstall, PrimAdopted<PrimLexerContext>& nestedContext, const PrimString& sourceId,
  const PrimString& aString);
 PrimLexerContext(Install doInstall, PrimAdopted<PrimLexerContext>& nestedContext, const PrimString& sourceId,
  PrimAdopted<std::istream>& aStream);
 virtual ~PrimLexerContext();
 void advance(size_t numCols) { _source_col += numCols; _detabbed_col += numCols; }
 void advance(size_t numCols, size_t detabbedCols) { _source_col += numCols; _detabbed_col += detabbedCols; }
 void advance_line();
 struct yy_buffer_state *buffer() { return _buffer; }
 void buffer_line(const unsigned char *lineText, size_t lineLength, AutoAdvance autoAdvance = AUTO_ADVANCE);
 size_t col() const { return _source_col; }
 const PrimString& current_line() const;
 size_t detabbed_col() const { return _detabbed_col; }
 const PrimString& file() const { return *_file; }
 const char *file_str() const { return _file.str(); }
 size_t line() const { return _line; }
 PrimAdopted<PrimLexerContext>& next() { return _next; }
 const PrimLexerContext *next() const { return _next.get(); }
 bool pop();
 virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
 virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
 virtual std::ostream& print_message(std::ostream& s) const;
 virtual std::ostream& print_this(std::ostream& s) const;
 void push(const PrimString& aFile, struct yy_buffer_state *aBuffer);
 std::istream *push(const PrimString& sourceId, PrimAdopted<std::istream>& aStream,
  struct yy_buffer_state *aBuffer);
 void set_auto_advance();
 void set_buffer(struct yy_buffer_state *currentBuffer) { _buffer = currentBuffer; }
 void set_current_line_context(const PrimLineContext *lineContext);
 void set_eof();
 void set_string(const PrimString& aString) { _string = aString; }
 size_t source_col() const { return _source_col; }
 std::istream *stream() { return _stream.get(); }
 const PrimString& string() const { return *_string; }

public:
 static bool& debug_input();
 static bool& debug_lex();
 static bool& debug_lex2yacc();
 static bool& debug_yacc();
};

#endif
 
