#include <Fog/FogIncludeAll.h>





TYPEINFO_SINGLE(FogMacroId, Super)

FogKeyword& FogMacroId::make(const PrimId& anId)
{
 static FogKeywordMapOfRef identifierMap;
 FogKeyword *anMacroId = identifierMap.find(anId);
 if (!anMacroId)
 {
  anMacroId = new FogMacroId(anId);
  if (anMacroId)
   identifierMap.adopt(anMacroId);
 }
 if (anMacroId)
  return *anMacroId;
 else
  return FogMacroId::mutable_null();
}

FogTokenType::TokenType FogMacroId::pp_token_type_enum() const { return FogTokenType::MacroId; }
