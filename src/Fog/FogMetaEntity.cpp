#include <Fog/FogIncludeAll.h>











TYPEINFO_SINGLE(FogMetaEntity, Super)
PRIMREF_NULL_CLAS(FogMetaEntity)
    virtual bool set_value(const FogToken& tokenValue, FogScopeContext& inScope) { return false; }
};
PRIMREF_DERIVED_NULL_IMPL(FogMetaEntity)
PRIMREFS_IMPL(FogMetaEntity)
PRIMMAPOFREFS_IMPL(FogMetaEntity)
FOGTOKEN_ACTUAL_IMPL(FogMetaEntity)
TMPL_HACK_FIX_DO(FogMetaEntity)

FogMetaEntity::FogMetaEntity()
:
    _slot(FogMetaSlot::immutable_null())
{}

FogMetaEntity::FogMetaEntity(FogToken& inToken, const FogMetaSlot& metaSlot)
:
    _token(inToken),
    _slot(metaSlot)
{}

FogMetaEntity::~FogMetaEntity() {}

void FogMetaEntity::destroy()
{
    _slot.reset();
    _token.reset();
    Super::destroy();
}

bool FogMetaEntity::get_dyadic(FogTokenRef& returnValue, FogScopeContext& inScope,
        FogTokenType::TokenType dyadicOp, const FogToken& rightValue) const
{
    switch (dyadicOp)
    {
        case '(':
        {
            if (inScope.requires_resolution())
                ERRMSG("Meta-function rather than meta-variable expected for " << viz(*this));
            returnValue = FogFailure::make();
            return false;
        }
        case FogTokenType::ARROW:
        {
            if (inScope.requires_resolution())
                ERRMSG("Meta-iterator expected for " << viz(*this));
            returnValue = FogFailure::make();
            return false;
        }
        default:
            return Super::get_dyadic(returnValue, inScope, dyadicOp, rightValue);
    }
}

bool FogMetaEntity::get_monadic(FogTokenRef& returnValue, FogScopeContext& inScope,
        FogTokenType::TokenType monadicOp) const
{
    switch (monadicOp)
    {
        case '*':
            if (inScope.requires_resolution())
                ERRMSG("Meta-iterator expected for " << viz(*this));
            returnValue = FogFailure::make();
            return false;
        case '=':               //   Monadic = is used to access a MetaVariable (a counterpart to dyadic f(args))
            if (inScope.requires_resolution())
                ERRMSG("Meta-variable rather than meta-function expected for " << viz(*this));
            returnValue = FogFailure::make();
            return false;
        default:
            break;
    }
    return Super::get_monadic(returnValue, inScope, monadicOp);
}

const PrimId& FogMetaEntity::id() const { return _slot->id(); }
FogToken::IsExposed FogMetaEntity::is_exposed() const { return _slot->is_exposed(); }
FogMetaEntity *FogMetaEntity::is_meta_entity() { return this; }

bool FogMetaEntity::make_specifier(FogMakeSpecifierContext& makeSpecifierContext)
{
    FogTokenRef returnValue;
    return get_object(returnValue, makeSpecifierContext) && returnValue->make_specifier(makeSpecifierContext);
}

std::ostream& FogMetaEntity::print_long_id(std::ostream& s) const
{
    _token->print_long_id(s);
    return s << "::" << id();
}

char FogMetaEntity::print_named(std::ostream& s, const PrimId *scopeId, char tailChar) const
{
    tailChar = _token->print_named(s, scopeId, tailChar);
    tailChar = FogStream::space_and_emit(s, tailChar, "::");
    tailChar = FogStream::space_and_emit(s, tailChar, id().str());
    return tailChar;
}

std::ostream& FogMetaEntity::print_this(std::ostream& s) const
{
    return print_long_id(s);
}

std::ostream& FogMetaEntity::print_viz(std::ostream& s) const
{
    return s << "\"meta-entity: ";
    print_long_id(s);
    return s << '\"';
}

bool FogMetaEntity::resolve_semantics(FogSemanticsContext& theSemantics) const
{
    theSemantics.assign(type().is_semantics());
    return true;
}

const FogMetaType& FogMetaEntity::type() const { return _slot->type(); }
