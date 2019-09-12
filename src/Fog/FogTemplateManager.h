
#ifndef FOGTEMPLATEMANAGER_HXX
#define FOGTEMPLATEMANAGER_HXX






class FogTemplateManager : public PrimPrint
{
    typedef PrimPrint Super;
    typedef FogTemplateManager This;
    TYPEDECL_SINGLE(This, Super)
public:
    enum Classification
    {
        NON_TEMPLATE,                               //   Normal untemplated case
                                                    //    the interface, the implementation
        PRIMARY_TEMPLATE,                           //   First and most general templating
                                                    //    primary interface, primary implementation 
        PARTIAL_SPECIALISATION,                     //   Refined redeclaration with free parameters
                                                    //    independent interface, independent implementation
//        COMPLETE_SPECIALISATION,                    // Refined redeclaration without free parameters
//                                                    //  independent interface, independent implementation
        IMPLICIT_INSTANTIATION,                     //   Refined redeclaration without free parameters
                                                    //    independent interface, independent implementation
        EXPLICIT_SPECIALISATION                     //   Refined declaration with (some) redeclared members
                                                    //    unchanged interface, possibly changed implementations
    };
private:                                        //   Defining contents.
    FogEntity& _primary;                            //   Primary definition of template.
    const Classification _classification;           //   Nature of template.
    const FogTemplateArgsRefToConst _arguments;     //   Encoded arguments if a template specialisation.
    const FogTemplateParameterSpecifiersRefToConst _parameter_specifiers;   //   Formal parameters if a template declaration.
    FogEntitySetOfRef _secondaries;                 //   Set of all instantiations and specialisations.
private:
    FogTemplateManager(const This&);
    This& operator=(const This&);
public:
    FogTemplateManager(FogEntity& primaryEntity);
    FogTemplateManager(FogEntity& primaryEntity, FogMakerContext& makerContext);
    FogTemplateManager(FogMakeTemplateContext& makeTemplateContext);
    const FogTemplateArgs& arguments() const { return *_arguments; }
    void check_classification(const FogMakeTemplateContext& makeTemplateContext) const;
    void create_usages(FogUsageContext& usageContext);
    void derive_from(FogScope& derivedScope, FogBaseClass& baseClass);
    void derive_from(FogScope& derivedScope, FogFunction& baseFunction, FogPotentialDeclaration *potentialDeclaration);
    void derive_from(FogScope& derivedScope, FogTypedef& aTypedef, FogPotentialDeclaration *potentialDeclaration);
    void derive_from(FogScope& derivedScope, FogVariable& aVariable, FogPotentialDeclaration *potentialDeclaration);
    void destroy();
    bool do_compile(FogCompileContext& inScope);
    bool do_derive(FogDeriveContext& inScope);
    void emit_prefix(FogEmitContext& emitContext) const;
    void emit_suffix(FogEmitContext& emitContext) const;
    FogEntity *find_template(FogMakeTemplateContext& makeTemplateContext);
    bool has_distinct_primary() const
        { return (_classification != NON_TEMPLATE) && (_classification != PRIMARY_TEMPLATE); }
    void install(FogEntity& anEntity);
    void install_parameters(FogMakeEntityContext& makeEntityContext);
    bool is_explicit_specialisation() const { return _classification == EXPLICIT_SPECIALISATION; }
    bool is_instantiable() const { return !is_specialisable(); }
    bool is_implicit_instantiation() const { return _classification == IMPLICIT_INSTANTIATION; }
    bool is_instantiation() const { return is_implicit_instantiation(); }
    bool is_partial_specialisation() const { return _classification == PARTIAL_SPECIALISATION; }
    bool is_primary() const { return _classification == PRIMARY_TEMPLATE; }
    bool is_specialisable() const
        { return (_classification == PRIMARY_TEMPLATE) || (_classification == PARTIAL_SPECIALISATION); }
    bool is_specialisation() const
        { return (_classification == PARTIAL_SPECIALISATION) || (_classification == IMPLICIT_INSTANTIATION); }
    bool is_templated() const { return _classification != NON_TEMPLATE; }
    FogScopeEnums::IsUsingTemplate is_using() const;
    FogEntity *make_template(FogMakeTemplateContext& makeTemplateContext);
    const FogTemplateParameterSpecifiers& parameter_specifiers() const;
    FogEntity& primary() { return _primary; }
    const FogEntity& primary() const { return _primary; }
    virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
    char print_prefix(std::ostream& s, char tailChar) const;
    char print_suffix(std::ostream& s, char tailChar) const;
    virtual std::ostream& print_this(std::ostream& s) const;
    const FogTemplateArgs& template_arguments() const { return *_arguments; }
    void validate() const;
private:
    static const char *classification(Classification aClassification);
    static Classification classify_primary(const FogMakerContext& makerContext);
    static Classification classify_secondary(const FogMakeTemplateContext& makeTemplateContext);
};
#endif
 
