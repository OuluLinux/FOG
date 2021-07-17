
#ifndef FOGITERATIONDOMAIN_HXX
#define FOGITERATIONDOMAIN_HXX




class FogIterationDomain : public FogToken {
	typedef FogToken Super;
	typedef FogIterationDomain This;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	FOGTOKEN_READ_ONLY_DECLS
private:
	const FogMetaType& _meta_type;                  //   Common type of all domain elements.
	FogTokenListOfRef _tokens;
	size_t _source_lines;                           //   Number of FogTokenType::Line tokens in _tokens.
	size_t _executable_tokens;                      //   Number of non-whitespace tokens in _tokens
private:
	FogIterationDomain(const This&);
	FogIterationDomain& operator=(const This&);
protected:
	virtual ~FogIterationDomain();
	FogIterationDomain();
public:
	FogIterationDomain(const FogMetaType& metaType);
	void add(FogToken& aToken);
	//    void add(const FogTokenListOfRefToConst& someConstTokens);
	virtual bool emit(FogEmitContext& emitContext) const;
	virtual size_t executable_tokens() const;
	virtual bool get_string(PrimIdHandle& returnId, FogScopeContext& inScope);
	virtual FogIterationDomain* is_iteration_domain();
	virtual IsExposed is_exposed() const;
	virtual const FogMetaType& meta_type() const;
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_this(std::ostream& s) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
	virtual size_t source_columns() const;
	virtual size_t source_lines() const;
	size_t tally() const { return _tokens.tally(); }
	virtual FogTokenListOfRef& tokens() { return Super::tokens(); }
	virtual const FogTokenListOfRefToConst& tokens() const;
	friend void operator==(const FogIterationDomain& firstConstTokens, const FogIterationDomain& secondConstTokens);
	//        { return &firstLine == &secondLine; }
};
#endif

