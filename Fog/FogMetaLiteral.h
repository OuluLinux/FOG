
#ifndef FOGMETALITERAL_HXX
#define FOGMETALITERAL_HXX



class FogMetaLiteral : public FogExpr {
	typedef FogExpr Super;
	typedef FogMetaLiteral This;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_MEMBER_DECLS
	FOGTOKEN_LEAF_DECLS
private:                                //   Defining contents
	FogTokenRef _potential;                 //   The source tree.
	FogTokenRef _actual;                    //   The actual resolved tree.
private:
	This& mutate() const { return *(This*)this; }
protected:
	FogMetaLiteral(const This& thatId);
	virtual ~FogMetaLiteral();
public:
	explicit FogMetaLiteral(FogToken& aToken);
	virtual bool get_object(FogTokenRef& tokenValue, FogScopeContext& inScope) const;
	virtual FogName* get_type();
	virtual bool make_specifier(FogMakeSpecifierContext& makeSpecifierContext);
	virtual bool morph_to(FogTokenRef& returnValue, const FogMetaType& metaType, IsExposed isExposed,
	                      FogScopeContext& inScope) const;
	virtual FogTokenType::TokenType pp_token_type_enum() const;
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
	virtual FogTokenType::TokenType token_type_enum() const;
};
#endif

