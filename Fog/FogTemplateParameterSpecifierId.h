
#ifndef FOGTEMPLATEPARAMETERSPECIFIERID_HXX
#define FOGTEMPLATEPARAMETERSPECIFIERID_HXX



class FogTemplateParameterSpecifierId : public FogKeyword {
	NEEDS_A_FRIEND_FOR_GNUC
	typedef FogTemplateParameterSpecifierId This;
	typedef FogKeyword Super;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	FOGTOKEN_ACTUAL_DECLS
public:
	enum Form { VALUE, TYPE, TEMPLATED };
private:
	//    FogTemplateParameterSpecifierRefToConst _template_parameter_specifier;
	const Form _form;
private:
	FogTemplateParameterSpecifierId(const This&);
	FogTemplateParameterSpecifierId& operator=(const This&);
private:
	FogTemplateParameterSpecifierId(const PrimId& anId, Form aForm);
protected:
	FogTemplateParameterSpecifierId();
public:
	virtual void create_usage(FogUsageContext& usageContext) const;
	virtual const FogTemplateParameterSpecifierId* deduce_template_parameter_type(FogName& actualTypeName,
	        FogNameRef& deducedType) const;
	virtual bool emit(FogEmitContext& emitContext) const;
	virtual void find_entities_in(FogScopeContext& inScope, FogEntityFinding& theFinding) const;
	virtual FogTemplateParameterSpecifierId* is_template_parameter_specifier_id();
	virtual FogEntity* make_type_entity(FogMakeEntityContext& makeEntityContext) const;
	virtual const FogMetaType& meta_type() const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
private:
	static Form form(const FogTemplateParameterSpecifier& templateParameterSpecifier);
public:
	static FogTemplateParameterSpecifierId& make(const PrimId& anId,
	        const FogTemplateParameterSpecifier& templateParameterSpecifier);
};
#endif

