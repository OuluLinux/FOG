
#ifndef FOGITERATOREXPR_HXX
#define FOGITERATOREXPR_HXX




class FogIteratorExpr : public FogExpr
{
    typedef FogExpr Super;
    typedef FogIteratorExpr This;
    TYPEDECL_SINGLE(This, Super)
    FOGTOKEN_LEAF_DECLS
private:
    FogMetaIteratorEntityRefToConst _iterator;
    FogIterationDomainRefToConst _tokens;
private:
    This& mutate() const { return *(This *)this; }
protected:
    FogIteratorExpr(const This& anExpr);
    virtual ~FogIteratorExpr();
public:
    FogIteratorExpr(const FogIterationDomain& tokenList);
    FogIteratorExpr(const FogMetaIteratorEntity& anIterator);
    virtual bool emit(FogEmitContext& emitContext) const;
    virtual const PrimNumber& get_number(FogScopeContext& inScope) const;
    virtual bool get_number_token(FogTokenRef& returnValue, FogScopeContext&) const;
    virtual bool get_object(FogTokenRef& tokenValue, FogScopeContext& inScope) const;
    virtual const FogMetaType& meta_type() const;
    virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
    virtual char print_named(std::ostream& s, const PrimId *fullId, char tailChar) const;
};
#endif
 
