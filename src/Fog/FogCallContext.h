
#ifndef FOGCALLCONTEXT_HXX
#define FOGCALLCONTEXT_HXX



class FogCallContext : public FogDecoratedScopeContext
{
    typedef FogCallContext This;
    typedef FogDecoratedScopeContext Super;
    TYPEDECL_SINGLE(This, Super)
private:
    FogMetaFunctionEntity& _meta_function;            //   Invoked meta function
    const FogTokenListOfRef& _arguments;              //   Actual meta-function arguments.
private:
    FogCallContext(This& callContext);
    This& operator=(const This& aContext);
    This& mutate() const { return *(This *)this; }
public:
    FogCallContext(FogScopeContext& scopeContext, FogMetaFunctionEntity& metaFunctionEntity,
        const FogTokenListOfRef& actualArguments);
    virtual ~FogCallContext();
    FogScopeContext& calling_context() { return scope_context(); }
    const FogScopeContext& calling_context() const { return scope_context(); }
    virtual FogToken& dynamic_token();
    virtual const FogToken& dynamic_token() const;
    virtual FogCallContext *find_call_context();
    virtual FogMakeEntityContext *find_make_entity_context();
    virtual bool find_slots(FogMetaSlotFinder& theFinder);
    FogToken *get_argument(const FogMetaArgument& theArgument);
    FogBaseClass *get_base_class();
    FogEntity *get_entity();
    const FogExpr *get_expr_argument(size_t argumentNumber) const;
    FogFunction *get_function();
    const FogName *get_name_argument(size_t argumentNumber) const;
    FogScope *get_scope();
    FogSpecifier *get_specifier();
    bool get_string_argument(PrimIdHandle& aString, size_t argumentNumber);
    const FogToken *get_token_argument(size_t argumentNumber) const;
    virtual const FogScopeContext *meta_context() const;
    virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
    virtual Resolution resolution() const;
};
#endif
 
