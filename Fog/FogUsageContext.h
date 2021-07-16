
#ifndef FOGUSAGECONTEXT_HXX
#define FOGUSAGECONTEXT_HXX



class FogUsageContext : public FogScopeContext
{
    typedef FogScopeContext Super;
    typedef FogUsageContext This;
    TYPEDECL_SINGLE(This, Super)
private:
    FogUsageContext(const This& aContext);
    This& operator=(const This& aContext);
protected:
    FogUsageContext() {}
public:
    virtual FogEntity& entity() = 0;
    virtual const FogOfUseBy& of_use_by() const = 0;
};

//  
//    FogStaticUsageContext defines the concrete behaviour for anEntity ofUseBy something in staticToken.
//  
class FogStaticUsageContext : public FogUsageContext
{
    typedef FogUsageContext Super;
    typedef FogStaticUsageContext This;
    TYPEDECL_SINGLE(This, Super)
private:
    FogToken& _static_token;                //   Initial token scope.
    FogEntity& _entity;                     //   The entity making the usage.
    const FogOfUseBy& _of_use_by;           //   Required usage
public:
    FogStaticUsageContext(FogScope& aScope, FogEntity& anEntity, const FogOfUseBy& ofUseBy);
    virtual ~FogStaticUsageContext();
    virtual FogToken& dynamic_token();
    virtual const FogToken& dynamic_token() const;
    virtual FogEntity& entity();
    virtual bool find_slots(FogMetaSlotFinder& theFinder);
    virtual const FogTemplateParameterSpecifier *find_template_parameter_specifier(const PrimId& anId)
        { return Super::find_template_parameter_specifier(anId); }
    virtual const FogTemplateParameterSpecifier *find_template_parameter_specifier
                                                                    (const FogTemplateParameterSpecifierId& anId);
    virtual const FogOfUseBy& of_use_by() const;
    virtual FogToken& static_token();
    virtual const FogToken& static_token() const;
};

//  
//    FogDecoratedUsageContext defines the abstract behaviour for a FogUsageContext Decorator. 
//  
class FogDecoratedUsageContext : public FogUsageContext
{
    typedef FogUsageContext Super;
    typedef FogDecoratedUsageContext This;
    TYPEDECL_SINGLE(This, Super)
private:
    FogUsageContext& _context;              //   Context that this decorates.                                                 
protected:
    FogDecoratedUsageContext(FogUsageContext& usageContext);
public:
    virtual FogToken& dynamic_token();
    virtual const FogToken& dynamic_token() const;
    virtual FogEntity& entity();
    virtual FogScopeContext *find_context(size_t dollarCount);
    virtual bool find_slots(FogMetaSlotFinder& theFinder);
    virtual const FogTemplateParameterSpecifier *find_template_parameter_specifier(const PrimId& anId);
    virtual const FogTemplateParameterSpecifier *find_template_parameter_specifier
                                                                    (const FogTemplateParameterSpecifierId& anId);
    virtual const FogScopeContext *meta_context() const;
    virtual const FogOfUseBy& of_use_by() const;
    virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
    virtual Resolution resolution() const;
    virtual FogToken& static_token();
    virtual const FogToken& static_token() const;
    virtual const FogTemplateParameterSpecifiers *template_parameters() const;
};

//  
//    FogNestedUsageContext decorates a FogUsageContext to change the name resolution scope. 
//  
class FogNestedUsageContext : public FogDecoratedUsageContext
{
    typedef FogDecoratedUsageContext Super;
    typedef FogNestedUsageContext This;
    TYPEDECL_SINGLE(This, Super)
private:
    FogToken& _dynamic_token;               //   Adjusted token scope.
public:
    FogNestedUsageContext(FogUsageContext& usageContext, FogToken& inToken);
    virtual ~FogNestedUsageContext();
    virtual FogToken& dynamic_token();
    virtual const FogToken& dynamic_token() const;
    virtual bool find_slots(FogMetaSlotFinder& theFinder);
    virtual InScope in_scope() const;
};

//  
//    FogChangedUsageContext decorates a FogUsageContext to change the usage strategy.
//  
class FogChangedUsageContext : public FogDecoratedUsageContext
{
    typedef FogDecoratedUsageContext Super;
    typedef FogChangedUsageContext This;
    TYPEDECL_SINGLE(This, Super)
private:
    const FogOfUseBy& _of_use_by;
public:
    FogChangedUsageContext(FogUsageContext& usageContext, const FogOfUseBy& ofUseBy);
    virtual ~FogChangedUsageContext();
    virtual const FogOfUseBy& of_use_by() const;
};
#endif
 
