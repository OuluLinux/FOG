
#ifndef FOGENUMERATORSPECIFIER_HXX
#define FOGENUMERATORSPECIFIER_HXX



class FogEnumeratorSpecifier : public FogSpecifier
{
 typedef FogSpecifier Super;
 typedef FogEnumeratorSpecifier This;
 TYPEDECL_SINGLE(This, Super)
 PRIMREF_DERIVED_DECLS(This)
 FOGTOKEN_DERIVED_DECLS
 FOGTOKEN_MEMBER_DECLS
 FOGTOKEN_LEAF_DECLS
private:
 FogExprRef _value;      //   Declarator defining enumerator value.
protected:
 FogEnumeratorSpecifier();
 virtual ~FogEnumeratorSpecifier();
public:
 FogEnumeratorSpecifier(FogName& anId, FogExpr *aValue);
 virtual const FogEnumeratorSpecifier *is_raw_enumerator() const;
 virtual const FogMetaType& meta_type() const;
 FogEnumerator *new_enumerator(FogScopeContext& inScope) const;
 virtual std::ostream& print_this(std::ostream& s) const;
 virtual std::ostream& print_viz(std::ostream& s) const;
 virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
};
#endif
 
