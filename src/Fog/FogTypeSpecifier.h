
#ifndef FOGTYPESPECIFIER_HXX
#define FOGTYPESPECIFIER_HXX




class FogTypeSpecifier : public FogScopeSpecifier
{
    typedef FogScopeSpecifier Super;
    typedef FogTypeSpecifier This;
    TYPEDECL_SINGLE(This, Super)
    FOGTOKEN_READ_ONLY_DECLS
private:
    const FogAccessHandle _nested_access;           //   Default accessibility of nested declarations.
    FogRawRef _declarations;                        //   The member declarations.
private:
    This& mutate() const { return *(This *)this; }
protected:
    FogTypeSpecifier(FogName *aName);
    FogTypeSpecifier(const FogTag& aTag, FogName& aName, const FogAccess& nestedAccess, FogRaw *rawDeclarations = 0);
    virtual ~FogTypeSpecifier();
    void make_body(FogMakeEntityContext& makeEntityContext, FogScope& aScope) const;
public:
    virtual const FogMetaType& meta_type() const;
    virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
    void set_declarations(FogRaw *someDeclarations) { _declarations = someDeclarations; }
};
#endif
 
