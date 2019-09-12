
#ifndef FOGMETAITERATORENTITY_HXX
#define FOGMETAITERATORENTITY_HXX




class FogMetaIteratorEntity : public FogMetaEntity
{
    typedef FogMetaEntity Super;
    typedef FogMetaIteratorEntity This;
    TYPEDECL_SINGLE(This, Super)
    PRIMREF_DERIVED_DECLS(This)
private:
    FogIterationDomainRefToConst _domain;           //   Iteration domain.
    size_t _index;                                  //   Current iterator position
                                                    //   0 ... _tally-1 in domain
                                                    //   _tally can decrement
                                                    //   _tally+1 ... done
    FogTokenRefToConst _value;
private:
    FogMetaIteratorEntity(const FogMetaIteratorEntity& anIterator, int anIndex);
    This& mutate() const { return *(This *)this; }
    void update();
protected:
    FogMetaIteratorEntity();
    virtual ~FogMetaIteratorEntity();
public:
    FogMetaIteratorEntity(FogToken& inToken, const FogMetaIterator& metaIterator);
    virtual void destroy();
    virtual bool emit(FogEmitContext& emitContext) const;
    virtual bool get_character(PrimIdHandle& returnId, FogScopeContext& inScope);
    virtual bool get_dyadic(FogTokenRef& returnValue, FogScopeContext& inScope,
        FogTokenType::TokenType dyadicOp, const FogToken& rightValue) const;
    virtual bool get_identifier(PrimIdHandle& returnId, FogScopeContext& inScope);
    virtual bool get_monadic(FogTokenRef& returnValue, FogScopeContext& inScope,
        FogTokenType::TokenType monadicOp) const;
    virtual const PrimNumber& get_number(FogScopeContext& inScope) const;
    virtual bool get_string(PrimIdHandle& returnId, FogScopeContext& inScope);
    virtual bool get_number_token(FogTokenRef& returnValue, FogScopeContext& inScope) const;
//    virtual bool get_object(FogTokenRef& returnValue, FogScopeContext& inScope) const;
    virtual FogMetaIteratorEntity *is_meta_iterator_entity();
    bool is_ok() const;
    virtual const FogMetaType& meta_type() const;
    virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_this(std::ostream& s) const;
    virtual std::ostream& print_viz(std::ostream& s) const;
    virtual bool set_value(const FogToken& tokenValue, FogScopeContext& inScope);
    FogToken& token();
//    const FogToken& token() const;
//    virtual FogTokenType::TokenType token_type_enum() const;
};
#endif
 
