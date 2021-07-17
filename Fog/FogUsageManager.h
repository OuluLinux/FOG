
#ifndef FOGUSAGEMANAGER_HXX
#define FOGUSAGEMANAGER_HXX






struct FogUsageManager_Element;

//  .bugbug _dependents, _independents would be better as intrusive linked lists

class FogUsageManager : public PrimPrint, public FogUsageEnums {
	typedef PrimPrint Super;
	TYPEDECL_SINGLE(FogUsageManager, Super)
	
private:
	FogUsageListOfRef _usages;      //   List of all usages.
	
private:
	//   Working contents
	FogUsageManager_Element* _elements;    //   Array of INDEXES elements.
	size_t _first_index;       //   First non-empty _independents list.
	
private:
	void show_cycle(const FogUsageListOfRefToConst& thePath, const FogUsage& cycleStart) const;
	
public:
	FogUsageManager();
	virtual ~FogUsageManager();
	void add_dependent(FogUsage& aUsage);
	void add_independent(FogUsage& aUsage);
	void compile();
	void destroy();
	FogUsage* make_enum_usage(FogEntity& anEntity, const FogDeclSpecifierValue& declSpecifiers);
	FogUsage* make_file_usage(FogTargetFile& targetFile);
	FogUsage* make_friend_usage(FogScope& ofScope, FogEntity& anEntity);
	FogUsage* make_function_usage(FogEntity& anEntity, const FogDeclSpecifierValue& declSpecifiers);
	FogUsage* make_implementation_usage(FogEntity& anEntity);
	FogUsage* make_inline_usage(FogScope& aScope);
	FogUsage* make_interface_usage_finish(FogEntity& anEntity);
	FogUsage* make_interface_usage_start(FogEntity& anEntity);
	FogUsage* make_name_usage(FogEntity& anEntity);
	FogUsage* make_typedef_usage(FogEntity& anEntity, const FogDeclSpecifierValue& declSpecifiers);
	FogUsage* make_variable_usage(FogEntity& anEntity, const FogDeclSpecifierValue& declSpecifiers);
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	void promote_to_independent(FogUsage& aUsage);
	
public:
	static FogUsageManager& mutable_null();
};

#endif

