
#ifndef FOGFUNCTION_HXX
#define FOGFUNCTION_HXX









class FogFunction : public FogDerivable {
	typedef FogDerivable Super;
	typedef FogFunction This;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
private:
	//   Defining contents.
	FogTemplateManager _templates;                  //   The templates.
private:
	//   Contents defined by do_compile()
	FogInlineHandle _in_line;                       //   Form of inline access.
	FogVirtualHandle _virtual;                      //   Nature of virtual access.
	FogCodeTokensRefToConst _code;                  //   The code to be emitted.
	FogCtorInitListOfRefToConst _ctors;             //   Ctor inits to be emitted.
	int _sort_class;                                //   Classification of function type.
	bool _is_not_implemented;                       //   Set true by FogCode::compile_code.
	bool _has_explicit_implementation_file;         //   Set true by set_implementation_file
	bool _explicit_no_implementation;
	
	FogCtorInitMapOfRefToConst _raw_ctor_inits;
	FogCodeTokensRef _raw_derived_codes[FogSegment::NUM_SEGMENTS];
	FogCodeTokensRef _raw_local_codes[FogSegment::NUM_SEGMENTS];
	
private:
	FogFunction(This& primaryFunction, FogMakeTemplateContext& makeTemplateContext);
	void compile_code(FogCodeTokensRef localCodes[], FogCodeTokensRef derivedCodes[]);
	void compile_ctors(FogCtorInitMapOfRefToConst& ctorInits, bool isCopy);
	void compile_friends();
	void compile_inline();
	void compile_missing();
	void compile_sort_class(FogScopeContext& scopeContext);
	void compile_virtual(FogCompileContext& inScope);
	void emit_partial_interface(FogEmitContext& emitContext) const;
	size_t executable_tokens() const;
	bool has_distinct_primary() const { return _templates.has_distinct_primary(); }
	bool has_implementation() const;
	bool is_primary() const { return _templates.is_primary(); }
	bool is_specialisation() const { return _templates.is_specialisation(); }
	virtual FogUsage* make_implementation_usage();
	virtual FogUsage* make_interface_usage_finish();
	This& primary_specialisation() { return (This&)_templates.primary(); }
	const This& primary_specialisation() const { return (const This&)_templates.primary(); }
	
protected:
	FogFunction();
	virtual ~FogFunction();
	
public:
	FogFunction(FogMakerContext& makerContext);
	virtual void adopt(FogScopeContext& inScope, FogPotentialDeclaration& potentialDeclaration);
	const FogLazyBool& can_be_concrete(FogScopeContext& inScope);
	virtual void create_usages();
	FogFunction* deduce_specialisation(FogMakeSpecifierContext& makerContext);
	virtual bool do_compile(FogCompileContext& inScope);
	virtual bool do_derive(FogDeriveContext& inScope);
	virtual void emit_friend_interface(FogStream& s);
	virtual void emit_implementation(FogStream& s);
	virtual void emit_interface(FogStream& s);
	virtual void emit_template_prefix(FogEmitContext& emitContext) const;
	void emit_template_suffix(FogEmitContext& emitContext) const { _templates.emit_suffix(emitContext); }
	FogFunction* find_specialisation(FogMakerContext& makerContext);
	virtual FogEntity* find_template(FogMakeTemplateContext& makeTemplateContext);
	const FogInline& get_inline() const;
	const FogFunctionSpecifier* get_specifier() const;
	const FogVirtual& get_virtual() const;
	virtual FogTargetFile* implementation_file();
	const FogTargetFile* implementation_file() const { return Super::implementation_file(); }
	virtual void install_names();
	virtual FogEntity* instantiate(FogScopeContext& inScope, InstantiateOption ifRequired);
	virtual FogUsage& interface_usage_finish();
	virtual FogUsage& interface_usage_start();
	bool is_concrete() const { return !get_virtual().is_pure_virtual(); }
	virtual bool is_instantiation() const;
	virtual bool is_templated() const;
	virtual FogFunction* is_function();
	virtual const FogMetaType& meta_type() const;
	virtual FogEntity* new_template(FogMakeTemplateContext& makeTemplateContext);
	const FogExprListOfRef& parameters();
	const FogExprListOfRefToConst& parameters() const;
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_this(std::ostream& s) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	FogCtorInitMapOfRefToConst& raw_ctor_inits() { return _raw_ctor_inits; }
	FogCodeTokensRef* raw_derived_codes() { return _raw_derived_codes; }
	FogCodeTokensRef* raw_local_codes() { return _raw_local_codes; }
	virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
	virtual void set_implementation_file(FogTargetFile* aFile);
	virtual void set_is_pure(const FogLazyBool& isPure);
	void set_not_implemented() { _is_not_implemented = true; }
	const FogModifier& signature() const;
	virtual int sort_class() const;
	//    const FogFunctionSpecifier& specifier() const;
	const FogTemplateArgs& template_arguments() const { return _templates.template_arguments(); }
	virtual FogTemplateManager& template_manager();
	virtual const FogTemplateParameterSpecifiers& template_parameters() const;
	virtual bool validate() const;
	
	void set_explicit_no_implementation() {_explicit_no_implementation = true;}
	bool is_explicit_no_implementation() {return _explicit_no_implementation;}
	
public:
	static int compare(const FogFunctionRefToConst* p1, const FogFunctionRefToConst* p2);
};
#endif

