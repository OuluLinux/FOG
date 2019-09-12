#include <Fog/FogIncludeAll.h>















TYPEINFO_SINGLE(FogMultipleId, Super)
FOGTOKEN_LEAF_IMPL(FogMultipleId)

FogMultipleId::FogMultipleId(const FogElementType& elementType)
:
    _element_type(elementType),
    _resolved(NOT_YET_RESOLVED_GLOBALLY),
    _resolved_in(0),
    _has_at(false),
    _has_dollar(false)
{}

FogMultipleId::FogMultipleId(const This& thatName)
:
    Super(thatName),
    _element_type(thatName._element_type),
    _tokens(thatName._tokens),
    _resolved_id(thatName._resolved_id),
    _resolved(thatName._resolved),
    _resolved_in(thatName._resolved_in),
    _has_at(thatName._has_at),
    _has_dollar(thatName._has_dollar)
{
    if (_resolved != CANNOT_RESOLVE_GLOBALLY)
        ERRMSG("BUG - should not copy resolvable " << viz(*this));
}

FogMultipleId::~FogMultipleId() {}

//  
//    Add aToken to the multiple identifier.
//  
void FogMultipleId::add(FogToken& aToken)
{
    _tokens.add(aToken);
    _resolved_id.reset();
    if (!aToken.is_actual(FogScopeContext::weak()))
        _resolved = CANNOT_RESOLVE_GLOBALLY;
    else if (_resolved == GLOBALLY_RESOLVED)
        _resolved = NOT_YET_RESOLVED_GLOBALLY;
    else if (_resolved == LOCALLY_RESOLVED)
    {
        ERRMSG("BUG - should not add to locally resolved " << viz(*this));
        _resolved = NOT_YET_RESOLVED_GLOBALLY;
    }
    FogName *aName = aToken.is_name();
    if (aName)
    {
        if (aName->has_at())
            _has_at = true;
        if (aName->has_dollar())
            _has_dollar = true;
    }
}

void FogMultipleId::create_usage(FogUsageContext& aContext) const
{
    if (_element_type.is_identifier())
        Super::create_usage(aContext);                  //   resolve referee of _resolved_id
}

bool FogMultipleId::emit(FogEmitContext& emitContext) const
{
//      if (is_null())
//          return false;
    PrimIdHandle resolvedId(_resolved_id);
    if (!resolvedId && !resolve_id(resolvedId, emitContext))
        return false;
    switch (_element_type.value())
    {
        case FogElementType::CHARACTER:
        case FogElementType::WIDE_CHARACTER:
            emitContext.emit_character(*resolvedId, _element_type.is_wide());
            return true;
        case FogElementType::STRING:
        case FogElementType::WIDE_STRING:
            emitContext.emit_string(*resolvedId, _element_type.is_wide());
            return true;
        case FogElementType::IDENTIFIER:
            return emitContext.emit_id(*resolvedId, *this);
        case FogElementType::NUMBER:
            return emitContext.emit_identifier(*resolvedId);
        default:
            ERRMSG("BUG -- unsupported type of FogMultipleId.");
            return false;
    }
}

bool FogMultipleId::get_identifier(PrimIdHandle& returnId, FogScopeContext& inScope)
{
    switch (_element_type.value())
    {
        case FogElementType::IDENTIFIER:
            return Super::get_identifier(returnId, inScope);
        case FogElementType::CHARACTER:
        case FogElementType::WIDE_CHARACTER:
            return get_character(returnId, inScope);
        case FogElementType::STRING:
        case FogElementType::WIDE_STRING:
            return get_string(returnId, inScope);
        case FogElementType::NUMBER:                    //  .bugbug ?
        default:
            ERRMSG("BUG -- unsupported type of FogMultipleId::get_identifier.");
            return false;
    }
}

