
#ifndef FOGTARGETFILE_HXX
#define FOGTARGETFILE_HXX







class FogTargetFile : public FogToken {
	typedef FogToken Super;
	typedef FogTargetFile This;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	FOGTOKEN_ACTUAL_DECLS
private:         //   Defining contents.
	const PrimIdHandle _pathed_id;    //   Complete external file name.
	const PrimIdHandle _short_id;
	const FogSourceFile* _source_file;   //   Non-0 if target overlays a source.
	PrimIdHandle _guard;      //   Upper cased _short_id used as guard.
	FogUtilityHandle _utility;     //   Status of contributions.
	bool _explicit_guard : 1;     //   True if guard defined explicitly.
	bool _is_interface : 1;      //   True if an interface file.
	bool _is_input : 1;       //   True if accessed by /use=.
private:         //   Accrued contents.
	FogUsageMapOfRef _external_map;    //   Map of all external entities used by this file.
	FogUsageListOfRef _internals;    //   Ordered list of internally defined entities.
private:         //   Accrued and subsequently trashed contents.
	FogTargetFileMapOfRef _used_by;    //   Files by which this file is used.
	FogTargetFileMapOfRef _use_of;    //   Files that are used by this file..
private:         //   Compiled contents.
	FogTargetFileListOfRefToConst _external_files; //   Ordered list of required include files.
	PrimIdMap _visible_files;     //   Map of all file names visible to user of this file.
	FogUsageMapOfRefToConst _visible_names;  //   Map of all names visible to user of this file.
	size_t _file_number;      //   Allocated file number in independency order.
	size_t _min_usage_number;     //   Smallest usage number of _internals.
	size_t _max_usage_number;     //   Largest usage number of _internals.
	FogUsage* _usage;       //   Usage node for entities dependent on this file.
private:
	FogTargetFile(const This&);
	This& operator=(const This&);
private:
	const PrimId& id() const { return unique_id(); }
	friend struct PrimMapOfRefToConstFunctions<FogTargetFile>;
private:
	void diagnose_conflicts() const;
	void diagnose_non_emission() const;
	void diagnose_non_inline_implementations() const;
	void emit_contents(FogStream& s, const FogRoot& aRoot) const;
	void remove_just_use_of(FogTargetFile& aFile, FogFileManager& fileManager);
	void set_max_utility(const FogUtility& aUtility);
	virtual void set_new_utility(const FogUtility& aUtility);
protected:
	FogTargetFile();
public:
	FogTargetFile(const FogSourceFile& sourceFile);
	FogTargetFile(const PrimId& pathedId, const PrimId& fileIdent);
	virtual void add_external(FogUsage& aUsage);
	virtual void add_include(FogTargetFile& aFile);
	virtual void add_internal(FogUsage& aUsage);
	virtual void compile_bottom();
	virtual void compile_top();
	virtual void destroy();
	void emit_dependencies(std::ostream& s, const PrimIdList& sourceList) const;
	void emit_file(FogRoot& aRoot) const;
	void emit_guarded_include(FogStream& s) const;
	const PrimId& full_file_name() const { return *_pathed_id; }
	bool is_frozen_input() const;
	bool is_input() const;
	size_t max_usage_number() const { return _max_usage_number; }
	size_t min_usage_number() const { return _min_usage_number; }
	FogTargetFile* pick_cyclic_use_of(FogFileManager& fileManager);
	virtual void precompile(FogFileManager& fileManager);
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_this(std::ostream& s) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	void remove_all_uses(FogFileManager& fileManager, size_t fileNumber);
	void remove_use_of(FogTargetFile& aFile, FogFileManager& fileManager);
	virtual void set_file_number(size_t aNumber);
	virtual void set_guard(const PrimId& aGuard);
	virtual void set_is_input();
	void set_max_utility(const FogEntity& anEntity);
	void set_max_utility(const FogFileIdent& fileIdent);
	const PrimId& short_id() const { return *_short_id; }
	const FogSourceFile* source_file() const { return _source_file; }
	const PrimId& unique_id() const { return *_pathed_id; }
	FogUsage& usage(FogUsageManager& usageManager);
	const FogTargetFileMapOfRefToConst& use_of() const { return _use_of; }
	const FogUtility& utility() const { return *_utility; }
	
private:
	static int compare_least_dependent_first(const FogTargetFileRefToConst* p1,
	        const FogTargetFileRefToConst* p2);
public:
	static int compare(const FogTargetFileRefToConst* p1, const FogTargetFileRefToConst* p2);
	static int compare_most_dependent_first(const FogTargetFileRefToConst* p1,
	                                        const FogTargetFileRefToConst* p2);
};
#endif

