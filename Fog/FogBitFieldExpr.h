
#ifndef FOGBITFIELDEXPR_HXX
#define FOGBITFIELDEXPR_HXX



class FogBitFieldExpr : public FogDyadicExpr {
	NEEDS_A_FRIEND_FOR_GNUC
	typedef FogDyadicExpr Super;
	typedef FogBitFieldExpr This;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_LEAF_DECLS
	
private:
	virtual ~FogBitFieldExpr() {}
	
	virtual FogTokenType::TokenType dyadic_token() const;
	
protected:
	FogBitFieldExpr(const This& anExpr) : Super(anExpr) {}
	
public:
	FogBitFieldExpr(FogExpr& firstExpr, FogExpr& secondExpr) : Super(firstExpr, secondExpr) {}
	
	virtual bool emit(FogEmitContext& emitContext) const;
	virtual bool make_ctor_init(FogRawCtorInitRef& ctorInit);
	virtual bool make_specifier(FogMakeSpecifierContext& makeSpecifierContext);
	virtual char print_named(std::ostream& s, const PrimId *fullId, char tailChar) const;
	virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
	virtual void set_decl_specifier_expression(FogExprRef& anExpr, FogDeclSpecifier& declSpecifiers);
};

#endif

