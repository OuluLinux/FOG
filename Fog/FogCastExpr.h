
#ifndef FOGCASTEXPR_HXX
#define FOGCASTEXPR_HXX




class FogCastExpr : public FogDecoratedExpr {
	typedef FogDecoratedExpr Super;
	typedef FogCastExpr This;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_MEMBER_DECLS
private:
	FogExprRef _cast;
private:
	This& mutate() const { return *(This*)this; }
	virtual const char* midfix() const;
	virtual const char* prefix() const;
	virtual const char* suffix() const;
protected:
	FogCastExpr(const This& anExpr);
	FogCastExpr(FogExpr& typeId, FogExpr& anExpr);
	virtual ~FogCastExpr();
	FogExpr& cast() { return *_cast; }
	const FogExpr& cast() const { return *_cast; }
public:
	virtual size_t executable_tokens() const;
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
};

#define FOGCASTEXPR_INTERFAC(T) \
	class T : public FogCastExpr \
	{ \
		typedef FogCastExpr Super; \
		typedef T This; \
		TYPEDECL_SINGLE(This, Super) \
		FOGTOKEN_LEAF_DECLS \
	private: \
		T(const This& anExpr) : Super(anExpr) {} \
		virtual const char *midfix() const; \
		virtual const char *prefix() const; \
		virtual const char *suffix() const; \
	public: \
		T(FogExpr& typeId, FogExpr& anExpr) : Super(typeId, anExpr) {}

#define FOGCASTEXPR_INTERFACE(T) FOGCASTEXPR_INTERFAC(T) };

FOGCASTEXPR_INTERFACE(FogConstCastExpr)
FOGCASTEXPR_INTERFACE(FogDynamicCastExpr)
FOGCASTEXPR_INTERFAC(FogExplicitCastExpr)
virtual void make_actual_token(FogTokenRefToConst& actualToken, FogMakeActualContext& makeActualContext) const;
virtual bool make_specifier(FogMakeSpecifierContext& makeSpecifierContext);
virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
private:
This& mutate() const { return *(This*)this; }
};
FOGCASTEXPR_INTERFAC(FogIndexCastExpr)
FogExpr& expr() { return Super::expr(); }
const FogExpr& expr() const { return Super::expr(); }
const FogExpr& index() const { return cast(); }
virtual FogIndexCastExpr* is_index_cast_expr();
virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
};
FOGCASTEXPR_INTERFACE(FogReinterpretCastExpr)
FOGCASTEXPR_INTERFACE(FogStaticCastExpr)
#endif

