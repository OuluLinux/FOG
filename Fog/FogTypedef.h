
#ifndef FOGTYPEDEF_HXX
#define FOGTYPEDEF_HXX



class FogTypedef : public FogDerivable {
	typedef FogDerivable Super;
	typedef FogTypedef This;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	
public:
	enum TypedefOf { INVALID, NAME, TYPE, FUNCTION };
	
private:
	FogEntity* const _entity;   //   Resolved value of typedef.
	const TypedefOf _of;
	
private:
	inline bool is_free() const;
	//  	virtual FogUsage *make_interface_usage_finish();
	virtual FogUsage* make_interface_usage_start();
	virtual FogUsage* make_name_usage();
	This& mutate() const {
		return *(This*)this;
	}
	
protected:
	FogTypedef();
	virtual ~FogTypedef();
	
public:
	FogTypedef(FogMakerContext& makerContext, FogEntity* anEntity);
	virtual void adopt(FogScopeContext& inScope, FogPotentialDeclaration& potentialDeclaration);
	virtual void create_usages();
	virtual bool do_compile(FogCompileContext& inScope);
	virtual bool do_derive(FogDeriveContext& inScope);
	virtual void emit_forward_reference(FogStream& s);
	virtual void install_names();
	virtual FogTargetFile* interface_file_source();
	virtual FogUsage& interface_usage_finish();
	virtual bool is_type() const;
	virtual FogTypedef* is_typedef();
	virtual const FogMetaType& meta_type() const;
	virtual FogTargetFile* name_file();
	virtual std::ostream& print_viz(std::ostream& s) const;
	FogEntity* resolve_alias() {
		return _entity;
	}
	
	const FogEntity* resolve_alias() const {
		return _entity;
	}
	
	virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
	
private:
	TypedefOf of(FogMakerContext& makerContext);
	
public:
	static int compare(const FogTypedefRefToConst* p1, const FogTypedefRefToConst* p2);
};

#endif

