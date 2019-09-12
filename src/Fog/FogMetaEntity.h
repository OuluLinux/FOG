
#ifndef FOGMETAENTITY_HXX
#define FOGMETAENTITY_HXX



class FogMetaEntity : public FogMetaToken
{
    typedef FogMetaToken Super;
    typedef FogMetaEntity This;
    TYPEDECL_SINGLE(This, Super)
    PRIMREF_DERIVED_DECLS(This)
    FOGTOKEN_ACTUAL_DECLS
private:
    FogTokenRef _token;
    FogMetaSlotRefToConst _slot;
private:
    FogMetaEntity(const This&);
    This& operator=(const This&);
protected:
    FogMetaEntity();
    FogMetaEntity(FogToken& inToken, const FogMetaSlot& metaSlot);
    virtual ~FogMetaEntity();
    const FogMetaSlot& slot() const { return *_slot; }
public:
    virtual void destroy();
    const PrimId& id() const;
    virtual bool get_dyadic(FogTokenRef& returnValue, FogScopeContext& inScope,
        FogTokenType::TokenType dyadicOp, const FogToken& rightValue) const;
    virtual bool get_monadic(FogTokenRef& returnValue, FogScopeContext& inScope,
        FogTokenType::TokenType monadicOp) const;
    IsExposed is_exposed() const;
    virtual FogMetaEntity *is_meta_entity();
    virtual bool make_specifier(FogMakeSpecifierContext& makeSpecifierContext);
    virtual std::ostream& print_long_id(std::ostream& s) const;
    virtual char print_named(std::ostream& s, const PrimId *scopeId, char tailChar) const;
    virtual std::ostream& print_this(std::ostream& s) const;
    virtual std::ostream& print_viz(std::ostream& s) const;
    virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
    virtual bool set_value(const FogToken& tokenValue, FogScopeContext& inScope) = 0;
    FogToken& token() { return *_token; }
    const FogToken& token() const { return *_token; }
    const FogMetaType& type() const;
};
#endif
 