bool FogMultipleId::get_meta_entity_in(FogTokenRef& returnValue, FogScopeContext& inScope) const
{
    switch (_element_type.value())
    {
        case FogElementType::IDENTIFIER:
            return Super::get_meta_entity_in(returnValue, inScope);
        default:
            return false;
    }
}

//  
//  --This might be a good place to cache the _resolved_id if it's independent of inScope.
//  
bool FogMultipleId::get_object(FogTokenRef& tokenValue, FogScopeContext& inScope) const
{
    switch (_element_type.value())
    {
        case FogElementType::IDENTIFIER:
            return Super::get_object(tokenValue, inScope);
        default:
        {
            tokenValue = mutate();             //  .bugbug cast ?
            return true;
        }
    }
}

bool FogMultipleId::has_at() const
{
    switch (_resolved)
    {
        case NOT_YET_RESOLVED_GLOBALLY:
        case CANNOT_RESOLVE_GLOBALLY:
            return _has_at;
        case GLOBALLY_RESOLVED:
        case LOCALLY_RESOLVED:
        default:
            return false;
    }
}

bool FogMultipleId::has_dollar() const
{
    switch (_resolved)
    {
        case NOT_YET_RESOLVED_GLOBALLY:
        case CANNOT_RESOLVE_GLOBALLY:
            return _has_dollar;
        case GLOBALLY_RESOLVED:
        case LOCALLY_RESOLVED:
        default:
            return false;
    }
}

void FogMultipleId::install(FogInstallContext& installContext) const
{
    Super::install(installContext);
}

bool FogMultipleId::is_actual(const FogScopeContext& scopeContext) const
{
    switch (_resolved)
    {
        case NOT_YET_RESOLVED_GLOBALLY:
        {
            mutate().resolve_globally();
            break;
        }
        case GLOBALLY_RESOLVED:
            break;
        case LOCALLY_RESOLVED:
            break;
        case CANNOT_RESOLVE_GLOBALLY:
        {
            for (FogTokenConstListOfRefToConstIterator p(_tokens); p; ++p)
                if (!p->is_actual(scopeContext))
                    return false;
            break;
        }
    }
    return Super::is_actual(scopeContext);
}

const PrimId *FogMultipleId::is_resolved() const
{
    switch (_resolved)
    {
        case CANNOT_RESOLVE_GLOBALLY:
            return 0;
        case NOT_YET_RESOLVED_GLOBALLY:
        {
            mutate().resolve_globally();
            break;
        }
        case GLOBALLY_RESOLVED:
            break;
        case LOCALLY_RESOLVED:
            break;
    }
    return _resolved_id.pointer();
}

const FogTemplateParameterSpecifier *FogMultipleId::is_unnormalised_template_parameter
    (FogScopeContext& scopeContext) const { return 0; }

bool FogMultipleId::is_wide() const { return _element_type.is_wide(); }

void FogMultipleId::make_actual_from(FogMakeActualContext& makeActualContext)
{
    Super::make_actual_from(makeActualContext);
    for (FogTokenListOfRefToConstIterator p(_tokens); p; ++p)
        p->make_actual(p.ref(), makeActualContext);
//      if (makeActualContext.requires_at_resolution())
    if (makeActualContext.is_resolve_ats())
    {
        switch (_resolved)
        {
            case CANNOT_RESOLVE_GLOBALLY:
                if (!resolve_id(_resolved_id, makeActualContext))
                    ERRMSG("BUG -- failed to make_actual for " << viz(*this));
                _resolved = LOCALLY_RESOLVED;
                _resolved_in = &makeActualContext.dynamic_token();
                break;
            case NOT_YET_RESOLVED_GLOBALLY:
            {
                ERRMSG("BUG did not expect to make_actual for unresolved " << viz(*this));
                is_resolved();
                break;
            }
            case GLOBALLY_RESOLVED:
            case LOCALLY_RESOLVED:
                ERRMSG("BUG should not make_actual for resolved " << viz(*this));
                break;
        }
    }
}

