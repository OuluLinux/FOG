
#ifndef FOGMACROID_HXX
#define FOGMACROID_HXX



class FogMacroId : public FogIdentifier
{
 NEEDS_A_FRIEND_FOR_GNUC
 typedef FogMacroId This;
 typedef FogIdentifier Super;
 TYPEDECL_SINGLE(This, Super)
private:
 FogMacroId(const This&);
 FogMacroId& operator=(const This&);
private:
 FogMacroId(const PrimId& anId) : Super(anId) {}
public:
 virtual FogTokenType::TokenType pp_token_type_enum() const;
public:
 static FogKeyword& make(const PrimId& anId);
};
#endif
 
