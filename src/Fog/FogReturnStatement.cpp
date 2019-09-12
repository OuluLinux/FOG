#include <Fog/FogIncludeAll.h>










TYPEINFO_SINGLE(FogReturnStatement, Super)
FOGTOKEN_LEAF_IMPL(FogReturnStatement)

FogReturnStatement::FogReturnStatement(const FogExpr *anExpr)
:
    _expr(anExpr)
{}

FogReturnStatement::FogReturnStatement(const This& aStatement)
:
    Super(aStatement),
    _expr(aStatement._expr)
{}

FogReturnStatement::~FogReturnStatement() {}

void FogReturnStatement::create_usage(FogUsageContext& aContext) const
{
    Super::create_usage(aContext);
    _expr->create_usage(aContext);
}

bool FogReturnStatement::emit(FogEmitContext& emitContext) const
{
    emitContext.emit_hash_line(line());
    emitContext.emit_space_and_text("return");
    if (_expr)
    {
        FogSeparatedEmitContext separatedContext(emitContext, ",", 0);
        _expr->emit(separatedContext);
    }
    emitContext.emit_space_and_text(";");
    return true;
}

size_t FogReturnStatement::executable_tokens() const { return 1 + _expr->executable_tokens(); }

void FogReturnStatement::install(FogInstallContext& installContext) const
{
    Super::install(installContext);
    _expr->install(installContext);
}

bool FogReturnStatement::is_actual(const FogScopeContext& scopeContext) const
{
    return Super::is_actual(scopeContext) && _expr->is_actual(scopeContext);
}

void FogReturnStatement::make_actual_from(FogMakeActualContext& makeActualContext)
{
    Super::make_actual_from(makeActualContext);
    _expr->make_actual(_expr.to_const(), makeActualContext);
}

void FogReturnStatement::merge_from(FogMergeContext& mergeContext, const This& thatExpr)
{
    Super::merge_from(mergeContext, thatExpr);
    thatExpr._expr->merge_into(mergeContext, _expr.to_const());
}

const FogMerge& FogReturnStatement::needs_merge_from(FogMergeContext& mergeContext, const This& thatExpr) const
{
    FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, thatExpr));
    needsMerge |= _expr->needs_merge(mergeContext, *thatExpr._expr);
    return needsMerge;
}

std::ostream& FogReturnStatement::print_depth(std::ostream& s, int aDepth) const
{
    Super::print_depth(s, aDepth);
    _expr->print_deep(s, aDepth);
    return s;
}

std::ostream& FogReturnStatement::print_members(std::ostream& s, int aDepth) const
{
    Super::print_members(s, aDepth);
    _expr->print_on(s, aDepth);
    return s;
}

char FogReturnStatement::print_named(std::ostream& s, const PrimId *fullId, char tailChar) const
{
    if (is_meta())
        tailChar = FogStream::space_and_emit(s, tailChar, "auto ");
    tailChar = FogStream::space_and_emit(s, tailChar, "return ");
    tailChar = _expr->print_named(s, 0, tailChar);
    tailChar = FogStream::space_and_emit(s, tailChar, ")");
    return tailChar;
}

void FogReturnStatement::set_is_meta()
{
    ERRMSG("Meta-return-statements are not supported.");
}
