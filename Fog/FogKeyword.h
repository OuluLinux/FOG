
#ifndef FOGKEYWORD_HXX
#define FOGKEYWORD_HXX




class FogKeyword : public FogWordName {
	typedef FogKeyword This;
	typedef FogWordName Super;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	FOGTOKEN_SHAREDLEAF_DECLS
private:
	const PrimIdHandle _id;
private:
	FogKeyword(const This&);
	FogKeyword& operator=(const This&);
protected:
	FogKeyword(const PrimId& anId) : _id(anId) {}
	FogKeyword(const char* anId = 0) : _id(anId) {}
public:
	const PrimId& id() const { return *_id; }
	virtual const PrimId* is_resolved() const;
	virtual const PrimId* is_word() const;
	virtual const FogMetaType& meta_type() const;
	virtual char print_named(std::ostream& s, const PrimId* scopeId, char tailChar) const;
	virtual bool resolve_id(PrimIdHandle& returnId, FogScopeContext& scopeContext) const;
	virtual size_t source_columns() const;
	const char* str() const { return _id->str(); }
};
#endif

