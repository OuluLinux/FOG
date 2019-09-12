
#ifndef FOGTREECALLEXPR_HXX
#define FOGTREECALLEXPR_HXX



class FogTreeCallExpr : public FogDecoratedExpr
{
    typedef FogDecoratedExpr Super;
    typedef FogTreeCallExpr This;
    TYPEDECL_SINGLE(This, Super)
    FOGTOKEN_MEMBER_DECLS
    FOGTOKEN_LEAF_DECLS
private:
    FogTokensRef _args;
private:
    FogTreeCallExpr(const This& anExpr);
    This& mutate() const { return *(This *)this; }
protected:
    virtual ~FogTreeCallExpr();
public:
    FogTreeCallExpr(FogExpr& anExpr, FogTokens& theArgs);
    FogTokens& args() { return *_args; }
    virtual size_t executable_tokens() const;
    FogExpr& expr() { return Super::expr(); }
    const FogExpr& expr() const { return Super::expr(); }
    virtual bool get_object(FogTokenRef& tokenValue, FogScopeContext& inScope) const;
    virtual FogTreeCallExpr *is_tree_call_expr();
    virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_viz(std::ostream& s) const;
    virtual bool resolve_slot(FogTokenRef& returnValue, FogScopeContext& scopeContext,
        const FogToken& objectValue) const;
};
#endif
 
