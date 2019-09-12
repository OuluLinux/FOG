
#ifndef FOGDECLSPECIFIEDNAME_HXX
#define FOGDECLSPECIFIEDNAME_HXX




class FogDeclSpecifiedName : public FogDecoratedName
{
 NEEDS_A_FRIEND_FOR_GNUC
    typedef FogDecoratedName Super;
    typedef FogDeclSpecifiedName This;
    TYPEDECL_SINGLE(This, Super)
//    FOGTOKEN_MEMBER_DECLS
    FOGTOKEN_LEAF_DECLS
private:
    FogDeclSpecifierValue _decl_specifiers;
private:
    virtual ~FogDeclSpecifiedName();
    This& mutate() const { return *(This *)this; }
public:
    FogDeclSpecifiedName(FogName& aName);
    FogDeclSpecifiedName(FogName& aName, const FogDeclSpecifierValue& declSpecifiers);
    virtual const FogDeclSpecifierValue& decl_specifiers() const;
    virtual bool emit(FogEmitContext& emitContext) const;
    virtual bool make_specifier(FogMakeSpecifierContext& makeSpecifierContext);
    virtual bool make_type_specifier(FogMakeSpecifierContext& makeSpecifierContext);
    virtual char print_named(std::ostream& s, const PrimId *fullId, char tailChar) const;
    virtual std::ostream& print_viz(std::ostream& s) const;
    virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
    virtual void set_decl_specifier_name(FogNameRef& aName, const FogDeclSpecifier& declSpecifiers);
    virtual void set_destructor_name(FogNameRef& aName);
};
#endif
 
