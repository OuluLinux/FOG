
#ifndef FOGSEMANTICSCONTEXT_HXX
#define FOGSEMANTICSCONTEXT_HXX




class FogSemanticsContext : public FogDecoratedScopeContext, public FogSemantics
{
    typedef FogSemanticsContext This;
    typedef FogDecoratedScopeContext Super;
    TYPEDECL_SINGLE(This, Super)
public:
    enum DiagnosticBits
    {
        NO_DIAGNOSTIC = 0,
        BAD_IDENTIFIER = 1 << 0,
        BAD_TEMPLATE = 1 << 1
    };
private:
    This& operator=(const This& aContext);
protected:
    FogSemanticsContext(FogScopeContext& scopeContext) : Super(scopeContext) {}
    FogSemanticsContext(FogSemanticsContext& scopeContext) : Super(scopeContext) {}
    virtual ~FogSemanticsContext() {}
public:
    void assign(const FogSemantics& someSemantics) { FogSemantics::operator=(someSemantics); }
    virtual std::ostream& print_this(std::ostream& s) const;
    virtual void set_diagnostic(DiagnosticBits aDiaganotic);
};

//  
//    A FogChildSemanticsContext establishes conditions for resolution of the semantics of a sub-tree..
//  
//    Constructed by Fog*Expr::resolve_semantics amongst many others
//  
class FogChildSemanticsContext : public FogSemanticsContext
{
    typedef FogSemanticsContext Super;
    typedef FogChildSemanticsContext This;
    TYPEDECL_SINGLE(This, Super)
private:
    FogChildSemanticsContext(const This& scopeContext);
    This& operator=(const This& scopeContext);
    FogSemanticsContext& semantics_context() { return (FogSemanticsContext&)scope_context(); }
public:
    FogChildSemanticsContext(FogSemanticsContext& scopeContext);
    virtual void set_diagnostic(DiagnosticBits aDiaganotic);
};

//  
//    A FogMetaSemanticsContext decorates a FogSemanticsContext to enable resolution at the meta-level.
//  
//    Constructed by FogExpressionStatement::resolve_semantics.
//  
class FogMetaSemanticsContext : public FogSemanticsContext
{
    typedef FogSemanticsContext Super;
    typedef FogMetaSemanticsContext This;
    TYPEDECL_SINGLE(This, Super)
private:
    FogMetaSemanticsContext(const This& scopeContext);
    This& operator=(const This& scopeContext);
    FogSemanticsContext& semantics_context() { return (FogSemanticsContext&)scope_context(); }
public:
    FogMetaSemanticsContext(FogSemanticsContext& scopeContext);
    virtual const FogScopeContext *meta_context() const;
    virtual void set_diagnostic(DiagnosticBits aDiaganotic);
};

//  
//    A FogNestedSemanticsContext decorates a FogSemanticsContext to establish a nested scope.
//  
//    Constructed by FogGlobalName::resolve_semantics, FogNestedName::resolve_semantics.
//  
class FogNestedSemanticsContext : public FogSemanticsContext
{
    typedef FogSemanticsContext Super;
    typedef FogNestedSemanticsContext This;
    TYPEDECL_SINGLE(This, Super)
private:
    FogToken& _dynamic_token;               //   Current token.
    const InScope _in_scope;                //   Scoping with which a find is to be resolved.
private:
    FogNestedSemanticsContext(const This& scopeContext);
    This& operator=(const This& scopeContext);
    FogSemanticsContext& semantics_context() { return (FogSemanticsContext&)scope_context(); }
public:
    FogNestedSemanticsContext(FogSemanticsContext& scopeContext, FogToken& inToken);
    virtual FogToken& dynamic_token();
    virtual const FogToken& dynamic_token() const;
    virtual bool find_slots(FogMetaSlotFinder& theFinder);
    virtual InScope in_scope() const;
    virtual void set_diagnostic(DiagnosticBits aDiaganotic);
};

//  
//    A FogStrongSemanticsContext establishes conditions for strong resolution of semantics..
//  
//    Constructed by
//  
class FogStrongSemanticsContext : public FogSemanticsContext
{
    typedef FogSemanticsContext Super;
    typedef FogStrongSemanticsContext This;
    TYPEDECL_SINGLE(This, Super)
private:
    FogStrongSemanticsContext(const This& scopeContext);
    This& operator=(const This& scopeContext);
public:
    FogStrongSemanticsContext(FogScopeContext& scopeContext);
    virtual FogScopeContext::Resolution resolution() const;
};

//  
//    A FogWeakSemanticsContext establishes conditions for weak resolution of semantics..
//  
//    Constructed by FogExpr::compile_parsed.
//  
class FogWeakSemanticsContext : public FogSemanticsContext
{
    typedef FogSemanticsContext Super;
    typedef FogWeakSemanticsContext This;
    TYPEDECL_SINGLE(This, Super)
private:
    DiagnosticBits _diagnostics;
private:
    FogWeakSemanticsContext(const This& scopeContext);
    This& operator=(const This& scopeContext);
public:
    FogWeakSemanticsContext(FogScopeContext& scopeContext);
    const char *reason() const;
    virtual FogScopeContext::Resolution resolution() const;
    virtual void set_diagnostic(DiagnosticBits aDiaganotic);
};
#endif
 
