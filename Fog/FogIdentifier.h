
#ifndef FOGIDENTIFIER_HXX
#define FOGIDENTIFIER_HXX



class FogIdentifier : public FogKeyword {
	typedef FogIdentifier This;
	typedef FogKeyword Super;
	TYPEDECL_SINGLE(This, Super)
	//  	PRIMREF_DERIVED_DECLS(This)
	
private:
	const bool _is_valid;        //   true if identifier is semantically valid as an identifier.
	
private:
	FogIdentifier(const This&);
	FogIdentifier& operator=(const This&);
	
protected:
	FogIdentifier(const PrimId& anId);
	
public:
	virtual const FogTemplateParameterSpecifier* is_unnormalised_template_parameter
	(FogScopeContext& scopeContext) const;
	virtual const FogMetaType& meta_type() const;
	virtual FogTokenType::TokenType pp_token_type_enum() const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
	virtual NameSort sort_class() const;
	virtual FogTokenType::TokenType token_type_enum() const;
	
public:
	static FogKeyword& make(const char* aBuffer, size_t aLength) {
		return make(*PrimIdHandle(aBuffer, aLength));
	}
	
	static FogKeyword& make(const PrimId& anId);
};

#endif

