#include <Fog/FogIncludeAll.h>

#include <iostream>






TYPEINFO_SINGLE(FogPotentialDeclarationMonitor, PrimMonitor)

std::ostream& FogPotentialDeclarationMonitor::print_message(std::ostream& s) const {
	if (_prefix)
		s << _prefix << " ";
		
	s << viz(_declaration);
	
	if (_suffix)
		s << _suffix;
		
	return s;
}

FogPotentialDeclarationMonitor::FogPotentialDeclarationMonitor(const char *aPrefix, const FogPotentialDeclaration& aPotentialDeclaration,
		const char *aSuffix) : _prefix(aPrefix), _declaration(aPotentialDeclaration), _suffix(aSuffix) {
	// NOTE: enable for debugging
    if (0) {
	    std::cout << ">   " << aPrefix << " ";
	    aPotentialDeclaration.print_viz(std::cout);
	    std::cout << std::endl;
	}
}
