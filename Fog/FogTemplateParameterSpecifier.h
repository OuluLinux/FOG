
#ifndef FOGTEMPLATEPARAMETERSPECIFIER_HXX
#define FOGTEMPLATEPARAMETERSPECIFIER_HXX



class FogTemplateParameterSpecifier : public FogObjectSpecifier
{
 typedef FogObjectSpecifier Super;
 typedef FogTemplateParameterSpecifier This;
 TYPEDECL_SINGLE(This, Super)
 PRIMREF_DERIVED_DECLS(This)
 FOGTOKEN_DERIVED_DECLS
 FOGTOKEN_MEMBER_DECLS
private:
 const FogNameRefToConst _unnormalised_name; //   Actual source name.
 FogExprRefToConst _initializer;
 size_t _number;       //   Sequential nesting count.
private:
//  	FogTemplateParameterSpecifier(const This&);
 This& operator=(const This&);
private:
 This& mutate() const { return *(This *)this; }
protected:
 FogTemplateParameterSpecifier() : _number(0) {}
 FogTemplateParameterSpecifier(FogMakeSpecifierContext& makeSpecifierContext, FogName& nameName);
 virtual ~FogTemplateParameterSpecifier() {}
public:
 virtual void emit_suffix(FogEmitContext& emitContext) const = 0;
 virtual const FogTemplateParameterSpecifier *find(FogScopeContext& scopeContext, const PrimId& anId) const;
 const FogTemplateParameterSpecifier *find(const FogTemplateParameterSpecifierId& anId) const;
 const FogExpr& initializer() const { return *_initializer; }
 virtual FogTemplateParameterSpecifier *is_template_parameter_specifier();
 virtual FogTemplatedParameterSpecifier *is_templated_parameter_specifier();
 const FogTemplatedParameterSpecifier *is_templated_parameter_specifier() const
  { return mutate().is_templated_parameter_specifier(); }
 virtual FogTypeParameterSpecifier *is_type_parameter_specifier();
 const FogTypeParameterSpecifier *is_type_parameter_specifier() const
  { return mutate().is_type_parameter_specifier(); }
 virtual FogValueParameterSpecifier *is_value_parameter_specifier();
 const FogValueParameterSpecifier *is_value_parameter_specifier() const
  { return mutate().is_value_parameter_specifier(); }
 virtual FogEntity *make_name_entity(FogMakeEntityContext& makeEntityContext) const;
 virtual const FogMetaType& meta_type() const;
 virtual char print_suffix(std::ostream& s, char tailChar) const;
 virtual void set_initializer(const FogExpr& anExpr);
 void set_template_parameter_number(size_t aNumber, const FogTemplateParameterSpecifier *parentSpecifier);
 size_t template_parameter_number() const { return _number; }
    const FogName& unnormalised_name() const { return *_unnormalised_name; }
private:
 FogName& default_name();
};

class FogTemplatedParameterSpecifier : public FogTemplateParameterSpecifier
{
 typedef FogTemplateParameterSpecifier Super;
 typedef FogTemplatedParameterSpecifier This;
 TYPEDECL_SINGLE(This, Super)
 FOGTOKEN_MEMBER_DECLS
 FOGTOKEN_LEAF_DECLS
private:
 FogTemplateParameterSpecifierListOfRefToConst _parameters;   //   List of these formal parameters.
//  	FogRawTemplateRef _template;
protected:
 virtual ~FogTemplatedParameterSpecifier();
public:
 FogTemplatedParameterSpecifier(FogMakeSpecifierContext& makeSpecifierContext, FogSpecifier& nameName);
 virtual void emit_suffix(FogEmitContext& emitContext) const;
 virtual FogTemplatedParameterSpecifier *is_templated_parameter_specifier();
 virtual const FogMetaType& meta_type() const;
 virtual std::ostream& print_this(std::ostream& s) const;
 virtual std::ostream& print_viz(std::ostream& s) const;
 virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
};

class FogTypeParameterSpecifier : public FogTemplateParameterSpecifier
{
 typedef FogTemplateParameterSpecifier Super;
 typedef FogTypeParameterSpecifier This;
 TYPEDECL_SINGLE(This, Super)
//  	FOGTOKEN_MEMBER_DECLS
 FOGTOKEN_LEAF_DECLS
private:
 const FogTag& _tag;
protected:
 virtual ~FogTypeParameterSpecifier();
 const FogMerge& needs_merge_from(FogMergeContext& mergeContext, const This& aToken) const;
public:
 FogTypeParameterSpecifier(FogMakeSpecifierContext& makeSpecifierContext, FogScopeSpecifier& nameName);
 virtual bool emit(FogEmitContext& emitContext) const;
 virtual void emit_suffix(FogEmitContext& emitContext) const;
 virtual FogTypeParameterSpecifier *is_type_parameter_specifier();
 virtual const FogMetaType& meta_type() const;
 virtual char print_named(std::ostream& s, const PrimId *scopeId, char tailChar) const;
 virtual std::ostream& print_viz(std::ostream& s) const;
 virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
};

class FogValueParameterSpecifier : public FogTemplateParameterSpecifier
{
 typedef FogTemplateParameterSpecifier Super;
 typedef FogValueParameterSpecifier This;
 TYPEDECL_SINGLE(This, Super)
 FOGTOKEN_LEAF_DECLS
protected:
 virtual ~FogValueParameterSpecifier();
//  	const FogMerge& needs_merge_from(FogMergeContext& mergeContext, const This& aToken) const;
public:
 FogValueParameterSpecifier(FogMakeSpecifierContext& makeSpecifierContext, FogName& nameName);
 virtual void emit_suffix(FogEmitContext& emitContext) const;
 virtual FogValueParameterSpecifier *is_value_parameter_specifier();
 virtual const FogMetaType& meta_type() const;
 virtual std::ostream& print_viz(std::ostream& s) const;
 virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
};
#endif
 
