
#ifndef FOGCASESTATEMENT_HXX
#define FOGCASESTATEMENT_HXX




class FogCaseStatement : public FogStatement
{
    typedef FogStatement Super;
    typedef FogCaseStatement This;
    TYPEDECL_SINGLE(This, Super)
    FOGTOKEN_MEMBER_DECLS
    FOGTOKEN_LEAF_DECLS
private:
    FogExprRef _case;               //   nil for default case
    FogRawRef _statement;
private:
    This& mutate() const { return *(This *)this; }
protected:
    FogCaseStatement(const This& aStatement);
    virtual ~FogCaseStatement();
public:
    FogCaseStatement(FogRaw& aStatement);
    FogCaseStatement(FogExpr& anExpr, FogRaw& aStatement);
    virtual size_t executable_tokens() const;
    virtual FogRaw *is_case(FogScopeContext& scopeContext, const FogToken& theCase);
    virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
};
#endif
 
