
#ifndef FOGMETAVARIABLEENTITY_HXX
#define FOGMETAVARIABLEENTITY_HXX




class FogMetaVariableEntity : public FogMetaEntity
{
    typedef FogMetaEntity Super;
    typedef FogMetaVariableEntity This;
    TYPEDECL_SINGLE(This, Super)
private:
    FogTokenRef _value;
private:
    FogMetaVariableEntity(const This&);
    This& operator=(const This&);
    This& mutate() const { return *(This *)this; }
protected:
    virtual ~FogMetaVariableEntity();
public:
    FogMetaVariableEntity(FogToken& inToken, const FogMetaVariableSlot& metaObject);
    virtual void destroy();
    virtual bool emit(FogEmitContext& emitContext) const;
    virtual bool get_monadic(FogTokenRef& returnValue, FogScopeContext& inScope,
        FogTokenType::TokenType monadicOp) const;
    virtual bool get_object(FogTokenRef& returnValue, FogScopeContext& inScope) const;
    virtual FogMetaVariableEntity *is_meta_variable_entity();
    virtual const FogMetaType& meta_type() const;
    virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_viz(std::ostream& s) const;
    virtual bool set_value(const FogToken& tokenValue, FogScopeContext& inScope);
};
#endif
 
