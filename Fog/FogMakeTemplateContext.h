
#ifndef FOGMAKETEMPLATECONTEXT_HXX
#define FOGMAKETEMPLATECONTEXT_HXX



class FogMakeTemplateContext : public FogMakeEntityContext {
	typedef FogMakeTemplateContext This;
	typedef FogMakeEntityContext Super;
	TYPEDECL_SINGLE(This, Super)
private:                                                    //   Working context
	FogScopeContext& _context;
	FogEntity& _primary;                                        //   Primary template.
	const FogTemplateArgsRefToConst _template_arguments;
	const IsReference _is_reference;
private:
	mutable PrimIdHandle _local_id;
	mutable PrimIdHandle _global_id;
private:                                                        //     (used to determine inline strategy)
	FogMakeTemplateContext& operator=(const This& aContext);
	void make_templated_id(PrimIdHandle& madeId, const PrimId& unscopedId) const;
	This& mutate() const { return *(This*)this; }
	FogScopeContext& scope_context() { return _context; }
	const FogScopeContext& scope_context() const { return _context; }
public:
	FogMakeTemplateContext(FogScopeContext& scopeContext, FogEntity& primaryEntity,
	                       const FogTemplateArgs& templateArguments, IsReference isReference);
	virtual ~FogMakeTemplateContext();
	virtual const FogDeclSpecifierValue& decl_specifiers() const;
	virtual FogToken& dynamic_token();
	virtual const FogToken& dynamic_token() const;
	virtual FogScopeContext* find_context(size_t dollarCount);
	virtual bool find_formal_slots(FogMetaSlotFinder& theFinder);
	virtual bool find_slots(FogMetaSlotFinder& theFinder);
	virtual const FogTemplateParameterSpecifier* find_template_parameter_specifier(const PrimId& anId);
	virtual const FogTemplateParameterSpecifier* find_template_parameter_specifier
	(const FogTemplateParameterSpecifierId& anId);
	virtual const PrimId& global_id() const;
	virtual const PrimId& global_signature_id() const;
	virtual InScope in_scope() const;
	IsReference is_reference() const { return _is_reference; }
	virtual const PrimId& local_id() const;
	virtual const PrimId& local_signature_id() const;
	virtual const PrimId& long_id() const;
	virtual const FogScopeContext* meta_context() const;
	FogEntity& primary() { return _primary; }
	const FogEntity& primary() const { return _primary; }
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_resolution(std::ostream& s) const;
	virtual Resolution resolution() const;
	virtual const PrimId& short_id() const;
	virtual FogToken& static_token();
	virtual const FogToken& static_token() const;
	FogTargetFile* target_file() const;
	const FogTemplateArgs& template_args() const { return *_template_arguments; }
	virtual const FogTemplateParameterSpecifiers* template_parameters() const;
	const FogUtility& utility() const;
};
#endif

