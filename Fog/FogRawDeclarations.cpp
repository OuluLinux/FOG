#include <Fog/FogIncludeAll.h>







TYPEINFO_SINGLE(FogRawDeclarations, Super)
FOGTOKEN_LEAF_IMPL(FogRawDeclarations)

FogRawDeclarations::FogRawDeclarations() {
	CONDMSG(Fog::debug_make(), "Created " << *this);
}

FogRawDeclarations::~FogRawDeclarations() {}

void FogRawDeclarations::create_usage(FogUsageContext& aContext) const {
	ERRMSG("BUG -- did not expect to FogRawDeclarations::create_usage.");
	Super::create_usage(aContext);
}

std::ostream& FogRawDeclarations::print_viz(std::ostream& s) const {
	s << "\"declaration-seq : ";
	print_named(s, 0, 0);
	return s << '\"';
}
