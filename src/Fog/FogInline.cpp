#include <Fog/FogIncludeAll.h>






TYPEINFO_SINGLE(FogInline, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogInline)
PRIMREFS_IMPL(FogInline)
TMPL_HACK_FIX_DO(FogInline)

FogInline *FogInline_Flyweights::_flyweights[FogInlineEnums::ARRAY_SIZE] = { 0 };
bool FogInline_Flyweights::_initialised = initialise();

struct FogInline_Flyweights_Initializer
{
    const char *_name;
    FogDeclSpecifier::Mask _decl_specifier_enum;
    FogInlineMask::Enum _inline_enum;
};
    
bool FogInline_Flyweights::initialise()
{
    static const FogInline_Flyweights_Initializer flyweightInits[] =
    {
        { "inline/*if-short*/", FogDeclSpecifier::INVALID_MASK, FogInline::EXPLICIT_IF_SHORT },
        { "inline/*in-implementation*/", FogDeclSpecifier::INLINE_IN_IMPLEMENTATION_MASK, FogInline::IN_IMPLEMENTATION },
        { "inline/*in-interface*/", FogDeclSpecifier::INLINE_IN_INTERFACE_MASK, FogInline::IN_INTERFACE },
        { "/*with-friend*/", FogDeclSpecifier::INLINE_AS_FRIEND_MASK, FogInline::WITH_FRIEND },
        { "/*!inline*/", FogDeclSpecifier::NOT_INLINE_MASK, FogInline::OUT_OF_LINE },
        { 0, FogDeclSpecifier::INVALID_MASK, FogInline::INVALID }
    };
    static FogInlineRef flyWeights[FogInlineEnums::ARRAY_SIZE];
    for (const FogInline_Flyweights_Initializer *p = flyweightInits; p->_name; ++p)
    {
        flyWeights[p->_inline_enum].adopt(new FogInline(p->_name, p->_decl_specifier_enum, p->_inline_enum));
        _flyweights[p->_inline_enum] = flyWeights[p->_inline_enum].pointer();
    }
    _flyweights[FogInline::EXPLICIT_IF_SHORT] = &FogTokenType::get_inline(FogTokenType::INLINE);
    for (int i = 0; i < (sizeof(_flyweights)/sizeof(_flyweights[0])); i++)
    {
        if (!_flyweights[i])
        {
            char buf[40];
            std::ostrstream s(buf, sizeof(buf));
            s << "inline#" << i << std::ends;
            flyWeights[i].adopt(new FogInline(s.str(), FogDeclSpecifier::INVALID_MASK, FogInline::Enum(i)));
            _flyweights[i] = flyWeights[i].pointer();
        }
    }
    return true;
}

FogInline::FogInline(const char *aName, Mask aMask, Enum anEnum)
:
    Super(aName, aMask),
    _in_line(anEnum)
{}

FogInline::FogInline(const FogInline_Initializer& anInit)
:
    Super(anInit._decl_specifier),
    _in_line(anInit._inline_enum)
{}

FogInline& FogInline::flyweight(const FogDeclSpecifierValue& aValue)
{
    int inlineBits = 0;
    if (aValue.is_explicit_inline_if_short()) inlineBits |= EXPLICIT_IF_SHORT;
    if (aValue.is_inline_in_implementation()) inlineBits |= IN_IMPLEMENTATION;
    if (aValue.is_inline_in_interface()) inlineBits |= IN_INTERFACE;
    if (aValue.is_with_friend()) inlineBits |= WITH_FRIEND;
    if (aValue.is_not_inline()) inlineBits |= OUT_OF_LINE;
    return flyweight(Enum(inlineBits)); 
}

const FogInline& FogInline::merge_inline(const FogInline& anInline) const
{
    FogInlineMask mergedInline = _in_line | anInline._in_line;
    if (mergedInline.is_inline() && mergedInline.is_out_of_line())
        ERRMSG("BUG - should not merge incompatible inlines \"" << *this << "\" and \"" << anInline << '\"');
    return flyweight(mergedInline.value());
}

const FogMerge& FogInline::needs_merge_inline(const FogInline& anInline) const
{
    if (_in_line == anInline._in_line)
        return FogMerge::both_valid();
    FogInlineMask mergedInline = _in_line | anInline._in_line;
    if (_in_line == mergedInline)
        return FogMerge::right_invalid();
    if (anInline.mask() == mergedInline)
        return FogMerge::left_invalid();
    if (mergedInline.is_inline() && mergedInline.is_out_of_line())
        return FogMerge::incompatible_inline();
    return FogMerge::both_invalid();
}
