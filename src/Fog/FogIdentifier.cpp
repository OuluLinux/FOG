#include <Fog/FogIncludeAll.h>








TYPEINFO_SINGLE(FogIdentifier, Super)

FogIdentifier::FogIdentifier(const PrimId& anId)
:
 Super(anId),
 _is_valid(is_valid(anId))
{}

const FogTemplateParameterSpecifier *FogIdentifier::is_unnormalised_template_parameter
 (FogScopeContext& scopeContext) const
{
 if (!_is_valid)
  return 0;
 return scopeContext.find_template_parameter_specifier(id());
}

FogKeyword& FogIdentifier::make(const PrimId& anId)
{
 static FogKeywordMapOfRef identifierMap;
 FogKeyword *anIdentifier = identifierMap.find(anId);
 if (!anIdentifier)
 {
  anIdentifier = new FogIdentifier(anId);
  if (anIdentifier)
   identifierMap.adopt(anIdentifier);
 }
 if (anIdentifier)
  return *anIdentifier;
 else
  return FogIdentifier::mutable_null();
}

const FogMetaType& FogIdentifier::meta_type() const { return FogMetaType::identifier_type(); }
FogTokenType::TokenType FogIdentifier::pp_token_type_enum() const { return FogTokenType::Identifier; }

std::ostream& FogIdentifier::print_viz(std::ostream& s) const
{
 return s << "\"identifier: " << id() << '\"';
}

bool FogIdentifier::resolve_semantics(FogSemanticsContext& theSemantics) const
 { return resolve_identifier_semantics(theSemantics); }
FogToken::NameSort FogIdentifier::sort_class() const { return NAME_NAME; }
FogTokenType::TokenType FogIdentifier::token_type_enum() const { return FogTokenType::Identifier; }
