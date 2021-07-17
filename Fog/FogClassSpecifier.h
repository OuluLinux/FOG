
#ifndef FOGCLASSSPECIFIER_HXX
#define FOGCLASSSPECIFIER_HXX




class FogClassSpecifier : public FogTypeSpecifier {
	typedef FogClassSpecifier This;
	typedef FogTypeSpecifier Super;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_READ_ONLY_DECLS
	
private:
	FogBaseSpecifiersRef _bases;      //   The base classes.
	
protected:
	FogClassSpecifier(const This& thatSpecifier);
	virtual ~FogClassSpecifier();
	
public:
	FogClassSpecifier(const FogTag& aTag, FogName* aName, FogBaseSpecifiers* baseSpecifiers);
	virtual FogEntity* make_type_entity(FogMakeEntityContext& makeEntityContext) const;
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
};

#endif

