
#ifndef FOGSCOPECONTEXT_HXX
#define FOGSCOPECONTEXT_HXX




class FogScopeContext : public PrimPrint, public FogScopeEnums
{
    typedef PrimPrint Super;
    typedef FogScopeContext This;
    TYPEDECL_SINGLE(This, Super)
    
    const This* parent_scope = NULL;
public:
    enum Resolution
    {
        RESOLVE_ATS,                    //   Everything must be resolved
        RESOLVE_DOLLARS,                //   All $-expressions must be resolved, @-expression must not be resolved.
        RESOLVE_MULTI_DOLLARS,          //   $-expressions referring to formals and multi-$ expressions only
        RESOLVE_META_FUNCTION_FORMALS,  //   $-expressions referring to formals and multi-$ expressions only
        LOCATE_META_FUNCTION_FORMALS,   //   $-expressions referring to formals to be identified
        LOCATE_TEMPLATE_FORMALS,        //   Formal template parameters should be normalised
        REPAIR_SEMANTICS,               //   Repair template/arithmetic misparses
        RESOLVE_TYPE_STRONGLY,          //   Need to know meta-type preciselyto resolve_semantics
        RESOLVE_TYPE_WEAKLY,            //   Use simple potential semantics for meta-types
        RESOLVE_USAGE,                  //   Resolve during create_usage
        RESOLVE_EMISSION,               //   Resolve during emit
        RESOLVE_LAZY,                   //   Resolve whatever is possible for constant folding
        RESOLVE_MASK = RESOLVE_ATS | RESOLVE_DOLLARS | RESOLVE_MULTI_DOLLARS | RESOLVE_META_FUNCTION_FORMALS
                     | LOCATE_META_FUNCTION_FORMALS | LOCATE_TEMPLATE_FORMALS | REPAIR_SEMANTICS
                     | RESOLVE_TYPE_STRONGLY | RESOLVE_TYPE_WEAKLY | RESOLVE_USAGE | RESOLVE_EMISSION | RESOLVE_LAZY,
    //  
        SCOPE_KNOWN = 0,
        SCOPE_UNKNOWN = 16,
        SCOPE_MASK = SCOPE_KNOWN | SCOPE_UNKNOWN
    };

private:
    This& operator=(const This& scopeContext);
    This& mutate() const { return *(This *)this; }

protected:
    FogScopeContext() {}
    FogScopeContext(const This& scopeContext) : Super(scopeContext), parent_scope(&scopeContext) {}
    FogScopeContext(const This& scopeContext, int useless___no_super_scope) : parent_scope(&scopeContext) {}

public:
	const This* get_parent() const {return parent_scope;}
	
