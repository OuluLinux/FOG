#include <Fog/FogIncludeAll.h>









TYPEINFO_SINGLE(FogForStatement, Super)
FOGTOKEN_LEAF_IMPL(FogForStatement)

FogForStatement::FogForStatement(FogExpr *initExpr, const FogExpr *testExpr, FogExpr *stepExpr, FogRaw& bodyStatement)
:
    _init(initExpr),
    _test(testExpr),
    _step(stepExpr),
    _body(bodyStatement)
{}

FogForStatement::FogForStatement(const This& aStatement)
:
    Super(aStatement),
    _init(aStatement._init),
    _test(aStatement._test),
    _step(aStatement._step),
    _body(aStatement._body)
{}

FogForStatement::~FogForStatement() {}

bool FogForStatement::compile_parsed(FogParseContext& parseContext)
{
    FogMetaParseContext metaContext(parseContext);
    FogUnbreakableParseContext initContext(metaContext, *_init);
    if (!_init->compile_parsed(initContext))
        return false;
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
            if (!bodyContext.do_continue())
                return false;
        }
        FogUnbreakableParseContext stepContext(metaContext, *_step);
        if (!_step->compile_declaration(stepContext))
            return false;
    }
}

void FogForStatement::create_usage(FogUsageContext& aContext) const
{
    Super::create_usage(aContext);
    _init->create_usage(aContext);
    _test->create_usage(aContext);
    _test->create_usage(aContext);
    _body->create_usage(aContext);
}

bool FogForStatement::emit(FogEmitContext& emitContext) const
{
    if (is_meta())
    {
        FogEmitParseContext parseContext(emitContext, *this);
        FogUnbreakableParseContext initContext(parseContext, *_init);
        if (!_init->compile_parsed(initContext))
            return false;
        while (true)
        {
            const PrimNumber& testValue = _test->get_number(parseContext);
            if (testValue.is_null() || testValue.is_zero())
                return true;
            FogBreakableEmitContext bodyContext(emitContext, *_body);
            if (!_body->emit(bodyContext))
            {
                if (bodyContext.do_break())
                    break;
                if (!bodyContext.do_continue())
                    return false;
            }
            FogUnbreakableParseContext stepContext(parseContext, *_step);
            if (!_step->compile_parsed(stepContext))
                return false;
        }
    }
    else
    {
        emitContext.emit_hash_line(line());
        {
            FogSeparatedEmitContext separatedContext(emitContext, ",", 0);
            separatedContext.emit_space_and_text("for (");
            if (_init)
                _init->emit(separatedContext);
            separatedContext.emit_space_and_text(";");
            if (_test)
                _test->emit(separatedContext);
            separatedContext.emit_space_and_text(";");
            if (_step)
                _step->emit(separatedContext);
            separatedContext.emit_space_and_text(")");
        }
        {
            FogSeparatedEmitContext separatedContext(emitContext, "", "\n");
            FogIndentEmitContext indentedContext(separatedContext, 1, FogEmitContext::INDENTS);
            _body->emit(indentedContext);
        }
    }
    return true;
}

size_t FogForStatement::executable_tokens() const
{
    size_t executableTokens = 1 + _init->executable_tokens() +
            10 * (_test->executable_tokens() + _step->executable_tokens() + _body->executable_tokens());
    return executableTokens;
}

void FogForStatement::install(FogInstallContext& installContext) const
{
    Super::install(installContext);
    _init->install(installContext);
    _test->install(installContext);
    _step->install(installContext);
    _body->install(installContext);
}

bool FogForStatement::is_actual(const FogScopeContext& scopeContext) const
{
    return Super::is_actual(scopeContext) && _init->is_actual(scopeContext) && _test->is_actual(scopeContext)
        && _step->is_actual(scopeContext) && _body->is_actual(scopeContext);
}

void FogForStatement::make_actual_from(FogMakeActualContext& makeActualContext)
{
    Super::make_actual_from(makeActualContext);
    _init->make_actual(_init.to_const(), makeActualContext);
    if (makeActualContext.requires_formal_location_or_resolution())
    {
        _test->make_actual(_test.to_const(), makeActualContext);
        _step->make_actual(_step.to_const(), makeActualContext);
        _body->make_actual(_body.to_const(), makeActualContext);
    }
}

void FogForStatement::merge_from(FogMergeContext& mergeContext, const This& thatExpr)
{
    Super::merge_from(mergeContext, thatExpr);
    thatExpr._init->merge_into(mergeContext, _init.to_const());
    thatExpr._test->merge_into(mergeContext, _test.to_const());
    thatExpr._step->merge_into(mergeContext, _step.to_const());
    thatExpr._body->merge_into(mergeContext, _body.to_const());
}

const FogMerge& FogForStatement::needs_merge_from(FogMergeContext& mergeContext, const This& thatExpr) const
{
    FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, thatExpr));
    needsMerge |= _init->needs_merge(mergeContext, *thatExpr._init);
    needsMerge |= _test->needs_merge(mergeContext, *thatExpr._test);
    needsMerge |= _step->needs_merge(mergeContext, *thatExpr._step);
    needsMerge |= _body->needs_merge(mergeContext, *thatExpr._body);
    return needsMerge;
}

std::ostream& FogForStatement::print_depth(std::ostream& s, int aDepth) const
{
    Super::print_depth(s, aDepth);
    _init->print_deep(s, aDepth);
    _test->print_deep(s, aDepth);
    _step->print_deep(s, aDepth);
    _body->print_deep(s, aDepth);
    return s;
}

std::ostream& FogForStatement::print_members(std::ostream& s, int aDepth) const
{
    Super::print_members(s, aDepth);
    _init->print_on(s, aDepth);
    _test->print_on(s, aDepth);
    _step->print_on(s, aDepth);
    _body->print_on(s, aDepth);
    return s;
}

char FogForStatement::print_named(std::ostream& s, const PrimId *fullId, char tailChar) const
{
    if (is_meta())
        tailChar = FogStream::space_and_emit(s, tailChar, "auto ");
    tailChar = FogStream::space_and_emit(s, tailChar, "for (");
    tailChar = _init->print_named(s, 0, tailChar);
    tailChar = FogStream::space_and_emit(s, tailChar, ";");
    tailChar = _test->print_named(s, 0, tailChar);
    tailChar = FogStream::space_and_emit(s, tailChar, ";");
    tailChar = _step->print_named(s, 0, tailChar);
    tailChar = FogStream::space_and_emit(s, tailChar, ") ...");
    return tailChar;
}

void FogForStatement::set_is_meta()
{
    Super::set_is_meta();
    _body->set_exposed();
}