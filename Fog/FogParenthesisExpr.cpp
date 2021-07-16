#include <Fog/FogIncludeAll.h>











TYPEINFO_SINGLE(FogParenthesisExpr, Super)
PRIMREF_NULL_CLAS(FogParenthesisExpr)
    FOGTOKEN_NULL_LEAF
};
PRIMREF_DERIVED_NULL_IMPL(FogParenthesisExpr)
PRIMREFS_IMPL(FogParenthesisExpr)
FOGTOKEN_LEAF_IMPL(FogParenthesisExpr)
TMPL_HACK_FIX_DO(FogParenthesisExpr)

FogParenthesisExpr::FogParenthesisExpr()
:
    Super(FogListOfExpr::mutable_null())
{}

FogParenthesisExpr::FogParenthesisExpr(FogListOfExpr& exprList, const FogCv& aCv, void *exceptionSpecification)
:
    Super(exprList),
    _cv(aCv)
//    _exceptions(anExpr._exceptions)
{}

FogParenthesisExpr::FogParenthesisExpr(const This& anExpr)
:
    Super(anExpr),
    _cv(anExpr._cv),
    _exceptions(anExpr._exceptions)
{}

void FogParenthesisExpr::create_usage(FogUsageContext& aContext) const
{
    Super::create_usage(aContext);
    for (FogExprConstListOfRefToConstIterator p(_exceptions); p; ++p)
        p->create_usage(aContext);
}

bool FogParenthesisExpr::emit(FogEmitContext& emitContext) const
{
    emitContext.emit_space_and_text("(");
    {
        FogSeparatedEmitContext separatedContext(emitContext, ", ", 0);
        expr().emit(separatedContext);
    }
    emitContext.emit_space_and_text(")");
    emitContext.emit_cv(_cv->decl_specifier_value());
    for (FogExprConstListOfRefToConstIterator p(_exceptions); p; ++p)
        p->emit(emitContext);
    return true;
}

size_t FogParenthesisExpr::executable_tokens() const
{
    return expr().executable_tokens();
}

bool FogParenthesisExpr::get_object(FogTokenRef& tokenValue, FogScopeContext& inScope) const
{
    return expr().get_object(tokenValue, inScope);
}

void FogParenthesisExpr::install(FogInstallContext& installContext) const
{
    Super::install(installContext);
//    for (FogExprConstListOfRefToConstIterator p(_exceptions); p; ++p)
//        p->install(installContext);
}

bool FogParenthesisExpr::is_actual(const FogScopeContext& scopeContext) const
{
    if (!Super::is_actual(scopeContext))
        return false;
    for (FogExprConstListOfRefToConstIterator p(_exceptions); p; ++p)
        if (!p->is_actual(scopeContext))
            return false;
    return true;
}

FogParenthesisExpr *FogParenthesisExpr::is_parenthesis_expr() { return this; }

void FogParenthesisExpr::make_actual_from(FogMakeActualContext& makeActualContext)
{
    Super::make_actual_from(makeActualContext);
    for (FogExprListOfRefToConstIterator p(_exceptions); p; ++p)
        p->make_actual(p.ref(), makeActualContext);
}

bool FogParenthesisExpr::make_specifier(FogMakeSpecifierContext& makeSpecifierContext)
{
    FogListOfExpr& exprList = exprs();
    if (exprList.tally() == 1)                                  //   (Semi-)gratuitous parenthesis.
        return exprList.exprs()[0]->make_specifier(makeSpecifierContext);
    return false;                                               //   Occurs during presumed type attempt in CallExpr
//    ERRMSG("INVESTIGATE -- FogParenthesisExpr::make_specifier for non unit-list for " << viz(*this));
//    return expr().make_specifier(makeSpecifierContext);
}

void FogParenthesisExpr::merge_from(FogMergeContext& mergeContext, const This& thatExpr)
{
    Super::merge_from(mergeContext, thatExpr);
    _cv = _cv->merge_cv(*thatExpr._cv);
    FogExpr::merge_long_into(mergeContext, _exceptions, thatExpr._exceptions);
}

const FogMerge& FogParenthesisExpr::needs_merge_from(FogMergeContext& mergeContext, const This& anExpr) const
{
    FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, anExpr));
    needsMerge |= _cv->needs_merge_cv(*anExpr._cv);
#if 0
    FogExprConstListOfRefToConstIterator p1(_exceptions);
    FogExprConstListOfRefToConstIterator p2(anExpr._exceptions);
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
#endif
    return needsMerge;
}

FogModifier *FogParenthesisExpr::new_function_modifier(FogMakeSpecifierContext& makeSpecifierContext)
{
    FogFunctionModifier *theDecl = new FogFunctionModifier(*_cv);
    if (!theDecl)
        return 0;
    if (!exprs().make_parameters(*theDecl, makeSpecifierContext))
        return 0;
    for (FogExprConstListOfRefToConstIterator p2(_exceptions); p2; ++p2)
        theDecl->add_exception_specification(*p2);
    return theDecl;
}

