
#ifndef FOGFUNCTIONSPECIFIER_HXX
#define FOGFUNCTIONSPECIFIER_HXX




class FogFunctionSpecifier : public FogObjectSpecifier {
	typedef FogObjectSpecifier Super;
	typedef FogFunctionSpecifier This;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	FOGTOKEN_DERIVED_DECLS
	FOGTOKEN_LEAF_DECLS
private:
	FogScopeMapOfRef _reverse_friends;                          //   Scope registering this as a friend.
private:
	FogFunctionSpecifier(const This& functionSpecifier);
	bool has_body() const;
	This& mutate() const { return *(This*)this; }
protected:
	FogFunctionSpecifier();
	virtual ~FogFunctionSpecifier();
	void merge_from(FogMergeContext& mergeContext, const This& thatDecl);
	const FogMerge& needs_merge_from(FogMergeContext& mergeContext, const This& thatDecl) const;
public:
	FogFunctionSpecifier(FogMakeSpecifierContext& makeSpecifierContext, FogName& nameName);
	virtual void add_friend(FogScope& aScope);
	virtual bool compile_specifier(FogMakeSpecifierContext& makeSpecifierContext);
	virtual void instantiate();
	bool is_constructor(FogScopeContext& inScope) const;
	bool is_copy(const PrimId& anId) const;
	virtual FogFunctionSpecifier* is_function_specifier();
	virtual FogEntity* make_name_entity(FogMakeEntityContext& makeEntityContext) const;
	virtual const FogMetaType& meta_type() const;
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	const FogScopeMapOfRef& reverse_friends() const;
	virtual void set_colon_value(FogExpr& anExpr);
	virtual void set_initializer(const FogExpr& anExpr);
};
#endif

