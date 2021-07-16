#include <Fog/FogIncludeAll.h>









TYPEINFO_SINGLE(FogDeleteExpr, Super)
FOGTOKEN_LEAF_IMPL(FogDeleteExpr)

FogDeleteExpr::FogDeleteExpr(const This& anExpr)
:
    Super(anExpr),
    _is_global(anExpr._is_global),
    _is_array(anExpr._is_array)
{}

void FogDeleteExpr::create_usage(FogUsageContext& aContext) const
{
    Super::create_usage(aContext);
}

bool FogDeleteExpr::emit(FogEmitContext& emitContext) const
{
    if (_is_global)
        emitContext.emit_space_and_text(" ::");
    emitContext.emit_space_and_text(_is_array ? "delete[] " : "delete ");
    expr().emit(emitContext);
    return true;
}

size_t FogDeleteExpr::executable_tokens() const
{
    return 1 + expr().executable_tokens();
}

void FogDeleteExpr::install(FogInstallContext& installContext) const
{
    Super::install(installContext);
}

bool FogDeleteExpr::is_actual(const FogScopeContext& scopeContext) const
{
    return Super::is_actual(scopeContext);
}

void FogDeleteExpr::make_actual_from(FogMakeActualContext& makeActualContext)
{
    Super::make_actual_from(makeActualContext);
}

void FogDeleteExpr::merge_from(FogMergeContext& mergeContext, const This& thatExpr)
{
    Super::merge_from(mergeContext, thatExpr);
    _is_global = merge_bool("global scope", _is_global != 0, thatExpr._is_global != 0) ? GLOBAL_DELETE : SCOPED_DELETE;
}

const FogMerge& FogDeleteExpr::needs_merge_from(FogMergeContext& mergeContext, const This& anExpr) const
{
    FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, anExpr));
    if (_is_global != anExpr._is_global)
        needsMerge |= FogMerge::incompatible_allocator();
    return needsMerge;
}

char FogDeleteExpr::print_named(std::ostream& s, const PrimId *fullId, char tailChar) const
{
    if (_is_global)
        tailChar = FogStream::space_and_emit(s, tailChar, " ::");
    tailChar = FogStream::space_and_emit(s, tailChar, _is_array ? "delete[] " : "delete ");
    tailChar = expr().print_named(s, fullId, tailChar);
    return tailChar;
}

std::ostream& FogDeleteExpr::print_viz(std::ostream& s) const
{
    s << "\"delete-expression: ";
    print_this(s);
    return s << '\"';
}

bool FogDeleteExpr::resolve_semantics(FogSemanticsContext& theSemantics) const
{
    FogChildSemanticsContext exprSemantics(theSemantics);
    if (!expr().resolve_semantics(exprSemantics))
        return false;
    theSemantics.reset();
    if (exprSemantics.is_basic_expression())
        theSemantics.set(FogSemantics::IS_BASIC_EXPRESSION);
    return true;
}
