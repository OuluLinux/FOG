
#ifndef FOGSCOPE_HXX
#define FOGSCOPE_HXX




class FogScope : public FogEntity {

	friend class FogScope_Functions;            //   To access find_local_name
	typedef FogEntity Super;
	typedef FogScope This;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	
private:
	//   Defining contents.
	FogScope_Bases _bases;                          //   Base classes.
	FogScope_Derivations _derivations;              //   Derived classes.
	FogScope_Enums _enums;                          //   Member enumerations.
	FogScope_Friends _friends;                      //   Friends
	FogScope_Functions _functions;                  //   Member functions.
	FogScope_Implementation _implementation;        //   Implementation of scope.
	FogScope_Interface _interface;                  //   Interface of scope.
	FogScope_Names _names;                          //   Potentially visibles non-type names.
	FogScope_Nested _nested;                        //   Nested classes.
	FogScope_Typedefs _typedefs;                    //   Member typedefs.
	FogScope_Types _types;                          //   Potentially visible type names.
	FogScope_Using _usings;                         //   Using declarations.
	FogScope_Variables _variables;                  //   Member variables.
	FogTagHandle _tag;                              //   Scope type.
	FogAccessHandle _access;                        //   Accessibility within scope.
	
bool _braced :
	1;                               //   Set true if ever declared with {}
	
private:
	//   Compiled attributes.
	FogLazyBoolHandle _is_leaf;                     //   True until a derived class defined.
	
bool _is_frozen :
	1;                            //   True once class structure immutable.
	
bool _done_meta_construct :
	1;                  //   True once do_meta_construct() invoked.
	FogUsage* _inline_usage;                        //   Dummy usage node between used files and inline functions.
	
private:
	//   Defining contents (last to maximise validity of *this during construction)
	FogTemplateManager _templates;                  //   The templates.
	
private:
	bool find_local_entity(FogEntityFinder& theFinder);
	bool find_local_name(FogEntityFinder& theFinder);
	bool find_local_slot(FogMetaSlotFinder& theFinder);
	bool find_local_type(FogEntityFinder& theFinder);
	bool find_scope_entity(FogEntityFinder& theFinder);
	bool find_scope_slot(FogMetaSlotFinder& theFinder);
	bool has_distinct_primary() const {
		return _templates.has_distinct_primary();
	}
	
	bool is_primary() const {
		return _templates.is_primary();
	}
	
	virtual FogUsage* make_implementation_usage();
	void meta_execute(FogScopeContext& inScope, const PrimId& anId);
	This& mutate() const {
		return *(This*)this;
	}
	
	bool needs_emission() const;
	
protected:
	FogScope();
	FogScope(const FogTag& aTag, FogMakerContext& makerContext);
	FogScope(const FogTag& aTag, FogScope& aScope, const PrimId& anId);
	FogScope(FogScope& primaryScope, FogMakeTemplateContext& makeTemplateContext);
	virtual ~FogScope();
	void emit_base_interface(FogEmitContext& emitContext) const;
	void install_types();
	
public:
	virtual const FogAccess& access() const;
	void add_base(FogBaseClass& baseClass);
	bool add_derivation(FogScope& aScope) {
		return _derivations.add_derivation(aScope);
	}
	
	virtual void add_file_space(FogScope& fileSpace);
	void add_friend(FogEntity& anEntity) {
		_friends.add(*this, anEntity);
	}
	
	void add_function(FogFunction& aFunction, FogPotentialDeclaration* potentialDeclaration);
	void add_name(FogEntity& anEntity) {
		add_name(anEntity.short_id(), anEntity);
	}
	
	void add_name(const PrimId& typeId, FogEntity& anEntity);
	FogScope* add_scope(FogScope& aScope);
	virtual void add_type(FogEntity& anEntity);
	void add_type(const PrimId& typeName, FogEntity& typeValue);
	void add_type(FogEntity& typeName, FogEntity& typeValue);
	void add_typedef(FogTypedef& aTypedef, FogPotentialDeclaration* potentialDeclaration);
	void add_use_for_implementation(FogTargetFile& aFile) {
		_implementation.add_use_for_implementation(aFile);
	}
	
	void add_use_for_interface(FogTargetFile& aFile) {
		_interface.add_use_for_interface(aFile);
	}
	
	void add_using(FogUsing& aUsing);
	void add_variable(FogVariable& aVariable, FogPotentialDeclaration* potentialDeclaration);
	FogScopeListOfRef& all_bases() {
		return _bases.all_bases();
	}
	
	const FogScopeListOfRefToConst& all_bases() const {
		return _bases.all_bases();
	}
	
	void changed_base_class(FogBaseClass& baseClass);
	virtual void create_usages();
	FogScopeListOfRef& ctor_bases() {
		return _bases.ctor_bases();
	}
	
	const FogScopeListOfRefToConst& ctor_bases() const {
		return _bases.ctor_bases();
	}
	
