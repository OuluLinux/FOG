#include <Fog/FogIncludeAll.h>








TYPEINFO_SINGLE(FogCodeTokens, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogCodeTokens)
PRIMREFS_IMPL(FogCodeTokens)
FOGTOKEN_READ_ONLY_IMPL(FogCodeTokens)
TMPL_HACK_FIX_DO(FogCodeTokens)

FogCodeTokens::FogCodeTokens()
:
    _executable_tokens(0),
    _source_lines(0)
{}


FogCodeTokens::FogCodeTokens(const This& tokenList)
:
    Super(tokenList),
    _tokens(tokenList._tokens),
    _executable_tokens(tokenList._executable_tokens),
    _source_lines(tokenList._source_lines)
{}

FogCodeTokens::~FogCodeTokens() {}

void FogCodeTokens::add(FogToken& aToken)
{
    _tokens.add(aToken);
    _executable_tokens += aToken.executable_tokens();
    _source_lines += aToken.source_lines();
    aToken.set_exposed();       //   Outer {} supplied by function.
}

bool FogCodeTokens::emit(FogEmitContext& emitContext) const
{
    bool doneSomething = false;
    for (FogTokenConstListOfRefToConstIterator p(_tokens); p; ++p)
        doneSomething |= p->emit(emitContext);
    return doneSomething;
}

size_t FogCodeTokens::executable_tokens() const { return _executable_tokens; }

bool FogCodeTokens::get_string(PrimIdHandle& returnId, FogScopeContext& inScope)
{
    PrimOstrstream s;
    for (FogTokenConstListOfRefIterator p(_tokens); p; ++p)
    {
        PrimIdHandle theId;
        if (p->get_string(theId, inScope))
            s.write(theId->str(), theId->length());
    }
    size_t aSize = s.pcount();
    returnId = PrimIdHandle(s.str(), aSize);
    return true;
}

bool FogCodeTokens::is_actual(const FogScopeContext& scopeContext) const
{
    for (FogTokenConstListOfRefToConstIterator p(_tokens); p; ++p)
        if (!p->is_actual(scopeContext))
            return false;
    return Super::is_actual(scopeContext);
}

//  FogCodeTokens *FogCodeTokens::is_const_tokens() { return this; }

void FogCodeTokens::make_actual_from(FogMakeActualContext& makeActualContext)
{
    Super::make_actual_from(makeActualContext);
    for (FogTokenListOfRefToConstIterator p(_tokens); p; ++p)
        p->make_actual(p.ref(), makeActualContext);
}

std::ostream& FogCodeTokens::print_depth(std::ostream& s, int aDepth) const
{
    Super::print_depth(s, aDepth);
    if (_tokens.tally())
    {
        s << indent(aDepth) << "tokens\n";
        _tokens.print_depth(s, aDepth+1);
    }
    return s;
}

std::ostream& FogCodeTokens::print_members(std::ostream& s, int aDepth) const
{
    Super::print_members(s, aDepth);
    if (_tokens.tally())
    {
        s << indent(aDepth) << "tokens\n";
        _tokens.print_members(s, aDepth+1);
    }
    return s;
}

std::ostream& FogCodeTokens::print_this(std::ostream& s) const { return s; }

std::ostream& FogCodeTokens::print_viz(std::ostream& s) const
{
    s << "\"tokens: ";
    print_this(s);
    return s << '\"';
}

size_t FogCodeTokens::source_columns() const
{
    size_t sourceCols = 0;
    for (FogTokenConstListOfRefToConstIterator p(_tokens); p; ++p)
        sourceCols += p->source_columns();
    return sourceCols;
}

size_t FogCodeTokens::source_lines() const { return _source_lines; }

const FogTokenListOfRefToConst& FogCodeTokens::tokens() const { return _tokens; }
