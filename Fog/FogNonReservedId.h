
#ifndef FOGNONRESERVEDID_HXX
#define FOGNONRESERVEDID_HXX



class FogNonReservedId : public FogKeyword {
	typedef FogNonReservedId This;
	typedef FogKeyword Super;
	TYPEDECL_SINGLE(This, Super)
private:
	const FogTokenType::TokenType _token_type;
private:
	FogNonReservedId(const This&);
	FogNonReservedId& operator=(const This&);
	This& mutate() const { return *(This*)this; }
public:
	FogNonReservedId(const char* anId, FogTokenType::TokenType tokenEnum);
	virtual const FogTemplateParameterSpecifier* is_unnormalised_template_parameter
	(FogScopeContext& scopeContext) const;
	virtual const FogMetaType& meta_type() const;
	virtual FogTokenType::TokenType pp_token_type_enum() const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
	virtual NameSort sort_class() const;
	virtual FogTokenType::TokenType token_type_enum() const;
};
#endif

