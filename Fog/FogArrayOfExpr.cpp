#include <Fog/FogIncludeAll.h>














TYPEINFO_SINGLE(FogArrayOfExpr, Super)
FOGTOKEN_LEAF_IMPL(FogArrayOfExpr)

FogArrayOfExpr::FogArrayOfExpr()
:
    _next_index(0)
{}

FogArrayOfExpr::FogArrayOfExpr(const This& anExpr)
:
    Super(anExpr),
    _potential_exprs(anExpr._potential_exprs),
    _floating_exprs(anExpr._floating_exprs),
    _fixed_exprs(anExpr._fixed_exprs),
    _next_index(anExpr._next_index)
{}

FogArrayOfExpr::FogArrayOfExpr(const FogExprListOfRefToConst& exprList)
:
    _potential_exprs(exprList),
    _next_index(0)
{}

FogArrayOfExpr::~FogArrayOfExpr() {}

void FogArrayOfExpr::add(size_t anIndex, const FogExpr& anExpr)
{
    if (_fixed_exprs.tally() <= anIndex)
        _fixed_exprs.set_tally(anIndex + 1);
    if (_fixed_exprs[anIndex])
        WRNMSG("Redefining index " << anIndex);
    _fixed_exprs[anIndex] = anExpr;
    _next_index = anIndex + 1;
}

void FogArrayOfExpr::create_usage(FogUsageContext& aContext) const
{
    Super::create_usage(aContext);
    for (FogExprConstListOfRefToConstIterator p1(_fixed_exprs); p1; ++p1)
        p1->create_usage(aContext);
    for (FogExprConstListOfRefToConstIterator p2(_floating_exprs); p2; ++p2)
        p2->create_usage(aContext);
}

bool FogArrayOfExpr::emit(FogEmitContext& emitContext) const
{
    const char *oldSeparator = emitContext.separator();
    if (!oldSeparator)
        ERRMSG("INVESTIGATE -- no separator defined for FogArrayOfExpr::emit " << viz(*this));
    if (!_fixed_exprs.tally() && !_floating_exprs.tally())
        emitContext.emit_space_and_text(" {}");
    else
    {
        emitContext.start();
        emitContext.emit_space_and_text("{");
        FogSeparatedEmitContext separatedContext(emitContext, oldSeparator ? oldSeparator : ",", "\n");
        {
            FogIndentEmitContext indentedContext(separatedContext, 1, FogEmitContext::INDENTS);
            FogExprConstListOfRefToConstIterator p1(_fixed_exprs);
            FogExprConstListOfRefToConstIterator p2(_floating_exprs);
            while (p1 || p2)
            {
                indentedContext.start();
                if (!p1 || p1->is_null())
                {
                    if (!p2 || p2->is_null())
                        indentedContext.emit_space_and_text("0");
                    else
                        p2->emit(indentedContext);
                }
                else
                {
                    if (p2 && !p2->is_null())
                        ERRMSG("Floating and fixed initialiser for " << viz(*this));
                    p1->emit(indentedContext);
                }
                if ((p1.to_go() > 1) || (p2.to_go() > 1))
                    indentedContext.separate();
                if (p1)
                    ++p1;
                if (p2)
                    ++p2;
            }
        }
        separatedContext.terminate();
        emitContext.emit_space_and_text("}");
    }
    return true;
}

bool FogArrayOfExpr::get_object(FogTokenRef& returnValue, FogScopeContext& inScope) const
{
    if ((_fixed_exprs.tally() == 0) && (_floating_exprs.tally() == 1))
        return _floating_exprs[0]->get_object(returnValue, inScope);
    else if ((_fixed_exprs.tally() == 1) && (_floating_exprs.tally() == 0))
        return _fixed_exprs[0]->get_object(returnValue, inScope);
    else
        return Super::get_object(returnValue, inScope);
}

void FogArrayOfExpr::install(FogInstallContext& installContext) const
{
    Super::install(installContext);
    for (FogExprConstListOfRefToConstIterator p1(_fixed_exprs); p1; ++p1)
        p1->install(installContext);
    for (FogExprConstListOfRefToConstIterator p2(_floating_exprs); p2; ++p2)
        p2->install(installContext);
}

bool FogArrayOfExpr::is_actual(const FogScopeContext& scopeContext) const
{
    if (_potential_exprs.tally() && (!_floating_exprs.tally() && !_fixed_exprs.tally()))
        return false;
    return Super::is_actual(scopeContext);
}

void FogArrayOfExpr::make_actual_from(FogMakeActualContext& makeActualContext)
{
    Super::make_actual_from(makeActualContext);
//      if (makeActualContext.requires_at_resolution())
    if (makeActualContext.is_resolve_ats())
    {
        FogExprConstListOfRefToConstIterator p(_potential_exprs);
        for ( ; p && !p->is_index_cast_expr(); ++p)
        {
            FogExprRefToConst theExpr;
            p->make_actual(theExpr, makeActualContext);
            _floating_exprs.add(*theExpr);
        }
        for ( ; p; ++p)
        {
            const FogIndexCastExpr *indexExpr = p->is_index_cast_expr();
            if (indexExpr)
            {
                const PrimNumber& theIndex = indexExpr->index().get_number(makeActualContext);
                if (!theIndex.is_nat())
                    ERRMSG("Unsigned number required rather than array index " << theIndex);
                else
                {
                    FogExprRefToConst theExpr;
                    indexExpr->expr().make_actual(theExpr, makeActualContext);
                    add(theIndex.as_nat(), *theExpr);
                }
            }
            else
            {
                FogExprRefToConst theExpr;
                p->make_actual(theExpr, makeActualContext);
                add(_next_index, *theExpr);
            }
        }
    }
    else
    {
        for (FogExprListOfRefToConstIterator p(_potential_exprs); p; ++p)
            p->make_actual(p.ref(), makeActualContext);
    }
}

