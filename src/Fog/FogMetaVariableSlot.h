
#ifndef FOGMETAVARIABLESLOT_HXX
#define FOGMETAVARIABLESLOT_HXX




class FogMetaVariableSlot : public FogMetaSlot
{
    typedef FogMetaSlot Super;
    typedef FogMetaVariableSlot This;
    TYPEDECL_SINGLE(This, Super)
    PRIMREF_DERIVED_DECLS(This)
    
private:
    FogTokenRefToConst _value;                  //   Initial or default value.
private:
    FogMetaVariableSlot(const This&);
    This& operator=(const This&);
private:
    This& mutate() const { return *(This *)this; }
private:
    virtual FogMetaEntity *new_meta_entity(FogToken& inToken) const;
protected:
    FogMetaVariableSlot();
    FogMetaVariableSlot(FogToken& staticMetaScope,
        const FogMetaType& metaType, IsExposed isExposed, const char *anId, const FogToken *aToken = 0);
    FogMetaVariableSlot(FogToken& staticMetaScope,
        const FogMetaType& metaType, IsExposed isExposed, const PrimId& anId, const FogToken *aToken = 0);
    FogMetaVariableSlot(FogMakerContext& makerContext,
        const FogStatic& aStatic, const FogMetaType& metaType, IsExposed isExposed, const FogToken& aToken);
    virtual ~FogMetaVariableSlot();
public:
    virtual bool emit(FogEmitContext& emitContext) const;
    FogMetaEntity *get_meta_entity(FogScopeContext& inScope) const;
    virtual bool get_object(FogTokenRef& returnValue, FogScopeContext& inScope) const;
    virtual const FogMetaVariableSlot *is_meta_variable_slot() const;
    virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_viz(std::ostream& s) const;
    virtual bool put_value(FogScopeContext& putScope, const FogToken& tokenValue, FogScopeContext& getScope) const;
    virtual bool resolve_object(FogTokenRef& returnValue, FogScopeContext& inScope) const;
    const FogToken& token() const { return *_value; }
};

#define FOGMETAVARIABLE_CLASS_METHOD(CLASS, RETURNS, NAME, ENCAPSULATED) \
class CLASS : public FogMetaVariableSlot \
{ \
    typedef FogMetaVariableSlot Super; \
    typedef CLASS This; \
    TYPEDECL_SINGLE(This, Super) \
private: \
    CLASS(); \
public: \
    virtual bool get_object(FogTokenRef& returnValue, FogScopeContext& inScope) const \
        { return resolve_object(returnValue, inScope); } \
    virtual bool resolve_object(FogTokenRef& returnValue, FogScopeContext& inScope) const; \
    CLASS(FogToken& staticMetaScope); \
}; \
TYPEINFO_SINGLE(CLASS, Super) \
CLASS::CLASS(FogToken& staticMetaScope) \
: \
    Super(staticMetaScope, FogMetaType::name2(RETURNS,_type()), ENCAPSULATED, NAME) \
{} \
bool CLASS::resolve_object(FogTokenRef& returnValue, FogScopeContext& inScope) const

#endif
 
