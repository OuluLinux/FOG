
#ifndef FOGBASESPECIFIER_HXX
#define FOGBASESPECIFIER_HXX



class FogBaseSpecifier : public FogSpecifier {
	typedef FogSpecifier Super;
	typedef FogBaseSpecifier This;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	FOGTOKEN_DERIVED_DECLS
	FOGTOKEN_LEAF_DECLS
	
protected:
	FogBaseSpecifier();
	virtual ~FogBaseSpecifier();
	
public:
	FogBaseSpecifier(FogName& aName);
	virtual const FogMetaType& meta_type() const;
	void make_base(FogMakeEntityContext& makeEntityContext, FogScope& inScope) const;
	virtual std::ostream& print_this(std::ostream& s) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	void set_access(const FogAccess& anAccess);
	void set_virtual(const FogVirtual& aVirtual);
};

#endif

