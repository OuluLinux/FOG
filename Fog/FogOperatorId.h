
#ifndef FOGOPERATORID_HXX
#define FOGOPERATORID_HXX




class FogOperatorId : public FogEntityName {
	NEEDS_A_FRIEND_FOR_GNUC
	typedef FogEntityName Super;
	typedef FogOperatorId This;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	FOGTOKEN_SHAREDLEAF_DECLS
private:
	const PrimIdHandle _id;
private:
	FogOperatorId(const PrimId& anId);
protected:
	FogOperatorId();
	virtual ~FogOperatorId();
public:
	const PrimId& id() const { return *_id; }
	virtual bool is_operator() const;
	virtual const PrimId* is_resolved() const;
	virtual const FogMetaType& meta_type() const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	virtual char print_named(std::ostream& s, const PrimId* scopeId, char tailChar) const;
	virtual bool resolve_id(PrimIdHandle& returnId, FogScopeContext& scopeContext) const;
	virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
	virtual NameSort sort_class() const;
	virtual size_t source_columns() const;
	const char* str() const { return _id->str(); }
public:
	static FogOperatorId& make(const PrimId& anId);
};
#endif