	virtual const FogAccess& default_access() const;
	const FogExpr* default_initialiser(bool isCopy) const;
	void derive_from(FogBaseClass& baseClass);
	void derive_from(FogFunction& baseFunction, FogPotentialDeclaration* potentialDeclaration);
	void derive_from(FogTypedef& baseTypedef, FogPotentialDeclaration* potentialDeclaration);
	void derive_from(FogVariable& baseVariable, FogPotentialDeclaration* potentialDeclaration);
	FogBaseClassListOfRef& direct_bases() {
		return _bases.direct_bases();
	}
	
	const FogBaseClassListOfRefToConst& direct_bases() const {
		return _bases.direct_bases();
	}
	
	FogFunctionListOfRef& distinct_function_list() {
		return _functions.distinct_function_list();
	}
	
	const FogFunctionListOfRefToConst& distinct_function_list() const {
		return _functions.distinct_function_list();
	}
	
	FogTypedefListOfRef& distinct_typedef_list() {
		return _typedefs.distinct_typedef_list();
	}
	
	const FogTypedefListOfRefToConst& distinct_typedef_list() const {
		return _typedefs.distinct_typedef_list();
	}
	
	FogVariableListOfRef& distinct_variable_list() {
		return _variables.distinct_variable_list();
	}
	
	const FogVariableListOfRefToConst& distinct_variable_list() const {
		return _variables.distinct_variable_list();
	}
	
	virtual bool do_compile(FogCompileContext& inScope);
	virtual bool do_derive(FogDeriveContext& inScope);
	bool do_meta_construct(FogScopeContext& inScope);
	virtual void destroy();
	bool done_meta_construct() const {
		return _done_meta_construct;
	}
	
	virtual void emit_forward_reference(FogStream& s);
	virtual void emit_friend_interface(FogStream& s);
	virtual void emit_implementation(FogStream& s);
	virtual void emit_interface(FogStream& s);
	virtual void emit_scope(FogEmitContext& emitContext, const FogScope& inScope) const;
	virtual void emit_scope_head(FogStream& s) const;
	virtual void emit_scope_tail(FogStream& s) const;
	virtual void emit_template_prefix(FogEmitContext& emitContext) const;
	
	void emit_scope_namespace(FogEmitContext& emitContext, const FogScope& inScope) const;
	
	void emit_template_suffix(FogEmitContext& emitContext) const {
		_templates.emit_suffix(emitContext);
	}
	
	const FogEnumListOfRef& enums() {
		return _enums.enums();
	}
	
	const FogScope* find_bases(PrimIdMap& visitMap,
	                           FogScopeListOfRef& nonVirtualBases, FogScopeListOfRef& virtualBases) const {
		return _bases.find_bases(*this, visitMap, nonVirtualBases, virtualBases);
	}
	
	virtual void find_entities(FogEntityFinder& theFinder);
	virtual void find_entities_in(FogScopeContext& inScope, FogEntityFinding& theFinding) const;
	virtual FogEntity* find_entity(const PrimId& anId, InScope inScope, FindStrategy aStrategy);
	FogEntity* find_entity(const PrimId& anId, InScope inScope) {
		return find_entity(anId, inScope, FIND_ENTITY);
	}
	
	const FogEntity* find_entity(const PrimId& anId, InScope inScope) const {
		return mutate().find_entity(anId, inScope);
	}
	
	FogFunction* find_local_function(const FogFunction& aFunction) {
		return _functions.find_local_function(aFunction);
	}
	
	const FogFunction* find_local_function(const FogFunction& aFunction) const {
		return mutate().find_local_function(aFunction);
	}
	
	const FogEntityListOfRefToConst* find_local_names(const PrimId& anId) const {
		return _names.find_local_names(anId);
	}
	
	virtual const FogMetaSlot* find_local_slot(const PrimId& anId) const;
	FogEntity* find_local_type(const PrimId& anId, ResolveAlias resolveAlias = KEEP_ALIAS) {
		return _types.find_local_type(anId, resolveAlias);
	}
	
	FogTypedef* find_local_typedef(const FogTypedef& aTypedef) {
		return _typedefs.find_local_typedef(aTypedef);
	}
	
	FogVariable* find_local_variable(const FogVariable& aVariable) {
		return _variables.find_local_variable(aVariable);
	}
	
	FogEntity* find_name(const PrimId& anId, InScope inScope) {
		return find_entity(anId, inScope, FIND_NAME);
	}
	
	const FogEntity* find_name(const PrimId& anId, InScope inScope) const {
		return mutate().find_name(anId, inScope);
	}
	
	virtual bool find_slots(FogMetaSlotFinder& theFinder);
	virtual FogEntity* find_template(FogMakeTemplateContext& makeTemplateContext);
	FogEntity* find_type(const PrimId& anId, InScope inScope) {
		return find_entity(anId, inScope, FIND_TYPE);
	}
	
	const FogEntity* find_type(const PrimId& anId, InScope inScope) const {
		return mutate().find_type(anId, inScope);    //   Phase this one out.
	}
	
