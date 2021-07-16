#include <Fog/FogIncludeAll.h>












TYPEINFO_SINGLE(FogScopedExpr, Super)
FOGTOKEN_LEAF_IMPL(FogScopedExpr)

FogScopedExpr::FogScopedExpr(FogExpr& anExpr, FogRawSubspace& aSubspace)
:
    Super(anExpr),
    _subspace(aSubspace)
{}

FogScopedExpr::FogScopedExpr(const This& anExpr)
:
    Super(anExpr),
    _subspace(anExpr._subspace)     //  .bugbug clone ?
{}

FogScopedExpr::~FogScopedExpr() {}

void FogScopedExpr::create_usage(FogUsageContext& aContext) const
{
    ERRMSG("BUG -- should not invoke FogScopedExpr::create_usage.");
    Super::create_usage(aContext);
    _subspace->create_usage(aContext);
}

bool FogScopedExpr::emit(FogEmitContext& emitContext) const
{
    expr().emit(emitContext);
    emitContext.emit_space_and_text("\n");
    FogIndentEmitContext indentedContext(emitContext, 1, FogEmitContext::INDENTS);
    _subspace->emit(indentedContext);
    return true;
}

size_t FogScopedExpr::executable_tokens() const
{
    return Super::executable_tokens();
}

FogScope& FogScopedExpr::inner_scope()
{
    return FogRoot::mutable_null();         //   Occurs as context of UnresolvableParseContext
}

void FogScopedExpr::install(FogInstallContext& installContext) const
{
    ERRMSG("BUG -- should not invoke FogScopedExpr::install.");
    Super::install(installContext);
    _subspace->install(installContext);
}

bool FogScopedExpr::is_actual(const FogScopeContext& scopeContext) const
{
    return Super::is_actual(scopeContext) && _subspace->is_actual(scopeContext);
}

void FogScopedExpr::make_actual_from(FogMakeActualContext& makeActualContext)
{
    Super::make_actual_from(makeActualContext);
    if (makeActualContext.is_normalise_templates())
        _subspace->make_actual(_subspace.to_const(), makeActualContext);
    else if (makeActualContext.is_locate_formals())
    {
        FogBracedScopeContext bracedContext(makeActualContext, *this);
        FogUnresolvableScopeContext resolutionContext(bracedContext);
        _subspace->make_actual(_subspace.to_const(), resolutionContext);
    }
    else if (makeActualContext.is_resolve_dollars())
    {
        FogBracedScopeContext bracedContext(makeActualContext, *this);
        FogMultiDollarScopeContext resolutionContext(bracedContext);
        _subspace->make_actual(_subspace.to_const(), resolutionContext);
    }
    else if (makeActualContext.is_resolve_multi_dollars())
    {
        FogBracedScopeContext bracedContext(makeActualContext, *this);
        FogMultiDollarScopeContext resolutionContext(bracedContext);
        _subspace->make_actual(_subspace.to_const(), resolutionContext);
    }
    else if (makeActualContext.is_resolve_formals())
    {
        FogBracedScopeContext bracedContext(makeActualContext, *this);
        FogUnresolvableScopeContext resolutionContext(bracedContext);
        _subspace->make_actual(_subspace.to_const(), resolutionContext);
    }
    else
    {
        ERRMSG("INVESTIGATE -- use of unclassified FogScopedExpr::make_actual_from.");
        _subspace->make_actual(_subspace.to_const(), makeActualContext);
    }
}

bool FogScopedExpr::make_specifier(FogMakeSpecifierContext& makeSpecifierContext)
{
    if (!expr().make_specifier(makeSpecifierContext))
        return false;
    FogObjectSpecifier *objectSpecifier = makeSpecifierContext.get_object_specifier(*this);
    if (objectSpecifier)
        objectSpecifier->add_subspace(*_subspace);
    return true;
}

void FogScopedExpr::merge_from(FogMergeContext& mergeContext, const This& thatExpr)
{
    Super::merge_from(mergeContext, thatExpr);
    thatExpr._subspace->merge_into(mergeContext, _subspace.to_const());
}

const FogMerge& FogScopedExpr::needs_merge_from(FogMergeContext& mergeContext, const This& anExpr) const
{
    FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, anExpr));
    needsMerge |= _subspace->needs_merge(mergeContext, *anExpr._subspace);
    return needsMerge;
}

