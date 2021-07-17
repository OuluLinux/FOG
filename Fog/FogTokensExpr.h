
#ifndef FOGTOKENSEXPR_HXX
#define FOGTOKENSEXPR_HXX




class FogTokensExpr : public FogExpr {
	typedef FogExpr Super;
	typedef FogTokensExpr This;
	TYPEDECL_SINGLE(This, Super)
	//      PRIMREF_DERIVED_DECLS(This)
	FOGTOKEN_DERIVED_DECLS
	FOGTOKEN_MEMBER_DECLS
	FOGTOKEN_LEAF_DECLS
private:
	FogTokenListOfRef _tokens;
private:
	This& mutate() const { return *(This*)this; }
protected:
	FogTokensExpr(const This& anExpr);
	virtual ~FogTokensExpr() {}
public:
	FogTokensExpr(FogTokens& someTokens);
	virtual size_t executable_tokens() const;
	virtual bool get_object(FogTokenRef& returnValue, FogScopeContext& inScope) const;
	//      virtual bool morph_to(FogTokenRef& returnValue, const FogMetaType& metaType, IsExposed isExposed,
	//        FogScopeContext& inScope) const;
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
	size_t tally() const { return _tokens.tally(); }
	virtual FogTokenListOfRef& tokens();
	virtual const FogTokenListOfRefToConst& tokens() const;
};
#endif