std::ostream& FogParenthesisExpr::print_depth(std::ostream& s, int aDepth) const
{
    Super::print_depth(s, aDepth);
    _exceptions.print_depth(s, aDepth+1);
    return s;
}

std::ostream& FogParenthesisExpr::print_members(std::ostream& s, int aDepth) const
{
    Super::print_members(s, aDepth);
    _exceptions.print_members(s, aDepth+1);
    return s;
}

char FogParenthesisExpr::print_named(std::ostream& s, const PrimId *fullId, char tailChar) const
{
    tailChar = FogStream::space_and_emit(s, tailChar, "(");
    tailChar = Super::print_named(s, fullId, tailChar);
    tailChar = FogStream::space_and_emit(s, tailChar, ")");
    if (_cv->is_cv())
        tailChar = FogStream::space_and_emit(s, tailChar, _cv->str());
    for (FogExprConstListOfRefToConstIterator p(_exceptions); p; ++p)
    {
        if (p->is_null())
            tailChar = FogStream::space_and_emit(s, tailChar, "0");
        else
            tailChar = p->print_named(s, 0, tailChar);
        if (p.to_go() > 1)
            tailChar = FogStream::space_and_emit(s, tailChar, ", ");
    }
    return tailChar;
}

std::ostream& FogParenthesisExpr::print_viz(std::ostream& s) const
{
    s << "\"parenthesis-expression: ";
    print_named(s, 0, ' ');
    return s << '\"';
}

bool FogParenthesisExpr::resolve_semantics(FogSemanticsContext& theSemantics) const
{
    const FogListOfExpr& exprList = exprs();
    bool isParenthesis = !_cv->is_cv() && !_exceptions.tally();
    bool isExpression = isParenthesis && (exprList.tally() >= 1);
    bool isMetaParameterDeclarationClause = true;
    bool isParameterDeclarationClause = true;
    bool isFirst = true;
    theSemantics.reset();
    for (FogExprConstListOfRefToConstIterator p(exprList.exprs()); p; ++p)
    {
        bool isLast = p.to_go() <= 1;
        FogChildSemanticsContext termSemantics(theSemantics);
        if (!p->resolve_semantics(termSemantics))
            return false;
        if (!termSemantics.is_assignment_expression())
            isExpression = false;
        if (!termSemantics.is_meta_parameter_declaration())
            isMetaParameterDeclarationClause = false;
        if (!termSemantics.is_parameter_declaration() && !(isLast && termSemantics.is_ellipsis()))
            isParameterDeclarationClause = false;
        if (isFirst && isLast)
        {
            if (termSemantics.is_bit_field_declarator_id_opt())
                theSemantics.set(FogSemantics::IS_BIT_FIELD_DECLARATOR_ID_OPT);
            if (termSemantics.is_function_declarator_id())
                theSemantics.set(FogSemantics::IS_FUNCTION_DECLARATOR_ID);
            if (termSemantics.is_function_declarator())
                theSemantics.set(FogSemantics::IS_FUNCTION_DECLARATOR);
//            if (termSemantics.is_meta_elem_declarator())
//                theSemantics.set(FogSemantics::IS_META_ELEM_DECLARATOR);
            if (termSemantics.is_object_declarator_id())
                theSemantics.set(FogSemantics::IS_OBJECT_DECLARATOR_ID);
            if (termSemantics.is_parameter_declarator_opt())
                theSemantics.set(FogSemantics::IS_PARAMETER_DECLARATOR_OPT);
            if (termSemantics.is_type_id_declarator_opt())
                theSemantics.set(FogSemantics::IS_TYPE_ID_DECLARATOR_OPT);
            if (isParenthesis && termSemantics.is_type_id())
                theSemantics.set(FogSemantics::IS_TYPE_CAST);
        }
        isFirst = false;
    }
    if (!exprList.tally())
        theSemantics.set(FogSemantics::IS_EPSILON);
    if (isExpression)
        theSemantics.set(FogSemantics::IS_PRIMARY_EXPRESSION);
    if (isMetaParameterDeclarationClause)
        theSemantics.set(FogSemantics::IS_META_PARAMETER_DECLARATION_CLAUSE);
    if (isParameterDeclarationClause)
        theSemantics.set(FogSemantics::IS_PARAMETER_DECLARATION_CLAUSE);
    return true;
}

void FogParenthesisExpr::set_decl_specifier_expression(FogExprRef& anExpr, FogDeclSpecifier& declSpecifiers)
{
    anExpr.adopt(new FogCallExpr(declSpecifiers, *this));
}
