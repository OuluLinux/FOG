
#ifndef FOGSPECIFIER_HXX
#define FOGSPECIFIER_HXX






class FogSpecifier : public FogDecoratedName {
	typedef FogDecoratedName Super;
	typedef FogSpecifier This;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	FOGTOKEN_DERIVED_DECLS
	//    FOGTOKEN_MEMBER_DECLS
private:
	FogUtilityHandle _utility;                  //   Status of specifier.
	//      FogLineRef _line;                           //.bugbug occludes FogRaw::_line
	FogDeclSpecifierValue _decl_specifiers;
	FogMetaEntityMapOfRef _meta_entities;       //   Associated meta-entities.
private:
	This& mutate() const { return *(This*)this; }
	virtual void set_new_utility(const FogUtility& aUtility);
protected:
	FogSpecifier();
	FogSpecifier(FogName& aName);
	FogSpecifier(FogName& aName, const FogUtility& aUtility);
	FogSpecifier(const This& thatSpecifier);
	virtual ~FogSpecifier();
	void merge_from(FogMergeContext& mergeContext, const This& aToken);
	const FogMerge& needs_merge_from(FogMergeContext& mergeContext, const This& aToken) const;
public:
	const FogAccess& access() const;
	virtual FogMetaEntity* adopt_meta_entity(FogMetaEntity* metaEntity);
	virtual const FogLazyBool& can_be_concrete(FogScopeContext& inScope) const;
	virtual bool compile_specifier(FogMakeSpecifierContext& makeSpecifierContext);
	virtual void compile_subspace(FogParseContext& parseContext, FogSpecifier& actualSpecifier) const;
	virtual const FogDeclSpecifierValue& decl_specifiers() const;
	virtual const FogLazyBool& derive_at(FogScopeContext& inScope) const;
	This* derived_clone() const { return (This*)clone(); }
	virtual void destroy();
	virtual bool emit(FogEmitContext& emitContext) const;
	virtual FogEntity* find_entity_in(FogScopeContext& inScope, FindStrategy aStrategy) const;
	virtual FogMetaEntity* find_meta_entity(const PrimId& anId);
	virtual bool get_dyadic(FogTokenRef& returnValue, FogScopeContext& inScope,
	                        FogTokenType::TokenType dyadicOp, const FogToken& rightValue) const;
	FogFunctionSpecifier* get_function_specifier(const FogToken& inToken);
	const FogFunctionSpecifier* get_function_specifier(const FogToken& inToken) const
	{ return mutate().get_function_specifier(inToken); }
	FogMetaFunctionSpecifier* get_meta_function_specifier(const FogToken& inToken);
	FogMetaParameterSpecifier* get_meta_parameter_specifier(const FogToken& inToken);
	FogObjectSpecifier* get_object_specifier(const FogToken& inToken);
	FogParameterSpecifier* get_parameter_specifier(const FogToken& inToken);
	FogTemplateParameterSpecifier* get_template_parameter_specifier(const FogToken& inToken);
	const FogTemplateParameterSpecifier* get_template_parameter_specifier(const FogToken& inToken) const
	{ return mutate().get_template_parameter_specifier(inToken); }
	FogTypedefSpecifier* get_typedef_specifier(const FogToken& inToken);
	const FogTypedefSpecifier* get_typedef_specifier(const FogToken& inToken) const
	{ return mutate().get_typedef_specifier(inToken); }
	FogVariableSpecifier* get_variable_specifier(const FogToken& inToken);
	const FogVariableSpecifier* get_variable_specifier(const FogToken& inToken) const
	{ return mutate().get_variable_specifier(inToken); }
	virtual void instantiate();
	virtual FogAnonId* is_anon();
	bool is_explicit_instantiation() const;
	virtual FogFunctionSpecifier* is_function_specifier();
	const FogFunctionSpecifier* is_function_specifier() const { return mutate().is_function_specifier(); }
	virtual FogLinkageSpecifier* is_linkage_specifier();
	const FogLinkageSpecifier* is_linkage_specifier() const { return mutate().is_linkage_specifier(); }
	virtual FogMetaFunctionSpecifier* is_meta_function_specifier();
	const FogMetaFunctionSpecifier* is_meta_function_specifier() const { return mutate().is_meta_function_specifier(); }
	virtual FogMetaObjectSpecifier* is_meta_object_specifier();
	const FogMetaObjectSpecifier* is_meta_object_specifier() const { return mutate().is_meta_object_specifier(); }
	virtual FogMetaParameterSpecifier* is_meta_parameter_specifier();
	const FogMetaParameterSpecifier* is_meta_parameter_specifier() const { return mutate().is_meta_parameter_specifier(); }
	virtual FogMetaVariableSpecifier* is_meta_variable_specifier();
	const FogMetaVariableSpecifier* is_meta_variable_specifier() const { return mutate().is_meta_variable_specifier(); }
	virtual FogObjectSpecifier* is_object_specifier();
	const FogObjectSpecifier* is_object_specifier() const { return mutate().is_object_specifier(); }
	virtual FogParameterSpecifier* is_parameter_specifier();
	const FogParameterSpecifier* is_parameter_specifier() const { return mutate().is_parameter_specifier(); }
	virtual FogScopeSpecifier* is_scope_specifier();
	const FogScopeSpecifier* is_scope_specifier() const { return mutate().is_scope_specifier(); }
	virtual FogSpecifier* is_specifier();
	virtual FogTemplateParameterSpecifier* is_template_parameter_specifier();
	const FogTemplateParameterSpecifier* is_template_parameter_specifier() const { return mutate().is_template_parameter_specifier(); }
	virtual FogTypedefSpecifier* is_typedef_specifier();
	const FogTypedefSpecifier* is_typedef_specifier() const { return mutate().is_typedef_specifier(); }
	virtual FogObjectSpecifier* is_using_specifier();
	const FogObjectSpecifier* is_using_specifier() const { return mutate().is_using_specifier(); }
	virtual FogVariableSpecifier* is_variable_specifier();
	const FogVariableSpecifier* is_variable_specifier() const { return mutate().is_variable_specifier(); }
	//      virtual FogLine& line() const;
	virtual FogEntity* make_entity(FogMakerContext& makerContext) const;
	virtual bool make_specifier(FogMakeSpecifierContext& makeSpecifierContext);
	virtual const FogMetaType& meta_type() const;
	FogName& name() { return Super::name(); }
	const FogName& name() const { return Super::name(); }
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual bool resolve_id(PrimIdHandle& returnId, FogScopeContext& inScope) const;
	virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
	virtual void set_decl_specifier_name(FogNameRef& aName, const FogDeclSpecifier& declSpecifiers);
	void set_decl_specifier(const FogDeclSpecifier& declSpecifier);
	void set_decl_specifiers(const FogDeclSpecifierValue& declSpecifiers);
	//      virtual void set_line(FogLine& aLine);
	void set_max_utility(const FogUtility& aUtility) { if (aUtility > utility()) set_new_utility(aUtility); }
	void set_utility(const FogUtility& aUtility) { if (aUtility != utility()) set_new_utility(aUtility); }
	virtual const FogUtility& utility() const;
};
#endif

