#include <Fog/FogIncludeAll.h>













TYPEINFO_SINGLE(FogTreeLiteral, Super)
FOGTOKEN_LEAF_IMPL(FogTreeLiteral)

FogTreeLiteral::FogTreeLiteral(FogExpr& anExpr, size_t dollarCount) 
:
    _expr(anExpr),
    _dollar_count(dollarCount)
{}

FogTreeLiteral::FogTreeLiteral(const This& thatId)
:
    Super(thatId),
    _expr(thatId._expr),
    _dollar_count(thatId._dollar_count),
    _actual(thatId._actual)
{
    if (_actual)
        ERRMSG("BUG - did not expect to copy " << viz(*this));
}

FogTreeLiteral::~FogTreeLiteral() {}

bool FogTreeLiteral::compile_parsed(FogParseContext& parseContext)
{
    if (_actual)
    {
        FogTokenRef actualValue;
        FogMultiDollarScopeContext resolutionScope(parseContext);
        _actual->make_actual(actualValue, resolutionScope);
        FogTokenParseContext nestedContext(parseContext, *actualValue);
        return actualValue->compile_declaration(nestedContext);
    }
    ERRMSG("BUG -- did not expect to FogTreeLiteral::compile_parsed with non-actual " << viz(*this));
    FogTokenRef tokenValue;
    if (!get_object(tokenValue, parseContext))
        return false;
    FogTokenRef actualValue;
    FogMultiDollarScopeContext resolutionScope(parseContext);
    tokenValue->make_actual(actualValue, resolutionScope);
    FogTokenParseContext nestedContext(parseContext, *actualValue);
    return actualValue->compile_declaration(nestedContext);
}

void FogTreeLiteral::create_usage(FogUsageContext& aContext) const
{
    if (!_actual)
        ERRMSG("BUG - did not expect to FogTreeLiteral::create_usage for non-actual " << viz(*this));
    _actual->create_usage(aContext);
}

bool FogTreeLiteral::emit(FogEmitContext& emitContext) const
{
    if (_actual)
        return _actual->emit(emitContext);
    if (emitContext.emit_raw())
    {
        if (!_dollar_count)
            emitContext.emit_space_and_text("@");
        else
            for (size_t i = _dollar_count; i-- > 0; )
                emitContext.emit_space_and_text("$");
        emitContext.emit_space_and_text("{");
        _expr->emit(emitContext);
        emitContext.emit_space_and_text("}");
        return true;
    }
    FogTokenRef returnValue;
    FogMetaScopeContext metaContext(emitContext);
    if (!_expr->get_object(returnValue, metaContext))
        return false;
    return returnValue->emit(emitContext);
}

size_t FogTreeLiteral::executable_tokens() const { return 1000; }        //  .bugbug arbitrary large number 

void FogTreeLiteral::find_entities_in(FogScopeContext& inScope, FogEntityFinding& theFinding) const
{
    if (_actual)
        _actual->find_entities_in(inScope, theFinding);
    else
        Super::find_entities_in(inScope, theFinding);
}

const FogMetaArgument *FogTreeLiteral::find_formal_in(FogScopeContext& inScope) const
{
    if (_actual)                //   Never happens
        ERRMSG("BUG - did not expect actual in FogTreeLiteral::find_formal_in for " << viz(*this));
    FogScopeContext *foundContext = inScope.find_context(_dollar_count > 0 ? _dollar_count-1 : 0);
    if (!foundContext)
        return 0;
    return _expr->find_formal_in(*foundContext);
}

bool FogTreeLiteral::find_meta_entity_in(FogTokenRef& returnValue, FogScopeContext& inScope) const
{
    if (_actual)                //   Never happens
        ERRMSG("BUG - did not expect actual in FogTreeLiteral::find_meta_entity_in for " << viz(*this));
    return get_object(returnValue, inScope);
}

