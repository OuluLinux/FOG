
#ifndef FOGAUTO_HXX
#define FOGAUTO_HXX



class FogAuto : public FogScope {
	typedef FogScope Super;
	typedef FogAuto This;
	TYPEDECL_SINGLE(This, Super)
	
private:
	virtual FogUsage* make_interface_usage_finish();
	virtual FogUsage* make_interface_usage_start();
	virtual FogUsage* make_name_usage();
	FogScope& scope() {
		return non_virtual_scope();
	}
	
	const FogScope& scope() const {
		return non_virtual_scope();
	}
	
protected:
	virtual ~FogAuto();
	
public:
	FogAuto(FogRoot& globalScope);
	virtual void create_usages();
	virtual const FogAccess& default_access() const;
	virtual const FogInline& default_inline() const;
	virtual void emit_interface_finish(FogStream& s) const;
	virtual void emit_interface_start(FogStream& s) const;
	virtual void emit_scope_head(FogStream& s) const;
	virtual void emit_scope_tail(FogStream& s) const;
	virtual FogScope& name_emit_scope();
	virtual FogTargetFile* name_file();
	virtual FogScope& name_scope();
};

#endif