    bool at_meta() const { return meta_context() != 0; }
    virtual bool at_root() const;
    FogEntity& dynamic_entity();
    FogScope& dynamic_scope();
    const FogScope& dynamic_scope() const;
    virtual FogToken& dynamic_token() = 0;
    virtual const FogToken& dynamic_token() const = 0;
    FogFileManager& file_manager();
    virtual FogCallContext *find_call_context();
    const FogCallContext *find_call_context() const { return mutate().find_call_context(); }
    virtual FogScopeContext *find_context(size_t dollarCount);
    const FogScopeContext *find_context(size_t dollarCount) const { return mutate().find_context(dollarCount); }
    virtual void find_entities(FogEntityFinder& theFinder);
    virtual bool find_formal_slots(FogMetaSlotFinder& theFinder);
    virtual FogMakeEntityContext *find_make_entity_context();
    const FogMakeEntityContext *find_make_entity_context() const { return mutate().find_make_entity_context(); }
    virtual bool find_slots(FogMetaSlotFinder& theFinder) = 0;
    virtual const FogTemplateParameterSpecifier *find_template_parameter_specifier(const PrimId& anId);
    virtual const FogTemplateParameterSpecifier *find_template_parameter_specifier
                                                                    (const FogTemplateParameterSpecifierId& anId);
    FogRoot& global_scope();
    virtual InScope in_scope() const;
    virtual FogEmitContext *is_emit_context();
    const FogEmitContext *is_emit_context() const { return mutate().is_emit_context(); }
    bool is_locate_formals() const { return (resolution() & RESOLVE_MASK) == LOCATE_META_FUNCTION_FORMALS; }
    bool is_normalise_templates() const { return (resolution() & RESOLVE_MASK) == LOCATE_TEMPLATE_FORMALS; }
    bool is_repair_semantics() const { return (resolution() & RESOLVE_MASK) == REPAIR_SEMANTICS; }
    bool is_resolve_ats() const { return (resolution() & RESOLVE_MASK) == RESOLVE_ATS; }
    bool is_resolve_dollars() const { return (resolution() & RESOLVE_MASK) == RESOLVE_DOLLARS; }
    bool is_resolve_emission() const { return (resolution() & RESOLVE_MASK) == RESOLVE_EMISSION; }
    bool is_resolve_formals() const { return (resolution() & RESOLVE_MASK) == RESOLVE_META_FUNCTION_FORMALS; }
    bool is_resolve_lazy() const { return (resolution() & RESOLVE_MASK) == RESOLVE_LAZY; }
    bool is_resolve_multi_dollars() const { return (resolution() & RESOLVE_MASK) == RESOLVE_MULTI_DOLLARS; }
    bool is_resolve_type_strongly() const { return (resolution() & RESOLVE_MASK) == RESOLVE_TYPE_STRONGLY; }
    bool is_resolve_type_weakly() const { return (resolution() & RESOLVE_MASK) == RESOLVE_TYPE_WEAKLY; }
    bool is_resolve_usage() const { return (resolution() & RESOLVE_MASK) == RESOLVE_USAGE; }
    bool is_unresolvable() const;
    virtual const FogScopeContext *meta_context() const;
    virtual std::ostream& print_resolution(std::ostream& s) const;
    virtual std::ostream& print_this(std::ostream& s) const;
    virtual std::ostream& print_viz(std::ostream& s) const;
    bool requires_formal_location_or_resolution() const;
    bool requires_resolution() const;
    virtual Resolution resolution() const;
    FogScope& static_scope();
    const FogScope& static_scope() const;
    virtual FogToken& static_token() = 0;
    virtual const FogToken& static_token() const = 0;
    virtual const FogTemplateParameterSpecifiers *template_parameters() const;
    const This& to_const() const { return *this; }

public:
    static FogScopeContext& null();
    static FogScopeContext& weak();
    
    
    // Piggypack variables
    FogFunction* function = NULL;
};

//  
//  .bugbug Just about every instance of this class is a bug.
//  
class FogStaticScopeContext : public FogScopeContext
{
    typedef FogScopeContext Super;
    typedef FogStaticScopeContext This;
    TYPEDECL_SINGLE(This, Super)
private:
                                        //   Defining contents.
    FogToken& _static_token;                        //   Current token.
    const InScope _in_scope;                        //   Scoping with which a find is to be resolved.
private:
    This& operator=(const This& scopeContext);
public:
    FogStaticScopeContext(FogToken& aToken, InScope inScope = IN_ANY_SCOPE);
    virtual FogToken& dynamic_token();
    virtual const FogToken& dynamic_token() const;
    virtual bool find_slots(FogMetaSlotFinder& theFinder);
    virtual InScope in_scope() const;
    virtual FogToken& static_token();
    virtual const FogToken& static_token() const;
};

class FogDecoratedScopeContext : public FogScopeContext
{
    typedef FogScopeContext Super;
    typedef FogDecoratedScopeContext This;
    TYPEDECL_SINGLE(This, Super)
private:
    FogScopeContext& _context;                                              
private:
    This& operator=(const This& scopeContext);
protected:
    FogDecoratedScopeContext(FogScopeContext& scopeContext) : _context(scopeContext), Super(scopeContext, 123) {}
    FogDecoratedScopeContext(This& scopeContext) : _context(scopeContext), Super(scopeContext, 123) {}
public:
    FogScopeContext& scope_context() { return _context; }
    const FogScopeContext& scope_context() const { return _context; }
    virtual bool at_root() const;
    virtual FogToken& dynamic_token();
    virtual const FogToken& dynamic_token() const;
    virtual FogCallContext *find_call_context();
    virtual FogScopeContext *find_context(size_t dollarCount);
    virtual bool find_formal_slots(FogMetaSlotFinder& theFinder);
    virtual FogMakeEntityContext *find_make_entity_context();
    virtual bool find_slots(FogMetaSlotFinder& theFinder);
    virtual const FogTemplateParameterSpecifier *find_template_parameter_specifier(const PrimId& anId);
    virtual const FogTemplateParameterSpecifier *find_template_parameter_specifier
                                                                    (const FogTemplateParameterSpecifierId& anId);
    virtual InScope in_scope() const;
    virtual const FogScopeContext *meta_context() const;
    virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
    virtual Resolution resolution() const;
    virtual FogToken& static_token();
    virtual const FogToken& static_token() const;
    virtual const FogTemplateParameterSpecifiers *template_parameters() const;
};

