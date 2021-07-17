
#ifndef FOGCONDEXPR_HXX
#define FOGCONDEXPR_HXX




class FogCondExpr : public FogDecoratedExpr {
	typedef FogDecoratedExpr Super;
	typedef FogCondExpr This;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_MEMBER_DECLS
	FOGTOKEN_LEAF_DECLS
private:
	//   Defining contents.
	FogExprRef _true;
	FogExprRef _false;
protected:
	FogCondExpr(const This& anExpr);
	virtual ~FogCondExpr() {}
public:
	FogCondExpr(FogExpr& firstExpr, FogExpr& secondExpr, FogExpr& thirdExpr)
		: Super(firstExpr), _true(secondExpr), _false(thirdExpr) {}
	virtual size_t executable_tokens() const;
	virtual bool get_object(FogTokenRef& returnValue, FogScopeContext& inScope) const;
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual bool put_value(FogScopeContext& putScope, const FogToken& tokenValue, FogScopeContext& getScope) const;
	virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
};
#endif

