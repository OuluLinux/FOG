
#ifndef FOGTEMPLATEARG_HXX
#define FOGTEMPLATEARG_HXX



class FogTemplateArg : public FogDecoratedExpr {
	typedef FogDecoratedExpr Super;
	typedef FogTemplateArg This;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	FOGTOKEN_DERIVED_DECLS
private:
	This& operator=(const This&);
protected:
	FogTemplateArg() {}
	FogTemplateArg(FogExpr& anExpr) : Super(anExpr) {}
public:
	virtual bool emit(FogEmitContext& emitContext) const;
	virtual size_t executable_tokens() const;
	virtual bool matches(FogMergeContext& mergeContext, const FogTemplateParameterSpecifier& templateParameter) const;
	virtual const FogMetaType& meta_type() const;
	virtual std::ostream& print_this(std::ostream& s) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
public:
	static FogTemplateArg* new_template_type_argument(FogExpr& anExpr);
	static FogTemplateArg* new_template_value_argument(FogExpr& anExpr);
};

#endif

