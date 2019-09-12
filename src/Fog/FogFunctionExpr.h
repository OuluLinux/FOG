
#ifndef FOGFUNCTIONEXPR_HXX
#define FOGFUNCTIONEXPR_HXX



class FogFunctionExpr : public FogDecoratedExpr
{
 typedef FogDecoratedExpr Super;
 typedef FogFunctionExpr This;
 TYPEDECL_SINGLE(This, Super)
 FOGTOKEN_MEMBER_DECLS
 FOGTOKEN_LEAF_DECLS
private:
 FogCodeRef _code;
private:
 FogFunctionExpr(const This& anExpr);
 This& mutate() const { return *(This *)this; }
protected:
 virtual ~FogFunctionExpr();
public:
 FogFunctionExpr(FogExpr& anExpr, FogCode& theCode);
    virtual size_t executable_tokens() const;
 virtual bool make_specifier(FogMakeSpecifierContext& makeSpecifierContext);
 virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
 virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
 virtual std::ostream& print_viz(std::ostream& s) const;
 virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
 virtual void set_decl_specifier_expression(FogExprRef& anExpr, FogDeclSpecifier& declSpecifiers);
};
#endif
 
