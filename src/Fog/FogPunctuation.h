
#ifndef FOGPUNCTUATION_HXX
#define FOGPUNCTUATION_HXX



class FogPunctuation : public FogKeyword
{
 typedef FogPunctuation This;
 typedef FogKeyword Super;
 TYPEDECL_SINGLE(This, Super)
private:
 const FogTokenType::TokenType _pp_token_type;
 const FogTokenType::TokenType _token_type;
private:
 FogPunctuation(const This&);
 FogPunctuation& operator=(const This&);
public:
 FogPunctuation(const char *anId, FogTokenType::TokenType ppTokenEnum, FogTokenType::TokenType tokenEnum);
 virtual bool emit(FogEmitContext& emitContext) const;
 virtual const FogMetaType& meta_type() const;
 virtual FogTokenType::TokenType pp_token_type_enum() const;
 virtual std::ostream& print_viz(std::ostream& s) const;
 virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
 virtual FogTokenType::TokenType token_type_enum() const;
};
#endif
 
