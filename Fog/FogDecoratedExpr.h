
#ifndef FOGDECORATEDEXPR_HXX
#define FOGDECORATEDEXPR_HXX




class FogDecoratedExpr : public FogExpr {
	typedef FogExpr Super;
	typedef FogDecoratedExpr This;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_DERIVED_DECLS
	FOGTOKEN_MEMBER_DECLS
private:
	FogExprRef _expr;
private:
	This& mutate() const { return *(This*)this; }
protected:
	FogDecoratedExpr() {}
	FogDecoratedExpr(FogExpr& anExpr) : _expr(anExpr) {}
	FogDecoratedExpr(FogExpr* anExpr) : _expr(anExpr) {}
	FogDecoratedExpr(const This& anExpr);
	virtual ~FogDecoratedExpr() {}
	FogExpr& expr() { return *_expr; }
	const FogExpr& expr() const { return *_expr; }
	void set_decorated_decl_specifier_expression(FogExprRef& anExpr, FogDeclSpecifier& declSpecifiers);
	void set_destructor_expression();
public:
	virtual const FogDeclSpecifierValue& decl_specifiers() const;
	virtual size_t executable_tokens() const =  0;
	virtual FogName* get_type();
	virtual bool make_typed_expression(FogExprRef& theExpr, FogName& theType);
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual void set_destructor_expression(FogExprRef& anExpr) { Super::set_destructor_expression(anExpr); }
	virtual void set_line(FogLine& aLine);
};
#endif

