#include <Fog/FogIncludeAll.h>





TYPEINFO_SINGLE(FogPunctuation, Super)

FogPunctuation::FogPunctuation(const char *anId, FogTokenType::TokenType ppTokenEnum,
  FogTokenType::TokenType tokenEnum)
:
 Super(anId),
 _pp_token_type(ppTokenEnum),
 _token_type(tokenEnum)
{}

bool FogPunctuation::emit(FogEmitContext& emitContext) const { return emitContext.emit_space_and_text(id().str()); }
const FogMetaType& FogPunctuation::meta_type() const { return FogMetaType::punctuation_type(); }
FogTokenType::TokenType FogPunctuation::pp_token_type_enum() const { return _pp_token_type; }
std::ostream& FogPunctuation::print_viz(std::ostream& s) const { return s << "\"punctuation: " << id() << '\"'; }

bool FogPunctuation::resolve_semantics(FogSemanticsContext& theSemantics) const
{
 theSemantics.assign(FogSemantics::IS_PUNCTUATION);
 return true;
}

FogTokenType::TokenType FogPunctuation::token_type_enum() const { return _token_type; }
