
#ifndef FOGTHISEXPR_HXX
#define FOGTHISEXPR_HXX




class FogThisExpr : public FogExpr
{
 typedef FogExpr Super;
 typedef FogThisExpr This;
 TYPEDECL_SINGLE(This, Super)
 FOGTOKEN_LEAF_DECLS
protected:
 FogThisExpr(const This& anExpr);
 virtual ~FogThisExpr();
public:
 FogThisExpr();
 virtual bool emit(FogEmitContext& emitContext) const;
 virtual bool get_object(FogTokenRef& tokenValue, FogScopeContext& inScope) const;
 virtual char print_named(std::ostream& s, const PrimId *fullId, char tailChar) const;
 virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
};
#endif
 
