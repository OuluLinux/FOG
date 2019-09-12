
#ifndef FOGFAILURE_HXX
#define FOGFAILURE_HXX



class FogFailure : public FogName
{
    NEEDS_A_FRIEND_FOR_GNUC
    typedef FogFailure This;
    typedef FogName Super;
    TYPEDECL_SINGLE(This, Super)
    FOGTOKEN_SHAREDLEAF_DECLS
private:
    FogFailure(const This&);
    FogFailure& operator=(const This&);
private:
    FogFailure();
    This& mutate() const { return *(This *)this; }
public:
    virtual bool compile_parsed(FogParseContext& parseContext);
    virtual bool emit(FogEmitContext& emitContext) const;
    virtual void find_entities(FogEntityFinder& theFinder);
    virtual void find_entities_in(FogScopeContext& inScope, FogEntityFinding& theFinding) const;
    virtual FogEntity *find_entity_in(FogScopeContext& inScope, FindStrategy aStrategy) const;
    virtual const FogMetaArgument *find_formal_in(FogScopeContext& scopeContext) const;
    virtual bool find_meta_entity_in(FogTokenRef& returnValue, FogScopeContext& inScope) const;
    virtual bool get_character(PrimIdHandle& returnId, FogScopeContext& inScope);
    virtual bool get_dyadic(FogTokenRef& returnValue, FogScopeContext& inScope,
        FogTokenType::TokenType dyadicOp, const FogToken& rightValue) const;
    virtual FogEntity *get_entity_in(FogScopeContext& inScope, FindStrategy aStrategy) const;
    virtual bool get_identifier(PrimIdHandle& returnId, FogScopeContext&);
    virtual bool get_meta_entity_in(FogTokenRef& returnValue, FogScopeContext& inScope) const;
    virtual bool get_monadic(FogTokenRef& returnValue, FogScopeContext& inScope,
        FogTokenType::TokenType monadicOp) const;
    virtual bool get_number_token(FogTokenRef& returnValue, FogScopeContext&) const;
    virtual bool get_string(PrimIdHandle& returnId, FogScopeContext& inScope);
    virtual FogScope& inner_scope();
    virtual FogFailure *is_failure();
    virtual const PrimId *is_resolved() const;
    virtual void make_built_in(FogNameRef& aName, const FogBuiltInTypeId& anId);
    virtual bool make_ctor_init(FogRawCtorInitRef& ctorInit);
    virtual FogEntity *make_entity(FogMakerContext& makerContext) const;
    virtual FogEntity *make_name_entity(FogMakeEntityContext& makeEntityContext) const;
    virtual FogEntity *make_type_entity(FogMakeEntityContext& makeEntityContext) const;
    virtual bool make_specifier(FogMakeSpecifierContext& makeSpecifierContext);
    virtual bool make_type_specifier(FogMakeSpecifierContext& makeSpecifierContext);
    virtual bool make_typed_expression(FogExprRef& theExpr, FogName& theType);
    virtual const FogMetaType& meta_type() const;
    virtual bool morph_to(FogTokenRef& returnValue, const FogMetaType& metaType, IsExposed isExposed,
        FogScopeContext& inScope) const;
    virtual char print_named(std::ostream& s, const PrimId *scopeId, char tailChar) const;
    virtual bool put_value(FogScopeContext& putScope, const FogToken& tokenValue, FogScopeContext& getScope) const;
    virtual bool resolve_id(PrimIdHandle& returnId, FogScopeContext& scopeContext) const;
    virtual bool resolve_slot(FogTokenRef& returnValue, FogScopeContext& scopeContext,
        const FogToken& objectValue) const;
    virtual void set_decl_specifier_expression(FogExprRef& anExpr, FogDeclSpecifier& declSpecifiers);
    virtual void set_decl_specifier_name(FogNameRef& aName, const FogDeclSpecifier& declSpecifiers);
    virtual void set_decl_specifier_raw(FogRawRef& aRaw, FogDeclSpecifier& declSpecifiers);
    virtual void set_destructor_expression(FogExprRef& anExpr);
    virtual void set_destructor_name(FogNameRef& aName);
    virtual NameSort sort_class() const;
    virtual size_t source_columns() const;
    virtual const FogTag& tag() const;
public:
    static FogName& make();
};
#endif
 
