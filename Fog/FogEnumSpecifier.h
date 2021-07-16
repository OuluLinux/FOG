
#ifndef FOGENUMSPECIFIER_HXX
#define FOGENUMSPECIFIER_HXX




class FogEnumSpecifier : public FogScopeSpecifier
{
 typedef FogEnumSpecifier This;
 typedef FogScopeSpecifier Super;
 TYPEDECL_SINGLE(FogEnumSpecifier, Super)
 FOGTOKEN_MEMBER_DECLS
 FOGTOKEN_LEAF_DECLS

private:
 FogEnumeratorSpecifiersRef _enumerators;  //   Enumerators defining the enum.
 
protected:
 FogEnumSpecifier(const This &thatSpecifier);
 virtual ~FogEnumSpecifier();
public:
 FogEnumSpecifier(FogName *aName, FogEnumeratorSpecifiers *theEnumerators);
 virtual const FogEnumSpecifier *is_raw_enum() const;
 virtual FogEntity *make_type_entity(FogMakeEntityContext& makeEntityContext) const;
 virtual const FogMetaType& meta_type() const;
 virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
 virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
 virtual std::ostream& print_viz(std::ostream& s) const;
 virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
};
#endif
 
