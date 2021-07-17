
#ifndef FOGTOKENS_HXX
#define FOGTOKENS_HXX




class FogTokens : public FogToken {
	typedef FogToken Super;
	typedef FogTokens This;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	FOGTOKEN_DERIVED_DECLS
	FOGTOKEN_READ_ONLY_DECLS
private:
	FogTokenListOfRef _tokens;
	IsExposed _is_exposed;
private:
	FogTokens& operator=(const This&);
protected:
	FogTokens(const This&);
	virtual ~FogTokens();
public:
	FogTokens(IsExposed isExposed = IS_EXPOSED);
	void add(FogToken& aToken);
	void add(const FogTokenListOfRef& someTokens);
	virtual bool compile_declaration(FogParseContext& parseContext) const;
	virtual bool emit(FogEmitContext& emitContext) const;
	virtual bool get_object(FogTokenRef& returnValue, FogScopeContext& inScope) const;
	virtual bool get_string(PrimIdHandle& returnId, FogScopeContext& inScope);
	virtual bool is_compound() const;
	virtual IsExposed is_exposed() const;
	virtual IsList is_list() const;
	virtual FogTokens* is_tokens();
	virtual FogLine& line() const;
	virtual bool morph_to(FogTokenRef& returnValue, const FogMetaType& metaType, IsExposed isExposed,
	                      FogScopeContext& inScope) const;
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual char print_named(std::ostream& s, const PrimId* scopeId, char tailChar) const;
	virtual std::ostream& print_this(std::ostream& s) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
	virtual void set_exposed();
	virtual void set_line(FogLine& aLine);
	size_t tally() const { return _tokens.tally(); }
	virtual FogTokenListOfRef& tokens();
	virtual const FogTokenListOfRefToConst& tokens() const;
	friend void operator==(const FogTokens& firstTokens, const FogTokens& secondTokens);
	//        { return &firstLine == &secondLine; }
};
#endif