bool FogTreeLiteral::get_object(FogTokenRef& returnValue, FogScopeContext& inScope) const
{
    if (_actual)
    {
        returnValue = _actual;
        return true;
    }
    else
    {
        FogScopeContext *foundContext = inScope.find_context(_dollar_count > 0 ? _dollar_count-1 : 0);
        if (!foundContext)
            return false;
        FogMetaScopeContext metaScopeContext(*foundContext);
        if (inScope.is_resolve_ats())
        {
            if (!metaScopeContext.is_resolve_ats())
                ERRMSG("INVESTIGATE -- expected is_resolve_ats for " << viz(metaScopeContext));
//              return !_dollar_count && !inScope.is_unresolvable() ? _expr->get_object(returnValue, metaScopeContext) : false;
            return _expr->get_object(returnValue, metaScopeContext);
        }
        else if (inScope.is_resolve_dollars())
        {
//              if (!_dollar_count || inScope.is_unresolvable())
            if (!_dollar_count)
                return false;
            if (metaScopeContext.is_resolve_dollars())
                return _expr->get_object(returnValue, metaScopeContext);
            FogResolutionScopeContext resolutiomContext(metaScopeContext, FogScopeContext::RESOLVE_DOLLARS);
            return _expr->get_object(returnValue, resolutiomContext);
        }
        else if (inScope.is_locate_formals())
        {
            if (!metaScopeContext.is_locate_formals())
                ERRMSG("INVESTIGATE -- expected is_locate_formals for " << viz(metaScopeContext));
            return _expr->get_object(returnValue, metaScopeContext);
        }
        else if (inScope.is_resolve_multi_dollars())
        {
            if (_dollar_count <= 1)
                return false;
            if (metaScopeContext.is_resolve_multi_dollars())
                return _expr->get_object(returnValue, metaScopeContext);
            FogResolutionScopeContext resolutionContext(metaScopeContext, FogScopeContext::RESOLVE_MULTI_DOLLARS);
//              FogMultiDollarScopeContext resolutionContext(metaScopeContext); -- doesn't work - may be SCOPE_KNOWN
            return _expr->get_object(returnValue, resolutionContext);
        }
        else if (inScope.is_resolve_type_strongly())        //   Occurs during semantic repair involing a $
        {
            if (!metaScopeContext.is_resolve_type_strongly())
                ERRMSG("INVESTIGATE -- expected is_resolve_type_strongly for " << viz(metaScopeContext));
            return _expr->get_object(returnValue, metaScopeContext);
        }
        else
        {
            ERRMSG("INVESTIGATE -- unclassified FogTreeLiteral::get_object for " << viz(*this));
            return true;
        }
    }
}

FogName *FogTreeLiteral::get_type()
{
    ERRMSG("INVESTIGATE - did not expect to FogTreeLiteral::get_type for " << viz(*this));
    if (_actual)
        return _actual->get_type();
    else
        return _expr->get_type();                   //  .bugbug indirection ?
}

bool FogTreeLiteral::has_at() const { return !_actual && (_dollar_count == 0); }
bool FogTreeLiteral::has_dollar() const { return !_actual && (_dollar_count > 0); }

void FogTreeLiteral::install(FogInstallContext& installContext) const
{
    Super::install(installContext);
    if (!_actual)
        ERRMSG("BUG - did not expect to FogTreeLiteral::install for non-actual " << viz(*this));
    _actual->install(installContext);
}

bool FogTreeLiteral::is_actual(const FogScopeContext& scopeContext) const
{
    return !_actual.is_null(); // hack fix, might be "!actual.is_null()"
}

bool FogTreeLiteral::is_compound() const
{
    if (_actual)
        return _actual->is_compound();
    else
        return Super::is_compound();
}

const PrimId *FogTreeLiteral::is_resolved() const
{
    const FogName *aName = _actual->is_name();
    return aName ? aName->is_resolved() : 0;
}

const FogTemplateParameterSpecifier *FogTreeLiteral::is_unnormalised_template_parameter
    (FogScopeContext& scopeContext) const { return 0; }

