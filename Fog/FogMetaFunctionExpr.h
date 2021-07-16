
#ifndef FOGMETAFUNCTIONEXPR_HXX
#define FOGMETAFUNCTIONEXPR_HXX



class FogMetaFunctionExpr : public FogDecoratedExpr
{
    typedef FogDecoratedExpr Super;
    typedef FogMetaFunctionExpr This;
    TYPEDECL_SINGLE(This, Super)
    FOGTOKEN_MEMBER_DECLS
    FOGTOKEN_LEAF_DECLS
private:
    FogTokensRef _args;
    const IsExposed _is_exposed;               //   True if exposed return
    FogTokenStatementsRef _body;
private:
    FogMetaFunctionExpr(const This& anExpr);
    This& mutate() const { return *(This *)this; }
protected:
    virtual ~FogMetaFunctionExpr();
public:
    FogMetaFunctionExpr(FogExpr& anExpr, FogTokens& argList, IsExposed isExposed, FogTokenStatements& theBody);
    virtual size_t executable_tokens() const;
    const FogToken *find(FogScopeContext& scopeContext, const PrimId& anId) const;
    virtual bool make_specifier(FogMakeSpecifierContext& makeSpecifierContext);
    virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_long_id(std::ostream& s) const;
    virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_viz(std::ostream& s) const;
    virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
    virtual void set_decl_specifier_expression(FogExprRef& anExpr, FogDeclSpecifier& declSpecifiers);
};
#endif
 
