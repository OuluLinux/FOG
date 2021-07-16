
#ifndef FOGSCOPE_FUNCTIONS_HXX
#define FOGSCOPE_FUNCTIONS_HXX





class FogScope_Functions
{
private:    
    FogFunctionListOfRef _distinct_function_list;   //   (Eventually) sorted list of potentially visible functions.
    FogFunctionMapOfRef _distinct_function_map;     //   Set of potentially visible functions indexed by local signature.
public:
    FogScope_Functions();
    bool can_be_concrete(FogScopeContext& inScope);
    void create_usages();
    void derive_from(FogScope& derivedScope, FogBaseClass& baseClass);
    void derive_from(FogScope& derivedScope, FogFunction& baseFunction, FogPotentialDeclaration *potentialDeclaration);
    void destroy();
    FogFunctionListOfRef& distinct_function_list() { return _distinct_function_list; }
    const FogFunctionListOfRefToConst& distinct_function_list() const { return _distinct_function_list; }
    void do_compile(FogCompileContext& inScope);
    void do_derive(FogDeriveContext& inScope);
    FogFunction *find_local_function(const FogFunction& aFunction);
    FogFunction *find_local_function(const PrimId& anId);
    bool has_inline_functions() const;
 void instantiate_into(FogScope& thisScope, FogScope& instantiatingScope);
    bool is_concrete() const;
    FogFunction *make_function(FogMakerContext& makerContext);
    bool needs_emission() const { return _distinct_function_list.tally() != 0; }
    std::ostream& print_depth(std::ostream& s, int aDepth) const;
    std::ostream& print_members(std::ostream& s, int aDepth) const;
    void validate() const;
};
#endif
 
