
#ifndef FOGUSINGSPECIFIER_HXX
#define FOGUSINGSPECIFIER_HXX



class FogUsingSpecifier : public FogSpecifier {
	typedef FogSpecifier Super;
	typedef FogUsingSpecifier This;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	FOGTOKEN_LEAF_DECLS
protected:
	FogUsingSpecifier();
	virtual ~FogUsingSpecifier() {}
public:
	FogUsingSpecifier(FogName& nameName);
	virtual FogEntity* make_name_entity(FogMakeEntityContext& makeEntityContext) const;
	virtual FogEntity* make_type_entity(FogMakeEntityContext& makeEntityContext) const;
	virtual const FogMetaType& meta_type() const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
};
#endif

