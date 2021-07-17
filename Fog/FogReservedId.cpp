#include <Fog/FogIncludeAll.h>







TYPEINFO_SINGLE(FogReservedId, Super)

FogReservedId::FogReservedId(const char *anId, FogTokenType::TokenType ppTokenEnum,
        FogTokenType::TokenType tokenEnum)
:
    Super(anId),
    _pp_token_type(ppTokenEnum),
    _token_type(tokenEnum)
{}

bool FogReservedId::emit(FogEmitContext& emitContext) const { return emitContext.emit_identifier(id()); }

bool FogReservedId::get_object(FogTokenRef& returnValue, FogScopeContext& inScope) const
{
    returnValue = mutate();
    return true;
}

const FogMetaType& FogReservedId::meta_type() const { return FogMetaType::reserved_type(); }

bool FogReservedId::morph_to(FogTokenRef& returnValue, const FogMetaType& metaType, IsExposed isExposed,
    FogScopeContext& inScope) const
{
    if (metaType.is(FogMetaType::statement_type()))
    {
        FogTokenType::TokenType tokenType = token_type_enum();
        switch (tokenType)
        {
            case FogTokenType::BREAK:
                returnValue.adopt(new FogBreakStatement(FogBreakStatement::BREAK));
                return true;
            case FogTokenType::CONTINUE:
                returnValue.adopt(new FogBreakStatement(FogBreakStatement::CONTINUE));
                return true;
            case FogTokenType::RETURN:
                returnValue.adopt(new FogReturnStatement(0));
                return true;
            default: break;
        }
    }
    return Super::morph_to(returnValue, metaType, isExposed, inScope);
}

FogTokenType::TokenType FogReservedId::pp_token_type_enum() const { return _pp_token_type; }

bool FogReservedId::resolve_semantics(FogSemanticsContext& theSemantics) const
{
    theSemantics.assign(FogSemantics::IS_RESERVED);
    return true;
}

FogTokenType::TokenType FogReservedId::token_type_enum() const { return _token_type; }
