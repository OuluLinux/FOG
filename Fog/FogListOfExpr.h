
#ifndef FOGLISTOFEXPR_HXX
#define FOGLISTOFEXPR_HXX




class FogListOfExpr : public FogExpr
{
    typedef FogExpr Super;
    typedef FogListOfExpr This;
    TYPEDECL_SINGLE(This, Super)
    PRIMREF_DERIVED_DECLS(This)
    FOGTOKEN_DERIVED_DECLS
    FOGTOKEN_MEMBER_DECLS
    FOGTOKEN_LEAF_DECLS
private:
    FogExprListOfRef _exprs;
    const IsExposed _is_exposed;
private:
    This& mutate() const { return *(This *)this; }
protected:
    FogListOfExpr(const This& anExpr);
    virtual ~FogListOfExpr() {}
public:
    FogListOfExpr(IsExposed isExposed = IS_EXPOSED);
    void add(FogExpr& anExpr);
    virtual bool compile_parsed(FogParseContext& parseContext);
    virtual const FogDeclSpecifierValue& decl_specifiers() const;
    virtual size_t executable_tokens() const;
    FogExprListOfRef& exprs() { return _exprs; }
    const FogExprListOfRefToConst& exprs() const { return _exprs; }
    virtual bool get_object(FogTokenRef& returnValue, FogScopeContext& inScope) const;
    virtual IsExposed is_exposed() const;
    virtual IsList is_list() const;
    virtual FogListOfExpr *is_list_of_expr();
    bool make_parameters(FogFunctionModifier& functionModifier, FogMakeSpecifierContext& makeSpecifierContext);
    virtual bool make_specifier(FogMakeSpecifierContext& makeSpecifierContext);
    virtual bool make_typed_expression(FogExprRef& theExpr, FogName& theType);
    virtual bool morph_to(FogTokenRef& returnValue, const FogMetaType& metaType, IsExposed isExposed,
        FogScopeContext& inScope) const;
    virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_viz(std::ostream& s) const;
    virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
    virtual void set_decl_specifier_expression(FogExprRef& anExpr, FogDeclSpecifier& declSpecifiers);
    size_t tally() const { return _exprs.tally(); }
    virtual FogTokenListOfRef& tokens();
    virtual const FogTokenListOfRefToConst& tokens() const;
};
#endif
 
