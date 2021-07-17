
#ifndef FOGROOT_HXX
#define FOGROOT_HXX













class FogRoot : public FogScope {
	typedef FogScope Super;
	typedef FogRoot This;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	
private:
	PrimTime _time;        //   Time of construction.
	PrimStringHandle _program;     //   Generating program name.
	FogScopeMapOfRef _linkages;     //   Map of all extern linkages.
	PrimIdList _source_list;     //   Sorted list of all source files.
	PrimIdList _missing_interface_file;   //   List of entities without an interface file.
	PrimIdList _missing_implementation_file; //   List of entities without an implementation file.
	PrimIdList _missing_implementation;   //   List of entities without an implementation.
	FogReader _reader;       //   Source parser.
	FogUsageManager _usage;      //   Entity usage graph.
	FogFileManager _files;      //   File usage graph.
	FogScopeListOfRefToConst _file_spaces;  //   The list that owns all file-spaces
	FogScopeListOfRef _scopes;     //   List of all scopes in construction order.
	FogScopeRef _meta_base;      //   The ultimate meta-base class.
	FogScopeRef _std;       //   The std namespace.
	FogStaticScopeContext _root_context;  //   Context of global parsing.
	
private:
	This& mutate() const {
		return *(This*)this;
	}
	
	virtual void set_new_utility(const FogUtility& aUtility);
	
protected:
	FogRoot();
	virtual ~FogRoot();
	void install_types();
	
public:
	FogRoot(const PrimId& anId);
	virtual void add_file_space(FogScope& fileSpace) {
		_file_spaces.add(fileSpace);
	}
	
	void add_missing_interface_file(const PrimId& anId);
	void add_missing_implementation_file(const PrimId& anId);
	void add_missing_implementation(const PrimId& anId);
	void add_ordered_scope(FogScope& aScope) {
		_scopes.add(aScope);
	}
	
	bool compile();
	virtual const FogInline& default_inline() const;
	virtual void destroy();
	void emit_banner(FogStream& s) const;
	void emit_dependencies(const PrimString& fileName) const;
	void emit_files();
	virtual void emit_template_prefix(FogEmitContext& emitContext) const;
	FogFileManager& file_manager() {
		return _files;
	}
	
	const FogFileManager& file_manager() const {
		return _files;
	}
	
	virtual void find_entities_in(FogScopeContext& inScope, FogEntityFinding& theFinding) const;
	virtual const PrimId& global_id() const;
	virtual const PrimId& global_signature_id() const;
	virtual FogUsage& inline_usage();
	FogBuiltInScope* install_built_in(FogBuiltInScope* theType, const char* p1);
	FogBuiltInScope* install_built_in(FogBuiltInScope* theType, const char* p1, const char* p2);
	FogBuiltInScope* install_built_in(FogBuiltInScope* theType, const char* p1, const char* p2, const char* p3);
	virtual FogUsage& interface_usage_finish();
	virtual FogUsage& interface_usage_start();
	virtual bool is_global_scope() const;
	virtual bool is_name_scope() const;
	virtual bool is_name_space() const;
	virtual const PrimId& local_id() const;
	virtual const PrimId& local_signature_id() const;
	virtual const PrimId& long_id() const;
	virtual FogScope* make_linkage(const FogLinkageSpecifier& rawLinkage);
	FogScope& meta_base() {
		return *_meta_base;
	}
	
	const FogScope& meta_base() const {
		return *_meta_base;
	}
	
	virtual FogScope& name_emit_scope();
	virtual FogScope& name_emit_transient_scope();
	virtual FogScope& name_scope();
	virtual FogScope& name_space();
	virtual FogUsage& name_usage();
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_long_id(std::ostream& s) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_this(std::ostream& s) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	virtual FogReader& reader();
	const FogReader& reader() const {
		return mutate().reader();
	}
	
	FogScopeContext& root_context() {
		return _root_context;
	}
	
	void set_program(const PrimString& anId) {
		_program = anId;
	}
	
	virtual const PrimId& short_id() const;
	const FogSourceFile* source_file() const {
		return _reader.source_file();
	}
	
	const FogScope& std_scope() const {
		return *_std;
	}
	
	const PrimTime& time() const {
		return _time;
	}
	
	FogUsageManager& usage_manager() {
		return _usage;
	}
	
	const FogUtility& utility_mode() const {
		return _reader.utility_mode();
	}
	
public:
	static FogRoot& null() {
		return mutable_null();
	}
};



#endif

