
#ifndef FOGCLASS_HXX
#define FOGCLASS_HXX



class FogClass : public FogScope {
	typedef FogScope Super;
	typedef FogClass This;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	
public:
	enum Anonymity {
		NORMAL_TYPE,    //   Conventionally named object such as: class X.
		NAMED_ANON_TYPE,   //   Explicitly named anonymous scope such as: typedef class { ... } X;
		UNNAMED_ANON_TYPE,   //   Unnamed anonymous scope such as: class { ... } x;
		ANON_OBJECT     //   Anonymous union such as: union { ... };
	};
	
private:
	const Anonymity _anonymity;
	
private:
	FogClass(This& primaryClass, FogMakeTemplateContext& makeTemplateContext);
	inline bool inline_is_namespace() const;
	virtual FogUsage *make_interface_usage_finish();
	virtual FogUsage *make_interface_usage_start();
	virtual FogUsage *make_name_usage();
	FogScope& scope() {
		return non_virtual_scope();
	}
	
	const FogScope& scope() const {
		return non_virtual_scope();
	}
	
protected:
	FogClass();
	virtual ~FogClass();
	
public:
	FogClass(const FogTag& aTag, FogMakerContext& makerContext, const FogAnonId *anonId);
	virtual const FogAccess& default_access() const;
	virtual const FogInline& default_inline() const;
	virtual void emit_forward_reference(FogStream& s);
	virtual void emit_interface_finish(FogStream& s) const;
	virtual void emit_interface_start(FogStream& s) const;
	virtual void emit_scope_head(FogStream& s) const;
	virtual void emit_scope_tail(FogStream& s) const;
	virtual bool has_access() const;
	virtual FogClass *is_class();
	virtual bool is_name_scope() const;
	virtual bool is_name_space() const;
	virtual FogClass *is_namespace();
	virtual bool is_type() const;
	virtual FogScope& name_emit_transient_scope();
	virtual FogScope& name_emit_scope();
	virtual FogTargetFile *name_file();
	virtual FogScope& name_scope();
	virtual FogScope& name_space();
	virtual FogEntity *new_template(FogMakeTemplateContext& makeTemplateContext);
	
private:
	static Anonymity anonymity(const FogAnonId *anonId);
	
public:
	static int compare(const FogClassRefToConst *p1, const FogClassRefToConst *p2);
};

#endif

