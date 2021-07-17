#include <Fog/FogIncludeAll.h>








TYPEINFO_SINGLE(FogFileIds, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogFileIds)
PRIMREFS_IMPL(FogFileIds)
FOGTOKEN_ACTUAL_IMPL(FogFileIds)
TMPL_HACK_FIX_DO(FogFileIds)

FogFileIds::FogFileIds() {}
FogFileIds::~FogFileIds() {}

std::ostream& FogFileIds::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	
	if (_ids.tally()) {
		s << indent(aDepth) << "file-ids\n";
		_ids.print_depth(s, aDepth + 1);
	}
	
	return s;
}

std::ostream& FogFileIds::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	
	if (_ids.tally()) {
		s << indent(aDepth) << "file-ids\n";
		_ids.print_members(s, aDepth + 1);
	}
	
	return s;
}

std::ostream& FogFileIds::print_this(std::ostream& s) const { return s; }

std::ostream& FogFileIds::print_viz(std::ostream& s) const {
	s << "\"file-ids: ";
	print_this(s);
	return s << '\"';
}
