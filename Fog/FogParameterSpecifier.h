
#ifndef FOGPARAMETERSPECIFIER_HXX
#define FOGPARAMETERSPECIFIER_HXX




class FogParameterSpecifier : public FogObjectSpecifier {
	typedef FogObjectSpecifier Super;
	typedef FogParameterSpecifier This;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	FOGTOKEN_DERIVED_DECLS
	FOGTOKEN_MEMBER_DECLS
	FOGTOKEN_LEAF_DECLS
private:
	FogExprRefToConst _initializer;
private:
	//    FogParameterSpecifier(const This& aDecl);       -- default ok
protected:
	FogParameterSpecifier();
	virtual ~FogParameterSpecifier();
public:
	FogParameterSpecifier(FogMakeSpecifierContext& makeSpecifierContext, FogName& nameName);
	virtual bool compile_specifier(FogMakeSpecifierContext& makeSpecifierContext);
	virtual const FogTemplateParameterSpecifierId* deduce_template_parameter
	(FogParameterSpecifier& actualParameterSpecifier, FogExprRef& deducedValue) const;
	virtual FogParameterSpecifier* is_parameter_specifier();
	virtual const FogMetaType& meta_type() const;
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	virtual void set_initializer(const FogExpr& anExpr);
};
#endif

