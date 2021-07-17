
#ifndef FOGINDEXEXPR_HXX
#define FOGINDEXEXPR_HXX



class FogIndexExpr : public FogDyadicExpr {
	NEEDS_A_FRIEND_FOR_GNUC
	typedef FogDyadicExpr Super;
	typedef FogIndexExpr This;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_LEAF_DECLS
private:
	virtual ~FogIndexExpr() {}
	virtual FogTokenType::TokenType dyadic_token() const;
protected:
	FogIndexExpr(const This& anExpr) : Super(anExpr) {}
public:
	FogIndexExpr(FogExpr& firstExpr, FogExpr& secondExpr) : Super(firstExpr, secondExpr) {}
	virtual bool emit(FogEmitContext& emitContext) const;
	FogExpr& expr() { return Super::expr(); }
	const FogExpr& expr() const { return Super::expr(); }
	virtual FogIndexExpr* is_index_expr();
	virtual bool make_specifier(FogMakeSpecifierContext& makeSpecifierContext);
	FogExpr* new_index_cast_expression(FogExpr& anExpr);
	virtual char print_named(std::ostream& s, const PrimId* fullId, char tailChar) const;
	virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
	virtual void set_decl_specifier_expression(FogExprRef& anExpr, FogDeclSpecifier& declSpecifiers);
};
#endif

