
#ifndef FOGNESTEDNAME_HXX
#define FOGNESTEDNAME_HXX



class FogNestedName : public FogDecoratedName
{
 NEEDS_A_FRIEND_FOR_GNUC
    typedef FogDecoratedName Super;
    typedef FogNestedName This;
    TYPEDECL_SINGLE(This, Super)
    FOGTOKEN_MEMBER_DECLS
    FOGTOKEN_LEAF_DECLS
private:
    FogNameRef _scope;                  //   The name nesting.
private:
    FogNestedName(const This& aName);
    virtual ~FogNestedName();
    FogToken *find_scope(FogScopeContext& inScope) const;
public:
    FogNestedName(FogName& scopeName, FogName& childName);
    virtual FogEntity *find_entity_in(FogScopeContext& inScope, FindStrategy aStrategy) const;
    virtual const FogMetaSlot *find_slot_in(FogScopeContext& scopeContext) const;
    virtual bool get_object(FogTokenRef& returnValue, FogScopeContext& inScope) const;
    virtual FogEntity *make_entity(FogMakerContext& makerContext) const;
    virtual FogEntity *make_name_entity(FogMakeEntityContext& makeEntityContext) const;
    virtual bool make_specifier(FogMakeSpecifierContext& makeSpecifierContext);
    virtual bool make_type_specifier(FogMakeSpecifierContext& makeSpecifierContext);
    virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
    virtual bool put_value(FogScopeContext& putScope, const FogToken& tokenValue, FogScopeContext& getScope) const;
    virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
};
#endif
 
