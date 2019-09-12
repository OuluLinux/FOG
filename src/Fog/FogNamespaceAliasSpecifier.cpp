#include <Fog/FogIncludeAll.h>







TYPEINFO_SINGLE(FogNamespaceAliasSpecifier, Super)
//  PRIMREF_NULL_CLAS(FogNamespaceAliasSpecifier)
//      FOGTOKEN_NULL_LEAF
//  };
//  PRIMREF_DERIVED_NULL_IMPL(FogNamespaceAliasSpecifier)
//  PRIMREFS_IMPL(FogNamespaceAliasSpecifier)
FOGTOKEN_LEAF_IMPL(FogNamespaceAliasSpecifier)
//TMPL_HACK_FIX_DO(FogNamespaceAliasSpecifier)

FogNamespaceAliasSpecifier::FogNamespaceAliasSpecifier() {}

FogNamespaceAliasSpecifier::FogNamespaceAliasSpecifier(FogName& aliasName, FogName& aliasedName)
:
    Super(aliasName),
    _value(aliasedName)
{}

void FogNamespaceAliasSpecifier::create_usage(FogUsageContext& aContext) const
{
    Super::create_usage(aContext);
    _value->create_usage(aContext);
}

bool FogNamespaceAliasSpecifier::emit(FogEmitContext& emitContext) const
{
    emitContext.emit_space_and_text("namepsace ");
    Super::emit(emitContext);
    if (_value)
    {
        emitContext.emit_space_and_text(" = ");
        _value->emit(emitContext);
    }
    return true;
}

void FogNamespaceAliasSpecifier::install(FogInstallContext& installContext) const
{
    Super::install(installContext);
    _value->install(installContext);
}

bool FogNamespaceAliasSpecifier::is_actual(const FogScopeContext& scopeContext) const
{
    return Super::is_actual(scopeContext) && _value->is_actual(scopeContext);
}

void FogNamespaceAliasSpecifier::make_actual_from(FogMakeActualContext& makeActualContext)
{
    Super::make_actual_from(makeActualContext);
    _value->make_actual(_value.to_const(), makeActualContext);
}

FogEntity *FogNamespaceAliasSpecifier::make_name_entity(FogMakeEntityContext& makeEntityContext) const
{
    ERRMSG("ROUNDTUIT -- missing code for FogNamespaceAliasSpecifier::make_name_entity.");
    return 0;
}

FogEntity *FogNamespaceAliasSpecifier::make_type_entity(FogMakeEntityContext& makeEntityContext) const { return 0; }

void FogNamespaceAliasSpecifier::merge_from(FogMergeContext& mergeContext, const This& thatExpr)
{
    Super::merge_from(mergeContext, thatExpr);
    thatExpr._value->merge_into(mergeContext, _value.to_const());
}

const FogMetaType& FogNamespaceAliasSpecifier::meta_type() const
    { return FogMetaType::namespace_alias_definition_type(); }

const FogMerge& FogNamespaceAliasSpecifier::needs_merge_from(FogMergeContext& mergeContext, const This& thatExpr) const
{
    FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, thatExpr));
    needsMerge |= _value->needs_merge(mergeContext, *thatExpr._value);
    return needsMerge;
}

char FogNamespaceAliasSpecifier::print_named(std::ostream& s, const PrimId *fullId, char tailChar) const
{
    tailChar = FogStream::space_and_emit(s, tailChar, "namespace ");
    tailChar = Super::print_named(s, fullId, tailChar);
    if (_value)
    {
        tailChar = FogStream::space_and_emit(s, tailChar, " = ");
        tailChar = _value->print_named(s, 0, tailChar);
    }
    return tailChar;
}

std::ostream& FogNamespaceAliasSpecifier::print_viz(std::ostream& s) const
{
    s << "\"namespace-alias-definition: ";
    print_this(s);
    return s << '\"';
}

bool FogNamespaceAliasSpecifier::resolve_semantics(FogSemanticsContext& theSemantics) const
{
    FogChildSemanticsContext nameSemantics(theSemantics);
    if (!name().resolve_semantics(nameSemantics))
        return false;
    theSemantics.reset();
    if (nameSemantics.is_scoped_identifier())
        theSemantics.assign(FogSemantics::IS_NAMESPACE_ALIAS_DEFINITION);
    return true;
}
