#include <Fog/FogIncludeAll.h>





FogLexerUnget::FogLexerUnget()
	:
	_tally(0),
	_capacity(0)
{}

FogLexerUnget::FogLexerUnget(const FogLexerUnget& lexerUnget)
	:
	_tally(0),
	_capacity(0) {
	if (lexerUnget._tally && set_capacity(lexerUnget._tally))
		copy(lexerUnget._contents.get(), lexerUnget._tally);
}

FogLexerUnget::~FogLexerUnget() {}

FogLexerUnget& FogLexerUnget::operator=(const FogLexerUnget& lexerUnget) {
	if ((this != &lexerUnget) && set_capacity(lexerUnget._tally))
		copy(lexerUnget._contents.get(), lexerUnget._tally);
		
	return *this;
}

void FogLexerUnget::copy(FogTokenContext* theContexts, size_t numContexts) {
	reset();
	FogTokenContext* p = theContexts;
	FogTokenContext* q = _contents.get();
	
	for (size_t i = numContexts; i; --i, _tally++)
		*q++ = *p++;
}

//
//  	Get the next character to tokenContext, returning true if one available.
//
bool FogLexerUnget::get(FogTokenContext& tokenContext) {
	if (!_tally)
		return false;
		
	tokenContext = _contents.get()[--_tally];
	return true;
}

//
//  	Peek the next character to tokenContext, returning true if one available.
//
bool FogLexerUnget::peek(FogTokenRef& tokenValue, bool skipWhite) {
	for (size_t i = _tally; i > 0; --i) {
		FogTokenContext& theContext = _contents.get()[i - 1];
		
		if (!theContext.token().is_skip(skipWhite)) {
			tokenValue = theContext.token();
			return true;
		}
	}
	
	return false;
}

void FogLexerUnget::reset() {
	while (_tally > 0)
		_contents.get()[--_tally].reset();
}

bool FogLexerUnget::set_capacity(size_t aCapacity) {
	if (aCapacity >= _capacity) {
		PrimAdoptedArray<FogTokenContext> oldContents(_contents);
		const size_t oldCapacity = _capacity;
		size_t newCapacity = 2 * aCapacity;
		
		if (newCapacity < 4)
			newCapacity = 4;
			
		_contents.reset(new FogTokenContext[newCapacity]);
		
		if (!_contents) {
			ERRMSG("FogLexerUnget failed to allocate memory for " << _capacity << " unget slots.");
			_contents = oldContents;
			return false;
		}
		
		_capacity = newCapacity;
		copy(oldContents.get(), _tally);
	}
	
	return true;
}

//
//  	Mark the most recent returned token as unconsumed and therefore to be returned again by the next call
//  	to get(). Returns true if successful, false if there is no data source available for the get().
//
bool FogLexerUnget::unget(const FogTokenContext& tokenContext, bool wsNeeded) {
	set_capacity(wsNeeded ? _tally + 2 : _tally + 1);
	_contents.get()[_tally++] = tokenContext;
	
	if (wsNeeded)
		_contents.get()[_tally++] =
		        FogTokenContext(tokenContext.line(), tokenContext.column(), FogTokenType::make_spacing(0));
		        
	return true;
}

std::ostream& operator<<(std::ostream& s, const FogLexerUnget& lexerUnget) {
	if (!lexerUnget._tally)
		s << "no-unget";
	//  	else if (lexerUnget._ws_needed)
	//  		s << "unget WS then " << lexerUnget._current;
	//  	else
	//  		s << "unget " << lexerUnget._current;
	else
		s << "unget ...";
		
	return s;
}
