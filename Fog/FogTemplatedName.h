
#ifndef FOGTEMPLATEDNAME_HXX
#define FOGTEMPLATEDNAME_HXX



class FogTemplatedName : public FogDecoratedName {
	NEEDS_A_FRIEND_FOR_GNUC
	typedef FogDecoratedName Super;
	typedef FogTemplatedName This;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_MEMBER_DECLS
	FOGTOKEN_LEAF_DECLS
private:
	//   Defining context
	FogExprRef _exprs;
private:
	//   Context established by make_actual
	FogTemplateArgsRefToConst _template_args;
private:
	FogTemplatedName(const This& aName);
	virtual ~FogTemplatedName();
	FogListOfExpr& exprs() { return (FogListOfExpr&) * _exprs; }
	This& mutate() const { return *(This*)this; }
public:
	FogTemplatedName(FogName& aName, FogListOfExpr& exprList);
	const FogListOfExpr& exprs() const { return (const FogListOfExpr&) * _exprs; }
	virtual FogEntity* find_entity_in(FogScopeContext& inScope, FindStrategy aStrategy) const;
	virtual const FogMetaSlot* find_slot_in(FogScopeContext& scopeContext) const;
	virtual bool get_object(FogTokenRef& tokenValue, FogScopeContext& inScope) const;
	virtual FogTemplatedName* is_templated_name();
	virtual FogEntity* make_entity(FogMakerContext& makerContext) const;
	virtual FogEntity* make_name_entity(FogMakeEntityContext& makeEntityContext) const;
	virtual bool make_specifier(FogMakeSpecifierContext& makeSpecifierContext);
	virtual FogEntity* make_type_entity(FogMakeEntityContext& makeEntityContext) const;
	virtual bool make_type_specifier(FogMakeSpecifierContext& makeSpecifierContext);
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual bool put_value(FogScopeContext& putScope, const FogToken& tokenValue, FogScopeContext& getScope) const;
	virtual bool resolve_id(PrimIdHandle& returnId, FogScopeContext& inScope) const;
	virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
	bool resolve_template(FogEntity& primaryEntity, FogTemplateArgsRefToConst& templateArgs,
	                      FogScopeContext& inScope) const;
	virtual NameSort sort_class() const;
};
#endif

