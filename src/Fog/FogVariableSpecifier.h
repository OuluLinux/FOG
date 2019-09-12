
#ifndef FOGVARIABLESPECIFIER_HXX
#define FOGVARIABLESPECIFIER_HXX




class FogVariableSpecifier : public FogObjectSpecifier
{
    typedef FogObjectSpecifier Super;
    typedef FogVariableSpecifier This;
    TYPEDECL_SINGLE(This, Super)
    FOGTOKEN_MEMBER_DECLS
    FOGTOKEN_LEAF_DECLS
private:
    FogExprRefToConst _initializer;
    FogExprRefToConst _field_width;
private:
//    FogVariableSpecifier(const This& aDecl);        -- default ok
protected:
    virtual ~FogVariableSpecifier();
public:
    FogVariableSpecifier(FogMakeSpecifierContext& makeSpecifierContext, FogName& nameName);
    FogVariableSpecifier(FogMakeSpecifierContext& makeSpecifierContext, FogScopeSpecifier& typeName);
    virtual bool compile_specifier(FogMakeSpecifierContext& makeSpecifierContext);
    void emit_implementation(FogEmitContext& emitContext) const;
    void emit_interface(FogEmitContext& emitContext) const;
    const FogExpr *initializer() const;
    virtual void instantiate();
    virtual FogVariableSpecifier *is_variable_specifier();
    virtual FogEntity *make_name_entity(FogMakeEntityContext& makeEntityContext) const;
    virtual const FogMetaType& meta_type() const;
    virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_viz(std::ostream& s) const;
    virtual void set_colon_value(FogExpr& anExpr);
    virtual void set_initializer(const FogExpr& anExpr);
};
#endif
 
