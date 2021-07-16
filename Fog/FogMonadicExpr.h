
#ifndef FOGMONADICEXPR_HXX
#define FOGMONADICEXPR_HXX



class FogMonadicExpr : public FogDecoratedExpr
{
    typedef FogDecoratedExpr Super;
    typedef FogMonadicExpr This;
    TYPEDECL_SINGLE(This, Super)
private:
    This& mutate() const { return *(This *)this; }
    virtual FogTokenType::TokenType monadic_token() const { return FogTokenType::init(); }
    virtual const char *prefix() const = 0;
    virtual const char *suffix() const = 0;
protected:
    FogMonadicExpr() {}
    FogMonadicExpr(FogExpr& anExpr) : Super(anExpr) {}
    FogMonadicExpr(FogExpr *anExpr) : Super(anExpr) {}
    FogMonadicExpr(const This& anExpr);
    virtual ~FogMonadicExpr() {}
public:
    virtual bool emit(FogEmitContext& emitContext) const;
    virtual size_t executable_tokens() const;
    virtual bool get_object(FogTokenRef& tokenValue, FogScopeContext& inScope) const;
    virtual char print_named(std::ostream& s, const PrimId *scopeId, char tailChar) const;
    virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
};

#define FOGMONADICDECL_INTERFAC(T) \
class T : public FogMonadicExpr \
{ \
    typedef FogMonadicExpr Super; \
    typedef T This; \
    TYPEDECL_SINGLE(This, Super) \
    FOGTOKEN_LEAF_DECLS \
private: \
    T(const This& anExpr) : Super(anExpr) {} \
    virtual FogTokenType::TokenType monadic_token() const; \
    virtual const char *prefix() const; \
    virtual const char *suffix() const; \
public: \
    T(FogExpr& anExpr) : Super(anExpr) {}

#define FOGMONADICDECL_INTERFACE(T) \
FOGMONADICDECL_INTERFAC(T) \
};

FOGMONADICDECL_INTERFAC(FogAddrExpr)
    virtual bool make_specifier(FogMakeSpecifierContext& makeSpecifierContext);
    virtual bool make_typed_expression(FogExprRef& theExpr, FogName& theType);
    virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
    virtual void set_decl_specifier_expression(FogExprRef& anExpr, FogDeclSpecifier& declSpecifiers);
};

FOGMONADICDECL_INTERFAC(FogComplExpr)
    virtual bool make_specifier(FogMakeSpecifierContext& makeSpecifierContext);
    virtual bool make_typed_expression(FogExprRef& theExpr, FogName& theType);
    virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
    virtual void set_decl_specifier_expression(FogExprRef& anExpr, FogDeclSpecifier& declSpecifiers);
};
FOGMONADICDECL_INTERFACE(FogLogNotExpr)
FOGMONADICDECL_INTERFACE(FogNegExpr)
FOGMONADICDECL_INTERFACE(FogParenExpr)
FOGMONADICDECL_INTERFAC(FogPostDecExpr)
    virtual bool get_object(FogTokenRef& tokenValue, FogScopeContext& inScope) const;
};
FOGMONADICDECL_INTERFAC(FogPostIncExpr)
    virtual bool get_object(FogTokenRef& tokenValue, FogScopeContext& inScope) const;
};
FOGMONADICDECL_INTERFAC(FogPreDecExpr)
    virtual bool get_object(FogTokenRef& tokenValue, FogScopeContext& inScope) const;
};
FOGMONADICDECL_INTERFAC(FogPreIncExpr)
    virtual bool get_object(FogTokenRef& tokenValue, FogScopeContext& inScope) const;
};

FOGMONADICDECL_INTERFAC(FogSizeofExpr)
    virtual void create_usage(FogUsageContext& usageContext) const;
};

class FogStarExpr : public FogMonadicExpr
{
    typedef FogMonadicExpr Super;
    typedef FogStarExpr This;
    TYPEDECL_SINGLE(This, Super)
    FOGTOKEN_MEMBER_DECLS
    FOGTOKEN_LEAF_DECLS
private:
    FogModifierRef _ptr_op;                             //  .bugbug merge
private:
    FogStarExpr(const This& anExpr);
    virtual FogTokenType::TokenType monadic_token() const;
    virtual const char *prefix() const;
    virtual const char *suffix() const;
public:
    FogStarExpr(FogModifier& ptrOp, FogExpr& anExpr);
    virtual bool make_specifier(FogMakeSpecifierContext& makeSpecifierContext);
    virtual bool make_typed_expression(FogExprRef& theExpr, FogName& theType);
    virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
    virtual void set_decl_specifier_expression(FogExprRef& anExpr, FogDeclSpecifier& declSpecifiers);
};

FOGMONADICDECL_INTERFAC(FogTypeidExpr)
    virtual void create_usage(FogUsageContext& usageContext) const;
};
#endif
 