void FogArrayOfExpr::merge_from(FogMergeContext& mergeContext, const This& anExpr)
{
    Super::merge_from(mergeContext, anExpr);
//    merge_long_into(mergeContext, _potential_exprs, anExpr._potential_exprs);
    merge_long_into(mergeContext, _fixed_exprs, _floating_exprs);
    if (_floating_exprs.tally() > _next_index)
        _next_index = _floating_exprs.tally();
    _floating_exprs.vacate();
    merge_long_into(mergeContext, _fixed_exprs, anExpr._fixed_exprs);
    for (FogExprConstListOfRefToConstIterator p(anExpr._floating_exprs); p; ++p)
        add(_next_index, *p);
}

//  
//    Occurs after make_actual!
//  
const FogMerge& FogArrayOfExpr::needs_merge_from(FogMergeContext& mergeContext, const This& anExpr) const
{
    FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, anExpr));
#if 0
    if (_potential_exprs.tally() != anExpr._potential_exprs.tally())
    {
        if (!_potential_exprs.tally())
            needsMerge |= FogMerge::left_invalid();
        if (!anExpr._potential_exprs.tally())
            needsMerge |= FogMerge::right_invalid();
    }
    else
    {
        FogExprConstListOfRefToConstIterator p1(_potential_exprs);
        FogExprConstListOfRefToConstIterator p2(anExpr._potential_exprs);
        for ( ; p1 && p2; ++p1, ++p2)
        {
            if (p1->is_null())
            {
                if (!p2->is_null())
                    needsMerge |= FogMerge::left_invalid();
            }
            else
            {
                if (!p2->is_null())
                    needsMerge |= p1->needs_merge(mergeContext, *p2);
                else
                    needsMerge |= FogMerge::right_invalid();
            }
        }
    }
#endif
    if (_floating_exprs.tally())
        needsMerge |= FogMerge::right_invalid();
    if (anExpr._floating_exprs.tally())
        needsMerge |= FogMerge::left_invalid();
    FogExprConstListOfRefToConstIterator p1(_fixed_exprs);
    FogExprConstListOfRefToConstIterator p2(anExpr._fixed_exprs);
    for ( ; p1 && p2; ++p1, ++p2)
    {
        if (p1->is_null())
        {
            if (!p2->is_null())
                needsMerge |= FogMerge::left_invalid();
        }
        else
        {
            if (!p2->is_null())
                needsMerge |= p1->needs_merge(mergeContext, *p2);
            else
                needsMerge |= FogMerge::right_invalid();
        }
    }
    if (p1)
        needsMerge |= FogMerge::right_invalid();
    else if (p2)
        needsMerge |= FogMerge::left_invalid();
    return needsMerge;
}

std::ostream& FogArrayOfExpr::print_depth(std::ostream& s, int aDepth) const
{
    Super::print_depth(s, aDepth);
    if (_potential_exprs.tally())
    {
        s << indent(aDepth) << "potential elements\n";
        _potential_exprs.print_depth(s, aDepth+1);
    }
    if (_floating_exprs.tally())
    {
        s << indent(aDepth) << "floating elements\n";
        _floating_exprs.print_members(s, aDepth+1);
    }
    if (_fixed_exprs.tally())
    {
        s << indent(aDepth) << "fixed elements\n";
        _fixed_exprs.print_members(s, aDepth+1);
    }
    return s;
}

std::ostream& FogArrayOfExpr::print_members(std::ostream& s, int aDepth) const
{
    Super::print_members(s, aDepth);
    if (_potential_exprs.tally())
    {
        s << indent(aDepth) << "potential elements\n";
        _potential_exprs.print_members(s, aDepth+1);
    }
    if (_floating_exprs.tally())
    {
        s << indent(aDepth) << "floating elements\n";
        _floating_exprs.print_members(s, aDepth+1);
    }
    if (_fixed_exprs.tally())
    {
        s << indent(aDepth) << "fixed elements\n";
        _fixed_exprs.print_members(s, aDepth+1);
    }
    return s;
}

char FogArrayOfExpr::print_named(std::ostream& s, const PrimId *fullId, char tailChar) const
{
    tailChar = FogStream::space_and_emit(s, tailChar, " {");
    for (FogExprConstListOfRefToConstIterator p(_potential_exprs); p; ++p)
    {
        tailChar = FogStream::space_and_emit(s, tailChar, " ");
        tailChar = p->print_named(s, 0, tailChar);
        tailChar = FogStream::space_and_emit(s, tailChar, p.to_go() > 1 ? "," : " ");
    }
    tailChar = FogStream::space_and_emit(s, tailChar, "}");
    return tailChar;
}

std::ostream& FogArrayOfExpr::print_viz(std::ostream& s) const
{
    s << "\"initialiser-list: ";
    print_named(s, 0, ' ');
    return s << '\"';
}

bool FogArrayOfExpr::resolve_semantics(FogSemanticsContext& theSemantics) const
{
    bool isInitializerList = true;
    for (FogExprConstListOfRefToConstIterator p(_potential_exprs); p; ++p)
    {
        FogChildSemanticsContext fixedSemantics(theSemantics);
        if (!p->resolve_semantics(fixedSemantics))
            return false;
        if (!fixedSemantics.is_initializer_clause())
            isInitializerList = false;
    }
    theSemantics.reset();
    if (isInitializerList)
        theSemantics.set(FogSemantics::IS_INITIALIZER_CLAUSE);
    return true;
}
