
#ifndef FOGMETAVARIABLESPECIFIER_HXX
#define FOGMETAVARIABLESPECIFIER_HXX



class FogMetaVariableSpecifier : public FogMetaObjectSpecifier
{
    typedef FogMetaObjectSpecifier Super;
    typedef FogMetaVariableSpecifier This;
    TYPEDECL_SINGLE(This, Super)
    FOGTOKEN_UNIQUE_DECLS

private:                                    //   Members for a variable
    FogExprRefToConst _initializer;

protected:
    virtual ~FogMetaVariableSpecifier();
public:
    FogMetaVariableSpecifier(FogMakeSpecifierContext& makeSpecifierContext, FogName& nameName);
    virtual FogMetaVariableSpecifier *is_meta_variable_specifier();
    virtual bool make_slot(FogMakerContext& makerContext) const;
    virtual const FogMetaType& meta_type() const;
    virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_viz(std::ostream& s) const;
    virtual void set_initializer(const FogExpr& anExpr);
};
#endif
 
