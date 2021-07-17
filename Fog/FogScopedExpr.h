
#ifndef FOGSCOPEDEXPR_HXX
#define FOGSCOPEDEXPR_HXX



class FogScopedExpr : public FogDecoratedExpr {
	typedef FogDecoratedExpr Super;
	typedef FogScopedExpr This;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_MEMBER_DECLS
	FOGTOKEN_LEAF_DECLS
private:
	FogRawSubspaceRef _subspace;
private:
	FogScopedExpr(const This& anExpr);
	This& mutate() const { return *(This*)this; }
protected:
	virtual ~FogScopedExpr();
public:
	FogScopedExpr(FogExpr& anExpr, FogRawSubspace& aSubspace);
	virtual size_t executable_tokens() const;
	virtual FogScope& inner_scope();
	virtual bool make_specifier(FogMakeSpecifierContext& makeSpecifierContext);
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_long_id(std::ostream& s) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
	virtual void set_decl_specifier_expression(FogExprRef& anExpr, FogDeclSpecifier& declSpecifiers);
};
#endif

