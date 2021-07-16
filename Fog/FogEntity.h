
#ifndef FOGENTITY_HXX
#define FOGENTITY_HXX









class FogEntity : public FogUnique {
	typedef FogUnique Super;
	typedef FogEntity This;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	
public:
	enum InstantiateOption { IF_REQUIRED, FORCED };
	
private:
	FogEntity(const FogEntity&);
	FogEntity& operator=(const FogEntity&);
	
private:
        //   Defining contents.
	FogRoot& _global_scope;     //   The root, global, unnamed namespace.
	FogScope& _scope;      //   Parent scope, *this at root.
	FogMetaSlotMapOfRefToConst _slots;  //   Miscellaneous slots.
	FogLazyBoolHandle _is_boundary;         //   True for abstract/concrete class, pure-virtual function boundary
	FogLazyBoolHandle _is_pure;             //   True for abstract class, pure-virtual function
	
private:
        //   Compiled slots.

	bool _done_compile : 1;     //   True once do_compile() invoked.
	
	bool _done_derive : 1;     //   True once do_derive() invoked.
	
private:

	friend struct PrimMapOfRefToConstFunctions<FogEntity>;
	
	friend struct PrimMapOfRefToConstFunctions<FogEnum>;
	
	friend struct PrimMapOfRefToConstFunctions<FogEnumerator>;
	
	friend struct PrimMapOfRefToConstFunctions<FogFunction>;
	
	friend struct PrimMapOfRefToConstFunctions<FogMetaEntity>;
	
	friend struct PrimMapOfRefToConstFunctions<FogScope>;
	
	friend struct PrimMapOfRefToConstFunctions<FogTypedef>;
	
	friend struct PrimMapOfRefToConstFunctions<FogTypeName>;
	
	friend struct PrimMapOfRefToConstFunctions<FogVariable>;
	const PrimId& id() const {
		return local_signature_id();
	}
	
private:
	This& mutate() const {
		return *(This *)this;
	}
	
protected:
	FogEntity();
	FogEntity(FogScope& aScope, const PrimId& shortId, const PrimId& longId);
	FogEntity(FogMakerContext& makerContext, const FogLazyBool& isPure = FogLazyBool::make_false());
	FogEntity(FogMakeTemplateContext& makeTemplateContext, const FogLazyBool& isPure = FogLazyBool::make_false());
	virtual ~FogEntity();
//  	virtual void install_slots();
	FogScope& non_virtual_scope() {
		return _scope;
	}
	
	const FogScope& non_virtual_scope() const {
		return _scope;
	}
	
public:
	virtual const FogAccess& access() const;
	virtual void add_type(FogEntity& aType);
	virtual void add_use_by(FogEntity& anEntity, const FogOfUseBy& ofUseBy);
	void add_use_by(FogUsageContext& usageContext);
	virtual const FogMetaSlot *adopt_slot(const FogMetaSlot *aSlot);
	bool compiled() const {
		return _done_compile;
	}
	
	virtual const FogInline& default_inline() const;
	virtual void destroy();
	virtual bool do_compile(FogCompileContext& inScope);
	virtual bool do_derive(FogDeriveContext& inScope);
	bool done_compile() const {
		return _done_compile;
	}
	
	bool done_derive() const {
		return _done_derive;
	}
	
	virtual bool emit(FogEmitContext& emitContext) const;
	virtual void emit_forward_reference(FogStream& s);
	virtual void emit_friend_interface(FogStream& s);
	virtual void emit_template_prefix(FogEmitContext& emitContext) const;
	virtual void find_entities_in(FogScopeContext& inScope, FogEntityFinding& theFinding) const;
	virtual const FogMetaSlot *find_local_slot(const PrimId& anId) const;
	virtual FogEntity *find_template(FogMakeTemplateContext& makeTemplateContext);
	virtual bool get_character(PrimIdHandle& returnId, FogScopeContext& inScope);
	virtual bool get_dyadic(FogTokenRef& returnValue, FogScopeContext& inScope,
			FogTokenType::TokenType dyadicOp, const FogToken& rightValue) const;
	virtual bool get_identifier(PrimIdHandle& returnId, FogScopeContext& inScope);
	virtual bool get_string(PrimIdHandle& returnId, FogScopeContext& inScope);
	virtual FogRoot& global_scope();
	const FogRoot& global_scope() const {
		return mutate().global_scope();
	}
	
	virtual FogTargetFile *implementation_file();
	const FogTargetFile *implementation_file() const {
		return mutate().implementation_file();
	}
	
	virtual FogScope& inner_scope();
	const FogScope& inner_scope() const {
		return mutate().inner_scope();
	}
	
	virtual FogEntity *instantiate(FogScopeContext& inScope, InstantiateOption ifRequired);
	virtual void instantiate_into(FogEntity& instantiatingEntity);
	virtual FogTargetFile *interface_file_sink();
	virtual FogTargetFile *interface_file_source();
	const FogTargetFile *interface_file_source() const {
		return mutate().interface_file_source();
	}
	
	virtual bool is_active() const;
	const FogLazyBool& is_boundary() const;
	virtual FogEntity *is_entity();
	virtual bool is_instantiation() const;
	virtual bool is_name_scope() const;
	virtual bool is_name_space() const;
	virtual bool is_global_scope() const;
	const FogLazyBool& is_pure() const {
		return *_is_pure;
	}
	
	virtual bool is_read_only() const;
	virtual bool is_templated() const;
	virtual bool is_type() const;
	virtual bool make_specifier(FogMakeSpecifierContext& makeSpecifierContext);
	virtual const FogMetaType& meta_type() const;
	virtual FogTargetFile *name_file();
	const FogTargetFile *name_file() const {
		return Super::name_file();
	}
	
	virtual FogScope& name_emit_scope();
	const FogScope& name_emit_scope() const {
		return mutate().name_emit_scope();
	}
	
	virtual FogScope& name_emit_transient_scope();
	const FogScope& name_emit_transient_scope() const {
		return mutate().name_emit_transient_scope();
	}
	
	virtual FogScope& name_scope();
	const FogScope& name_scope() const {
		return mutate().name_scope();
	}
	
	virtual FogScope& name_space();
	const FogScope& name_space() const {
		return mutate().name_space();
	}
	
	virtual FogEntity *new_template(FogMakeTemplateContext& makeTemplateContext);
	FogScope *outer_scope() {
		return this != (FogEntity *)&_global_scope ? &_scope : 0;
	}
	
	const FogScope *outer_scope() const {
		return this != (const FogEntity *)&_global_scope ? &_scope : 0;
	}
	
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual char print_named(std::ostream& s, const PrimId *scopeId, char tailChar) const;
	virtual FogReader& reader();
	const FogReader& reader() const {
		return mutate().reader();
	}
	
	virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
	virtual void set_access(const FogAccess& anAccess);
	virtual void set_implementation(FogCode& someCode);
	virtual void set_initializer(const FogExpr& anExpr);
	virtual void set_interface(FogCode& someCode);
	virtual void set_is_boundary(const FogLazyBool& isBoundary);
	virtual void set_is_pure(const FogLazyBool& isPure);
	virtual void set_using(const FogEntity& anEntity);
	virtual FogTemplateManager& template_manager();
	const FogTemplateManager& template_manager() const {
		return mutate().template_manager();
	}
	
	virtual const FogTemplateParameterSpecifiers& template_parameters() const;
	FogUsageManager& usage_manager();
	virtual bool validate() const;
};

#endif

