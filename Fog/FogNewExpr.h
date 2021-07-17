
#ifndef FOGNEWEXPR_HXX
#define FOGNEWEXPR_HXX




class FogNewExpr : public FogExpr {
	typedef FogExpr Super;
	typedef FogNewExpr This;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_MEMBER_DECLS
	FOGTOKEN_LEAF_DECLS
private:
	IsGlobalDelete _is_global;
	FogListOfExprRef _place;
	FogExprRef _type;
	FogExprRef _init;
private:
	FogNewExpr(const This& aDecl);
protected:
	virtual ~FogNewExpr();
public:
	FogNewExpr(FogListOfExpr* aPlace, FogExpr& typeId, FogExpr* anInit);
	FogNewExpr(FogListOfExpr& aPlace, FogListOfExpr* typeId, FogExpr* anInit);
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
	void set_global() { _is_global = GLOBAL_DELETE; }
};
#endif

