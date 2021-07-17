
#ifndef FOGRESERVEDID_HXX
#define FOGRESERVEDID_HXX



class FogReservedId : public FogKeyword {
	typedef FogReservedId This;
	typedef FogKeyword Super;
	TYPEDECL_SINGLE(This, Super)
private:
	const FogTokenType::TokenType _pp_token_type;
	const FogTokenType::TokenType _token_type;
private:
	FogReservedId(const This&);
	FogReservedId& operator=(const This&);
	This& mutate() const { return *(This*)this; }
public:
	FogReservedId(const char* anId, FogTokenType::TokenType ppTokenEnum, FogTokenType::TokenType tokenEnum);
	virtual bool emit(FogEmitContext& emitContext) const;
	virtual bool get_object(FogTokenRef& returnValue, FogScopeContext& inScope) const;
	virtual const FogMetaType& meta_type() const;
	virtual bool morph_to(FogTokenRef& returnValue, const FogMetaType& metaType, IsExposed isExposed,
	                      FogScopeContext& inScope) const;
	virtual FogTokenType::TokenType pp_token_type_enum() const;
	virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
	virtual FogTokenType::TokenType token_type_enum() const;
};
#endif

