
#ifndef FOGGLOBALNAME_HXX
#define FOGGLOBALNAME_HXX



class FogGlobalName : public FogDecoratedName {
	NEEDS_A_FRIEND_FOR_GNUC
	typedef FogDecoratedName Super;
	typedef FogGlobalName This;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_MEMBER_DECLS
	FOGTOKEN_LEAF_DECLS
private:
	const IsTemplate _is_template;
private:
	FogGlobalName(const This& aName);
	virtual ~FogGlobalName();
public:
	FogGlobalName(IsTemplate  isTemplate, FogName& childName);
	virtual FogEntity* find_entity_in(FogScopeContext& inScope, FindStrategy aStrategy) const;
	virtual const FogMetaSlot* find_slot_in(FogScopeContext& scopeContext) const;
	virtual bool get_object(FogTokenRef& returnValue, FogScopeContext& inScope) const;
	virtual FogEntity* make_entity(FogMakerContext& makerContext) const;
	virtual FogEntity* make_name_entity(FogMakeEntityContext& makeEntityContext) const;
	virtual bool make_specifier(FogMakeSpecifierContext& makeSpecifierContext);
	virtual FogEntity* make_type_entity(FogMakeEntityContext& makeEntityContext) const;
	virtual bool make_type_specifier(FogMakeSpecifierContext& makeSpecifierContext);
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual bool put_value(FogScopeContext& putScope, const FogToken& tokenValue, FogScopeContext& getScope) const;
	virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
};
#endif

