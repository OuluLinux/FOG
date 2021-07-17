
#ifndef FOGLINKAGESPECIFIER_HXX
#define FOGLINKAGESPECIFIER_HXX





class FogLinkageSpecifier : public FogTypeSpecifier {
	typedef FogLinkageSpecifier This;
	typedef FogTypeSpecifier Super;
	TYPEDECL_SINGLE(FogLinkageSpecifier, Super)
	
private:
	const PrimIdHandle _linkage;     //   String defining linkage space.
protected:
	FogLinkageSpecifier(const This& thatSpecifier);
	virtual ~FogLinkageSpecifier();
	virtual FogToken* clone() const;
public:
	FogLinkageSpecifier(const PrimId& anId, FogRaw* aDeclaration);
	virtual FogLinkageSpecifier* is_linkage_specifier();
	const PrimId& linkage() const { return *_linkage; }
	virtual FogEntity* make_type_entity(FogMakeEntityContext& makeEntityContext) const;
	virtual const FogMetaType& meta_type() const;
	virtual std::ostream& print_this(std::ostream& s) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
};
#endif

