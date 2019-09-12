
#ifndef FOGREFERENCEMODIFIER_HXX
#define FOGREFERENCEMODIFIER_HXX



class FogReferenceModifier : public FogModifier
{
    NEEDS_A_FRIEND_FOR_GNUC
    typedef FogModifier Super;
    typedef FogReferenceModifier This;
    TYPEDECL_SINGLE(This, Super)
    FOGTOKEN_LEAF_DECLS
private:
    FogReferenceModifier(const This& aDecl);
    virtual ~FogReferenceModifier();
public:
    FogReferenceModifier();
    virtual void deduce_template_parameter(FogExprRef& deducedExpr) const;
    virtual void deduce_template_parameter(FogName& deducedType, FogExprRef& deducedExpr) const;
    virtual bool emit_prefix(FogEmitContext& emitContext, bool needsBrackets) const;
    virtual bool is_formal_template_parameter_for(const FogModifier& actualModifier) const;
    virtual bool is_indirect() const;
    virtual bool is_reference() const;
    virtual const FogMetaType& meta_type() const;
    virtual char print_prefix(std::ostream& s, char tailChar, bool needsBrackets) const;
public:
    static FogModifier& make();
};
#endif
 
