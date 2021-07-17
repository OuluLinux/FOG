
#ifndef FOGSCOPEDMODIFIER_HXX
#define FOGSCOPEDMODIFIER_HXX




class FogScopedModifier : public FogModifier {
	NEEDS_A_FRIEND_FOR_GNUC
	typedef FogModifier Super;
	typedef FogScopedModifier This;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_LEAF_DECLS
private:
	FogModifierRefToConst _modifier;    //   The less qualified declarator.
	FogNameRefToConst _scope;      //   The scope name, null if global.
private:
	FogScopedModifier(const This& aDecl);
	virtual ~FogScopedModifier();
	FogToken* find_scope(FogScopeContext& inScope) const;
protected:
	virtual void make_actual_from(FogMakeActualContext& makeActualContext);
	void merge_from(FogMergeContext& mergeContext, const This& thatDecl);
	const FogMerge& needs_merge_from(FogMergeContext& mergeContext, const This& thatDecl) const;
public:
	FogScopedModifier(FogName& scopeDecl, FogModifier& childDecl);
	FogScopedModifier(FogModifier& childDecl);
	//  	virtual void compile() const;
	virtual void create_usage(FogUsageContext& usageContext) const;
	virtual bool emit_prefix(FogEmitContext& emitContext, bool needsBrackets) const;
	virtual void install(FogInstallContext& installContext) const;
	virtual bool is_actual(const FogScopeContext& scopeContext) const;
	virtual bool is_indirect() const;
	virtual const FogMetaType& meta_type() const;
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual char print_prefix(std::ostream& s, char tailChar, bool needsBrackets) const;
	virtual void set_cv(const FogCv& aCv);
};
#endif

