
#ifndef FOGDYADICEXPR_HXX
#define FOGDYADICEXPR_HXX



class FogDyadicExpr : public FogDecoratedExpr {
	typedef FogDecoratedExpr Super;
	typedef FogDyadicExpr This;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_MEMBER_DECLS
	
private:
	//   Defining contents.
	FogExprRef _expr1;
	
private:
	This& mutate() const {
		return *(This*)this;
	}
	
	virtual FogTokenType::TokenType dyadic_token() const {
		return FogTokenType::init();
	}
	
	virtual const char* separator() const {
		return "";
	}
	
protected:
	FogDyadicExpr(FogExpr& firstExpr, FogExpr& secondExpr) : Super(firstExpr), _expr1(secondExpr) {}
	
	FogDyadicExpr(const This& anExpr);
	virtual ~FogDyadicExpr() {}
	
	FogExpr& expr1() {
		return *_expr1;
	}
	
	const FogExpr& expr1() const {
		return *_expr1;
	}
	
public:
	virtual size_t executable_tokens() const;
	virtual bool get_object(FogTokenRef& tokenValue, FogScopeContext& inScope) const;
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
};

#define FOGDYADICEXPR_INTERFAC(T) \
	class T : public FogDyadicExpr \
	{ \
		typedef FogDyadicExpr Super; \
		typedef T This; \
		TYPEDECL_SINGLE(This, Super) \
		FOGTOKEN_LEAF_DECLS \
	private: \
		virtual FogTokenType::TokenType dyadic_token() const; \
		virtual const char *separator() const; \
		T(const This& anExpr) : Super(anExpr) {} \
	public: \
		T(FogExpr& firstExpr, FogExpr& secondExpr) : Super(firstExpr, secondExpr) {}

#define FOGDYADICEXPR_INTERFACE(T) FOGDYADICEXPR_INTERFAC(T) };

FOGDYADICEXPR_INTERFAC(FogAddExpr)
virtual void make_actual_from(FogMakeActualContext& makeActualContext);
};

FOGDYADICEXPR_INTERFAC(FogAndExpr)
virtual void make_actual_from(FogMakeActualContext& makeActualContext);
virtual bool make_specifier(FogMakeSpecifierContext& makeSpecifierContext);
virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
virtual void set_decl_specifier_expression(FogExprRef& anExpr, FogDeclSpecifier& declSpecifiers);
};

FOGDYADICEXPR_INTERFACE(FogArrowStarExpr)
FOGDYADICEXPR_INTERFACE(FogDivExpr)
FOGDYADICEXPR_INTERFACE(FogDotStarExpr)
FOGDYADICEXPR_INTERFACE(FogEqExpr)
FOGDYADICEXPR_INTERFACE(FogGeExpr)
FOGDYADICEXPR_INTERFACE(FogGtExpr)
FOGDYADICEXPR_INTERFACE(FogLeExpr)
FOGDYADICEXPR_INTERFACE(FogLogAndExpr)
FOGDYADICEXPR_INTERFACE(FogLogOrExpr)
FOGDYADICEXPR_INTERFACE(FogLtExpr)
FOGDYADICEXPR_INTERFACE(FogModExpr)

class FogMulExpr : public FogDyadicExpr {
	typedef FogDyadicExpr Super;
	typedef FogMulExpr This;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_LEAF_DECLS
	
private:
	FogModifierRef _ptr_op;        //  .bugbug merge
	
private:
	virtual FogTokenType::TokenType dyadic_token() const;
	virtual const char* separator() const;
	FogMulExpr(const This& anExpr);
	
public:
	FogMulExpr(FogExpr& firstExpr, FogModifier& ptrOp, FogExpr& secondExpr);
	virtual bool emit(FogEmitContext& emitContext) const;
	virtual void make_actual_from(FogMakeActualContext& makeActualContext);
	virtual bool make_specifier(FogMakeSpecifierContext& makeSpecifierContext);
	virtual char print_named(std::ostream& s, const PrimId* fullId, char tailChar) const;
	virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
	virtual void set_decl_specifier_expression(FogExprRef& anExpr, FogDeclSpecifier& declSpecifiers);
};

FOGDYADICEXPR_INTERFACE(FogNeExpr)
FOGDYADICEXPR_INTERFACE(FogOrExpr)
FOGDYADICEXPR_INTERFACE(FogShlExpr)
FOGDYADICEXPR_INTERFACE(FogShrExpr)

FOGDYADICEXPR_INTERFAC(FogSubExpr)
virtual void make_actual_from(FogMakeActualContext& makeActualContext);
};

FOGDYADICEXPR_INTERFACE(FogXorExpr)
#endif

