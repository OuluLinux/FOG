
#ifndef FOGINITEXPR_HXX
#define FOGINITEXPR_HXX



class FogInitExpr : public FogDecoratedExpr {
	typedef FogDecoratedExpr Super;
	typedef FogInitExpr This;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_LEAF_DECLS
private:
	FogInitExpr(const This& anExpr) : Super(anExpr) {}
public:
	FogInitExpr(FogExpr& anExpr) : Super(anExpr) {}
	virtual bool emit(FogEmitContext& emitContext) const;
	virtual size_t executable_tokens() const;
	virtual bool make_specifier(FogMakeSpecifierContext& makeSpecifierContext);
	virtual char print_named(std::ostream& s, const PrimId* scopeId, char tailChar) const;
	virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
};
#endif