std::ostream& FogScopedExpr::print_depth(std::ostream& s, int aDepth) const
{
    Super::print_depth(s, aDepth);
    _subspace->print_depth(s, aDepth+1);
    return s;
}

std::ostream& FogScopedExpr::print_long_id(std::ostream& s) const
{
    return s << ":{ ... }";        //   Occurs as context of UnresolvableParseContext
}

std::ostream& FogScopedExpr::print_members(std::ostream& s, int aDepth) const
{
    Super::print_members(s, aDepth);
    _subspace->print_members(s, aDepth+1);
    return s;
}

char FogScopedExpr::print_named(std::ostream& s, const PrimId *fullId, char tailChar) const
{
    tailChar = Super::print_named(s, fullId, tailChar);
    tailChar = FogStream::space_and_emit(s, tailChar, " :{ ");
    tailChar = _subspace->print_named(s, 0, tailChar);
    tailChar = FogStream::space_and_emit(s, tailChar, " }");
    return tailChar;
}

std::ostream& FogScopedExpr::print_viz(std::ostream& s) const
{
    s << "\"scoped-expression: ";
    print_this(s);
    return s << '\"';
}

bool FogScopedExpr::resolve_semantics(FogSemanticsContext& theSemantics) const
{
    FogChildSemanticsContext exprSemantics(theSemantics);
    if (!expr().resolve_semantics(exprSemantics))
        return false;
    theSemantics.reset();
    if (exprSemantics.is_bit_field_declaration_elem())
        theSemantics.set(FogSemantics::IS_BIT_FIELD_DECLARATION);
    if (exprSemantics.is_object_declaration())
        theSemantics.set(FogSemantics::IS_OBJECT_DECLARATION);
    if (exprSemantics.is_object_declarator())
        theSemantics.set(FogSemantics::IS_OBJECT_DECLARATOR);
    if (exprSemantics.is_function_declarative())
        theSemantics.set(FogSemantics::IS_FUNCTION_DEFINITION);
    if (exprSemantics.is_constructor_declarative())
        theSemantics.set(FogSemantics::IS_FUNCTION_DEFINITION);
    if (exprSemantics.is_typedef_declaration())
        theSemantics.set(FogSemantics::IS_TYPEDEF_DECLARATION);
    if (exprSemantics.is_using_declarative())
        theSemantics.set(FogSemantics::IS_USING_DECLARATION);
    if (exprSemantics.is_meta_ctor_declaration())
        theSemantics.set(FogSemantics::IS_META_CTOR_DEFINITION);
    if (exprSemantics.is_meta_dtor_declaration())
        theSemantics.set(FogSemantics::IS_META_DTOR_DEFINITION);
    if (exprSemantics.is_meta_function_declarator())
        theSemantics.set(FogSemantics::IS_META_FUNCTION_DEFINITOR);
    if (exprSemantics.is_meta_function_declaration())
        theSemantics.set(FogSemantics::IS_META_FUNCTION_DEFINITION);
    if (exprSemantics.is_meta_variable_declarator_elem())
        theSemantics.set(FogSemantics::IS_META_VARIABLE_DECLARATOR);
    if (exprSemantics.is_meta_variable_type_id())
        theSemantics.set(FogSemantics::IS_META_VARIABLE_DECLARATION);
    if (exprSemantics.is_auto_ctor_declaration())
        theSemantics.set(FogSemantics::IS_AUTO_CTOR_DEFINITION);
    if (exprSemantics.is_auto_dtor_declaration())
        theSemantics.set(FogSemantics::IS_AUTO_DTOR_DEFINITION);
    if (exprSemantics.is_auto_function_declarator())
        theSemantics.set(FogSemantics::IS_AUTO_FUNCTION_DEFINITOR);
    if (exprSemantics.is_auto_function_declaration())
        theSemantics.set(FogSemantics::IS_AUTO_FUNCTION_DEFINITION);
    if (exprSemantics.is_auto_variable_type_id())
        theSemantics.set(FogSemantics::IS_AUTO_VARIABLE_DECLARATION);
    return true;
}

void FogScopedExpr::set_decl_specifier_expression(FogExprRef& anExpr, FogDeclSpecifier& declSpecifiers)
{
    set_decorated_decl_specifier_expression(anExpr, declSpecifiers);
}
