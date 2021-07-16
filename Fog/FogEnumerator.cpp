#include <Fog/FogIncludeAll.h>
















TYPEINFO_SINGLE(FogEnumerator, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogEnumerator)
PRIMREFS_IMPL(FogEnumerator)
PRIMMAPOFREFS_IMPL(FogEnumerator)
TMPL_HACK_FIX_DO(FogEnumerator)

FogEnumerator::FogEnumerator()
:
    _previous(0),
    _has_previous(false),
    _has_value(false),
    _number(FogNumber::null())
{}

FogEnumerator::FogEnumerator(FogScope& aScope, const PrimId& anId, const FogExpr& initExpr)
:
    Super(aScope, anId, anId),
    _init(initExpr),
    _previous(0),
    _has_previous(false),
    _has_value(false),
    _number(FogNumber::null())
{
    scope().add_name(*this);
}

FogEnumerator::~FogEnumerator() {}

void FogEnumerator::emit_interface(FogStream& s)
{
    s << short_id();
    if (_init)
    {
        s.emit_space_and_text(" = ");
        FogStaticEmitContext emitContext(s, s.inner_scope(), FogEmitContext::GLOBAL_INTERFACE);
        _init->emit(emitContext);                       //  .bugbug get inScope, forUseAs from parameters.
    }
}

bool FogEnumerator::get_number_token(FogTokenRef& returnValue, FogScopeContext& inScope) const
{
    resolve_value(returnValue, inScope);
    return !returnValue->is_null();
}

bool FogEnumerator::is_active() const { return true; }
FogEnumerator *FogEnumerator::is_enumerator() { return this; }

//  
//    Return true if this enumerator matches anEnumerator sufficiently to represent the same entity.
//  
bool FogEnumerator::matches(FogEnumerator& anEnumerator)
{ 
    if (short_id() != anEnumerator.short_id())
        return false;
    if (_init->is_null())
    {
        if (!anEnumerator._init->is_null())
            return false;
        if (!_has_value)
            resolve_value();
        if (!anEnumerator._has_value)
            anEnumerator.resolve_value();
        if (&*_number != &*anEnumerator._number)
            return false;
    }
    else
    {
        if (anEnumerator._init->is_null())
            return false;
        FogStaticScopeContext staticScopeContext(scope());
        FogMergeContext mergeContext(staticScopeContext);
        const FogMerge& needsMerge = _init->needs_merge(mergeContext, *anEnumerator._init);
        if (!needsMerge.both_are_valid())
            return false;
    }
    return true;
}

const FogMetaType& FogEnumerator::meta_type() const { return FogMetaType::enumerator_type(); }

std::ostream& FogEnumerator::print_this(std::ostream& s) const
{
    s << unique_id();
    if (_init)
        s << " = " << viz(*_init);
    else if (_has_value)
        s << " = " << _number;
    return s;
}

std::ostream& FogEnumerator::print_viz(std::ostream& s) const
{
    return s << "\"enumerator: " << unique_id() << '\"';
}

void FogEnumerator::resolve_value(FogTokenRef& returnValue, FogScopeContext& inScope) const
{
    if (_has_value)
        returnValue =_number;
    else
    {
        if (_init)
        {
            if (!_init->get_number_token(returnValue, inScope))
                returnValue = FogNumber::make_zero();
        }
        else if (!_has_previous)
        {
            ERRMSG("BUG - should not invoke resolve_value of " << viz(*this) << " prior to set_previous.");
            returnValue = FogNumber::make_zero();
        }
        else if (!_previous)
            returnValue = FogNumber::make_zero();
        else
        {
            static FogNumber& plusOne = FogNumber::make_natural(1);
            FogTokenRef previousValue;
            if (!_previous->get_number_token(previousValue, inScope)
             || !previousValue->get_dyadic(returnValue, inScope, FogTokenType::character('+'), plusOne))
                returnValue = FogNumber::make_zero();
        }
    }
}

void FogEnumerator::resolve_value() const
{
    if (!_has_value)
    {
        FogStaticScopeContext inScope(mutate().scope());
        mutate()._has_value = true;
        if (_init)
        {
            if (!_init->get_number_token(mutate()._number, inScope))
                mutate()._number = FogNumber::make_zero();
        }
        else if (!_has_previous)
        {
            ERRMSG("BUG - should not invoke resolve_value of " << viz(*this) << " prior to set_previous.");
            mutate()._number = FogNumber::make_zero();
        }
        else if (!_previous)
            mutate()._number = FogNumber::make_zero();
        else
        {
            static FogNumber& plusOne = FogNumber::make_natural(1);
            FogTokenRef previousValue;
            if (!_previous->get_number_token(previousValue, inScope)
             || !previousValue->get_dyadic(mutate()._number, inScope, FogTokenType::character('+'), plusOne))
                mutate()._number = FogNumber::make_zero();
        }
    }
}

void FogEnumerator::set_previous(const FogEnumerator *anEnumerator)
{
    _has_previous = true;
    _previous = anEnumerator;
}