void FogTreeLiteral::make_actual_from(FogMakeActualContext& makeActualContext)
{
    if (_actual)
        ERRMSG("BUG - did not expect to FogTreeLiteral::make_actual_from for actualised " << viz(*this));
    else
    {
        _expr->make_actual(_expr.to_const(), makeActualContext);
//          if ((makeActualContext.is_resolve_ats() && !makeActualContext.is_unresolvable())
//           || (_dollar_count && makeActualContext.is_resolve_dollars() && !makeActualContext.is_unresolvable())
//           || ((_dollar_count > 1) && makeActualContext.is_resolve_multi_dollars()))
        if ((makeActualContext.is_resolve_ats())
         || (_dollar_count && makeActualContext.is_resolve_dollars())
         || ((_dollar_count > 1) && makeActualContext.is_resolve_multi_dollars()))
        {
            FogInScopeContext inScope(makeActualContext, IN_ANY_SCOPE);
            FogTokenRef slotValue;
            if (get_meta_entity_in(slotValue, inScope))
            {
                slotValue->make_actual(_actual.to_const(), makeActualContext);
                if (_actual->is_meta_variable_entity())
                    _actual->get_object(_actual, makeActualContext);
                else if (_actual->is_meta_function_entity())
                {
                    ERRMSG("Meta-variable rather than meta-function expected for " << viz(*_actual));
                    _actual = FogFailure::make();
                }
                if (_actual->is_meta_entity())
                    ERRMSG("INVESTIGATE meta-entity as actual for FogTreeLiteral::make_actual_from " << viz(*this));
            }
            else if (makeActualContext.requires_resolution())
                _actual = FogFailure::make();                                       //   Suppress further attempts
        }
    }
}

//  
//    $xx where xx is a meta-function formal are converted to FogArgumentId.
//  
void FogTreeLiteral::make_actual_token(FogTokenRefToConst& actualToken, FogMakeActualContext& makeActualContext) const
{
    if (makeActualContext.is_locate_formals())         //   @{$$...} too,
    {
        if (_actual)
            ERRMSG("BUG - did not expect to FogTreeLiteral::make_actual_token for actualised " << viz(*this));            
        FogInScopeContext inScope(makeActualContext, IN_ANY_SCOPE);
        FogScopeContext *foundContext = inScope.find_context(_dollar_count > 0 ? _dollar_count-1 : 0);
        if (foundContext)
        {
            FogExprRef anExpr;
            _expr->make_actual(anExpr.to_const(), makeActualContext);
            const FogMetaArgument *metaArgument = anExpr->find_formal_in(*foundContext);
            if (metaArgument)
            {
                actualToken.adopt(new FogArgumentId(*anExpr, _dollar_count, *metaArgument));
                return;
            }
        }
    }
    Super::make_actual_token(actualToken, makeActualContext);
}

bool FogTreeLiteral::make_specifier(FogMakeSpecifierContext& makeSpecifierContext)
{
//      if (!makeSpecifierContext.is_resolve_ats() && has_at())
//          return Super::make_specifier(makeSpecifierContext);
//      else if (!makeSpecifierContext.is_resolve_dollars() && has_dollar())
//          return Super::make_specifier(makeSpecifierContext);
    if (!_dollar_count)
        return Super::make_specifier(makeSpecifierContext);
    FogTokenRef tokenValue;
    if (!get_object(tokenValue, makeSpecifierContext))
        return false;
    return tokenValue->make_specifier(makeSpecifierContext);
}

void FogTreeLiteral::merge_from(FogMergeContext& mergeContext, const This& thatDecl)
{
    Super::merge_from(mergeContext, thatDecl);
    ERRMSG("BUG - did not expect to FogTreeLiteral::merge_from for " << viz(*this));
    thatDecl._expr->merge_into(mergeContext, _expr.to_const());
}

bool FogTreeLiteral::morph_to(FogTokenRef& returnValue, const FogMetaType& metaType, IsExposed isExposed,
    FogScopeContext& inScope) const
{
    FogTokenRef gotValue;
    if (!get_object(gotValue, inScope))
        return false;
    else
        return gotValue->morph_to(returnValue, metaType, isExposed, inScope);
}

const FogMerge& FogTreeLiteral::needs_merge_from(FogMergeContext& mergeContext, const This& thatId) const
{
    FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, thatId));
    if (_actual || thatId._actual)
    {
        if (_actual.pointer() != thatId._actual.pointer())
            needsMerge |= FogMerge::incompatible();
    }
    else
        needsMerge |= _expr->needs_merge(mergeContext, *thatId._expr);
    return needsMerge;
}

