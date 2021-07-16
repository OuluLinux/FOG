
#ifndef FOGMAKESPECIFIERCONTEXT_HXX
#define FOGMAKESPECIFIERCONTEXT_HXX





class FogBaseMakeSpecifierContext;

class FogMakeSpecifierContext : public FogDecoratedScopeContext
{
    typedef FogMakeSpecifierContext This;
    typedef FogDecoratedScopeContext Super;
    TYPEDECL_SINGLE(This, Super)
private:
    FogMakeSpecifierContext& operator=(const This& aContext);
    This& mutate() const { return *(This *)this; }
protected:
    FogMakeSpecifierContext(FogScopeContext& scopeContext) : Super(scopeContext) {}
    virtual ~FogMakeSpecifierContext() {}
public:
    const FogAccess& access() const;
    virtual FogBaseMakeSpecifierContext& base_make_specifier_context() = 0;
    const FogBaseMakeSpecifierContext& base_make_specifier_context() const
        { return mutate().base_make_specifier_context(); }
    bool compile_specifier();
    bool compile_type_specifier();
    const FogDeclSpecifierValue& decl_specifiers() const;
    const FogRaw& declaration() const;
    FogFunctionSpecifier *get_function_specifier(const FogToken& inToken);
    FogMetaFunctionSpecifier *get_meta_function_specifier(const FogToken& inToken);
    FogMetaParameterSpecifier *get_meta_parameter_specifier(const FogToken& inToken);
    FogObjectSpecifier *get_object_specifier(const FogToken& inToken);
    FogParameterSpecifier *get_parameter_specifier(const FogToken& inToken);
    FogTemplateParameterSpecifier *get_template_parameter_specifier(const FogToken& inToken);
    bool is_nested() const;
    FogLine& line() const;
    bool make_specifier(FogRaw& aSpecifier);
    FogParseContext& parse_context();
    const FogParseContext& parse_context() const { return mutate().parse_context(); }
    virtual std::ostream& print_viz(std::ostream& s) const;
    virtual Resolution resolution() const;
    virtual bool resolve_template(FogEntity& primaryEntity, FogTemplateArgsRefToConst& templateArgs);
    const FogMakeSemantics& semantics() const;
    void set_nested_scope(FogToken& inToken);
    FogSpecifier& specifier();
    const FogSpecifier& specifier() const { return mutate().specifier(); }
    FogTargetFile *target_file() const;
    const FogUtility& utility() const;
};

//  
//    Constructed by FogDerivable::make_used, FogExpr::compile_parsed, FogListOfExpr::compile_parsed
//    FogListOfExpr::make_parameters, FogMetaFunctionExpr::make_specifier, FogRawTemplate::compile_parsed
//  
class FogBaseMakeSpecifierContext : public FogMakeSpecifierContext
{
    typedef FogBaseMakeSpecifierContext This;
    typedef FogMakeSpecifierContext Super;
    TYPEDECL_SINGLE(This, Super)
private:
                                                    //   Defining context
    const FogMakeSemantics _semantics;                          //   Semantics for made specifier.
    const FogDeclSpecifierValue _decl_specifiers;               //   Corresponding decl-specifiers
private:
                                                    //   Working context
    FogToken *_token;                                           //   Scope of made specifier.
    FogSpecifierRef _specifier;                                 //   The made specifier
private:
    This& operator=(const This& aContext);
    This& mutate() const { return *(This *)this; }
public:
    FogBaseMakeSpecifierContext(FogParseContext& parseContext, const FogSemanticsContext& theSemantics,
        const FogDeclSpecifierValue& declSpecifiers, FogMakeSemantics::Context aContext);
    FogBaseMakeSpecifierContext(FogMakeSpecifierContext& makeSpecifierContext,
        const FogDeclSpecifierValue& declSpecifiers, FogMakeSemantics::Semantics makeSemantics);
    virtual ~FogBaseMakeSpecifierContext();
    bool adopt_specifier(FogSpecifier *aSpecifier);
    virtual FogBaseMakeSpecifierContext& base_make_specifier_context();
    const FogDeclSpecifierValue& decl_specifiers() const { return _decl_specifiers; }
    virtual FogToken& dynamic_token();
    virtual const FogToken& dynamic_token() const;
    virtual bool find_slots(FogMetaSlotFinder& theFinder);
    bool is_nested() const;
    bool make_specifier(FogRaw& aSpecifier);
    FogParseContext& parse_context() { return (FogParseContext&)scope_context(); }
    virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
    const FogMakeSemantics& semantics() const { return _semantics; }
    void set_nested_scope(FogToken& inToken);
    FogSpecifier& specifier() { return *_specifier; }
    const FogSpecifier& specifier() const { return mutate().specifier(); }
    virtual const FogTemplateParameterSpecifiers *template_parameters() const;
};

class FogDecoratedMakeSpecifierContext : public FogMakeSpecifierContext
{
    typedef FogMakeSpecifierContext Super;
    typedef FogDecoratedMakeSpecifierContext This;
    TYPEDECL_SINGLE(This, Super)
private:
    This& operator=(const This& scopeContext);
protected:
    FogDecoratedMakeSpecifierContext(FogMakeSpecifierContext& scopeContext) : Super(scopeContext) {}
    FogDecoratedMakeSpecifierContext(This& scopeContext) : Super(scopeContext) {}
    FogMakeSpecifierContext& make_specifier_context() { return (FogMakeSpecifierContext&)scope_context(); }
    const FogMakeSpecifierContext& make_specifier_context() const
        { return (const FogMakeSpecifierContext&)scope_context(); }
public:
    virtual FogBaseMakeSpecifierContext& base_make_specifier_context();
};

//  
//    A FogMetaParseContext decorates a FogParseContext to raise name resolution to the meta-level.
//    From FogDerivedClause::compile_parsed, FogDoWhileStatement::compile_parsed, etc.
//  
class FogMetaMakeSpecifierContext : public FogDecoratedMakeSpecifierContext
{
    typedef FogDecoratedMakeSpecifierContext Super;
    typedef FogMetaMakeSpecifierContext This;
    TYPEDECL_SINGLE(This, Super)
private:
    FogMetaMakeSpecifierContext(const This& makeSpecifierContext);
    This& operator=(const This& makeSpecifierContext);
public:
    FogMetaMakeSpecifierContext(FogMakeSpecifierContext& makeSpecifierContext);
    virtual const FogScopeContext *meta_context() const;
};

//  
//  	A FogTemplatedScopeContext decorates a FogScopeContext to establish template arguments.
//  
class FogTemplatedMakeSpecifierContext : public FogDecoratedMakeSpecifierContext
{
 typedef FogDecoratedMakeSpecifierContext Super;
 typedef FogTemplatedMakeSpecifierContext This;
 TYPEDECL_SINGLE(This, Super)
private:
 const FogTemplatedName& _templated_name;    //   Raw template argument expressions.
private:
 FogTemplatedMakeSpecifierContext(const This& scopeContext);
 This& operator=(const This& scopeContext);
public:
 FogTemplatedMakeSpecifierContext(FogMakeSpecifierContext& makerContext, const FogTemplatedName& templatedName);
 virtual void find_entities(FogEntityFinder& theFinder);
 virtual std::ostream& print_resolution(std::ostream& s) const;
    virtual bool resolve_template(FogEntity& primaryEntity, FogTemplateArgsRefToConst& templateArgs);
};
#endif
 
