
#ifndef FOGEXPR_HXX
#define FOGEXPR_HXX



class FogExpr : public FogRaw
{
    typedef FogRaw Super;
    typedef FogExpr This;
    TYPEDECL_SINGLE(This, Super)
    PRIMREF_DERIVED_DECLS(This)
    FOGTOKEN_DERIVED_DECLS
private:
    This& mutate() const { return *(This *)this; }
protected:
    FogExpr() {}
    FogExpr(const This& anExpr) : Super(anExpr) {}
    virtual ~FogExpr() {}
public:
    virtual bool compile_parsed(FogParseContext& parseContext);
    virtual const FogDeclSpecifierValue& decl_specifiers() const;
    virtual const FogMetaArgument *find_formal_in(FogScopeContext& scopeContext) const;
    virtual FogAnonId *is_anon();
    const FogAnonId *is_anon() const { return mutate().is_anon(); }
    virtual FogExpr *is_expr();
    virtual FogIndexCastExpr *is_index_cast_expr();
    const FogIndexCastExpr *is_index_cast_expr() const { return mutate().is_index_cast_expr(); }
    virtual FogIndexExpr *is_index_expr();
    const FogIndexExpr *is_index_expr() const { return mutate().is_index_expr(); }
    virtual FogParenthesisExpr *is_parenthesis_expr();
    const FogParenthesisExpr *is_parenthesis_expr() const { return mutate().is_parenthesis_expr(); }
    virtual FogTreeCallExpr *is_tree_call_expr();
    virtual bool make_ctor_init(FogRawCtorInitRef& ctorInit);
    virtual bool make_typed_expression(FogExprRef& theExpr, FogName& theType);
    virtual const FogMetaType& meta_type() const;
    virtual bool morph_to(FogTokenRef& returnValue, const FogMetaType& metaType, IsExposed isExposed,
        FogScopeContext& inScope) const;
    virtual std::ostream& print_this(std::ostream& s) const;
    virtual std::ostream& print_viz(std::ostream& s) const;
    virtual bool resolve_slot(FogTokenRef& returnValue, FogScopeContext& scopeContext,
        const FogToken& objectValue) const;
    virtual void set_decl_specifier_expression(FogExprRef& anExpr, FogDeclSpecifier& declSpecifiers);
    virtual void set_decl_specifier_raw(FogRawRef& aRaw, FogDeclSpecifier& declSpecifiers);
    virtual void set_destructor_expression(FogExprRef& anExpr);
};
#endif
 