//  
//    FogBracedScopeContext defines the nested context to maintain the nested lexical scope
//    established by FogMetaFunctionExpr::make_actual, FogScopedExpr::make_actual.
//  
class FogBracedScopeContext : public FogDecoratedScopeContext
{
    typedef FogDecoratedScopeContext Super;
    typedef FogBracedScopeContext This;
    TYPEDECL_SINGLE(This, Super)
private:
    FogToken& _dynamic_token;
public:
    FogBracedScopeContext(FogScopeContext& scopeContext, FogToken& aToken);
    virtual FogToken& dynamic_token();
    virtual const FogToken& dynamic_token() const;
    virtual FogScopeContext *find_context(size_t dollarCount);
    virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
};

//  
//    A FogInScopeContext decorates a FogScopeContext to revise the name resolution scope.
//  
class FogInScopeContext : public FogDecoratedScopeContext
{
    typedef FogDecoratedScopeContext Super;
    typedef FogInScopeContext This;
    TYPEDECL_SINGLE(This, Super)
private:
    const InScope _in_scope;                        //   Scoping with which a find is to be resolved.
private:
    FogInScopeContext(const This& scopeContext);
    This& operator=(const This& scopeContext);
public:
    FogInScopeContext(FogScopeContext& scopeContext, InScope inScope);
    virtual InScope in_scope() const;
};

//  
//    A FogMergeContext decorates a FogScopeContext to capture additional merge context.
//  
class FogMergeContext : public FogDecoratedScopeContext
{
    typedef FogDecoratedScopeContext Super;
    typedef FogMergeContext This;
    TYPEDECL_SINGLE(This, Super)
private:
    bool _left_is_anon;
    bool _right_is_anon;
private:
    FogMergeContext(const This& scopeContext);
    This& operator=(const This& scopeContext);
public:
    FogMergeContext(FogScopeContext& scopeContext, bool leftIsAnon = false, bool rightIsAnon = false);
    bool left_is_anon() const { return _left_is_anon; }
    bool right_is_anon() const { return _right_is_anon; }
};

//  
//    A FogMetaScopeContext decorates a FogScopeContext to enable or disable name resolution at the meta-level.
//  
//    Constructed for isMeta by
//            FogArgumentId::emit, FogArgumentId::get_object,
//            FogDerivedClause::derive_at,
//            FogExpr::compile_parsed,
//            FogMetaLiteral::morph_to,
//            FogTreeLiteral::emit, FogTreeLiteral::get_object
//    Constructed for !isMeta by
//            FogMetaIteratorEntity::set_value
//  
class FogMetaScopeContext : public FogDecoratedScopeContext
{
    typedef FogDecoratedScopeContext Super;
    typedef FogMetaScopeContext This;
    TYPEDECL_SINGLE(This, Super)
private:
    const bool _is_meta;
private:
    FogMetaScopeContext(const This& scopeContext);
    This& operator=(const This& scopeContext);
public:
    FogMetaScopeContext(FogScopeContext& scopeContext, bool isMeta = true);
    virtual const FogScopeContext *meta_context() const;
};

//  
//    A FogMultiDollarScopeContext decorates a FogScopeContext to constrain resolution to multi-$ expressions..
//  
class FogMultiDollarScopeContext : public FogDecoratedScopeContext
{
    typedef FogDecoratedScopeContext Super;
    typedef FogMultiDollarScopeContext This;
    TYPEDECL_SINGLE(This, Super)
private:
    FogMultiDollarScopeContext(const This& scopeContext);
    This& operator=(const This& scopeContext);
public:
    FogMultiDollarScopeContext(FogScopeContext& scopeContext);
    virtual Resolution resolution() const;
};

//  
//    A FogNestedScopeContext decorates a FogScopeContext to establish a nested scope.
//  
class FogNestedScopeContext : public FogDecoratedScopeContext
{
    typedef FogDecoratedScopeContext Super;
    typedef FogNestedScopeContext This;
    TYPEDECL_SINGLE(This, Super)
private:
    FogToken& _dynamic_token;               //   Current token.
    const InScope _in_scope;                //   Scoping with which a find is to be resolved.
private:
    FogNestedScopeContext(const This& scopeContext);
    This& operator=(const This& scopeContext);
public:
    FogNestedScopeContext(FogScopeContext& scopeContext, FogToken& inToken);
    FogNestedScopeContext(FogScopeContext& scopeContext, FogToken& inToken, InScope inScope);
    virtual FogToken& dynamic_token();
    virtual const FogToken& dynamic_token() const;
    virtual bool find_slots(FogMetaSlotFinder& theFinder);
    virtual InScope in_scope() const;
};

