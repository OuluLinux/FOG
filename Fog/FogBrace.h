
#ifndef FOGBRACE_HXX
#define FOGBRACE_HXX



class FogBrace : public FogToken {
	typedef FogToken Super;
	typedef FogBrace This;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	FOGTOKEN_SHAREDLEAF_DECLS
	
private:
	const size_t _end_column;     //   Number of spaces.
	
private:
	FogBrace(const This&);
	This& operator=(const This&);
	
protected:
	FogBrace(size_t endColumn = 0) : _end_column(endColumn) {}
	
	virtual ~FogBrace() {}
	
public:
	virtual size_t executable_tokens() const;
	size_t end_column() const {
		return _end_column;
	}
	
public:
	virtual FogTokenType::TokenType pp_token_type_enum() const;
	virtual std::ostream& print_this(std::ostream& s) const;
	virtual FogTokenType::TokenType token_type_enum() const;
	
public:
	static FogBrace& make_di_open_brace(size_t endColumn);
	static FogBrace& make_open_brace(size_t endColumn);
};

#endif

