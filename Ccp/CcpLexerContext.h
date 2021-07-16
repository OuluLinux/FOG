// **************************** RACAL Confidential ****************************
// 
// 	  Title:			Lexer include buffer context class interface
// 
// 	  File Name:		CcpLexerContext.hxx
// 
// 	  Author:			E.D.Willink
// 
// 	  SCCS:				%W% %G%
// 
// 	  Latest Modification:
//  EDW	1.1		Date:	 8-Oct-1996		Original.
//  EDW	1.2		Date:	28-Apr-1998		Adjust to PrimStringHandle, use std streams
// END
// 

#ifndef CCPLEXERCONTEXT_HXX
#define CCPLEXERCONTEXT_HXX


// 
// 	A CcpLexerContext maintains the lexing input context during processing of a file, and a stack
// 	of outer contexts for restoration as include file processing unwinds.
// 	This class is intended for the exclusive use of CcpLexer which is responsible for appropriate deletion
// 	of the parsing buffer.
// 
class CcpLexerContext : public PrimPrint
{
 TYPEDECL_SINGLE(CcpLexerContext, PrimPrint)
private:
 PrimAdopted<CcpLexerContext> _next;   //  Next buffer in stack.
 PrimStringHandle _file;      //  The source file.
 size_t _line;        //  The line number.
 size_t _col;        //  The current column.
 PrimAdoptedArray<unsigned char> _text_line; //  The current line contents.
 size_t _text_capacity;      //  The allocated size of _text_line.
 struct yy_buffer_state *_buffer;   //  The lexer buffer on stack, YY_CURRENT_BUFFER otherwise.
 
private:
 CcpLexerContext(CcpLexerContext& aContext);
 CcpLexerContext& operator=(const CcpLexerContext& aContext); //  Not implemented.

public:
 CcpLexerContext(const PrimString& aFile);
// 
// 	The destructor adds no functionality.
// 
 virtual ~CcpLexerContext() {}
// 
// 	Advance the context by numCols.
// 
 void advance(int numCols) { _col += numCols; }
// 
// 	Report the buffer.
// 
 struct yy_buffer_state *buffer() { return _buffer; }
 void buffer_line(const unsigned char *lineText, int lineLength, bool newLine = false);
// 
// 	Report the file.
// 
 const char *file() const { return _file->str(); }
// 
// 	Report the line.
// 
 size_t line() const { return _line; }
 std::ostream& message(std::ostream& s, const char *msg, const unsigned char *yyText, size_t yyLeng) const;
 bool pop();
 virtual std::ostream& print_this(std::ostream& s) const;
 void push(const PrimString& aFile, struct yy_buffer_state *aBuffer);
// 
// 	Return a pointer to the text line.
// 
 const unsigned char *text() { return _text_line.get(); }
};

#endif
