
#ifndef FOGENUMERATORSPECIFIERS_HXX
#define FOGENUMERATORSPECIFIERS_HXX




class FogEnumeratorSpecifiers : public FogToken {
	typedef FogToken Super;
	typedef FogEnumeratorSpecifiers This;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	FOGTOKEN_DERIVED_DECLS
	FOGTOKEN_MEMBER_DECLS
	FOGTOKEN_LEAF_DECLS
private:
	FogEnumeratorSpecifierListOfRef _enumerators;
private:
	FogEnumeratorSpecifiers(const This&);
	FogEnumeratorSpecifiers& operator=(const This&);
protected:
	virtual ~FogEnumeratorSpecifiers();
public:
	FogEnumeratorSpecifiers();
	void add(FogEnumeratorSpecifier& anEnumerator);
	const FogEnumeratorSpecifierListOfRefToConst& enumerators() const { return _enumerators; }
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_this(std::ostream& s) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
};
#endif

