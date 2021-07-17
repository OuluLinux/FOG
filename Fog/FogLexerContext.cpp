#include <Fog/FogIncludeAll.h>














struct FogLexerContext_Element : public PrimPrint {
	typedef PrimPrint Super;
	typedef FogLexerContext_Element This;
	TYPEDECL_SINGLE(This, Super)
public:          //   Defining context
	FogLexerContext& _lexer;     //   Lexer to handle error messages.
	PrimAdopted<FogLexerContext_Element> _next; //   Next buffer in stack.
	PrimAdopted<std::istream> _stream;   //   Stream used to provide input from push(.., aStream, ...).
	const FogUtility& _utility_mode;   //   Status of extra declarations.
	const FogSourceFile* _source_file;   //   The source file.
	struct yy_buffer_state* _buffer;   //   The lexer buffer on stack, YY_CURRENT_BUFFER otherwise.
public:          //   Working variables: current line.
	FogLineRefToConst _current_line;   //   Line currently in lexer.
	size_t _detabbed_col;      //   The current detabbed source column.
	size_t _pending_cols;      //   Detabbed column to advance before next get.
	bool _done;         //   True if error message generated for this cointext.
public:          //   Working variables: following lines.
	FogLineRef _next_line;      //   Line pending for lexer.
	size_t _next_next_line_number;    //   The line number for the line after _next_line.
	PrimStringHandle _read_buffer;    //   The logical source line (post phase 2, backslashes flattened).
	size_t _read_tally;       //   Number of characters in _text.
	size_t _read_index;       //   Read index from _text.
private:
	FogLexerContext_Element(const This&);
	This& operator=(const This&);
private:
	static const char* clean_up(char* buffer, size_t len);
	int get_line(std::istream& yyin);
	This& mutate() const { return *(This*)this; }
public:
	FogLexerContext_Element(FogLexerContext& aLexer, const FogSourceFile& sourceFile);
	FogLexerContext_Element(FogLexerContext& aLexer, PrimAdopted<FogLexerContext_Element>& nestedContext,
	                        const PrimId& sourceId, PrimAdopted<std::istream>& aStream, const FogUtility& utilityMode,
	                        const FogSourceFile& sourceFile);
	virtual ~FogLexerContext_Element();
	//  	void advance_line() { _current_line = _next_line; _done = false; }
	int get_line(std::istream& yyin, char* buffer, int max_size);
	PrimAdopted<FogLexerContext_Element>& next() { return _next; }
	const FogLexerContext_Element* next() const { return _next.get(); }
	virtual std::ostream& print_message_line(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_this(std::ostream& s) const;
};

TYPEINFO_SINGLE(FogLexerContext_Element, Super)
TYPEINFO_SINGLE(FogLexerContext, Super)

FogLexerContext::FogLexerContext(const FogSourceFile& sourceFile) {
	_context.reset(new FogLexerContext_Element(*this, sourceFile));
}

FogLexerContext::~FogLexerContext() {}

void FogLexerContext::advance_line() {
	if (!_context)
		ERRMSG("BUG - should not invoke FogLexerContext::advance_line for empty stack.");
	else {
		//  		clear_done();
		//  		_context->advance_line();
		_context->_current_line = _context->_next_line;
		//  		_context->_detabbed_col = 0;
		//  		_context->_pending_cols = 0;
		_context->_done = false;
	}
}

void FogLexerContext::advance_token(size_t detabbedCols) {
	if (!_context)
		ERRMSG("BUG - should not invoke FogLexerContext::advance_token for empty stack.");
	else {
		_context->_detabbed_col += _context->_pending_cols;
		_context->_pending_cols = detabbedCols;
		//  		_context->_detabbed_col += detabbedCols;
	}
}

FogLine& FogLexerContext::current_line() const {
	if (!_context) {
		ERRMSG("BUG - should not invoke FogLexerContext::current_line for empty stack.");
		return FogLine::mutable_null();
	}
	
	return *_context->_next_line;
}

size_t FogLexerContext::detabbed_col() const {
	if (!_context) {
		ERRMSG("BUG - should not invoke FogLexerContext::detabbed_col for empty stack.");
		return 0;
	}
	
	return _context->_detabbed_col;
}

//
//  	Acquire some more input to buffer[max_size] returning the number of characters to result.
//  	Returns result, which is -1 at end of input.
//
//  	This routine performs the first two phases of ANSI C/C++ processing.
//
//  	Phase 1:	Source and execution characters sets are equivalent so there is no mapping.
//  				Tri-graphs are replaced.
//
//  	Phase 2:	Back-slash new-line combinations are eliminated.
//  				Carriage returns are eliminated.
//
//  	Universal characters corresponding to graphical characters or whitespace are converted to ASCII.
//
//  	Inordinately long input lines, or continued input lines are spliced as a consequence of multiple
//  	invocations returning successive segments without a trailing new-line.
//
//  	Input is processed line by line, rather than the mega-read of the conventional implementation.
//  	This should not present an undue efficiency problem, since the use of an ifstream provides first
//  	level buffering. Line by line processing ensures that the FogLexerContext can buffer the current
//  	line and make it available for use in error messages.
//
int FogLexerContext::get_line(std::istream& yyin, char* buffer, int max_size) {
	if (!_context) {
		ERRMSG("BUG - should not invoke FogLexerContext::get_line for empty stack.");
		return -1;
	}
	
	return _context->get_line(yyin, buffer, max_size);
}

//
//  	Pop a context element, returning the non-0 buffer containing any continuation.
//
struct yy_buffer_state* FogLexerContext::pop_buffer() {
	if (!_context) {
		ERRMSG("BUG - should not invoke FogLexerContext::pop for empty stack.");
		return 0;
	}
	
