
#ifndef FOGMETASLOT_HXX
#define FOGMETASLOT_HXX





class FogMetaSlot : public FogToken
{
    typedef FogToken Super;
    typedef FogMetaSlot This;
    TYPEDECL_SINGLE(This, Super)
    PRIMREF_DERIVED_DECLS(This)
    FOGTOKEN_ACTUAL_DECLS
private:
    FogToken& _static_meta_scope;           //   Scope in which this slot defined.
    const PrimIdHandle _id;                 //   MetaSlot identity.
    const FogMetaType& _type;               //   Value/Return type of the function.
    const IsExposed _is_exposed;               //   True if _type for a tree.
    const IsStatic _is_static;              //   True if static meta-slot (FogStatic gives a construction order race)
private:
    FogMetaSlot(const This&);
    This& operator=(const This&);
private:
    This& mutate() const { return *(This *)this; }
protected:
    FogMetaSlot();
    FogMetaSlot(FogMakerContext& makerContext, const FogStatic& aStatic, const FogMetaType& metaType, IsExposed isExposed);
    FogMetaSlot(FogToken& staticMetaScope,
        IsStatic isStatic, const FogMetaType& metaType, IsExposed isExposed, const PrimId& anId);
    FogMetaSlot(FogToken& staticMetaScope,
        IsStatic isStatic, const FogMetaType& metaType, IsExposed isExposed, const char *anId);
    virtual ~FogMetaSlot();
    FogToken *get_scope_in(FogScopeContext& inScope) const;
    FogToken& static_meta_scope() { return _static_meta_scope; }
public:
    virtual void find_entities(FogEntityFinder& theFinder);
    virtual bool find_slots(FogMetaSlotFinder& theFinder);
    const PrimId& id() const { return *_id; }
    virtual FogScope& inner_scope();
    IsExposed is_exposed() const { return _is_exposed; }
    virtual const FogMetaArgument *is_meta_argument() const;
    virtual const FogMetaFunction *is_meta_function() const;
    virtual const FogMetaSlot *is_meta_slot() const;
    IsStatic is_static() const { return _is_static; }
    virtual std::ostream& print_long_id(std::ostream& s) const;
    virtual std::ostream& print_this(std::ostream& s) const;
    virtual std::ostream& print_viz(std::ostream& s) const;
    virtual bool put_value(FogScopeContext& putScope, const FogToken& tokenValue, FogScopeContext& getScope) const;
    virtual bool resolve_object(FogTokenRef& returnValue, FogScopeContext& inScope) const;
    const FogToken& static_meta_scope() const { return _static_meta_scope; }
    const FogMetaType& type() const { return _type; }
public:
    static const PrimId& constructor_id();
    static const PrimId& destructor_id();
};
#endif
 
