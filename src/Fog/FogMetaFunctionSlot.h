
#ifndef FOGMETAFUNCTIONSLOT_HXX
#define FOGMETAFUNCTIONSLOT_HXX






class FogMetaFunctionSlot : public FogMetaSlot
{
    typedef FogMetaSlot Super;
    typedef FogMetaFunctionSlot This;
    TYPEDECL_SINGLE(FogMetaFunctionSlot, This)
private:
    FogMetaArgumentMapOfRefToConst _formal_map;
    FogMetaArgumentListOfRefToConst _formal_list;
private:
    void add_built_ins();
protected:
    FogMetaFunctionSlot(FogMakerContext& makerContext,
        const FogStatic& aStatic, const FogMetaType& metaType, IsExposed isExposed);
    FogMetaFunctionSlot(FogToken& staticMetaScope,
        IsStatic isStatic, const FogMetaType& metaType, IsExposed isExposed, const char *anId);
    virtual ~FogMetaFunctionSlot();
    void add(const FogMetaArgument& metaArgument);
    void adopt(const FogMetaArgument& metaArgument);
public:
    void add(const FogMetaType& metaType, IsExposed isExposed, const PrimId& aFormal, const FogToken *aToken = 0);
    const FogMetaArgument *find( const PrimId& anId) const { return _formal_map.find(anId); }
    const FogMetaArgument& formal(size_t anIndex) const;
    const FogMetaArgumentListOfRefToConst& formals() const { return _formal_list; }
    virtual bool get_object(FogTokenRef& tokenValue, FogScopeContext& inScope) const;
    virtual const FogMetaFunctionSlot *is_meta_function_slot() const;
    virtual const FogMetaType& meta_type() const;
    virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_viz(std::ostream& s) const;
    virtual bool resolve_function(FogTokenRef& returnValue, FogCallContext& callContext) const = 0;
    virtual bool resolve_object(FogTokenRef& returnValue, FogScopeContext& inScope) const;
};

#define FOGMETAFUNCTION_CLASS(CLASS) \
class CLASS : public FogMetaFunctionSlot \
{ \
    typedef FogMetaFunctionSlot Super; \
    typedef CLASS This; \
    TYPEDECL_SINGLE(This, Super) \
private: \
    CLASS(); \
public: \
    virtual bool resolve_function(FogTokenRef& returnValue, FogCallContext& callContext) const; \
    CLASS(FogToken& staticMetaScope); \
}; \
TYPEINFO_SINGLE(CLASS, Super)

#define FOGMETAFUNCTION_CLASS_METHOD(CLASS, RETURNS, NAME, ENCAPSULATED) \
FOGMETAFUNCTION_CLASS(CLASS) \
CLASS::CLASS(FogToken& staticMetaScope) \
: \
    Super(staticMetaScope, IS_NOT_STATIC, FogMetaType::name2(RETURNS,_type()), ENCAPSULATED, NAME) \
{} \
bool CLASS::resolve_function(FogTokenRef& returnValue, FogCallContext& callContext) const

#endif
 
