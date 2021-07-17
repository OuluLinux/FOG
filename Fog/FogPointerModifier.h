
#ifndef FOGPOINTERMODIFIER_HXX
#define FOGPOINTERMODIFIER_HXX




class FogPointerModifier : public FogModifier {
	typedef FogModifier Super;
	typedef FogPointerModifier This;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	//    FOGTOKEN_LEAF_DECLS
	FOGTOKEN_SHAREDLEAF_DECLS
private:
	FogCvHandle _cv;
private:
	FogPointerModifier(const This& aDecl);
protected:
	virtual ~FogPointerModifier();
	void merge_from(FogMergeContext& mergeContext, const This& aDecl);
	const FogMerge& needs_merge_from(FogMergeContext& mergeContext, const This& aDecl) const;
public:
	FogPointerModifier(const FogCv& aCv = FogCv::initial());
	virtual void deduce_template_parameter(FogExprRef& deducedExpr) const;
	virtual void deduce_template_parameter(FogName& deducedType, FogExprRef& deducedExpr) const;
	virtual bool emit_prefix(FogEmitContext& emitContext, bool needsBrackets) const;
	virtual bool is_formal_template_parameter_for(const FogModifier& actualModifier) const;
	virtual bool is_indirect() const;
	virtual bool is_star() const;
	virtual const FogMetaType& meta_type() const;
	virtual char print_prefix(std::ostream& s, char tailChar, bool needsBrackets) const;
public:
	static FogPointerModifier& make(const FogCv& aCv = FogCv::initial());
	static FogPointerModifier& new_cv(const FogPointerModifier& aDecl, const FogCv& aCv);
};
#endif

