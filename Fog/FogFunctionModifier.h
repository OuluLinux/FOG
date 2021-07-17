
#ifndef FOGFUNCTIONMODIFIER_HXX
#define FOGFUNCTIONMODIFIER_HXX





class FogFunctionModifier : public FogModifier {
	NEEDS_A_FRIEND_FOR_GNUC
	typedef FogModifier Super;
	typedef FogFunctionModifier This;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_LEAF_DECLS
private:        //   Defining contents.
	FogCvHandle _cv;
	FogParameterSpecifierListOfRef _parameters;
	FogExprListOfRefToConst _exceptions;
	bool _ellipsis;
private:
	FogFunctionModifier(const This& aDecl);
	virtual ~FogFunctionModifier();
protected:
	virtual void make_actual_from(FogMakeActualContext& makeActualContext);
	void merge_from(FogMergeContext& mergeContext, const This& anExpr);
	const FogMerge& needs_merge_from(FogMergeContext& mergeContext, const This& thatDecl) const;
public:
	FogFunctionModifier(const FogCv& aCv);
	void add_parameter(FogParameterSpecifier& aParameter);
	void add_exception_specification(const FogExpr& typeId);
	virtual bool compile_specifier(FogMakeSpecifierContext& makeSpecifierContext);
	virtual void create_usage(FogUsageContext& usageContext) const;
	virtual bool emit_prefix(FogEmitContext& emitContext, bool needsBrackets) const;
	virtual bool emit_suffix(FogEmitContext& emitContext, bool needsBrackets) const;
	virtual void install(FogInstallContext& installContext) const;
	virtual bool is_actual(const FogScopeContext& scopeContext) const;
	bool is_assign(const PrimId& typeId) const;
	bool is_copy(const PrimId& typeId) const;
	virtual const FogFunctionModifier* is_function_modifier() const;
	virtual const FogMetaType& meta_type() const;
	const FogParameterSpecifierListOfRef& parameters() const { return _parameters; }
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual char print_prefix(std::ostream& s, char tailChar, bool needsBrackets) const;
	virtual char print_suffix(std::ostream& s, char tailChar, bool needsBrackets) const;
	void set_ellipsis() { _ellipsis = true; }
};
#endif

