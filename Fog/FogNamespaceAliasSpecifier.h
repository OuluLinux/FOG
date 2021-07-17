
#ifndef FOGNAMESPACEALIASDEFINITION_HXX
#define FOGNAMESPACEALIASDEFINITION_HXX



class FogNamespaceAliasSpecifier : public FogSpecifier {
	typedef FogSpecifier Super;
	typedef FogNamespaceAliasSpecifier This;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_DERIVED_DECLS
	FOGTOKEN_MEMBER_DECLS
	FOGTOKEN_LEAF_DECLS
private:
	FogNameRef _value;                      //   Aliased namespace name.
protected:
	FogNamespaceAliasSpecifier();
	virtual ~FogNamespaceAliasSpecifier() {}
public:
	FogNamespaceAliasSpecifier(FogName& aliasName, FogName& aliasedName);
	virtual FogEntity* make_name_entity(FogMakeEntityContext& makeEntityContext) const;
	virtual FogEntity* make_type_entity(FogMakeEntityContext& makeEntityContext) const;
	virtual const FogMetaType& meta_type() const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
};
#endif

