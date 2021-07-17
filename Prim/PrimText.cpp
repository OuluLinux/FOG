#include <Prim/PrimIncludeAll.h>




#ifndef NO_DATA
	TYPEINFO_SINGLE(PrimText, PrimPrint);
#endif

#ifndef NO_CODE






//
//  	Construct an empty array of text strings.
//
PrimText::PrimText(size_t numLines)
	:
	_lines(numLines > 0 ? new PrimStringHandle[numLines] : 0),
	_tally(0),
	_capacity(_lines ? numLines : 0)
{}

//
//  	Construct a copy someText.
//
PrimText::PrimText(const PrimText& someText)
	:
	_lines(someText._lines && (someText._tally > 0) ? new PrimStringHandle[someText._tally] : 0),
	_tally(0),
	_capacity(_lines ? someText._tally : 0) {
	const PrimStringHandle* p = someText._lines;
	PrimStringHandle* q = _lines;
	
	if (p && q) {
		for (size_t i = someText._tally; i-- > 0; )
			*q++ = *p++;
			
		_tally = someText._tally;
	}
}

//
//  	Construct array of text strings from someTexts[numLines].
//
PrimText::PrimText(const char* const someTexts[], size_t numLines)
	:
	_lines(someTexts && (numLines > 0) ? new PrimStringHandle[numLines] : 0),
	_tally(0),
	_capacity(_lines ? numLines : 0) {
	const char* const* p = someTexts;
	PrimStringHandle* q = _lines;
	
	if (p && q) {
		for (size_t i = numLines; i-- > 0; )
			*q++ = *p++;
			
		_tally = numLines;
	}
}

//
//  	Erase the entire text array on destruction.
//
PrimText::~PrimText() {
	erase();
}

//
//  	Assign a text character array as the entire text. If aString is 0 then the resulting text is empty.
//
PrimText& PrimText::operator=(const char* aString) {
	erase();
	
	if (aString)
		append(aString);
		
	return (*this);
}

//
//  	Assign a text string as the entire text. If aString is empty then the resulting text is empty.
//
PrimText& PrimText::operator=(const PrimString& aString) {
	erase();
	
	if (!aString.is_null())
		append(aString);
		
	return (*this);
}

//
//  	Assign someText lines as the text.
//
PrimText& PrimText::operator=(const PrimText& someText) {
	erase();
	
	if (set_capacity(someText._tally)) {
		const PrimStringHandle* pLine = someText._lines;
		PrimStringHandle* qLine = _lines;
		
		for ( ; _tally < someText._tally; _tally++)
			*qLine++ = *pLine++;
	}
	
	return (*this);
}

//
//  	Add a line of text following all previous lines.
//
void PrimText::append(const PrimString& aString) {
	if ((_tally < _capacity) || set_capacity(_tally + 1))
		_lines[_tally++] = aString;     //   Install the new object
}

//
//  	Compare this text with someText for sort comparison purposes, returns
//  	-1 if "this" < someText, 0 if "this" == someText, +1 if "this" > someText.
//  	The comparisons are performed on a line by line basis until a discrepancy or
//  	the end is found.
//
int PrimText::compare(const PrimText& someText) const {
	const PrimStringHandle* pLine, *qLine;
	size_t i = 0;
	
	for (pLine = _lines, qLine = someText._lines, i = 0;
	        (i < _tally) && (i < someText._tally); pLine++, qLine++, i++) {
		int theComparison = (*pLine)->compare(**qLine);
		
		if (theComparison != 0)
			return (theComparison);
	}
	
	if (_tally == someText._tally)
		return (0);
	else if (_capacity < someText._tally)
		return (-1);
	else
		return (1);
}

//
//  	Report whether any of the lines is an exact match fot aString.
//
bool PrimText::contains(const PrimString& aString) const {
	const PrimStringHandle* pLine = _lines;
	
	for (int i = _tally; --i >= 0; pLine++)
		if (*pLine == aString)
			return (true);
			
	return (false);
}

//
//  	Erase the entire text array
//
void PrimText::erase() {
	delete[] _lines, _lines = 0, _capacity = _tally = 0;
}

