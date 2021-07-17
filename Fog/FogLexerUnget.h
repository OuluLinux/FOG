
#ifndef FOGLEXERUNGET_HXX
#define FOGLEXERUNGET_HXX




class FogLexerUnget {
private:
	PrimAdoptedArray<FogTokenContext> _contents; //   Recorded tokens and associated lines.
	size_t _tally;         //   Number of entries in _contents.
	size_t _capacity;        //   Maximum number of entries in _contents.
private:
	void copy(FogTokenContext* theContexts, size_t numContexts);
	bool set_capacity(size_t aCapacity);
public:
	FogLexerUnget();
	FogLexerUnget(const FogLexerUnget& lexerUnget);
	~FogLexerUnget();
	FogLexerUnget& operator=(const FogLexerUnget& lexerUnget);
	bool get(FogTokenContext& tokenContext);
	bool is_empty() const { return _tally == 0; }
	bool is_full() const { return _tally > 0; }
	bool peek(FogTokenRef& tokenValue, bool skipWhite);
	void reset();
	bool unget(const FogTokenContext& tokenContext, bool wsNeeded);
	friend std::ostream& operator<<(std::ostream& s, const FogLexerUnget& lexerUnget);
};
#endif

