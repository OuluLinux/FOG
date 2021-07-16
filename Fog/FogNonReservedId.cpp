#include <Fog/FogIncludeAll.h>





TYPEINFO_SINGLE(FogNonReservedId, Super)

FogNonReservedId::FogNonReservedId(const char *anId, FogTokenType::TokenType tokenEnum)
:
    Super(anId),
    _token_type(tokenEnum)
{}

const FogTemplateParameterSpecifier *FogNonReservedId::is_unnormalised_template_parameter
 (FogScopeContext& scopeContext) const
{
 return scopeContext.find_template_parameter_specifier(id());
}

const FogMetaType& FogNonReservedId::meta_type() const { return FogMetaType::identifier_type(); }
FogTokenType::TokenType FogNonReservedId::pp_token_type_enum() const { return FogTokenType::Identifier; }

std::ostream& FogNonReservedId::print_viz(std::ostream& s) const
{
 return s << "\"non-reserved-word: " << id() << '\"';
}

bool FogNonReservedId::resolve_semantics(FogSemanticsContext& theSemantics) const
 { return resolve_identifier_semantics(theSemantics); }
FogToken::NameSort FogNonReservedId::sort_class() const { return NAME_NAME; }
FogTokenType::TokenType FogNonReservedId::token_type_enum() const { return _token_type; }
