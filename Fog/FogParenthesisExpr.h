
#ifndef FOGPARENTHESISEXPR_HXX
#define FOGPARENTHESISEXPR_HXX





class FogParenthesisExpr : public FogDecoratedExpr { //   Inherits 0, 1, or list of arguments
	typedef FogDecoratedExpr Super;
	typedef FogParenthesisExpr This;
	friend class FogCallExpr;                       //   To access exprs()
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	FOGTOKEN_DERIVED_DECLS
	FOGTOKEN_MEMBER_DECLS
	FOGTOKEN_LEAF_DECLS
private:
	FogCvHandle _cv;
	FogExprListOfRefToConst _exceptions;
private:
	This& mutate() const { return *(This*)this; }
protected:
	FogParenthesisExpr();
	FogParenthesisExpr(const This& anExpr);
	virtual ~FogParenthesisExpr() {}
	FogListOfExpr& exprs() { return (FogListOfExpr&)expr(); }
public:
	FogParenthesisExpr(FogListOfExpr& exprList, const FogCv& aCv, void* exceptionSpecification);
	virtual size_t executable_tokens() const;
	const FogListOfExpr& exprs() const { return (const FogListOfExpr&)expr(); }
	virtual bool get_object(FogTokenRef& tokenValue, FogScopeContext& inScope) const;
	virtual FogParenthesisExpr* is_parenthesis_expr();
	virtual bool make_specifier(FogMakeSpecifierContext& makeSpecifierContext);
	FogModifier* new_function_modifier(FogMakeSpecifierContext& makeSpecifierContext);
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
	virtual void set_decl_specifier_expression(FogExprRef& anExpr, FogDeclSpecifier& declSpecifiers);
	const FogExpr* terms() const { return expr().is_null() ? 0 : &expr(); }
};
#endif