	bool find_visible_entity(FogEntityFinder& theFinder) {
		return find_local_entity(theFinder) || find_scope_entity(theFinder);
	}
	
	bool find_visible_slot(FogMetaSlotFinder& theFinder) {
		return find_local_slot(theFinder) || find_scope_slot(theFinder);
	}
	
	virtual bool has_access() const;
	bool has_inline_functions() const {
		return _functions.has_inline_functions();
	}
	
	bool has_own_interface() const {
		return _interface.has_own_interface();
	}
	
	virtual FogTargetFile* implementation_file();
	const FogTargetFile* implementation_file() const {
		return Super::implementation_file();
	}
	
	virtual FogUsage& inline_usage();
	virtual FogScope& inner_scope();
	virtual void instantiate_into(FogEntity& instantiatingEntity);
	virtual FogTargetFile* interface_file_sink();
	virtual FogTargetFile* interface_file_source();
	const FogTargetFile* interface_file_source() const {
		return Super::interface_file_source();
	}
	
	virtual FogUsage& interface_usage_finish();
	virtual FogUsage& interface_usage_start();
	virtual bool is_active() const;
	bool is_auto() const;
	bool is_explicit_specialisation() const {
		return _templates.is_explicit_specialisation();
	}
	
	bool is_frozen() const {
		return _is_frozen;
	}
	
	virtual bool is_instantiation() const;
	const FogLazyBool& is_leaf() const {
		return *_is_leaf;
	}
	
	virtual FogScope* is_scope();
	bool is_specialisation() const {
		return _templates.is_specialisation();
	}
	
	virtual bool is_templated() const;
	FogEntity* make_enum(FogMakerContext& makerContext);
	FogEntity* make_function_entity(FogMakerContext& makerContext);
	virtual FogScope* make_linkage(const FogLinkageSpecifier& rawLinkage);
	FogEntity* make_linkage_entity(FogMakerContext& makerContext);
	FogEntity* make_meta_slot(FogMakerContext& makerContext);
	FogEntity* make_scope(FogMakerContext& makerContext);
	FogEntity* make_type(FogMakerContext& makerContext);
	FogTypedef* make_typedef(FogMakerContext& makerContext) {
		return _typedefs.make_typedef(makerContext);
	}
	
	FogEntity* make_typedef_entity(FogMakerContext& makerContext);
	FogTypeName* make_typename(FogMakerContext& makerContext, bool builtIn = false) {
		return _types.make_typename(makerContext, builtIn);
	}
	
	FogEntity* make_typename_entity(FogMakerContext& makerContext);
	FogEntity* make_using_entity(FogMakerContext& makerContext);
	FogVariable* make_variable(FogMakerContext& makerContext) {
		return _variables.make_variable(makerContext);
	}
	
	FogEntity* make_variable_entity(FogMakerContext& makerContext);
	virtual const FogMetaType& meta_type() const;
	virtual FogTargetFile* name_file();
	virtual FogUsage& name_usage();
	size_t nesting_depth() const {
		return _nested.nesting_depth();
	}
	
	virtual FogScope& parent_scope();
	This& primary() {
		return (This&)_templates.primary();
	}
	
	const This& primary() const {
		return (const This&)_templates.primary();
	}
	
	This& primary_instantiation() {
		return (This&)_templates.primary();
	}
	
	const This& primary_instantiation() const {
		return (const This&)_templates.primary();
	}
	
	This& primary_specialisation() {
		return (This&)_templates.primary();
	}
	
	const This& primary_specialisation() const {
		return (const This&)_templates.primary();
	}
	
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_long_id(std::ostream& s) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_this(std::ostream& s) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	virtual void set_access(const FogAccess& anAccess);
	void set_braced();
	void set_braced(FogMakeEntityContext& makeEntityContext);
	void set_default_access(const FogAccess& anAccess);
	//    void set_implementation_code(const FogList& someCode) { _implementation.set_code(someCode); }
	virtual void set_implementation_file(FogTargetFile* aFile);
	//    void set_interface_code(const FogList& someCode) { _interface.set_code(someCode); }
	virtual void set_interface_file(FogTargetFile* aFile);
	void set_no_interface() {
		_interface.set_no_interface();
	}
	void set_no_implementation();
	
	void set_not_leaf();
	void set_tag(const FogTag& aTag) {
		_tag = tag().merge_tag(aTag);
	}
	
	const FogTag& tag() const {
		return *_tag;
	}
	
	virtual FogTemplateManager& template_manager();
	virtual const FogTemplateParameterSpecifiers& template_parameters() const;
	virtual FogTokenType::TokenType token_type_enum() const;
	virtual bool validate() const;
	bool validate_utility(const FogEntity& anEntity) const;
	size_t virtual_bases() const {
		return _bases.virtual_bases();
	}
	
public:
	static int compare(const FogScope& p1, const FogScope& p2) {
		return Super::compare(p1, p2);
	}
	
	static int compare(const FogScopeRefToConst* p1, const FogScopeRefToConst* p2);
};

#endif

