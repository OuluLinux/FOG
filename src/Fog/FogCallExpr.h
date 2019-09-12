
#ifndef FOGCALLEXPR_HXX
#define FOGCALLEXPR_HXX



class FogCallExpr : public FogDecoratedExpr
{
    typedef FogDecoratedExpr Super;
    typedef FogCallExpr This;
    TYPEDECL_SINGLE(This, Super)
    FOGTOKEN_MEMBER_DECLS
    FOGTOKEN_LEAF_DECLS
private:
    FogParenthesisExprRef _parenthesis;
private:
    FogCallExpr(const This& anExpr);
protected:
    virtual ~FogCallExpr();
public:
    FogCallExpr(FogExpr& anExpr, FogParenthesisExpr& theParenthesis);
    virtual size_t executable_tokens() const;
    virtual bool get_object(FogTokenRef& tokenValue, FogScopeContext& inScope) const;
    virtual bool make_ctor_init(FogRawCtorInitRef& ctorInit);
    virtual bool make_specifier(FogMakeSpecifierContext& makeSpecifierContext);
    virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_viz(std::ostream& s) const;
    virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
    virtual void set_decl_specifier_expression(FogExprRef& anExpr, FogDeclSpecifier& declSpecifiers);
    virtual void set_destructor_expression(FogExprRef& anExpr);
};
#endif
 
