
#ifndef FOGOBJECTSPECIFIER_HXX
#define FOGOBJECTSPECIFIER_HXX




class FogObjectSpecifier : public FogSpecifier {    //   name and type decl_specifiers inherited
	typedef FogSpecifier Super;
	typedef FogObjectSpecifier This;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	FOGTOKEN_MEMBER_DECLS
	FOGTOKEN_DERIVED_DECLS
private:
	FogModifierListOfRef _modifiers;                //   Decl modifiers closest to name first.
	FogNameRef _type;                               //   The type, null if ctor/dtor
	FogRawSubspaceRef _subspace;                    //   Object statements body.
	bool _is_nested;                                //   True if nested scope formed part of name.
private:
	This& operator=(const This&);
private:
	This& mutate() const { return *(This*)this; }
protected:
	FogObjectSpecifier();
	FogObjectSpecifier(FogMakeSpecifierContext& makeSpecifierContext, FogName& nameName);
	FogObjectSpecifier(const This& aSpecifier);
	virtual ~FogObjectSpecifier();
	bool is_nested() const { return _is_nested; }
	
public:
	void add(FogModifier& aModifier) { _modifiers.add(aModifier); }
	void add_to_subspace(FogRaw& aRaw);
	void adopt(FogModifier* aModifier) { _modifiers.adopt(aModifier); }
	virtual const FogLazyBool& can_be_concrete(FogScopeContext& inScope) const;
	virtual bool compile_specifier(FogMakeSpecifierContext& makeSpecifierContext);
	virtual void compile_subspace(FogParseContext& parseContext, FogSpecifier& actualSpecifier) const;
	This* derived_clone() const { return (This*)clone(); }
	virtual const FogLazyBool& derive_at(FogScopeContext& inScope) const;
	const FogName* find_scalar_name() const;
	virtual FogName* get_type();
	const FogName* get_type() const { return mutate().get_type(); }
	const FogName* get_type_name() const;
	const FogFunctionModifier* has_function_signature() const;
	virtual void instantiate();
	virtual FogObjectSpecifier* is_object_specifier();
	const FogObjectSpecifier* is_object_specifier() const { return mutate().is_object_specifier(); }
	virtual FogEntity* make_entity(FogMakerContext& makerContext) const;
	virtual FogEntity* make_name_entity(FogMakeEntityContext& makeEntityContext) const;
	virtual bool make_specifier(FogMakeSpecifierContext& makeSpecifierContext);
	virtual FogEntity* make_type_entity(FogMakeEntityContext& makeEntityContext) const;
	virtual const FogMetaType& meta_type() const;
	const FogModifierListOfRef& modifiers() { return _modifiers; }
	const FogModifierListOfRefToConst& modifiers() const { return _modifiers; }
	const FogMerge& needs_header_merge_from(FogMergeContext& mergeContext, const This& aSpecifier) const;
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	//    virtual bool resolve_semantics(FogSemantics& theSemantics, FogScopeContext& inScope) const;
	virtual void set_colon_value(FogExpr& anExpr);
	virtual void set_initializer(const FogExpr& anExpr);
	void set_specifier(FogObjectSpecifier& objectSpecifier);
	void add_subspace(FogRawSubspace& aSubspace);
	void set_type(FogName& typeName);
	void set_used(const FogObjectSpecifier& usedSpecifier);
	void set_using(const FogObjectSpecifier& usingSpecifier);
	const FogRawSubspace& subspace() const { return *_subspace; }
	FogName& type() { return *_type; }
	const FogName& type() const { return *_type; }
};
#endif

