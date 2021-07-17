
#ifndef FOGMETATOKEN_HXX
#define FOGMETATOKEN_HXX




class FogMetaToken : public FogToken {
	NEEDS_A_FRIEND_FOR_GNUC
	typedef FogToken Super;
	typedef FogMetaToken This;
	TYPEDECL_SINGLE(This, Super)
private:
	FogMetaEntityMapOfRef _meta_entities;   //   Non-static meta-variables.
private:
	This& mutate() const { return *(This*)this; }
protected:
	FogMetaToken();
	virtual ~FogMetaToken();
public:
	virtual FogMetaEntity* adopt_meta_entity(FogMetaEntity* metaEntity);
	virtual void destroy();
	virtual FogMetaEntity* find_meta_entity(const PrimId& anId);
	const FogMetaEntity* find_meta_entity(const PrimId& anId) const { return mutate().find_meta_entity(anId); }
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
};
#endif

