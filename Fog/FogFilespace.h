
#ifndef FOGFILESPACE_HXX
#define FOGFILESPACE_HXX



class FogFilespace : public FogScope {
	typedef FogScope Super;
	typedef FogFilespace This;
	TYPEDECL_SINGLE(This, Super)
private:
	This& mutate() const { return *(This*)this; }
	FogScope& scope() { return non_virtual_scope(); }
	const FogScope& scope() const { return non_virtual_scope(); }
protected:
	FogFilespace();
	virtual ~FogFilespace();
public:
	FogFilespace(FogMakerContext& makerContext);
	virtual const FogAccess& default_access() const;
	virtual const FogInline& default_inline() const;
	virtual void emit_scope(FogEmitContext& emitContext, const FogScope& inScope) const;
	virtual void find_entities(FogEntityFinder& theFinder);
	virtual FogEntity* find_entity(const PrimId& anId, InScope inScope, FindStrategy aStrategy);
	virtual const PrimId& global_id() const;
	virtual const PrimId& global_signature_id() const;
	virtual bool has_access() const;
	virtual FogTargetFile* interface_file_source();
	virtual FogUsage& interface_usage_finish();
	virtual FogUsage& interface_usage_start();
	virtual const PrimId& local_id() const;
	virtual const PrimId& local_signature_id() const;
	virtual const PrimId& long_id() const;
	virtual FogUsage& name_usage();
	virtual FogScope& parent_scope();
	virtual void set_interface_file(FogTargetFile* aFile);
	virtual const PrimId& short_id() const;
};
#endif

