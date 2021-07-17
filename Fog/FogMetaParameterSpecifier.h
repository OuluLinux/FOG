
#ifndef FOGMETAPARAMETERSPECIFIER_HXX
#define FOGMETAPARAMETERSPECIFIER_HXX



class FogMetaParameterSpecifier : public FogMetaObjectSpecifier {
	typedef FogMetaObjectSpecifier Super;
	typedef FogMetaParameterSpecifier This;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	FOGTOKEN_UNIQUE_DECLS
	
private:                                    //   Members for a variable
	FogExprRefToConst _initializer;
	
protected:
	virtual ~FogMetaParameterSpecifier();
	FogMetaParameterSpecifier();
public:
	FogMetaParameterSpecifier(FogMakeSpecifierContext& makeSpecifierContext, FogName& nameName);
	virtual FogMetaParameterSpecifier* is_meta_parameter_specifier();
	virtual bool make_slot(FogMakerContext& makerContext) const;
	virtual const FogMetaType& meta_type() const;
	const FogMetaArgument* new_parameter(FogScopeContext& scopeContext, size_t anIndex) const;
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	virtual void set_initializer(const FogExpr& anExpr);
};
#endif

