
#ifndef FOGFILEID_HXX
#define FOGFILEID_HXX



class FogFileId : public FogToken {
	typedef FogToken Super;
	typedef FogFileId This;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(FogFileId)
	FOGTOKEN_DERIVED_DECLS
	FOGTOKEN_ACTUAL_DECLS
private:
	bool _is_input;
private:
	FogFileId(const FogFileId& fileId);
	FogFileId& operator=(const FogFileId& fileId);
protected:
	FogFileId() : _is_input(false) {}
	virtual ~FogFileId() {}
public:
	bool is_input() const { return _is_input; }
	virtual FogTargetFile* make(FogScopeContext& scopeContext) const = 0;
	void set_is_input() { _is_input = true; }
public:
	static FogFileIdent* new_id(const PrimId& anId);
	static FogFileId* new_implementation(const FogName& aDecl);
	static FogFileId* new_interface(const FogName& aDecl);
};
#endif

