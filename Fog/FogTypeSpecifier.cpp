#include <Fog/FogIncludeAll.h>







TYPEINFO_SINGLE(FogTypeSpecifier, Super)
FOGTOKEN_READ_ONLY_IMPL(FogTypeSpecifier)

FogTypeSpecifier::FogTypeSpecifier(FogName *aName)
:
    Super(FogTag::invalid_tag(), aName ? *aName : FogName::mutable_null())
{}

FogTypeSpecifier::FogTypeSpecifier(const FogTag& aTag, FogName& aName,
    const FogAccess& nestedAccess, FogRaw *rawDeclarations)
:
    Super(aTag, aName),
    _nested_access(nestedAccess),
    _declarations(rawDeclarations)
{}

FogTypeSpecifier::~FogTypeSpecifier() {}

bool FogTypeSpecifier::is_actual(const FogScopeContext& scopeContext) const
{
    if (!Super::is_actual(scopeContext))
        return false;;
    FogNestedScopeContext nestedContext((FogScopeContext&)scopeContext, mutate());      //  .bugbug casts
    return _declarations->is_actual(nestedContext);
}

void FogTypeSpecifier::make_actual_from(FogMakeActualContext& makeActualContext)
{
    Super::make_actual_from(makeActualContext);
    if (makeActualContext.is_normalise_templates())
        _declarations->make_actual(_declarations.to_const(), makeActualContext);
    else if (makeActualContext.is_locate_formals())
    {
        FogBracedScopeContext bracedContext(makeActualContext, *this);
        FogUnresolvableTypeScopeContext typeContext(bracedContext, *this);
        FogUnresolvableScopeContext resolutionContext(typeContext);
        _declarations->make_actual(_declarations.to_const(), resolutionContext);
    }
    else if (makeActualContext.is_resolve_dollars())
    {
//          FogNestedTypeParseContext nestedParseContext(*parseContext, *this);
        FogBracedScopeContext bracedContext(makeActualContext, *this);
        FogMultiDollarScopeContext resolutionContext(bracedContext);
        _declarations->make_actual(_declarations.to_const(), resolutionContext);
    }
    else if (makeActualContext.is_resolve_multi_dollars())
    {
//          FogNestedTypeParseContext nestedParseContext(*parseContext, *this);
        FogBracedScopeContext bracedContext(makeActualContext, *this);
        FogMultiDollarScopeContext resolutionContext(bracedContext);
        _declarations->make_actual(_declarations.to_const(), resolutionContext);
    }
    else if (makeActualContext.is_resolve_formals())
        _declarations->make_actual(_declarations.to_const(), makeActualContext);
    else
        ERRMSG("INVESTIGATE -- use of unclassified FogTypeSpecifier::make_actual_from.");
}

void FogTypeSpecifier::make_body(FogMakeEntityContext& makeEntityContext, FogScope& aScope) const
{
    aScope.set_braced(makeEntityContext);
    if (_declarations)
    {                           //   Nested parse context provides a locally updated access and utility
        FogMembersParseContext parseContext(makeEntityContext, *_declarations, aScope, *_nested_access);
        _declarations->compile_declaration(parseContext);
    }
}

const FogMetaType& FogTypeSpecifier::meta_type() const { return tag().specifier_meta_type(); }

std::ostream& FogTypeSpecifier::print_depth(std::ostream& s, int aDepth) const
{
    Super::print_depth(s, aDepth);
    _declarations->print_deep(s, aDepth);
    return s;
}

std::ostream& FogTypeSpecifier::print_members(std::ostream& s, int aDepth) const
{
    Super::print_members(s, aDepth);
    _declarations->print_on(s, aDepth);
    return s;
}
