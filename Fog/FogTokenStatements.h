
#ifndef FOGTOKENSTATEMENTS_HXX
#define FOGTOKENSTATEMENTS_HXX



class FogTokenStatements : public FogTokens {
	typedef FogTokenStatements This;
	typedef FogTokens Super;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	FOGTOKEN_DERIVED_DECLS
	FOGTOKEN_LEAF_DECLS
private:
	const FogMetaType& _meta_type;
private:
	This& mutate() const { return *(This*)this; }
protected:
	FogTokenStatements();
	virtual ~FogTokenStatements();
	virtual void make_actual_from(FogMakeActualContext& makeActualContext);
public:
	FogTokenStatements(const FogMetaType& metaType, FogTokens* someTokens = 0);
	virtual bool get_dyadic(FogTokenRef& returnValue, FogScopeContext& inScope,
	                        FogTokenType::TokenType dyadicOp, const FogToken& rightValue) const;
	virtual const FogMetaType& meta_type() const;
	virtual bool morph_to(FogTokenRef& returnValue, const FogMetaType& metaType, IsExposed isExposed,
	                      FogScopeContext& inScope) const;
	virtual char print_named(std::ostream& s, const PrimId* scopeId, char tailChar) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
};
#endif

