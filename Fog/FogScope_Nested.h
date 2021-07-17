
#ifndef FOGSCOPE_NESTED_HXX
#define FOGSCOPE_NESTED_HXX




class FogScope_Nested {
private:
	//   Defining contents.
	FogScopeMapOfRef _scope_map;                    //   Map of all nested scopes.
	const size_t _nesting_depth;                    //   Nesting depth of scope, 0 global.
private:
	FogScope_Nested(const FogScope_Nested&);
public:
	FogScope_Nested(size_t nestingDepth = 0) : _nesting_depth(nestingDepth) {}
	FogScope* add_scope(FogScope& toScope, FogScope& aScope);
	void destroy();
	void do_meta_construct(FogScopeContext& inScope);
	void do_derive(FogDeriveContext& inScope);
	FogScope* find(const PrimId& anId) { return _scope_map.find(anId); }
	const FogScope* find_local_class(const PrimId& anId) const { return _scope_map.find(anId); }
	bool needs_emission() const { return _scope_map.tally() != 0; }
	size_t nesting_depth() const { return _nesting_depth; }
	std::ostream& print_depth(std::ostream& s, int aDepth) const;
	std::ostream& print_members(std::ostream& s, int aDepth) const;
	const FogScopeMapOfRefToConst& scope_map() const { return _scope_map; }
	void validate() const;
};
#endif