	if (!_context->next())
		return 0;
		
	_context = _context->next();
	//  	if (!_context)
	//  		return 0;
	popped_file(_context->_utility_mode, _context->_source_file);
	return _context->_buffer;
}

std::ostream& FogLexerContext::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	
	for (const FogLexerContext_Element* p = _context.get(); p; p = p->next())
		p->print_deep(s, aDepth);
		
	return s;
}

std::ostream& FogLexerContext::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	
	for (const FogLexerContext_Element* p = _context.get(); p; p = p->next())
		p->print_on(s, aDepth);
		
	return s;
}

std::ostream& FogLexerContext::print_message_line(std::ostream& s, int aDepth) const {
	if (_context)
		_context->print_message_line(s, aDepth);
		
	//  	for (const FogLexerContext_Element *p = _context.get(); p; p = p->next())
	//  		p->print_message(s);
	return s;
}

std::ostream& FogLexerContext::print_this(std::ostream& s) const {
	if (_context)
		_context->print_this(s);
		
	return s;
}

//
//  	Push the existing input context defined by currentBuffer on to the stack, and establish a
//  	re-entrant context by reading input from aStream in utilityMode. The source is informally referred
//  	to as sourceId in diagnostics.
//
std::istream* FogLexerContext::push_buffer(const PrimId& sourceId, PrimAdopted<std::istream>& aStream,
        struct yy_buffer_state* currentBuffer, const FogUtility& utilityMode, const FogSourceFile& sourceFile) {
	if (!_context)
		ERRMSG("BUG - should not invoke FogLexerContext::push_buffer for empty stack.");
	else
		_context->_buffer = currentBuffer;
		
	_context.reset(new FogLexerContext_Element(*this, _context, sourceId, aStream,
	               pushed_file(utilityMode, sourceFile), sourceFile));
	return _context ? _context->_stream.get() : 0;
}

void FogLexerContext::set_line(const FogSourceFile* sourceFile, size_t lineNumber) {
	if (!_context)
		ERRMSG("BUG - should not invoke FogLexerContext::push for empty stack.");
	else {
		if (!sourceFile)
			sourceFile = &_context->_next_line->file();
			
		_context->_next_line.adopt(FogLine::new_line(sourceFile, lineNumber, _context->_next_line->text()));
		_context->_source_file = sourceFile;
		_context->_next_next_line_number = lineNumber;
	}
}

FogLexerContext_Element::FogLexerContext_Element(FogLexerContext& aLexer, const FogSourceFile& sourceFile)
	:
	_lexer(aLexer),
	_utility_mode(FogUtility::emit_utility()),
	_source_file(&sourceFile),
	_buffer(0),
	_detabbed_col(0),
	_pending_cols(0),
	_done(0),
	_next_next_line_number(1),
	_read_tally(0),
	_read_index(1)
{}

