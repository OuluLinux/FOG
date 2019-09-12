
#ifndef FOGSCOPE_TYPEDEFS_HXX
#define FOGSCOPE_TYPEDEFS_HXX





class FogScope_Typedefs
{
private:          //   Defining contents.
 FogTypedefMapOfRef _distinct_typedef_map;  //   Map of potentially visiable typedefs.
 FogTypedefListOfRef _distinct_typedef_list;  //   (Eventually) sorted list of potentially visible typedefs.
public:
 void create_usages();
 void derive_from(FogScope& derivedScope, FogBaseClass& baseClass);
 void derive_from(FogScope& derivedScope, FogTypedef& baseTypedef, FogPotentialDeclaration *potentialDeclaration);
 void destroy();
 FogTypedefListOfRef& distinct_typedef_list() { return _distinct_typedef_list; }
 const FogTypedefListOfRefToConst& distinct_typedef_list() const { return _distinct_typedef_list; }
 void do_compile(FogCompileContext& inScope);
 void do_derive(FogDeriveContext& inScope);
 FogTypedef *find_local_typedef(const FogTypedef& aTypedef);
 FogTypedef *make_typedef(FogMakerContext& makerContext);
 bool needs_emission() const;
 std::ostream& print_depth(std::ostream& s, int aDepth) const;
 std::ostream& print_members(std::ostream& s, int aDepth) const;
 void validate() const;
};
#endif
 
