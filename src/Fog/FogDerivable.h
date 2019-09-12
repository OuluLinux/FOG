
#ifndef FOGDERIVABLE_HXX
#define FOGDERIVABLE_HXX





class FogDerivable : public FogEntity
{
    typedef FogEntity Super;
    typedef FogDerivable This;
    TYPEDECL_SINGLE(This, Super)
private:                                                //   Defining contents.
    FogPotentialDeclarationListOfRef _potentials;           //   Contributing potential declaration(s).
    FogDerivable *_primary_instantiation;                   //   Primary definition of an instantiation.
    FogDerivable *_primary_specialisation;                  //   Primary definition of a specialisation.
    FogActualDeclaration _actual;                           //   Resolved actual declaration.
    FogEntity *_referenced_entity;                          //   Entity visible with this name in this scope
    bool _is_active;                                        //   Set true once some potential is active.
private:
    void actualize(FogScopeContext& inScope, FogDerivable& aDerivable);
    void actualize(FogScopeContext& inScope, FogPotentialDeclaration& potentialDeclaration);
    This& mutate() const { return *(This *)this; }
protected:
    FogDerivable();
    FogDerivable(FogMakerContext& makerContext, const FogLazyBool& isPure = FogLazyBool::make_false());
    FogDerivable(FogMakeTemplateContext& makeTemplateContext, const FogLazyBool& isPure = FogLazyBool::make_false());
    virtual ~FogDerivable();
    void set_primary_instantiation(FogDerivable& aDerivable);
    void set_primary_specialisation(FogDerivable& aDerivable);
    void set_referenced_entity(FogEntity *referencedEntity) { _referenced_entity = referencedEntity; }
public:
    virtual const FogAccess& access() const;
    FogActualDeclaration& actual_declaration() { return _actual; }
    const FogSpecifier& actual_specifier() const;
    void add(FogMakerContext& makerContext);
    virtual void adopt(FogScopeContext& inScope, FogPotentialDeclaration& potentialDeclaration);
    virtual void create_usages();
    const FogDeclSpecifierValue& decl_specifiers(FogScopeContext& inScope);
    const FogDeclSpecifierValue& decl_specifiers() const;
    virtual void destroy();
    virtual bool do_compile(FogCompileContext& inScope);
    virtual bool do_derive(FogDeriveContext& inScope);
    virtual void emit_template_prefix(FogEmitContext& emitContext) const;
    virtual void find_entities(FogEntityFinder& theFinder);
    virtual bool find_slots(FogMetaSlotFinder& theFinder);
//    const FogStatic& get_static() const;
    virtual void install_names();
    virtual FogUsage& interface_usage_finish();
    virtual FogUsage& interface_usage_start();
    virtual bool is_active() const;
//    bool is_const() const;
    virtual FogDerivable *is_derivable();
//    bool is_extern() const;
//    bool is_instantiation() const;
//    bool is_static() const;
//    bool is_volatile() const;
    FogDerivable *make_derived(FogDeriveContext& inScope, FogDerivable *aDerivable);
    virtual FogDerivable *make_used(FogDeriveContext& deriveContext, const FogObjectSpecifier& usingSpecifier);
    virtual const FogMetaType& meta_type() const;
    FogSpecifier& mutable_specifier();
    virtual FogUsage& name_usage();
    const FogSpecifier *potential_specifier() const;
    const FogPotentialDeclarationListOfRef& potentials() { return _potentials; }
    const FogPotentialDeclarationListOfRefToConst& potentials() const { return _potentials; }
    virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_this(std::ostream& s) const;
    FogScope& scope() { return non_virtual_scope(); }
    const FogScope& scope() const { return non_virtual_scope(); }
    virtual void set_interface_file(FogTargetFile *aFile);
    void set_is_active(FogPotentialDeclaration& potentialDeclaration);
    const FogObjectSpecifier& specifier() const;
    const FogObjectSpecifier& specifier(FogScopeContext& inScope);
    virtual bool validate() const;
};
#endif
 
