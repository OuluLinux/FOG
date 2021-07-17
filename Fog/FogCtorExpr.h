
#ifndef FOGCTOREXPR_HXX
#define FOGCTOREXPR_HXX



class FogCtorExpr : public FogDecoratedExpr {
	typedef FogDecoratedExpr Super;
	typedef FogCtorExpr This;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_LEAF_DECLS
private:
	FogCtorExpr(const This& anExpr) : Super(anExpr) {}
public:
	FogCtorExpr(FogExpr& anExpr) : Super(anExpr) {}
	virtual bool emit(FogEmitContext& emitContext) const;
	virtual size_t executable_tokens() const;
	virtual bool make_specifier(FogMakeSpecifierContext& makeSpecifierContext);
	virtual char print_named(std::ostream& s, const PrimId* scopeId, char tailChar) const;
	virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
};
#endif

