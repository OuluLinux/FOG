
#ifndef FOGVARIABLE_HXX
#define FOGVARIABLE_HXX



class FogVariable : public FogDerivable
{
    typedef FogDerivable Super;
    typedef FogVariable This;
    TYPEDECL_SINGLE(This, Super)
    PRIMREF_DERIVED_DECLS(This)
private:
    const FogVariableSpecifier *get_specifier() const;
    virtual FogUsage *make_implementation_usage();
    virtual FogUsage *make_interface_usage_finish();
    This& mutate() const { return *(This *)this; }
protected:
    FogVariable();
    virtual ~FogVariable();
public:
    FogVariable(FogMakerContext& makerContext);
    virtual void adopt(FogScopeContext& inScope, FogPotentialDeclaration& potentialDeclaration);
    virtual void create_usages();
    const FogExpr *default_initialiser(bool isCopy) const;
    virtual bool do_compile(FogCompileContext& inScope);
    virtual bool do_derive(FogDeriveContext& inScope);
    virtual void emit_implementation(FogStream& s);
    virtual void emit_interface(FogStream& s);
    virtual void install_names();
    virtual FogEntity *instantiate(FogScopeContext& inScope, InstantiateOption ifRequired);
    virtual FogUsage& interface_usage_finish();
    virtual FogUsage& interface_usage_start();
    virtual FogVariable *is_variable();
    virtual const FogMetaType& meta_type() const;
    virtual std::ostream& print_viz(std::ostream& s) const;
    virtual bool validate() const;
public:
    static int compare(const FogVariableRefToConst *p1, const FogVariableRefToConst *p2);
};
#endif
 