//  
//    A FogPotentialDeclarationScopeContext decorates a FogScopeContext to specify the context
//    of a potential declaration and support the at_root() access...
//  
class FogPotentialDeclarationScopeContext : public FogNestedScopeContext
{
    typedef FogNestedScopeContext Super;
    typedef FogPotentialDeclarationScopeContext This;
    TYPEDECL_SINGLE(This, Super)
private:
    const bool _at_root;
private:
    FogPotentialDeclarationScopeContext(const This& scopeContext);
    This& operator=(const This& scopeContext);
public:
    FogPotentialDeclarationScopeContext(FogScopeContext& scopeContext,
        FogPotentialDeclaration& potentialDeclaration, bool atRoot);
    virtual bool at_root() const;
};

//  
//    A FogResolutionScopeContext decorates a FogScopeContext to specify  potentially weaker name resolution.
//  
class FogResolutionScopeContext : public FogDecoratedScopeContext
{
    typedef FogDecoratedScopeContext Super;
    typedef FogResolutionScopeContext This;
    TYPEDECL_SINGLE(This, Super)
private:
    const Resolution _resolution;
private:
    FogResolutionScopeContext(const This& scopeContext);
    This& operator=(const This& scopeContext);
public:
    FogResolutionScopeContext(FogScopeContext& scopeContext, Resolution aResolution);
    virtual Resolution resolution() const;
};

//  
//    A FogTemplatedScopeContext decorates a FogScopeContext to establish template arguments.
//  
class FogTemplatedScopeContext : public FogDecoratedScopeContext
{
    typedef FogDecoratedScopeContext Super;
    typedef FogTemplatedScopeContext This;
    TYPEDECL_SINGLE(This, Super)
private:
    const FogTemplatedName& _templated_name;                //   Raw template argument expressions.
private:
    FogTemplatedScopeContext(const This& scopeContext);
    This& operator=(const This& scopeContext);
public:
    FogTemplatedScopeContext(FogScopeContext& scopeContext, const FogTemplatedName& templatedName);
    virtual void find_entities(FogEntityFinder& theFinder);
    virtual std::ostream& print_resolution(std::ostream& s) const;
};

//  
//    FogUnresolvableFunctionMakeEntityContext defines the nested context to maintain the nested lexical scope
//    established by FogMetaFunctionExpr::make_actual, FogScopedExpr::make_actual, .
//  
class FogUnresolvableFunctionScopeContext : public FogDecoratedScopeContext
{
    typedef FogDecoratedScopeContext Super;
    typedef FogUnresolvableFunctionScopeContext This;
    TYPEDECL_SINGLE(This, Super)
private:
    const FogMetaFunctionExpr& _function_expr;
public:
    FogUnresolvableFunctionScopeContext(FogScopeContext& scopeContext, FogMetaFunctionExpr& anExpr);
    virtual bool find_formal_slots(FogMetaSlotFinder& theFinder);
    virtual bool find_slots(FogMetaSlotFinder& theFinder);
};

//  
//    FogUnresolvableTypeMakeEntityContext defines the nested context to maintain the nested lexical scope established
//    by FogTypeSpecifier::make_actual.
//  
class FogUnresolvableTypeScopeContext : public FogDecoratedScopeContext
{
    typedef FogDecoratedScopeContext Super;
    typedef FogUnresolvableTypeScopeContext This;
    TYPEDECL_SINGLE(This, Super)
private:
    const FogTypeSpecifier& _type_specifier;
public:
    FogUnresolvableTypeScopeContext(FogScopeContext& scopeContext, FogTypeSpecifier& typeSpecifier);
    virtual bool find_slots(FogMetaSlotFinder& theFinder);
};

//  
//    FogUnresolvableCallContext defines the nested context to maintain the nested lexical scope established
//    by FogMetaFunctionExpr::make_actual, FogScopedExpr::make_actual.
//  
class FogUnresolvableScopeContext : public FogDecoratedScopeContext
{
    typedef FogDecoratedScopeContext Super;
    typedef FogUnresolvableScopeContext This;
    TYPEDECL_SINGLE(This, Super)
private:
    const Resolution _resolution;
public:
    FogUnresolvableScopeContext(FogScopeContext& scopeContext);
    virtual Resolution resolution() const;
};
#endif
 
