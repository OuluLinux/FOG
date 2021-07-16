
#ifndef FOGSCOPE_VARIABLES_HXX
#define FOGSCOPE_VARIABLES_HXX





class FogScope_Variables
{
private:          //   Defining contents.
 FogVariableMapOfRef _distinct_variable_map;  //   Map of potentially visible variables.
 FogVariableListOfRef _distinct_variable_list; //   Declaration ordered list of potentially visible variables.
public:
 void create_usages(FogScope& inScope);
 void derive_from(FogScope& derivedScope, FogBaseClass& baseClass);
 void derive_from(FogScope& derivedScope, FogVariable& baseVariable, FogPotentialDeclaration *potentialDeclaration);
 void destroy();
 FogVariableListOfRef& distinct_variable_list() { return _distinct_variable_list; }
 const FogVariableListOfRefToConst& distinct_variable_list() const { return _distinct_variable_list; }
 void do_compile(FogCompileContext& inScope);
 void do_derive(FogDeriveContext& inScope);
 FogVariable *find_local_variable(const FogVariable& aVariable);
 FogVariable *make_variable(FogMakerContext& makerContext);
 bool needs_emission() const;
 std::ostream& print_depth(std::ostream& s, int aDepth) const;
 std::ostream& print_members(std::ostream& s, int aDepth) const;
 void validate() const;
};
#endif
 
