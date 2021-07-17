
#ifndef FOGARRAYMODIFIER_HXX
#define FOGARRAYMODIFIER_HXX




class FogArrayModifier : public FogModifier {
	NEEDS_A_FRIEND_FOR_GNUC
	typedef FogModifier Super;
	typedef FogArrayModifier This;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_LEAF_DECLS
private:
	FogExprRefToConst _dimension;
private:
	FogArrayModifier(const This& aDecl);
	virtual ~FogArrayModifier();
protected:
	virtual void make_actual_from(FogMakeActualContext& makeActualContext);
	void merge_from(FogMergeContext& mergeContext, const This& thatDecl);
	const FogMerge& needs_merge_from(FogMergeContext& mergeContext, const This& thatDecl) const;
public:
	FogArrayModifier(const FogExpr* arrayDimension);
	virtual void create_usage(FogUsageContext& usageContext) const;
	virtual void deduce_template_parameter(FogExprRef& deducedExpr) const;
	virtual void deduce_template_parameter(FogName& deducedType, FogExprRef& deducedExpr) const;
	virtual bool emit_prefix(FogEmitContext& emitContext, bool needsBrackets) const;
	virtual bool emit_suffix(FogEmitContext& emitContext, bool needsBrackets) const;
	virtual void install(FogInstallContext& installContext) const;
	virtual bool is_actual(const FogScopeContext& scopeContext) const;
	virtual const FogArrayModifier* is_array_modifier() const;
	//      virtual bool is_formal_template_parameter_for(const FogModifier& actualModifier) const; -- no template arrays.
	virtual const FogMetaType& meta_type() const;
	virtual char print_prefix(std::ostream& s, char tailChar, bool needsBrackets) const;
	virtual char print_suffix(std::ostream& s, char tailChar, bool needsBrackets) const;
};
#endif

