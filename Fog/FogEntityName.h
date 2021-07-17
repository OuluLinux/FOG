
#ifndef FOGENTITYNAME_HXX
#define FOGENTITYNAME_HXX




class FogEntityName : public FogName {
	typedef FogName Super;
	typedef FogEntityName This;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	FOGTOKEN_DERIVED_DECLS
private:
	This& operator=(const This& thatName);
	This& mutate() const { return *(This*)this; }
protected:
	FogEntityName() {}
	FogEntityName(const This& thatName) : Super(thatName) {}
	virtual ~FogEntityName() {}
public:
	virtual bool emit(FogEmitContext& emitContext) const;
	virtual void find_entities_in(FogScopeContext& inScope, FogEntityFinding& theFinding) const;
	virtual FogDestructorId* is_destructor_id();
	const FogDestructorId* is_destructor_id() const { return mutate().is_destructor_id(); }
	virtual FogEntityName* is_entity_name();
	virtual FogEntity* make_entity(FogMakerContext& makerContext) const;
	virtual bool make_specifier(FogMakeSpecifierContext& makeSpecifierContext);
	virtual const FogMerge& needs_merge(FogMergeContext& mergeContext, const FogToken& aToken) const;
};
#endif

