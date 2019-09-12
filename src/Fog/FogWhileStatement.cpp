#include <Fog/FogIncludeAll.h>









TYPEINFO_SINGLE(FogWhileStatement, Super)
FOGTOKEN_LEAF_IMPL(FogWhileStatement)

FogWhileStatement::FogWhileStatement(const FogExpr& testExpr, FogRaw& bodyStatement)
:
    _test(testExpr),
    _body(bodyStatement)
{}

FogWhileStatement::FogWhileStatement(const This& aStatement)
:
    Super(aStatement),
    _test(aStatement._test),
    _body(aStatement._body)
{}

FogWhileStatement::~FogWhileStatement() {}

bool FogWhileStatement::compile_parsed(FogParseContext& parseContext)
{
    FogMetaParseContext metaContext(parseContext);
    while (true)
    {
        const PrimNumber& testValue = _test->get_number(metaContext);
        if (testValue.is_null() || testValue.is_zero())
            return true;
        FogBreakableParseContext bodyContext(parseContext, *_body);
        if (!_body->compile_declaration(bodyContext))
        {
            if (bodyContext.do_break())
                return true;
            if (bodyContext.do_continue())
                continue;
            return false;
        }
    }
}

void FogWhileStatement::create_usage(FogUsageContext& aContext) const
{
    Super::create_usage(aContext);
    _test->create_usage(aContext);
    _body->create_usage(aContext);
}

bool FogWhileStatement::emit(FogEmitContext& emitContext) const
{
    if (is_meta())
    {
        FogEmitParseContext parseContext(emitContext, *this);
        while (true)
        {
            const PrimNumber& testValue = _test->get_number(parseContext);
            if (testValue.is_null() || testValue.is_zero())
                return true;
            FogBreakableEmitContext bodyContext(emitContext, *_body);
            if (!_body->emit(bodyContext))
            {
                if (bodyContext.do_break())
                    return true;
                if (bodyContext.do_continue())
                    continue;
                return false;
            }
        }
    }
    else
    {
        emitContext.emit_hash_line(line());
        emitContext.emit_space_and_text("while (");
        {
            FogSeparatedEmitContext separatedContext(emitContext, ",", 0);
            _test->emit(separatedContext);
        }
        emitContext.emit_space_and_text(")");
        {
            FogSeparatedEmitContext separatedContext(emitContext, "", "\n");
            FogIndentEmitContext indentedContext(separatedContext, 1, FogEmitContext::INDENTS);
            _body->emit(indentedContext);
        }
    }
    return true;
}

size_t FogWhileStatement::executable_tokens() const
{
    size_t executableTokens = 1 + _test->executable_tokens() + _body->executable_tokens();
    return executableTokens;
}

void FogWhileStatement::install(FogInstallContext& installContext) const
{
    Super::install(installContext);
    _test->install(installContext);
    _body->install(installContext);
}

bool FogWhileStatement::is_actual(const FogScopeContext& scopeContext) const
{
    return Super::is_actual(scopeContext) && _test->is_actual(scopeContext) && _body->is_actual(scopeContext);
}

void FogWhileStatement::make_actual_from(FogMakeActualContext& makeActualContext)
{
    Super::make_actual_from(makeActualContext);
    _test->make_actual(_test.to_const(), makeActualContext);
    if (makeActualContext.requires_formal_location_or_resolution())
        _body->make_actual(_body.to_const(), makeActualContext);
}

void FogWhileStatement::merge_from(FogMergeContext& mergeContext, const This& thatExpr)
{
    Super::merge_from(mergeContext, thatExpr);
    thatExpr._test->merge_into(mergeContext, _test.to_const());
    thatExpr._body->merge_into(mergeContext, _body.to_const());
}

const FogMerge& FogWhileStatement::needs_merge_from(FogMergeContext& mergeContext, const This& thatExpr) const
{
    FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, thatExpr));
    needsMerge |= _test->needs_merge(mergeContext, *thatExpr._test);
    needsMerge |= _body->needs_merge(mergeContext, *thatExpr._body);
    return needsMerge;
}

std::ostream& FogWhileStatement::print_depth(std::ostream& s, int aDepth) const
{
    Super::print_depth(s, aDepth);
    _test->print_deep(s, aDepth);
    _body->print_deep(s, aDepth);
    return s;
}

std::ostream& FogWhileStatement::print_members(std::ostream& s, int aDepth) const
{
    Super::print_members(s, aDepth);
    _test->print_on(s, aDepth);
    _body->print_on(s, aDepth);
    return s;
}

char FogWhileStatement::print_named(std::ostream& s, const PrimId *fullId, char tailChar) const
{
    if (is_meta())
        tailChar = FogStream::space_and_emit(s, tailChar, "auto ");
    tailChar = FogStream::space_and_emit(s, tailChar, "while (");
    tailChar = _test->print_named(s, 0, tailChar);
    tailChar = FogStream::space_and_emit(s, tailChar, ")");
    return tailChar;
}

void FogWhileStatement::set_is_meta()
{
    Super::set_is_meta();
    _body->set_exposed();
}