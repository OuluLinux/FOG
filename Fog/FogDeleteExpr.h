
#ifndef FOGDELETEEXPR_HXX
#define FOGDELETEEXPR_HXX




class FogDeleteExpr : public FogDecoratedExpr {
	typedef FogDecoratedExpr Super;
	typedef FogDeleteExpr This;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_MEMBER_DECLS
	FOGTOKEN_LEAF_DECLS
private:
	IsGlobalDelete _is_global;
	const IsArrayDelete _is_array;
private:
	FogDeleteExpr(const This& anExpr);
public:
	FogDeleteExpr(FogExpr& anExpr, IsArrayDelete isArray) : Super(anExpr), _is_global(SCOPED_DELETE), _is_array(isArray) {}
	virtual size_t executable_tokens() const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
	void set_global() { _is_global = GLOBAL_DELETE; }
};
#endif

