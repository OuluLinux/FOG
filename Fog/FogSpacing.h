
#ifndef FOGSPACING_HXX
#define FOGSPACING_HXX



class FogSpacing : public FogToken {
	typedef FogToken Super;
	typedef FogSpacing This;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	FOGTOKEN_SHAREDLEAF_DECLS
private:
	const int _spaces;     //   Number of spaces.
private:
	FogSpacing(const This&);
	This& operator=(const This&);
protected:
	FogSpacing(size_t someSpaces = 0) : _spaces(someSpaces) {}
	virtual ~FogSpacing() {}
public:
	virtual size_t executable_tokens() const;
	int spaces() const { return _spaces; }
public:
	virtual bool emit(FogEmitContext& emitContext) const;
	virtual const FogSpacing* is_spacing() const;
	virtual const FogToken* is_white() const { return this; }
	virtual FogTokenType::TokenType pp_token_type_enum() const;
	virtual std::ostream& print_diagnostic(std::ostream& s, int aDepth) const;
	virtual char print_named(std::ostream& s, const PrimId* scopeId, char tailChar) const;
	virtual std::ostream& print_this(std::ostream& s) const;
	virtual std::ostream& print_source(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_stringized(std::ostream& s) const;
	virtual std::ostream& print_tokens(std::ostream& s, int aDepth) const;
	virtual size_t source_columns() const;
	virtual FogTokenType::TokenType token_type_enum() const;
public:
	static FogSpacing& make_spacing(int someSpaces);
};
#endif

