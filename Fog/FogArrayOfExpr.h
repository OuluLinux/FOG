
#ifndef FOGARRAYOFEXPR_HXX
#define FOGARRAYOFEXPR_HXX




class FogArrayOfExpr : public FogExpr {
	typedef FogExpr Super;
	typedef FogArrayOfExpr This;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_MEMBER_DECLS
	FOGTOKEN_LEAF_DECLS
private:
	//   Defining contents.
	FogExprListOfRefToConst _potential_exprs;   //   List of initialisers.
private:
	//   Built contents.
	FogExprListOfRefToConst _floating_exprs;    //   List of inits to concatenate during composition.
	FogExprListOfRefToConst _fixed_exprs;       //   Array of positioned inits.
	size_t _next_index;
private:
	FogArrayOfExpr(const This& anExpr);
	void add(size_t anIndex, const FogExpr& anExpr);
protected:
	virtual ~FogArrayOfExpr();
public:
	FogArrayOfExpr();
	FogArrayOfExpr(const FogExprListOfRefToConst& exprList);
	virtual bool get_object(FogTokenRef& returnValue, FogScopeContext& inScope) const;
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
};
#endif

