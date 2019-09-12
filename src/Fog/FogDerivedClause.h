
#ifndef FOGDERIVEDCLAUSE_HXX
#define FOGDERIVEDCLAUSE_HXX



class FogDerivedClause : public FogRaw
{
    typedef FogRaw Super;
    typedef FogDerivedClause This;
    TYPEDECL_SINGLE(This, Super)
    FOGTOKEN_MEMBER_DECLS
    FOGTOKEN_LEAF_DECLS
private:
    FogExprRef _expr;
    FogRawRef _raw;
private:
    FogDerivedClause(const This& anExpr);
    This& mutate() const { return *(This *)this; }
protected:
    virtual ~FogDerivedClause();
public:
    FogDerivedClause(FogExpr& anExpr, FogRaw *theRaw);
    virtual bool compile_parsed(FogParseContext& parseContext);
    virtual const FogLazyBool& derive_at(FogScopeContext& inScope) const;
    virtual size_t executable_tokens() const;
    const FogExpr& expr() const { return *_expr; }
    virtual FogName *get_type();
    virtual FogDerivedClause *is_derived_clause();
    virtual bool make_specifier(FogMakeSpecifierContext& makeSpecifierContext);
    virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_viz(std::ostream& s) const;
    FogRaw& raw() { return *_raw; }
    const FogRaw& raw() const { return *_raw; }
    virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
    virtual void set_line(FogLine& aLine);
};
#endif
 