//  void FogMultipleId::make_actual_token(FogTokenRefToConst& actualToken, FogMakeActualContext& makeActualContext) const
//  {
//    if (is_actual(makeActualContext))
//    {
//        actualToken = *this;        // Usually redundant, but necessary if a MetaSlot traversed.
//        return;
//    }
//    PrimIdHandle resolveId;
//    if (resolve_id(resolveId, makeActualContext))
//    {
//        actualToken = FogIdentifier::make(*resolveId);
//        return;
//    }
//    Super::make_actual_token(actualToken, makeActualContext);
//  }

bool FogMultipleId::make_specifier(FogMakeSpecifierContext& makeSpecifierContext)
{
    if (!_element_type.is_identifier())
        ERRMSG("INVESTIGATE unexpected make_specifier for " << viz(*this));
    PrimIdHandle anId;
    if (!get_identifier(anId, makeSpecifierContext))
    {
        if (has_at())
        {
//              ERRMSG("INVESTIGATE failed to make_specifier " << viz(*this));
            return Super::make_specifier(makeSpecifierContext);
        }
        ERRMSG("BUG failed to make_specifier " << viz(*this));
        return false;
    }
    FogName& theName = FogIdentifier::make(*anId);
    return theName.make_specifier(makeSpecifierContext);
}

void FogMultipleId::merge_from(FogMergeContext& mergeContext, const This& thatName)
{
    Super::merge_from(mergeContext, thatName);
    FogToken::merge_short_into(mergeContext, _tokens, thatName._tokens);
}

const FogMetaType& FogMultipleId::meta_type() const { return _element_type.meta_type(); }

const FogMerge& FogMultipleId::needs_merge_from(FogMergeContext& mergeContext, const This& thatName) const
{
    FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, thatName));
    if (&_element_type != &thatName._element_type)
        needsMerge |= FogMerge::incompatible_value();
    const PrimId *thisId = is_resolved();
    const PrimId *thatId = thatName.is_resolved();
    if (!thisId || !thatId || (thisId != thatId))
    {
        FogTokenConstListOfRefToConstIterator p1(_tokens);
        FogTokenConstListOfRefToConstIterator p2(thatName._tokens);
        if (p1.to_go() != p2.to_go())
            return FogMerge::set_incompatible(FogMerge::incompatible_identifier());
        for ( ; p1 && p2; ++p1, ++p2)
            needsMerge |= p1->needs_merge(mergeContext, *p2);
    }
    return needsMerge;
}

FogTokenType::TokenType FogMultipleId::pp_token_type_enum() const { return _element_type.token_type(); }

std::ostream& FogMultipleId::print_depth(std::ostream& s, int aDepth) const
{
    Super::print_depth(s, aDepth);
    for (FogTokenConstListOfRefToConstIterator p(_tokens); p; ++p)
        p->print_deep(s, aDepth);
    return s;
}

std::ostream& FogMultipleId::print_members(std::ostream& s, int aDepth) const
{
    Super::print_members(s, aDepth);
    for (FogTokenConstListOfRefToConstIterator p(_tokens); p; ++p)
        p->print_on(s, aDepth);
    return s;
}

char FogMultipleId::print_named(std::ostream& s, const PrimId *scopeId, char tailChar) const
{
    if (scopeId)
    {
        const char *p = scopeId->str();
        tailChar = FogStream::space_and_emit(s, tailChar, p);
        if (isalnum(tailChar) && p && !*p)
            tailChar = FogStream::space_and_emit(s, tailChar, " ");
        tailChar = FogStream::space_and_emit(s, tailChar, "::");        
    }
    if (_resolved_id)
        return FogStream::space_and_emit(s, tailChar, _resolved_id->str());
    for (FogTokenConstListOfRefToConstIterator p(_tokens); p; ++p, tailChar = 0)
        tailChar = p->print_named(s, 0, tailChar);
    return tailChar;
}