FogTokenType::TokenType FogTreeLiteral::pp_token_type_enum() const { return FogTokenType::TreeLiteral; }

std::ostream& FogTreeLiteral::print_depth(std::ostream& s, int aDepth) const
{
    Super::print_depth(s, aDepth);
    if (_actual)
        _actual->print_on(s, aDepth);
    else
        _expr->print_on(s, aDepth);
    return s;
}

std::ostream& FogTreeLiteral::print_members(std::ostream& s, int aDepth) const
{
    Super::print_members(s, aDepth);
    if (_actual)
        _actual->print_on(s, aDepth);
    else
        _expr->print_on(s, aDepth);
    return s;
}

char FogTreeLiteral::print_named(std::ostream& s, const PrimId *scopeId, char tailChar) const
{
//      if (Fog::debug_actual() && _actual)
    if (_actual)
        tailChar = _actual->print_named(s, scopeId, tailChar);
//      else
    else if (_expr)
//      if (_expr)
    {
        if (scopeId)
        {
            tailChar = FogStream::space_and_emit(s, tailChar, scopeId->str());
            tailChar = FogStream::space_and_emit(s, tailChar, "::");        
        }
        if (!_dollar_count)
            tailChar = FogStream::space_and_emit(s, tailChar, "@");
        else
            for (size_t i = _dollar_count; i-- > 0; )
                tailChar = FogStream::space_and_emit(s, tailChar, "$");
        tailChar = FogStream::space_and_emit(s, tailChar, "{");
        tailChar = _expr->print_named(s, 0, tailChar);
        tailChar = FogStream::space_and_emit(s, tailChar, "}");
//          if (_actual)
//              tailChar = FogStream::space_and_emit(s, tailChar, " = ");
    }
//      if (_actual)
//          tailChar = _actual->print_named(s, scopeId, tailChar);
    return tailChar;
}

std::ostream& FogTreeLiteral::print_viz(std::ostream& s) const
{
    s << "\"tree-literal: ";
//      if (Fog::debug_actual() && _actual)
    if (_actual)                            //   actual first since more diagnostic, line no gives source
        _actual->print_this(s);
//      else if (_expr)                     // expr first since more intelligible to user.
    else
    {
        if (!_dollar_count)
            s << '@';
        else
            for (size_t i = _dollar_count; i-- > 0; )
                s << '$';
        _expr->print_this(s);
    }
//      else
//          _actual->print_this(s);
//    if (_actual)
//        _actual->print_this(s);
//    else
//        _expr->print_this(s);
    return s << '\"';
}

bool FogTreeLiteral::resolve_id(PrimIdHandle& returnId, FogScopeContext& inScope) const
{
//      if (!_dollar_count && !inScope.requires_at_resolution())
    if (!_dollar_count && !inScope.is_resolve_ats())
        return false;
    if (_actual)
        return _actual->get_identifier(returnId, inScope);
    const FogEmitContext *emitContext = inScope.is_emit_context();
    if (emitContext && emitContext->emit_raw())                 //   If resolving within a diagnostic                     
        return false;
    FogTokenRef slotValue;
    FogInScopeContext inScopeContext(inScope, IN_ANY_SCOPE);
    if (get_meta_entity_in(slotValue, inScopeContext))
        return slotValue->get_identifier(returnId, inScope);
    returnId = PrimId::null();
    if (!inScope.requires_resolution())
        return false;
//    ERRMSG("BUG -- should not resolve_id from " << viz(*this) << " for " << viz(inScope));
    mutate()._actual = FogFailure::make();
    return true;    //   happens for a bad $x() in a meta-literal
}

bool FogTreeLiteral::resolve_semantics(FogSemanticsContext& theSemantics) const
{
    if (!_dollar_count)
    {
        theSemantics.assign_identifier(FogSemantics::POTENTIAL);
        return true;
    }
    if (_actual)
        _actual->resolve_semantics(theSemantics);
    FogTokenRef tempValue;
    if (!get_object(tempValue, theSemantics))
        return false;
    return tempValue->resolve_semantics(theSemantics);
}

FogTokenType::TokenType FogTreeLiteral::token_type_enum() const { return FogTokenType::TreeLiteral; }
