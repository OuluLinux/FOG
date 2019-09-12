
#ifndef FOGIFSTATEMENT_HXX
#define FOGIFSTATEMENT_HXX




class FogIfStatement : public FogStatement
{
    typedef FogStatement Super;
    typedef FogIfStatement This;
    TYPEDECL_SINGLE(This, Super)
    FOGTOKEN_MEMBER_DECLS
    FOGTOKEN_LEAF_DECLS
private:
    FogExprRefToConst _test;
    FogRawRef _true;
    FogRawRef _false;
private:
    This& mutate() const { return *(This *)this; }
protected:
    FogIfStatement(const This& aStatement);
    virtual ~FogIfStatement();
public:
    FogIfStatement(FogExpr& testExpr, FogRaw& trueStatement, FogRaw *falseStatement);
    virtual bool compile_parsed(FogParseContext& parseContext);
    virtual size_t executable_tokens() const;
    virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
    virtual void set_is_meta();
};
#endif
 