FogLexerContext_Element::FogLexerContext_Element(FogLexerContext& aLexer,
        PrimAdopted<FogLexerContext_Element>& nestedContext,
        const PrimId& sourceId, PrimAdopted<std::istream>& aStream, const FogUtility& utilityMode,
        const FogSourceFile& sourceFile)
	:
	_lexer(aLexer),
	_next(nestedContext),
	_stream(aStream),
	_utility_mode(utilityMode),
	_source_file(&sourceFile),
	_buffer(0),
	_detabbed_col(0),
	_pending_cols(0),
	_done(0),
	_next_next_line_number(1),
	_read_tally(0),
	_read_index(1)
{}

FogLexerContext_Element::~FogLexerContext_Element() {}

//
//  	Perform translation phase 1 to clean up the source buffer[len] returning a pointer past its new end,
//  	which is guaranteed to be no longer than the original length.
//
//  	ASCII printables are resurrected from universal characters.
//  	Carriage returns are suppressed.
//  	Trigraphs are resolved.
//
const char* FogLexerContext_Element::clean_up(char* buffer, size_t len) {
	const char* pEnd = buffer + len;
	char* q = buffer;
	
	for (const char* p = buffer; p < pEnd; ) {
		switch (*p) {
		case '\\':
			if ((p + 1) < pEnd) {						// Isolated \ gets lost.
				if (((p + 5) < pEnd) && (*(p + 1) == 'u') && (*(p + 2) == '0') && (*(p + 3) == '0')
				        && isdigit(*(p + 4)) && isxdigit(*(p + 5))) {
					int charCode = ((*(p + 4) - '0') << 4)
					               | (isdigit(*(p + 5)) ? (*(p + 5) - '0') : (tolower(*(p + 5)) - 'a' + 10));
					               
					if ((charCode == '\r') || (!isgraph(charCode) && !isspace(charCode)))
						*q++ = *p++;
					else {
						*q++ = charCode;
						p += 6;
					}
				}
				else if (((p + 9) < pEnd) && (*(p + 1) == 'U') && (*(p + 2) == '0') && (*(p + 3) == '0')
				         && (*(p + 4) == '0') && (*(p + 5) == '0') && (*(p + 6) == '0') && (*(p + 7) == '0')
				         && isdigit(*(p + 8)) && isxdigit(*(p + 9))) {
					int charCode = ((*(p + 8) - '0') << 4)
					               | (isdigit(*(p + 9)) ? (*(p + 9) - '0') : (tolower(*(p + 9)) - 'a' + 10));
					               
					if ((charCode == '\r') || (!isgraph(charCode) && !isspace(charCode)))
						*q++ = *p++;
					else {
						*q++ = charCode;
						p += 10;
					}
				}
				else {
					*q++ = *p++;
					*q++ = isgraph(*p) ? *p : '\\';			// \\ replaces garbage escape
					p++;
				}
			}
			
			break;
			
		case '\r':
			p++;
			break;
			
		case  '?':
			if (((p + 2) < pEnd) && (*(p + 1) == '?')) {
				switch (*(p + 2)) {
				case '=': *q++ = '#'; p += 3; break;
				
				case '(': *q++ = '['; p += 3; break;
				
				case '<': *q++ = '{'; p += 3; break;
				
				case '/': *q++ = '\\'; p += 3; break;
				
				case ')': *q++ = ']'; p += 3; break;
				
				case '>': *q++ = '}'; p += 3; break;
				
				case '\'': *q++ = '^'; p += 3; break;
				
				case '!': *q++ = '|'; p += 3; break;
				
				case '-': *q++ = '~'; p += 3; break;
				
				case '?': *q++ = '?'; p += 3; break;
				
				default:
					GLOBAL_WRNMSG("Unrecognised trigraph " << c_string(p, 3));
					*q++ = *p++;
					*q++ = *p++;
					*q++ = *p++;
					break;
				}
			}
			else
				*q++ = *p++;
				
			break;
			
		default:
			*q++ = *p++;
			break;
		}
	}
	
	return q;
}

//
//	Fill lexBuffer[maxSize] with as much of a logical line as will fit, returning the number of
//	characters tranbsferred. A long line is returned gradually by multiple calls.
//
int FogLexerContext_Element::get_line(std::istream& yyin, char* lexBuffer, int maxSize) {
	if (yyin.eof())
		return 0;
		
	while (_read_index > _read_tally) {			// == is \n still to go.
		int aStatus = get_line(yyin);
		
		if (aStatus <= 0)
			return aStatus;
			
		_read_index = 0;
	}
	
	int readSize = _read_tally - _read_index;
	
	if (readSize > maxSize)
		readSize = maxSize;
		
	memcpy(lexBuffer, _read_buffer.str() + _read_index, readSize);
	_read_index += readSize;
	
	if (readSize < maxSize) {
		lexBuffer[readSize++] = '\n';
		_read_index++;
	}
	
	return readSize;
}

