
#ifndef FOGBASESPECIFIERS_HXX
#define FOGBASESPECIFIERS_HXX




class FogBaseSpecifiers : public FogToken {
	typedef FogToken Super;
	typedef FogBaseSpecifiers This;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	FOGTOKEN_DERIVED_DECLS
	FOGTOKEN_MEMBER_DECLS
	FOGTOKEN_LEAF_DECLS
	
private:
	FogBaseSpecifierListOfRefToConst _bases;
	
private:
	FogBaseSpecifiers(const This&);
	FogBaseSpecifiers& operator=(const This&);
	
protected:
	virtual ~FogBaseSpecifiers();
	
public:
	FogBaseSpecifiers();
	void add(FogBaseSpecifier& aBase) {
		_bases.add(aBase);
	}
	
	const FogBaseSpecifierListOfRefToConst& bases() const {
		return _bases;
	}
	
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
};

#endif

