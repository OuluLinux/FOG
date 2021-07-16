
#ifndef FOGUNIQUE_HXX
#define FOGUNIQUE_HXX






class FogUnique : public FogMetaToken {
	typedef FogMetaToken Super;
	typedef FogUnique This;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	FOGTOKEN_ACTUAL_DECLS
	
private:
	const PrimIdHandle _short_id;    //   Entity identifier
	const PrimIdHandle _local_id;    //   Entity identifier plus template arguments
	const PrimIdHandle _local_signature_id;  //   Locally unique unscoped entity name
	const PrimIdHandle _long_id;    //   Scoped entity identifier
	const PrimIdHandle _global_id;    //   Scoped entity identifier plus template arguments
	const PrimIdHandle _global_signature_id; //   Globally unique scoped entity name -- inherited.
	FogUtilityHandle _utility;     //   Status of declaration.
	
private:
	FogUsage *_name_usage;      //   Usage information of/by the name.
	FogUsage *_interface_usage_start;   //   Usage information of/by the interface start.
	FogUsage *_interface_usage_finish;   //   Usage information of/by the interface finish.
	FogUsage *_implementation_usage;   //   Usage information of/by the implementation.
	FogTargetFile *_implementation_file;  //   File in which implementation was defined/has been allocated.
	FogTargetFile *_interface_file;    //   File in which interface was defined/has been allocated.
	
private:
	FogUnique(const This&);
	This& operator=(const This&);
	
private:

	friend struct PrimMapOfRefToConstFunctions<FogUnique>;
	const PrimId& id() const {
		return *_global_signature_id;
	}
	
	virtual FogUsage *make_implementation_usage();
	virtual FogUsage *make_interface_usage_finish();
	virtual FogUsage *make_interface_usage_start();
	virtual FogUsage *make_name_usage();
	This& mutate() const {
		return *(This *)this;
	}
	
	virtual void set_new_utility(const FogUtility& aUtility);
	
protected:
	FogUnique();
	FogUnique(const PrimId& shortId, const PrimId& longId);
	FogUnique(const FogMakerContext& makerContext);
	FogUnique(const FogMakeTemplateContext& makeTemplateContext);
	const FogTargetFile *raw_implementation_file() const {
		return _implementation_file;
	}
	
	FogTargetFile *raw_implementation_file() {
		return _implementation_file;
	}
	
	FogUsage *raw_implementation_usage() {
		return _implementation_usage;
	}
	
	const FogUsage *raw_implementation_usage() const {
		return _implementation_usage;
	}
	
	const FogTargetFile *raw_interface_file() const {
		return _interface_file;
	}
	
	FogTargetFile *raw_interface_file() {
		return _interface_file;
	}
	
	FogUsage *raw_interface_usage_finish() {
		return _interface_usage_finish;
	}
	
	const FogUsage *raw_interface_usage_finish() const {
		return _interface_usage_finish;
	}
	
	FogUsage *raw_interface_usage_start() {
		return _interface_usage_start;
	}
	
	const FogUsage *raw_interface_usage_start() const {
		return _interface_usage_start;
	}
	
	FogUsage *raw_name_usage() {
		return _name_usage;
	}
	
public:
	virtual void destroy();
	virtual void emit_forward_reference(FogStream& s) = 0;
	virtual void emit_implementation(FogStream& s);
	virtual void emit_interface(FogStream& s);
	virtual void emit_interface_finish(FogStream& s) const;
	virtual void emit_interface_start(FogStream& s) const;
	virtual const PrimId& file_id() const;
	virtual const PrimId& global_id() const;
	virtual const PrimId& global_signature_id() const;
	virtual FogTargetFile *implementation_file() {
		return _implementation_file;
	}
	
	const FogTargetFile *implementation_file() const {
		return mutate().implementation_file();
	}
	
	virtual FogUsage& implementation_usage();
	virtual FogTargetFile *interface_file_sink() {
		return _interface_file;
	}
	
	virtual FogTargetFile *interface_file_source() {
		return _interface_file;
	}
	
	const FogTargetFile *interface_file_source() const {
		return mutate().interface_file_source();
	}
	
	virtual FogUsage& interface_usage_finish();
	virtual FogUsage& interface_usage_start();
	virtual const PrimId& local_id() const;
	virtual const PrimId& local_signature_id() const;
	virtual const PrimId& long_id() const;
	virtual FogTargetFile *name_file() = 0;
	const FogTargetFile *name_file() const {
		return mutate().interface_file_source();
	}
	
	virtual FogUsage& name_usage();
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_long_id(std::ostream& s) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_this(std::ostream& s) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	const FogUsage *raw_name_usage() const {
		return _name_usage;
	}
	
	virtual void set_implementation_file(FogTargetFile *aFile);
	virtual void set_interface_file(FogTargetFile *aFile);
	void set_max_utility(const FogUtility& aUtility) {
		if (aUtility > utility())
			set_new_utility(aUtility);
	}
	
	void set_utility(const FogUtility& aUtility) {
		if (aUtility != utility())
			set_new_utility(aUtility);
	}
	
	virtual const PrimId& short_id() const;
	virtual int sort_class() const;
	const PrimId& unique_id() const {
		return global_signature_id();
	}
	
	virtual const FogUtility& utility() const;
	
public:
	static int compare(const FogUnique& p1, const FogUnique& p2);
	static int compare(const FogUniqueRefToConst *p1, const FogUniqueRefToConst *p2);
};

#endif

