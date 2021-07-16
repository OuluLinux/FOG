
#ifndef FOGCTORINIT_HXX
#define FOGCTORINIT_HXX





class FogCtorInit : public FogToken
{
    typedef FogToken Super;
    typedef FogCtorInit This;
    TYPEDECL_SINGLE(This, Super)
    PRIMREF_DERIVED_DECLS(This)
    FOGTOKEN_UNIQUE_DECLS
private:
    const FogEntityRefToConst _entity;          //   Constructed entity.
    const PrimIdHandle _id;                     //   Its name.
    const FogExprRefToConst _inits;             //   Initialisers.
private:
    FogCtorInit(const This& anInit);
    FogCtorInit& operator=(const This& anInit);
protected:
    FogCtorInit();
    virtual ~FogCtorInit();
public:
    FogCtorInit(const FogEntity& anEntity, const FogExpr& anInit);
    virtual void create_usage(FogUsageContext& usageContext) const;
    virtual bool emit(FogEmitContext& emitContext) const;
    const PrimId& id() const { return *_id; }
    const FogExpr& inits() const { return *_inits; }
    virtual std::ostream& print_this(std::ostream& s) const;
    virtual std::ostream& print_viz(std::ostream& s) const;
};
#endif
 