//
//  	Insert a line of text, before the anIndex line.
//
void PrimText::insert_above(const PrimString& aString, unsigned int anIndex) {
	if ((_tally < _capacity) || set_capacity(_tally + 1)) {
		size_t i = _tally++;
		
		for ( ; anIndex < i; --i)
			_lines[i] = _lines[i - 1];
			
		_lines[i] = aString;      //   Install the new object
	}
}

#ifndef NO_PRIM_STREAMS
//
//  	Display the full details of the text array as part of a deep diagnostic print-out.
//
std::ostream& PrimText::print_depth(std::ostream& s, int aDepth) const {
	const PrimStringHandle* p = _lines;
	
	for (size_t i = 0; i < tally(); i++, p++)
		s << indent(aDepth) << '\"' << *p << "\"\n";
		
	return (s);
}

//
//  	Display the details of the text array as part of a diagnostic print-out.
//
std::ostream& PrimText::print_members(std::ostream& s, int aDepth) const {
	const PrimStringHandle* p = _lines;
	
	for (size_t i = 0; i < tally(); i++, p++)
		s << indent(aDepth) << '\"' << *p << "\"\n";
		
	return (s);
}

//
//  	Display the number of lines as part of an inline diagnostic.
//
std::ostream& PrimText::print_this(std::ostream& s) const {
	return (s << _tally << '/' << _capacity);
}
#endif

//
//  	Expand the internal array to accommodate numLines. Returns false on memory failure.
//
bool PrimText::set_capacity(size_t numLines) {
	if (_capacity < numLines) {
		PrimStringHandle* oldLines = _lines;
		size_t oldCapacity = _capacity;
		_capacity = std::max(_capacity + numLines, (size_t)4);
		_lines = new PrimStringHandle[_capacity];
		
		if (!_lines) {
			PRIMERR(NO_MEM, "Failed to allocate memory for " << _capacity << " text lines.");
			_lines = oldLines;
			_capacity = oldCapacity;
			return (false);
		}
		
		const PrimStringHandle* p = oldLines;
		PrimStringHandle* q = _lines;
		
		for (size_t i = 0; i < _tally; i++)
			*q++ = *p++;
			
		delete[] oldLines;
	}
	
	return (true);
}

//
//  	Reorder the lines in accordance with compareRoutine.
//
void PrimText::sort(int (*compareRoutine)(const PrimStringHandle* v1, const PrimStringHandle* v2)) {
	qsort((char*)_lines, _tally, sizeof(*_lines), (int (*)(const void* v1, const void* v2))compareRoutine);
}

//
//  	Write out all members of this array to aFileName, returning true if successful.
//  	The output comprises one floating point number per line.
//
bool PrimText::write_to_file(const char* aFileName) const {
#ifdef NEEDS_STD
	std::ofstream s(aFileName, std::ios::out, ofstream_open_mode());
#else
	std::ofstream s(aFileName, std::ios::out);
#endif
	
	if (s)
		s << *this << "\n";
		
	return s ? true : false;
}

#ifndef NO_PRIM_STREAMS
//
//  	Write a set of counted lines to a debug stream.
//
std::ostream& operator<<(std::ostream& s, const PrimText& someText) {
	const PrimStringHandle* pLine;
	int i;
	
	for (pLine = someText._lines, i = someText._tally; --i >= 0; pLine++) {
		s << *pLine;
		
		if (i > 0)
			s << "\\n";     //   Show line break as \n
	}
	
	return s;
}

#ifndef NO_FSTREAM_SUPPORT


//
//  	Write a set of counted lines to a file stream.
//
std::ofstream& operator<<(std::ofstream& s, const PrimText& someText) {
	const PrimStringHandle* pLine;
	int i;
	
	for (pLine = someText._lines, i = someText._tally; --i >= 0; pLine++) {
		s << *pLine;
		
		if (i > 0)
			s << "\n";
	}
	
	return s;
}

//
//  	Read a set of counted lines from a file stream.
//
std::ifstream& operator>>(std::ifstream& s, PrimText& someText) {
	char lineBuf[4096];
	
	while (s.getline(lineBuf, sizeof(lineBuf)))
		someText.append(lineBuf);
		
	return s;
}
#endif
#endif
#endif
