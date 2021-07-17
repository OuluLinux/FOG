
#ifndef FOGMODIFIER_HXX
#define FOGMODIFIER_HXX




class FogModifier : public FogToken {
	typedef FogToken Super;
	typedef FogModifier This;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	FOGTOKEN_DERIVED_DECLS
private:
	FogModifier& operator=(const This& aDecl);
private:
	This& mutate() const { return *(This*)this; }
protected:
	FogModifier() {}
	FogModifier(const This& aDecl) {}
	virtual ~FogModifier() {}
public:
	virtual bool compile_specifier(FogMakeSpecifierContext& makeSpecifierContext);
	virtual void deduce_template_parameter(FogExprRef& deducedExpr) const;
	virtual void deduce_template_parameter(FogName& deducedType, FogExprRef& deducedExpr) const;
	virtual bool emit(FogEmitContext& emitContext) const;
	virtual bool emit_prefix(FogEmitContext& emitContext, bool needsBrackets) const;
	virtual bool emit_suffix(FogEmitContext& emitContext, bool needsBrackets) const;
	virtual bool is_formal_template_parameter_for(const FogModifier& actualModifier) const;
	virtual bool is_indirect() const;
	virtual FogModifier* is_modifier();
	virtual bool is_reference() const;
	virtual bool is_star() const;
	virtual char print_named(std::ostream& s, const PrimId* scopeId, char tailChar) const;
	virtual char print_prefix(std::ostream& s, char tailChar, bool needsBrackets) const;
	virtual char print_suffix(std::ostream& s, char tailChar, bool needsBrackets) const;
	virtual std::ostream& print_this(std::ostream& s) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
};
#endif

