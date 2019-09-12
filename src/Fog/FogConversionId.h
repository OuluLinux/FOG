
#ifndef FOGCONVERSIONID_HXX
#define FOGCONVERSIONID_HXX



class FogConversionId : public FogEntityName
{
 NEEDS_A_FRIEND_FOR_GNUC
 typedef FogEntityName Super;
 typedef FogConversionId This;
 TYPEDECL_SINGLE(This, Super)
 FOGTOKEN_MEMBER_DECLS
 FOGTOKEN_LEAF_DECLS
private:
 FogExprRefToConst _declarator;
 mutable PrimIdHandle _resolved_id;   //   NB spelling can be scope-dependent
private:
 FogConversionId(const This& aDecl);
 virtual ~FogConversionId();
 This& mutate() const { return *(This *)this; }
public:
 FogConversionId(const FogExpr& aDeclarator);
 virtual const PrimId *is_resolved() const;
 virtual std::ostream& print_viz(std::ostream& s) const;
 virtual bool resolve_id(PrimIdHandle& returnId, FogScopeContext& inScope) const;
 virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
 virtual NameSort sort_class() const;
};
#endif
 
