
#ifndef FOGSCOPE_NAMES_HXX
#define FOGSCOPE_NAMES_HXX






class FogScope_Names {
private:           //   Defining contents.
	FogEntitySetOfListOfRef _names;      //   All local non-type-names including overloads.
public:
	void add_name(const PrimId& anId, FogEntity& anEntity);
	void destroy() { _names.vacate(); }
	size_t find_local_name(FogEntityFinder& theFinder);
	const FogEntityListOfRefToConst* find_local_names(const PrimId& anId) const;
	std::ostream& print_depth(std::ostream& s, int aDepth) const;
	std::ostream& print_members(std::ostream& s, int aDepth) const;
};
#endif

