
#ifndef FOGRAWSUBSPACE_HXX
#define FOGRAWSUBSPACE_HXX



class FogRawSubspace : public FogRaws {
	typedef FogRawSubspace This;
	typedef FogRaws Super;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	FOGTOKEN_DERIVED_DECLS
	FOGTOKEN_LEAF_DECLS
protected:
	virtual ~FogRawSubspace();
public:
	FogRawSubspace();
	virtual const FogLazyBool& derive_at(FogScopeContext& inScope) const;
	virtual bool emit(FogEmitContext& emitContext) const;
	virtual IsExposed is_exposed() const;
	virtual std::ostream& print_viz(std::ostream& s) const;
};
#endif

