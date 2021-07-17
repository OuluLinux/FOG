
#ifndef FOGLEXERHASH_HXX
#define FOGLEXERHASH_HXX





class FogLexerTruth {

private:
	PrimAdopted<FogLexerTruth> _next;
	bool _is_enabled;
	
public:
	FogLexerTruth(): _is_enabled(true) {}
	
	FogLexerTruth(PrimAdopted<FogLexerTruth>& nextTruth, bool isEnabled)
		: _next(nextTruth), _is_enabled(isEnabled) {}
		
	bool disable() {
		_is_enabled = false;
		return _is_enabled;
	}
	
	bool enable() {
		_is_enabled = true;
		return _is_enabled;
	}
	
	bool is_enabled() const {
		return _is_enabled;
	}
	
	bool is_enabled_below() const {
		return !_next || _next->_is_enabled;
	}
	
	bool pop();
	bool push(bool aTruth);
};

class FogLexerHash : private FogLexerMacro {
	typedef FogLexerHash This;
	typedef FogLexerMacro Super;
	TYPEDECL_SINGLE(This, Super)
	
private:
	FogLexerTruth _truth;       //   Conditional context.
	
private:
	FogLexerHash(const This&);
	FogLexerHash& operator=(const This&);
	
private:
	virtual void define(const PrimId& macroId, const PrimIdList* formalList, FogTokens& someTokens) = 0;
	virtual void hash_include(const PrimId& fileStem) = 0;
	virtual const FogSourceFile* make_source_file(const PrimId& fileName) = 0;
	virtual void undefine(const PrimId& anId) = 0;
	
protected:
	FogLexerHash(std::istream& inputStream, const FogSourceFile& sourceFile, std::ostream* outputStream)
		: Super(inputStream, sourceFile, outputStream) {}
		
	virtual ~FogLexerHash() {}
	
	const char* get(FogTokenContext& tokenContext);
	using Super::include_file;
	
public:
	using Super::is_defined;
	using Super::print_message_line;
	using Super::print_this;
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	void reset();
};

#endif