//
//	Read a logical line into _read_buffer, performing translation phases 1 and 2.
//
//.bugbug phase 1 performed after backslash removal.
//
int FogLexerContext_Element::get_line(std::istream& yyin) {
	std::strstream s;
	size_t numBytes = 0;
	size_t myLine = _next_next_line_number;
	size_t trigraphThreshold = 0;						//.bugbug only 10% implemented
	char localBuffer[4096];
	
	while (true) {
		yyin.getline(localBuffer, sizeof(localBuffer) - 1, '\n');
		//		if (!yyin.getline(localBuffer, readSize, '\n') && !yyin.gcount())
		//			return 0;
		//			return -1;
		int len = yyin.gcount();
		
		if (yyin.eof()) {                                           // Partial final line
			_next_next_line_number++;
			CONDMSG(PrimLexerContext::debug_input(), std::setw(4) << (_next_next_line_number - 1) << ": " << localBuffer);
			s.write(localBuffer, len);
			numBytes += len;
			trigraphThreshold += len;
			break;
		}
		else if (yyin.fail()) {                                     // Partial long line
			yyin.clear(yyin.rdstate() & ~std::ios::failbit);
			CONDMSG(PrimLexerContext::debug_input(), std::setw(4) << (_next_next_line_number - 1) << ": " << localBuffer);
			s.write(localBuffer, len);
			numBytes += len;
			trigraphThreshold += len;
		}
		else if (!yyin)                                             // File failure
			return -1;
		else if ((len >= 2) && (localBuffer[len - 2] == '\\')) {    // Continued line
			_next_next_line_number++;
			CONDMSG(PrimLexerContext::debug_input(), std::setw(4) << (_next_next_line_number - 1) << ": " << localBuffer);
			s.write(localBuffer, len - 2);
			numBytes += len - 2;
			trigraphThreshold += len - 2;
		}
		else {                                                      // Normal complete line
			_next_next_line_number++;
			CONDMSG(PrimLexerContext::debug_input(), std::setw(4) << (_next_next_line_number - 1) << ": " << localBuffer);
			s.write(localBuffer, len - 1);
			numBytes += len - 1;
			trigraphThreshold += len - 1;
			break;
		}
	}
	
	char* pStart = s.str();
	const char* pEnd = clean_up(pStart, numBytes);
	_read_tally = pEnd - pStart;
	_read_buffer = PrimStringHandle(s.str(), _read_tally);
	delete[] s.str();
	_next_line.adopt(FogLine::new_line(_source_file, myLine, *_read_buffer));
	_detabbed_col = 0;
	_pending_cols = 0;
	return _read_tally ? (_read_tally + 1) : (yyin.eof() ? 0 : 1);	// +1 for \n which was read but not stored.
}

std::ostream& FogLexerContext_Element::print_message_line(std::ostream& s, int aDepth) const {
	if (_done)
		return s;
		
	mutate()._done = true;
	
	if (_next) {
		//		s << ' ';
		_next->print_message_line(s, aDepth);
	}
	
	s << indent(aDepth);
	
	if (_next)
		s << ' ';
		
	//	if (_at_eof || (_current_index < 0))
	//		s << "Processing " << c_string(_file.str());
	//	else
	{
		if (_current_line)
			s << "At " << viz(*_current_line) << "." << _detabbed_col << ": " << _current_line->text();
		else if (_source_file)
			s << "At " << viz(*_source_file) << "." << _detabbed_col;
		else
			s << "In null-file";
			
		//		if (_lines[_current_index]._text.get())
		//			s << ": " << c_string(_lines[_current_index]._text.get(), "", "", PrimCString::ECHO);
	}
	return s << std::endl;
}

std::ostream& FogLexerContext_Element::print_this(std::ostream& s) const {
	if (_current_line)
		s << viz(*_current_line) << "." << _detabbed_col << ": " << _current_line->text();
	else if (_source_file)
		s << viz(*_source_file) << "." << _detabbed_col;
	else
		s << "null-file , ";
		
	s << _utility_mode	;
	return s;
}
