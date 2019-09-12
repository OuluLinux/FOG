#include <Fog/FogIncludeAll.h>








TYPEINFO_SINGLE(FogIterationDomain, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogIterationDomain)
PRIMREFS_IMPL(FogIterationDomain)
FOGTOKEN_READ_ONLY_IMPL(FogIterationDomain)
TMPL_HACK_FIX_DO(FogIterationDomain)

FogIterationDomain::FogIterationDomain()
:
    _meta_type(FogMetaType::void_type()),
    _executable_tokens(0),
    _source_lines(0)
{}

FogIterationDomain::FogIterationDomain(const FogMetaType& metaType)
:
    _meta_type(metaType),
    _executable_tokens(0),
    _source_lines(0)
{}

FogIterationDomain::FogIterationDomain(const This& tokenList)
:
    Super(tokenList),
    _meta_type(tokenList._meta_type),
    _tokens(tokenList._tokens),
    _executable_tokens(tokenList._executable_tokens),
    _source_lines(tokenList._source_lines)
{}

FogIterationDomain::~FogIterationDomain() {}

void FogIterationDomain::add(FogToken& aToken)
{
    if (!aToken.is_exposed())
        _tokens.add(aToken);
    else
    {
        FogIterationDomain *aDomain = aToken.is_iteration_domain();
        if (aDomain)
            _tokens.add(aDomain->tokens());
        else
        {
            ERRMSG("BUG -- use of FogTokens::is_exposed.");
          _tokens.add(aToken);
        }
    }
    _executable_tokens += aToken.executable_tokens();
    _source_lines += aToken.source_lines();
}

bool FogIterationDomain::emit(FogEmitContext& emitContext) const
{
    bool doneSomething = false;
    for (FogTokenConstListOfRefToConstIterator p(_tokens); p; ++p)
        doneSomething |= p->emit(emitContext);
    return doneSomething;
}

size_t FogIterationDomain::executable_tokens() const { return _executable_tokens; }

bool FogIterationDomain::get_string(PrimIdHandle& returnId, FogScopeContext& inScope)
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

bool FogIterationDomain::is_actual(const FogScopeContext& scopeContext) const
{
    for (FogTokenConstListOfRefToConstIterator p(_tokens); p; ++p)
        if (!p->is_actual(scopeContext))
            return false;
    return Super::is_actual(scopeContext);
}

FogIterationDomain *FogIterationDomain::is_iteration_domain() { return this; }
FogToken::IsExposed FogIterationDomain::is_exposed() const { return IS_EXPOSED; }

void FogIterationDomain::make_actual_from(FogMakeActualContext& makeActualContext)
{
    Super::make_actual_from(makeActualContext);
    for (FogTokenListOfRefToConstIterator p(_tokens); p; ++p)
        p->make_actual(p.ref(), makeActualContext);
}

const FogMetaType& FogIterationDomain::meta_type() const { return _meta_type; }

std::ostream& FogIterationDomain::print_depth(std::ostream& s, int aDepth) const
{
    Super::print_depth(s, aDepth);
    if (_tokens.tally())
    {
        s << indent(aDepth) << "tokens\n";
        _tokens.print_depth(s, aDepth+1);
    }
    return s;
}

std::ostream& FogIterationDomain::print_members(std::ostream& s, int aDepth) const
{
    Super::print_members(s, aDepth);
    if (_tokens.tally())
    {
        s << indent(aDepth) << "tokens\n";
        _tokens.print_members(s, aDepth+1);
    }
    return s;
}

std::ostream& FogIterationDomain::print_this(std::ostream& s) const { return s; }

std::ostream& FogIterationDomain::print_viz(std::ostream& s) const
{
    s << "\"iteration-domain: ";
    print_this(s);
    return s << '\"';
}

bool FogIterationDomain::resolve_semantics(FogSemanticsContext& theSemantics) const
{
    theSemantics.reset();
//    theSemantics.set(FogSemantics::IS_LIST);
    theSemantics.set(FogSemantics::IS_COMPOUND_DECLARATION);        //  .bugbug Put in some real code
    return true;
}

size_t FogIterationDomain::source_columns() const
{
    size_t sourceCols = 0;
    for (FogTokenConstListOfRefToConstIterator p(_tokens); p; ++p)
        sourceCols += p->source_columns();
    return sourceCols;
}

size_t FogIterationDomain::source_lines() const { return _source_lines; }
const FogTokenListOfRefToConst& FogIterationDomain::tokens() const { return _tokens; }
