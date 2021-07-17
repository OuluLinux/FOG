
#ifndef FOGANONID_HXX
#define FOGANONID_HXX





class FogAnonId : public FogEntityName {
	NEEDS_A_FRIEND_FOR_GNUC
	friend struct PrimMapOfRefToConstFunctions<FogAnonId>;
	typedef FogEntityName Super;
	typedef FogAnonId This;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	FOGTOKEN_SHAREDLEAF_DECLS
private:
	const PrimIdHandle _id;                         //   The (internally) alloicated name.
	PrimIdHandle _explicit_id;                      //   Explicitly specified name.
	bool _abstract;                                 //   Set false once used.
private:
	FogAnonId(const PrimId& anId);
	const PrimId& anon_id() const { return is_named() ? *_explicit_id : *_id; }
	const PrimId& id() const { return *_id; }
protected:
	FogAnonId();
	virtual ~FogAnonId();
public:
	virtual bool emit(FogEmitContext& emitContext) const;
	bool is_abstract() const { return _abstract; }
	virtual FogAnonId* is_anon();
	bool is_named() const { return !!_explicit_id; }
	virtual const PrimId* is_resolved() const;
	virtual const FogMetaType& meta_type() const;
	virtual char print_named(std::ostream& s, const PrimId* scopeId, char tailChar) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	virtual bool resolve_id(PrimIdHandle& returnId, FogScopeContext& scopeContext) const;
	virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
	void set_concrete();
	void set_id(const PrimId& anId);
	virtual size_t source_columns() const;
	const char* str() const { return _id->str(); }
public:
	static FogName& abstract();
	static FogName& make();
};
#endif

