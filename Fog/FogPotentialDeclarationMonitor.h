
#ifndef FOGPOTENTIALDECLARATIONMONITOR_HXX
#define FOGPOTENTIALDECLARATIONMONITOR_HXX



//
//  	A FogPotentialDeclarationMonitor refines a PrimMonitor to implement a context message defined by a prefix
//  	text string, the visible description of a FogPotentialDeclaration and then a suffix text string that defaults
//  	to a full stop.
//

class FogPotentialDeclarationMonitor : public PrimMonitor {
	TYPEDECL_SINGLE(FogPotentialDeclarationMonitor, PrimMonitor)
	
private:
	const char* _prefix;
	const FogPotentialDeclaration& _declaration;
	const char* _suffix;
	
private:
	virtual std::ostream& print_message(std::ostream& s) const;
	
public:
	FogPotentialDeclarationMonitor(const char* aPrefix, const FogPotentialDeclaration& aPotentialDeclaration, const char* aSuffix = ".");
};

#endif

