
#ifndef FOGUSAGE_HXX
#define FOGUSAGE_HXX






class FogUsage : public PrimPrint, public FogUsageEnums {
	typedef PrimPrint Super;
	typedef FogUsage This;
	TYPEDECL_SINGLE(This, Super)
	PRIMSHARES_DECLS(This)
	PRIMREF_DECLS(This)
	
private:
	//   Defining contents.
	const PrimIdHandle _id;      //   Unique name.
	const TypeEnum _type;      //   Type of usage, for use as an array index by the manager.
	FogEntity* _entity;       //   Entity for which this maintains usage, 0 if no entity.
	
private:
	//   Accrued contents.
	FogUsageMapOfRef _use_of;     //   Usages used by _entity.
	FogUsageMapOfRef _used_by;     //   Interfaces/implementations used by _entity.
	FogUsageMapOfRef _precursors;    //   All usages to be emitted before this usage.
	FogTargetFile* _file;      //   File resolving this usage, 0 if a regeneratable name.
	size_t _usage_number;      //   Usage number in independency order.
	
private:
	FogUsage(const This&);
	This& operator=(const This&);
	
private:
	void add_precursors_to(FogTargetFile& aFile);
	//  	This& mutate() const { return *(This *)this; }
	
protected:
	FogUsage();
	FogUsage(const PrimId& anId, TypeEnum aType, FogEntity* anEntity);
	virtual ~FogUsage();
	bool is_used() const {
		return _used_by.tally() != 0;
	}
	
	void precompile(FogUsageManager& usageManager, FogTargetFile* aFile);
	void remove_just_use_of(FogUsage& aUsage, FogUsageManager& usageManager);
	
public:
	virtual void add_use_by(FogUsage& aUsage);
	virtual void compile();
	void destroy();
	virtual void emit(FogStream& s) = 0;
	FogEntity* entity() {
		return _entity;
	}
	
	const FogEntity* entity() const {
		return _entity;
	}
	
	FogTargetFile* file() {
		return _file;
	}
	
	const FogTargetFile* file() const {
		return _file;
	}
	
	const PrimId& id() const {
		return *_id;
	}
	
	virtual bool is_implementation() const;
	virtual bool is_interface() const;
	virtual bool is_weak() const;
	FogUsage* pick_cyclic_use_of(FogUsageManager& usageManager);
	virtual void precompile(FogUsageManager& usageManager) = 0;
	const FogUsageMapOfRef& precursors() {
		return _precursors;
	}
	
	const FogUsageMapOfRefToConst& precursors() const {
		return _precursors;
	}
	
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_this(std::ostream& s) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	void remove_all_uses(FogUsageManager& usageManager, size_t usageNumber);
	void remove_use_of(FogUsage& aUsage, FogUsageManager& usageManager);
	const FogScope* scope() const;
	virtual void set_weak();
	TypeEnum type() const {
		return _type;
	}
	
	const PrimId& unique_id() const {
		return *_id;
	}
	
	size_t usage_number() const {
		return _usage_number;
	}
	
	const FogUsageMapOfRefToConst& use_of() const {
		return _use_of;
	}
	
	virtual const FogTargetFile* used_file() const {
		return 0;
	}
	
public:
	static int compare(const FogUsageRefToConst* p1, const FogUsageRefToConst* p2);
	static int compare_least_dependent_first(const FogUsageRefToConst* p1, const FogUsageRefToConst* p2);
	static FogUsage* new_enum_usage(FogEntity& anEntity, const FogDeclSpecifierValue& declSpecifiers);
	static FogUsage* new_file_usage(FogTargetFile& targetFile);
	static FogUsage* new_friend_usage(FogScope& ofScope, FogEntity& anEntity);
	static FogUsage* new_function_usage(FogEntity& anEntity, const FogDeclSpecifierValue& declSpecifiers);
	static FogUsage* new_implementation_usage(FogEntity& anEntity);
	static FogUsage* new_inline_usage(FogScope& aScope);
	static FogUsage* new_interface_usage_finish(FogEntity& anEntity);
	static FogUsage* new_interface_usage_start(FogEntity& anEntity);
	static FogUsage* new_name_usage(FogEntity& anEntity);
	static FogUsage* new_typedef_usage(FogEntity& anEntity, const FogDeclSpecifierValue& declSpecifiers);
	static FogUsage* new_variable_usage(FogEntity& anEntity, const FogDeclSpecifierValue& declSpecifiers);
	
	
	// Piggyback values
	static const FogScope* filter_name;
	
};

#endif

