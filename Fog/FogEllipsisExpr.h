
#ifndef FOGELLIPSISEXPR_HXX
#define FOGELLIPSISEXPR_HXX




class FogEllipsisExpr : public FogExpr {
	typedef FogExpr Super;
	typedef FogEllipsisExpr This;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_LEAF_DECLS
protected:
	FogEllipsisExpr(const This& anExpr);
	virtual ~FogEllipsisExpr();
public:
	FogEllipsisExpr();
	virtual bool emit(FogEmitContext& emitContext) const;
	virtual char print_named(std::ostream& s, const PrimId* fullId, char tailChar) const;
	virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
};
#endif

