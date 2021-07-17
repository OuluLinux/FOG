
#ifndef FOGWORDNAME_HXX
#define FOGWORDNAME_HXX



class FogWordName : public FogEntityName {
	typedef FogEntityName Super;
	typedef FogWordName This;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	FOGTOKEN_DERIVED_DECLS
private:
	This& operator=(const This& thatName);
private:
	This& mutate() const { return *(This*)this; }
protected:
	FogWordName() {}
	FogWordName(const This& thatName) : Super(thatName) {}
	virtual ~FogWordName() {}
	virtual void make_actual_token(FogTokenRefToConst& actualToken, FogMakeActualContext& makeActualContext) const;
	bool resolve_identifier_semantics(FogSemanticsContext& theSemantics) const;
public:
	virtual void create_usage(FogUsageContext& usageContext) const;
	virtual bool emit(FogEmitContext& emitContext) const;
	virtual const FogMetaArgument* find_formal_in(FogScopeContext& scopeContext) const;
	virtual const FogMetaSlot* find_slot_in(FogScopeContext& scopeContext) const;
	virtual bool find_slots_in(FogScopeContext& inScope, FogMetaSlotFinding& theFinding) const;
	virtual bool get_character(PrimIdHandle& returnId, FogScopeContext& inScope);
	virtual bool get_identifier(PrimIdHandle& returnId, FogScopeContext& inScope);
	virtual bool get_number_token(FogTokenRef& returnValue, FogScopeContext& inScope) const;
	virtual bool get_object(FogTokenRef& returnValue, FogScopeContext& inScope) const;
	virtual bool get_string(PrimIdHandle& returnId, FogScopeContext& inScope);
	virtual bool is_actual(const FogScopeContext& scopeContext) const;
	virtual const FogTemplateParameterSpecifier* is_unnormalised_template_parameter
	(FogScopeContext& scopeContext) const;
	virtual void make_built_in(FogNameRef& aName, const FogBuiltInTypeId& anId);
	virtual FogEntity* make_type_entity(FogMakeEntityContext& makeEntityContext) const;
	virtual bool make_type_specifier(FogMakeSpecifierContext& makeSpecifierContext);
	virtual bool put_value(FogScopeContext& putScope, const FogToken& tokenValue, FogScopeContext& getScope) const;
	virtual void set_destructor_name(FogNameRef& aName);
protected:
	static bool is_valid(const PrimId& anId);
};
#endif

