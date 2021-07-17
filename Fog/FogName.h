
#ifndef FOGNAME_HXX
#define FOGNAME_HXX




class FogName : public FogExpr {
	typedef FogExpr Super;
	typedef FogName This;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	FOGTOKEN_DERIVED_DECLS
private:
	This& operator=(const This& thatName);
private:
	This& mutate() const { return *(This*)this; }
protected:
	FogName() {}
	FogName(const This& thatName) : Super(thatName) {}
	virtual ~FogName() {}
public:
	virtual const FogDeclSpecifierValue& decl_specifiers() const;
	virtual const FogTemplateParameterSpecifierId* deduce_template_parameter_type(FogName& actualTypeName,
	        FogNameRef& deducedType) const;
	This* derived_clone() const { return (This*)clone(); }
	virtual bool emit(FogEmitContext& emitContext) const = 0;
	virtual bool find_meta_entity_in(FogTokenRef& returnValue, FogScopeContext& inScope) const;
	virtual const FogMetaSlot* find_slot_in(FogScopeContext& scopeContext) const;
	virtual bool find_slots_in(FogScopeContext& inScope, FogMetaSlotFinding& theFinding) const;
	virtual FogEntity* get_entity_in(FogScopeContext& inScope, FindStrategy aStrategy) const;
	virtual bool get_meta_entity_in(FogTokenRef& returnValue, FogScopeContext& inScope) const;
	virtual FogName* get_type();
	const FogName* get_type() const { return mutate().get_type(); }
	virtual bool has_at() const;
	virtual bool has_dollar() const;
	virtual FogDeclSpecifier* is_decl_specifier();
	const FogDeclSpecifier* is_decl_specifier() const { return mutate().is_decl_specifier(); }
	virtual FogEntityName* is_entity_name();
	const FogEntityName* is_entity_name() const { return mutate().is_entity_name(); }
	virtual FogName* is_name();
	virtual const PrimId* is_resolved() const;
	virtual FogTemplateParameterSpecifierId* is_template_parameter_specifier_id();
	const FogTemplateParameterSpecifierId* is_template_parameter_specifier_id() const
	{ return mutate().is_template_parameter_specifier_id(); }
	virtual FogTemplatedName* is_templated_name();
	const FogTemplatedName* is_templated_name() const { return mutate().is_templated_name(); }
	virtual FogTypedName* is_typed_name();
	const FogTypedName* is_typed_name() const { return mutate().is_typed_name(); }
	virtual void make_built_in(FogNameRef& aName, const FogBuiltInTypeId& anId);
	virtual FogEntity* make_entity(FogMakerContext& makerContext) const;
	virtual FogEntity* make_name_entity(FogMakeEntityContext& makeEntityContext) const;
	virtual FogEntity* make_type_entity(FogMakeEntityContext& makeEntityContext) const;
	virtual bool make_type_specifier(FogMakeSpecifierContext& makeSpecifierContext);
	virtual bool make_typed_expression(FogExprRef& theExpr, FogName& theType);
	virtual const FogMetaType& meta_type() const;
	virtual std::ostream& print_long_id(std::ostream& s) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	virtual bool put_value(FogScopeContext& putScope, const FogToken& tokenValue, FogScopeContext& getScope) const;
	virtual bool resolve_id(PrimIdHandle& returnId, FogScopeContext& inScope) const;
	virtual bool resolve_slot(FogTokenRef& returnValue, FogScopeContext& scopeContext,
	                          const FogToken& objectValue) const;
	virtual void set_decl_specifier_expression(FogExprRef& anExpr, FogDeclSpecifier& declSpecifiers);
	virtual void set_decl_specifier_name(FogNameRef& aName, const FogDeclSpecifier& declSpecifiers);
	virtual void set_decl_specifier_raw(FogRawRef& aRaw, FogDeclSpecifier& declSpecifiers);
	virtual void set_destructor_expression(FogExprRef& anExpr);
	virtual void set_destructor_name(FogNameRef& aName);
	virtual NameSort sort_class() const;
	virtual const FogTag& tag() const;
public:
	static FogName& make_operator(const char* anId);
	static FogName& make_anon();
};
#endif

