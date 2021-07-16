
#ifndef FOGTEMPLATEPARAMETERSPECIFIERS_HXX
#define FOGTEMPLATEPARAMETERSPECIFIERS_HXX




class FogTemplateParameterSpecifiers : public FogToken
{
 typedef FogToken Super;
 typedef FogTemplateParameterSpecifiers This;
 TYPEDECL_SINGLE(This, Super)
 PRIMREF_DERIVED_DECLS(This)
 FOGTOKEN_MEMBER_DECLS
 FOGTOKEN_LEAF_DECLS
private:
 const FogTemplateParameterSpecifiersRefToConst _outer_parameters; //   Less nested parameters.
 const size_t _last_outer_parameter_number;       //   Cached less nested parameter count.
 FogTemplateParameterSpecifierListOfRefToConst _parameters;   //   List of these formal parameters.
 const IsExportTemplate _is_export : 1;        //   True if export accompanied template
 const IsUsingTemplate _is_using : 1;        //   True if using accompanied template
private:
 mutable PrimIdHandle _id;           //   Encoded id such as "< _1, _2 >"
private:
//  	FogTemplateParameterSpecifiers(const This&);
 This& operator=(const This&);
private:
 This& mutate() const { return *(This *)this; }
protected:
 FogTemplateParameterSpecifiers();
 virtual ~FogTemplateParameterSpecifiers();
public:
 FogTemplateParameterSpecifiers(const FogTemplateParameterSpecifiers *outerParameters,
  IsExportTemplate isExport, IsUsingTemplate isUsing);
 void add(FogTemplateParameterSpecifier& aParameter, const FogTemplateParameterSpecifier *parentSpecifier = 0);
 void emit_suffix(FogEmitContext& emitContext) const;
 const FogTemplateParameterSpecifier *find(FogScopeContext& scopeContext, const PrimId& anId) const;
 const FogTemplateParameterSpecifier *find(const FogTemplateParameterSpecifierId& anId, bool deepFind = false) const;
 size_t first_template_parameter_number() const;
 virtual const PrimId& id() const;
 IsExportTemplate is_export_template() const { return _is_export; }
 IsUsingTemplate is_using_template() const { return _is_using; }
 size_t last_template_parameter_number() const;
 const FogTemplateParameterSpecifierListOfRefToConst& parameters() const { return _parameters; }
 char print_suffix(std::ostream& s, char tailChar) const;
 virtual std::ostream& print_viz(std::ostream& s) const;
 size_t template_parameter_depth() const;
 const FogTemplateParameterSpecifier& template_parameter_number(size_t i) const;
public:
//  	static const FogTemplateParameterSpecifiers& empty();
};
#endif
 
