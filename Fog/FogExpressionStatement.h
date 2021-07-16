
#ifndef FOGSTATEMENTEXPR_HXX
#define FOGSTATEMENTEXPR_HXX



class FogExpressionStatement : public FogStatement
{
 NEEDS_A_FRIEND_FOR_GNUC
    typedef FogStatement Super;
    typedef FogExpressionStatement This;
    TYPEDECL_SINGLE(This, Super)
    FOGTOKEN_MEMBER_DECLS
    FOGTOKEN_LEAF_DECLS
private:
    FogExprRef _expr;
private:
    virtual ~FogExpressionStatement();
protected:
    FogExpressionStatement(const This& aStatement);
public:
    FogExpressionStatement(FogExpr& anExpr, bool isMeta = false);
    virtual bool compile_parsed(FogParseContext& parseContext);
    virtual size_t executable_tokens() const;
    virtual bool make_specifier(FogMakeSpecifierContext& makeSpecifierContext);
    virtual const FogMetaType& meta_type() const;
    virtual bool morph_to(FogTokenRef& returnValue, const FogMetaType& metaType, IsExposed isExposed,
        FogScopeContext& inScope) const;
    virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
 virtual std::ostream& print_viz(std::ostream& s) const;
    virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
};
#endif
 
