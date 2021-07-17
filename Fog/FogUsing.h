
#ifndef FOGUSING_HXX
#define FOGUSING_HXX







class FogUsing : public FogEntity {
	typedef FogEntity Super;
	typedef FogUsing This;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	
private:
	const FogAccess& _access;
	FogEntity& _entity;
	
private:
	FogUsing& operator=(const FogUsing&);
	FogUsing(const FogUsing&);
	virtual FogUsage* make_interface_usage_finish();
	FogUsing& mutate() const {
		return *(FogUsing*)this;
	}
	
protected:
	FogUsing();
	virtual ~FogUsing();
	
public:
	FogUsing(FogScope& aScope, const FogAccess& anAccess, FogEntity& usedEntity);
	virtual void create_usages();
	void emit_interface(FogStream& s);
	FogEntity& entity() {
		return _entity;
	}
	
	const PrimId& id() const {
		return unique_id();
	}
	
	virtual FogUsage& interface_usage_start();
	virtual const FogMetaType& meta_type() const;
	virtual std::ostream& print_this(std::ostream& s) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	FogScope& scope() {
		return non_virtual_scope();
	}
	
	const FogScope& scope() const {
		return non_virtual_scope();
	}
	
	//  	virtual FogTokenType::TokenType token_type_enum() const;
	
private:
	static PrimIdHandle make_long_id(FogScope& aScope, FogEntity& usedEntity);
};

#endif

