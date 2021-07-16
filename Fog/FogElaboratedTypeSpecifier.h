
#ifndef FOGELABORATEDTYPESPECIFIER_HXX
#define FOGELABORATEDTYPESPECIFIER_HXX



class FogElaboratedTypeSpecifier : public FogScopeSpecifier
{
    typedef FogElaboratedTypeSpecifier This;
    typedef FogScopeSpecifier Super;
    TYPEDECL_SINGLE(This, Super)
private:
    virtual void set_new_utility(const FogUtility& aUtility);
protected:
    FogElaboratedTypeSpecifier(const This &thatSpecifier);
    virtual ~FogElaboratedTypeSpecifier();
    virtual FogToken *clone() const;
public:
    FogElaboratedTypeSpecifier(const FogTag& aTag, FogName& aName);
    virtual FogEntity *make_type_entity(FogMakeEntityContext& makeEntityContext) const;
    virtual const FogMetaType& meta_type() const;
    FogName& name() { return Super::name(); }
    const FogName& name() const { return Super::name(); }
    virtual std::ostream& print_viz(std::ostream& s) const;
    virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
};
#endif
 
