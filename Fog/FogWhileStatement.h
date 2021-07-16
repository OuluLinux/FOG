
#ifndef FOGWHILESTATEMENT_HXX
#define FOGWHILESTATEMENT_HXX




class FogWhileStatement : public FogStatement
{
    typedef FogStatement Super;
    typedef FogWhileStatement This;
    TYPEDECL_SINGLE(This, Super)
    FOGTOKEN_MEMBER_DECLS
    FOGTOKEN_LEAF_DECLS
private:
    FogExprRefToConst _test;
    FogRawRef _body;
private:
    This& mutate() const { return *(This *)this; }
protected:
    FogWhileStatement(const This& aStatement);
    virtual ~FogWhileStatement();
public:
    FogWhileStatement(const FogExpr& testExpr, FogRaw& bodyStatement);
    virtual bool compile_parsed(FogParseContext& parseContext);
    virtual size_t executable_tokens() const;
    virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
    virtual void set_is_meta();
};
#endif
 
