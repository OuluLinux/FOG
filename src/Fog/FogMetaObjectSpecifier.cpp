#include <Fog/FogIncludeAll.h>








TYPEINFO_SINGLE(FogMetaObjectSpecifier, Super)
FOGTOKEN_UNIQUE_IMPL(FogMetaObjectSpecifier)

FogMetaObjectSpecifier::FogMetaObjectSpecifier() {}

FogMetaObjectSpecifier::FogMetaObjectSpecifier(FogMakeSpecifierContext& makeSpecifierContext, FogName& nameName)
:
    Super(makeSpecifierContext, nameName)
{}

FogMetaObjectSpecifier::~FogMetaObjectSpecifier() {}

bool FogMetaObjectSpecifier::compile_specifier(FogMakeSpecifierContext& makeSpecifierContext)
{
    if (!Super::compile_specifier(makeSpecifierContext))
        return false;
    PrimIdHandle typeId;
    type().resolve_id(typeId, makeSpecifierContext);
    const FogMetaType *metaType = &FogMetaType::get(*typeId);
    return true;
}

const FogStatic& FogMetaObjectSpecifier::get_static() const
{
    return decl_specifiers().is_static() ? FogStatic::yes_static() : FogStatic::not_static();
}

FogToken::IsExposed FogMetaObjectSpecifier::is_exposed() const
{
    const FogModifierListOfRefToConst& modList = modifiers();
    if (modList.tally() && modList[0]->is_array_modifier())
        return IS_EXPOSED;
    return IS_ENCAPSULATED;
}

FogMetaObjectSpecifier *FogMetaObjectSpecifier::is_meta_object_specifier() { return this; }

FogEntity *FogMetaObjectSpecifier::make_name_entity(FogMakeEntityContext& makeEntityContext) const
{
    FogEntityMakerContext makerContext(makeEntityContext, *this, &FogScope::make_meta_slot);
    return make_entity(makerContext);
}

FogEntity *FogMetaObjectSpecifier::make_type_entity(FogMakeEntityContext& makeEntityContext) const { return 0; }

const FogMetaType& FogMetaObjectSpecifier::resolve_type_meta_type(FogScopeContext& scopeContext) const
{
    PrimIdHandle typeId;
    type().resolve_id(typeId, scopeContext);
    return FogMetaType::get(*typeId);
}
