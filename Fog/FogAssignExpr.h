
#ifndef FOGASSIGNEXPR_HXX
#define FOGASSIGNEXPR_HXX



class FogAssignExpr : public FogDyadicExpr {
	typedef FogDyadicExpr Super;
	typedef FogAssignExpr This;
	TYPEDECL_SINGLE(This, Super)
protected:
	FogAssignExpr(FogExpr& firstExpr, FogExpr& secondExpr) : Super(firstExpr, secondExpr) {}
	FogAssignExpr(const This& anExpr) : Super(anExpr) {}
	virtual ~FogAssignExpr() {}
public:
	virtual bool get_object(FogTokenRef& tokenValue, FogScopeContext& inScope) const;
};

#define FOGASSIGNEXPR_INTERFAC(T) \
	class T : public FogAssignExpr \
	{ \
		typedef FogAssignExpr Super; \
		typedef T This; \
		TYPEDECL_SINGLE(This, Super) \
		FOGTOKEN_LEAF_DECLS \
	private: \
		virtual FogTokenType::TokenType dyadic_token() const; \
		virtual const char *separator() const; \
		T(const This& anExpr) : Super(anExpr) {} \
	public: \
		T(FogExpr& firstExpr, FogExpr& secondExpr) : Super(firstExpr, secondExpr) {}
#define FOGASSIGNEXPR_INTERFACE(T) FOGASSIGNEXPR_INTERFAC(T) };

FOGASSIGNEXPR_INTERFAC(FogAssExpr)
virtual bool emit(FogEmitContext& emitContext) const;
virtual bool get_object(FogTokenRef& tokenValue, FogScopeContext& inScope) const;
virtual bool make_specifier(FogMakeSpecifierContext& makeSpecifierContext);
virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
virtual void set_decl_specifier_expression(FogExprRef& anExpr, FogDeclSpecifier& declSpecifiers);
};
FOGASSIGNEXPR_INTERFACE(FogAssAddExpr)
FOGASSIGNEXPR_INTERFACE(FogAssAndExpr)
FOGASSIGNEXPR_INTERFACE(FogAssDivExpr)
FOGASSIGNEXPR_INTERFACE(FogAssModExpr)
FOGASSIGNEXPR_INTERFACE(FogAssMulExpr)
FOGASSIGNEXPR_INTERFACE(FogAssOrExpr)
FOGASSIGNEXPR_INTERFACE(FogAssShlExpr)
FOGASSIGNEXPR_INTERFACE(FogAssShrExpr)
FOGASSIGNEXPR_INTERFACE(FogAssSubExpr)
FOGASSIGNEXPR_INTERFACE(FogAssXorExpr)
#endif

