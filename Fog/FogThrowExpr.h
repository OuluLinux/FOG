
#ifndef FOGTHROWEXPR_HXX
#define FOGTHROWEXPR_HXX



class FogThrowExpr : public FogDecoratedExpr {
	typedef FogDecoratedExpr Super;
	typedef FogThrowExpr This;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_LEAF_DECLS
private:
	FogThrowExpr(const This& anExpr) : Super(anExpr) {}
public:
	FogThrowExpr(FogExpr* anExpr) : Super(anExpr) {}
	virtual bool emit(FogEmitContext& emitContext) const;
	virtual size_t executable_tokens() const;
	virtual bool get_object(FogTokenRef& tokenValue, FogScopeContext& inScope) const;
	virtual char print_named(std::ostream& s, const PrimId* scopeId, char tailChar) const;
	virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
};
#endif

