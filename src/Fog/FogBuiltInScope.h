
#ifndef FOGBUILTINSCOPE_HXX
#define FOGBUILTINSCOPE_HXX



class FogBuiltInScope : public FogScope {
	typedef FogScope Super;
	typedef FogBuiltInScope This;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	
private:
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
	FogBuiltInScope();
	virtual ~FogBuiltInScope();
	
public:
	FogBuiltInScope(FogRoot& globalScope, const PrimId& anId);
	virtual void create_usages();
	virtual const FogAccess& default_access() const;
	virtual const FogInline& default_inline() const;
	virtual void emit_interface_finish(FogStream& s) const;
	virtual void emit_interface_start(FogStream& s) const;
	virtual void emit_scope_head(FogStream& s) const;
	virtual void emit_scope_tail(FogStream& s) const;
	virtual bool is_name_scope() const;
	virtual const FogMetaType& meta_type() const;
	virtual FogScope& name_emit_scope();
	virtual FogTargetFile *name_file();
	virtual FogScope& name_scope();
	virtual FogTokenType::TokenType pp_token_type_enum() const;
//  	virtual FogTokenType::TokenType token_type_enum() const;
};

#endif

