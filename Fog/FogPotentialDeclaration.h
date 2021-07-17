
#ifndef FOGPOTENTIALDECLARATION_HXX
#define FOGPOTENTIALDECLARATION_HXX




class FogPotentialDeclaration : public FogMetaToken {
	typedef FogMetaToken Super;
	typedef FogPotentialDeclaration This;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	FOGTOKEN_SHAREDLEAF_DECLS
private:
	FogDerivable& _entity;                          //   Parent entity
	FogLazyBoolHandle _is_active;                   //   Disabled if a place-holder for a derived declaration
private:
	FogPotentialDeclaration(const This&);
	This& operator=(const This&);
	This& mutate() const { return *(This*)this; }
	void set_is_active(const FogLazyBool& isActive);
protected:
	inline FogPotentialDeclaration();
	inline FogPotentialDeclaration(FogDerivable& anEntity);
	inline virtual ~FogPotentialDeclaration();
public:
	bool actualize(FogScopeContext& inScope, FogActualDeclaration& actualDeclaration);
	const FogLazyBool& can_be_concrete(FogScopeContext& inScope);
	const FogLazyBool& derive_at(FogScopeContext& inScope) { return is_lazy_active(inScope); }
	FogDerivable& entity() { return _entity; }
	const FogDerivable& entity() const { return _entity; }
	virtual void find_entities(FogEntityFinder& theFinder);
	//      virtual bool find_slots(FogMetaSlotFinder& theFinder);
	virtual bool get_identifier(PrimIdHandle& returnId, FogScopeContext& inScope);
	virtual FogScope& inner_scope();
	const FogScope& inner_scope() const { return mutate().inner_scope(); }
	virtual FogPotentialDeclaration* is_potential_declaration();
	bool is_active(FogScopeContext& inScope);
	bool is_active() const;
	virtual const FogLazyBool& is_boundary() const = 0;
	const FogLazyBool& is_lazy_active() const { return *_is_active; }
	const FogLazyBool& is_lazy_active(FogScopeContext& inScope);
	const FogLazyBool& is_leaf() const;
	virtual const FogLazyBool& is_pure() const = 0;
	virtual bool is_root() const = 0;
	virtual bool is_root(FogScopeContext& inScope) const = 0;
	void make_derivation(FogDeriveContext& inScope, FogDerivable& aDerivable) const;
	//    void make_usage(FogDeriveContext& inScope, FogDerivable& aDerivable) const;
	virtual const FogMetaType& meta_type() const;
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_long_id(std::ostream& s) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual char print_named(std::ostream& s, const PrimId* scopeId, char tailChar) const;
	virtual std::ostream& print_this(std::ostream& s) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	virtual const FogScope& root_scope() const = 0;
	virtual const FogSpecifier& specifier() const = 0;
	virtual FogTargetFile* target_file() const = 0;
	virtual const FogUtility& utility() const = 0;
public:
	static void make_root(FogMakerContext& makerContext, FogDerivable& aDerivable);
};
#endif

