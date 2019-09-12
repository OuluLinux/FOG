#include <Fog/FogIncludeAll.h>







TYPEINFO_SINGLE(FogRawStatements, Super)
FOGTOKEN_LEAF_IMPL(FogRawStatements)

FogRawStatements::FogRawStatements()
:
    _is_exposed(IS_ENCAPSULATED)
{}

FogRawStatements::~FogRawStatements() {}

void FogRawStatements::create_usage(FogUsageContext& aContext) const
{
    ERRMSG("BUG -- did not expect to FogRawStatements::create_usage.");
    Super::create_usage(aContext);
}

FogToken::IsExposed FogRawStatements::is_exposed() const { return _is_exposed; }

void FogRawStatements::make_actual_from(FogMakeActualContext& makeActualContext)
{
//      Super::make_actual_from(makeActualContext);
    for (FogRawListOfRefToConstIterator p(raws()); p; ++p)
    {
     FogProgressMonitor aMonitor("Actualizing", *p);
        p->make_actual(p.ref(), makeActualContext);
    }
}

char FogRawStatements::print_named(std::ostream& s, const PrimId *scopeId, char tailChar) const
{
    if (raws().tally())
    {
        tailChar = raws()[0]->print_named(s, scopeId, tailChar);
        s << "; ";
    }
    s << "...";
    return '.';
}

std::ostream& FogRawStatements::print_viz(std::ostream& s) const
{
    s << "\"statement-seq : ";
    print_named(s, 0, 0);
    return s << '\"';
}

void FogRawStatements::set_exposed() { _is_exposed = IS_EXPOSED; }