std::ostream& FogMultipleId::print_viz(std::ostream& s) const
{
    s << "\"concatenated-" << _element_type << ": ";
    if (_resolved_id)
        s << *_resolved_id;
    else
        for (FogTokenConstListOfRefToConstIterator p(_tokens); p; ++p)
            p->print_named(s, 0, 0);
    return s << '\"';
}

void FogMultipleId::resolve_globally()
{
    FogStaticScopeContext scopeContext(FogScope::mutable_null());
    resolve_id(_resolved_id, scopeContext);
    _resolved = GLOBALLY_RESOLVED;
    _resolved_in = &FogScope::immutable_null();
}

bool FogMultipleId::resolve_id(PrimIdHandle& returnId, FogScopeContext& inScope) const
{
    if (_resolved_id)
        returnId = _resolved_id;
    else
    {
        PrimOstrstream s;
        for (FogTokenConstListOfRefIterator p(_tokens); p; ++p)
        {
            PrimIdHandle theId;
            switch (_element_type.value())
            {
                case FogElementType::IDENTIFIER:
                    if (!p->get_identifier(theId, inScope))
                    {               //   Happens during premature resolution of e.g. x $ { $ formal }
//                        ERRMSG("BUG failed to resolve_id " << viz(*this));
                        returnId = PrimId::null();
                        return false;
                    }
                    break;
                case FogElementType::CHARACTER:
                case FogElementType::WIDE_CHARACTER:
                    if (!p->get_character(theId, inScope))
                    {               //   Happens during premature resolution of e.g. x $ { $ formal }
//                        ERRMSG("BUG failed to resolve_id " << viz(*this));
                        returnId = PrimId::null();
                        return false;
                    }
                    break;
                case FogElementType::STRING:
                case FogElementType::WIDE_STRING:
                    if (!p->get_string(theId, inScope))
                    {               //   Happens during premature resolution of e.g. x $ { $ formal }
//                        ERRMSG("BUG failed to resolve_id " << viz(*this));
                        returnId = PrimId::null();
                        return false;
                    }
                    break;
                case FogElementType::NUMBER:                    //  .bugbug ?
                default:
                    ERRMSG("BUG -- unsupported type of FogMultipleId::resolve_id.");
                    return false;
            }
            s.write(theId->str(), theId->length());
        }
        size_t aSize = s.pcount();
        returnId = PrimIdHandle(s.str(), aSize);
    }
    return true;
}

bool FogMultipleId::resolve_semantics(FogSemanticsContext& theSemantics) const
{
    switch (_element_type.value())
    {
        case FogElementType::IDENTIFIER:
//              if (has_at())
//              {
//                  theSemantics.assign_identifier(FogSemantics::POTENTIAL);
//                  return true;
//              }
//              if (!inScope.requires_dollar_resolution() && has_dollar())
//              {
//                  theSemantics.assign_identifier(FogSemantics::POTENTIAL);
//                  return true;
//              }
            return resolve_identifier_semantics(theSemantics);
        case FogElementType::CHARACTER:
        case FogElementType::WIDE_CHARACTER:
        {
            theSemantics.assign(FogSemantics::IS_CHARACTER);
            return true;
        }
        case FogElementType::NUMBER:
        {
            theSemantics.assign(FogSemantics::IS_NUMBER);   //   No need to worry about IS_ZERO, since that's only for lexical zero.
            return true;
        }
        case FogElementType::STRING:
        case FogElementType::WIDE_STRING:
        {
            theSemantics.assign(FogSemantics::IS_STRING);
            return true;
        }
        default:
            ERRMSG("BUG -- unsupported type of FogMultipleId.");
            return false;
    }
}


FogToken::NameSort FogMultipleId::sort_class() const
{
    return NAME_NAME;               //  .bugbug should look for ctors/conversion/...
}

FogTokenType::TokenType FogMultipleId::token_type_enum() const { return _element_type.token_type(); }
