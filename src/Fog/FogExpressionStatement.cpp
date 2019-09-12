#include <Fog/FogIncludeAll.h>









TYPEINFO_SINGLE(FogExpressionStatement, Super)
FOGTOKEN_LEAF_IMPL(FogExpressionStatement)

FogExpressionStatement::FogExpressionStatement(const This& aStatement)
:
    Super(aStatement),
    _expr(aStatement._expr)
{}

FogExpressionStatement::FogExpressionStatement(FogExpr& anExpr, bool isMeta)
:
    _expr(anExpr)
{
    if (isMeta)
        set_is_meta();
}

FogExpressionStatement::~FogExpressionStatement() {}

bool FogExpressionStatement::compile_parsed(FogParseContext& parseContext)
{
    if (!is_meta())
        return _expr->compile_parsed(parseContext);
    FogMetaParseContext metaContext(parseContext);
    
    VERBOSE(printf("DEBUG   parseContext %x metaContext %x dynamic_token %x\n", &parseContext, &metaContext, &metaContext.dynamic_token());)
    
    return _expr->compile_parsed(metaContext);
}

void FogExpressionStatement::create_usage(FogUsageContext& aContext) const
{
    Super::create_usage(aContext);
    _expr->create_usage(aContext);
}

bool FogExpressionStatement::emit(FogEmitContext& emitContext) const
{
    if (is_meta())
    {
        FogEmitParseContext parseContext(emitContext, *this);
        FogUnbreakableParseContext exprContext(parseContext, *_expr);
        if (!_expr->compile_parsed(exprContext))
            return false;
        return true;
    }
    emitContext.emit_hash_line(line());
    _expr->emit(emitContext);
    if (!_expr->is_compound())
        emitContext.emit_space_and_text(";");
    return true;
}

size_t FogExpressionStatement::executable_tokens() const
{
    return 1 + _expr->executable_tokens();
}

void FogExpressionStatement::install(FogInstallContext& installContext) const
{
    Super::install(installContext);
    _expr->install(installContext);
}

bool FogExpressionStatement::is_actual(const FogScopeContext& scopeContext) const
{
    return Super::is_actual(scopeContext) && _expr->is_actual(scopeContext);
}

void FogExpressionStatement::make_actual_from(FogMakeActualContext& makeActualContext)
{
    Super::make_actual_from(makeActualContext);
    _expr->make_actual(_expr, makeActualContext);
}

bool FogExpressionStatement::make_specifier(FogMakeSpecifierContext& makeSpecifierContext)
{
    bool isOk = false;
    if (!is_meta())
        isOk = _expr->make_specifier(makeSpecifierContext);
    else
    {
        FogMetaMakeSpecifierContext metaContext(makeSpecifierContext);
        isOk = _expr->make_specifier(metaContext);
    }
    if (isOk)
        makeSpecifierContext.specifier().set_line(line());
    return isOk;
}

void FogExpressionStatement::merge_from(FogMergeContext& mergeContext, const This& thatStmt)
{
    Super::merge_from(mergeContext, thatStmt);
    thatStmt._expr->merge_into(mergeContext, _expr.to_const());
}

const FogMetaType& FogExpressionStatement::meta_type() const { return FogMetaType::expression_statement_type(); }

bool FogExpressionStatement::morph_to(FogTokenRef& returnValue, const FogMetaType& metaType, IsExposed isExposed,
    FogScopeContext& inScope) const
{
    if (metaType.is(FogMetaType::expression_type()))
    {
        returnValue = *_expr;
        return true;
    }
    else
        return Super::morph_to(returnValue, metaType, isExposed, inScope);
}

const FogMerge& FogExpressionStatement::needs_merge_from(FogMergeContext& mergeContext, const This& thatStmt) const
{
    FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, thatStmt));
    needsMerge |= _expr->needs_merge(mergeContext, *thatStmt._expr);
    return needsMerge;
}

std::ostream& FogExpressionStatement::print_depth(std::ostream& s, int aDepth) const
{
    Super::print_depth(s, aDepth);
    _expr->print_deep(s, aDepth);
    return s;
}

std::ostream& FogExpressionStatement::print_members(std::ostream& s, int aDepth) const
{
    Super::print_members(s, aDepth);
    _expr->print_on(s, aDepth);
    return s;
}

char FogExpressionStatement::print_named(std::ostream& s, const PrimId *fullId, char tailChar) const
{
    if (is_meta())
  tailChar = FogStream::space_and_emit(s, tailChar, "auto ");
    return _expr->print_named(s, fullId, tailChar);
}

std::ostream& FogExpressionStatement::print_viz(std::ostream& s) const
{
    s << (is_meta() ? "\"meta-expression-statement: " : "\"expression-statement: ");
 print_this(s);
 return s << '\"';
}

bool FogExpressionStatement::resolve_semantics(FogSemanticsContext& theSemantics) const
{
    if (!is_meta())
        return Super::resolve_semantics(theSemantics);
 FogMetaSemanticsContext exprSemantics(theSemantics);
    if (!_expr->resolve_semantics(exprSemantics))     //  .bugbug ? a fudge for an object-statement
        return false;
    if (exprSemantics.is_meta_variable_declaration())
  theSemantics.set(FogSemantics::IS_AUTO_VARIABLE_DECLARATION);
    return true;
}
