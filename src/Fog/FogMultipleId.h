
#ifndef FOGMULTIPLEID_HXX
#define FOGMULTIPLEID_HXX




//  .bugbug const FogElementType& _element_type clearly discriminates subclasses
class FogMultipleId : public FogWordName
{
    typedef FogWordName Super;
    typedef FogMultipleId This;
    TYPEDECL_SINGLE(This, Super)
    FOGTOKEN_DERIVED_DECLS
    FOGTOKEN_MEMBER_DECLS
    FOGTOKEN_LEAF_DECLS
private:
    enum Resolution { NOT_YET_RESOLVED_GLOBALLY, GLOBALLY_RESOLVED, CANNOT_RESOLVE_GLOBALLY, LOCALLY_RESOLVED };
private:
    FogTokenListOfRef _tokens;                      //   The concatenation.
    const FogElementType& _element_type;            //   The concatenated type.
    mutable PrimIdHandle _resolved_id;
    mutable Resolution _resolved;                   //   Set true once a context independent resolve has been made.
    const FogToken *_resolved_in;                   //   Diagnostic cache of where resolved - null context if global
    bool _has_at;                                   //   True of an @-expression needs resolution.
    bool _has_dollar;                               //   True of a $-expression needs resolution.
private:
    FogMultipleId(const This& thatName);
    This& mutate() const { return *(This *)this; }
    void resolve_globally();
protected:
    virtual ~FogMultipleId();
//    virtual void make_actual_token(FogTokenRefToConst& actualToken, FogMakeActualContext& makeActualContext) const;
public:
    FogMultipleId(const FogElementType& elementType);
    void add(FogToken& aToken);
    virtual bool get_identifier(PrimIdHandle& returnId, FogScopeContext& inScope);
    virtual bool get_meta_entity_in(FogTokenRef& returnValue, FogScopeContext& inScope) const;
    virtual bool get_object(FogTokenRef& returnValue, FogScopeContext& inScope) const;
    virtual bool has_at() const;
    virtual bool has_dollar() const;
    virtual const PrimId *is_resolved() const;
    virtual const FogTemplateParameterSpecifier *is_unnormalised_template_parameter
        (FogScopeContext& scopeContext) const;
    virtual bool is_wide() const;
    virtual bool make_specifier(FogMakeSpecifierContext& makeSpecifierContext);
    virtual const FogMetaType& meta_type() const;
    virtual FogTokenType::TokenType pp_token_type_enum() const;
    virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_viz(std::ostream& s) const;
    virtual bool resolve_id(PrimIdHandle& returnId, FogScopeContext& inScope) const;
    virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
    virtual NameSort sort_class() const;
    virtual FogTokenType::TokenType token_type_enum() const;
};
#endif
 
