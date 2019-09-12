
#ifndef FOGDOWHILESTATEMENT_HXX
#define FOGDOWHILESTATEMENT_HXX




class FogDoWhileStatement : public FogStatement
{
    typedef FogStatement Super;
    typedef FogDoWhileStatement This;
    TYPEDECL_SINGLE(This, Super)
    FOGTOKEN_MEMBER_DECLS
    FOGTOKEN_LEAF_DECLS
private:
    FogExprRefToConst _test;
    FogRawRef _body;
private:
    This& mutate() const { return *(This *)this; }
protected:
    FogDoWhileStatement(const This& aStatement);
    virtual ~FogDoWhileStatement();
public:
    FogDoWhileStatement(FogRaw& bodyStatement, const FogExpr& testExpr);
    virtual bool compile_parsed(FogParseContext& parseContext);
    virtual size_t executable_tokens() const;
    virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
    virtual void set_is_meta();
};
#endif
 
