
#ifndef FOGRAWCTORINIT_HXX
#define FOGRAWCTORINIT_HXX




class FogRawCtorInit : public FogToken
{
    typedef FogToken Super;
    typedef FogRawCtorInit This;
    TYPEDECL_SINGLE(This, Super)
    PRIMREF_DERIVED_DECLS(This)
    FOGTOKEN_UNIQUE_DECLS
private:
    const FogNameRefToConst _name;              //   Parsed name.
    const FogExprRefToConst _inits;             //   Initialisers.
private:
    FogRawCtorInit(const This& anInit);
    FogRawCtorInit& operator=(const This& anInit);
private:
    This& mutate() const { return *(This *)this; }
protected:
    FogRawCtorInit();
    virtual ~FogRawCtorInit();
public:
    FogRawCtorInit(FogName& aName, const FogExpr *initList);
    void compile_ctor(FogMakeEntityContext& makeEntityContext, FogCtorInitMapOfRefToConst& ctorInits) const;
    virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_this(std::ostream& s) const;
    virtual std::ostream& print_viz(std::ostream& s) const;
};
#endif
 
