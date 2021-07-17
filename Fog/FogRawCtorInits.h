
#ifndef FOGRAWCTORINITS_HXX
#define FOGRAWCTORINITS_HXX




class FogRawCtorInits : public FogToken {
	typedef FogToken Super;
	typedef FogRawCtorInits This;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	FOGTOKEN_DERIVED_DECLS
	FOGTOKEN_UNIQUE_DECLS
private:
	FogRawCtorInitListOfRefToConst _inits;
private:
	FogRawCtorInits(const This&);
	FogRawCtorInits& operator=(const This&);
protected:
	virtual ~FogRawCtorInits();
public:
	FogRawCtorInits();
	void add(FogRawCtorInit& ctorInit);
	const FogRawCtorInitListOfRefToConst& inits() const { return _inits; }
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_this(std::ostream& s) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
};
#endif

