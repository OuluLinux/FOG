
#ifndef FOGSCOPE_DERIVATIONS_HXX
#define FOGSCOPE_DERIVATIONS_HXX




class FogScope_Derivations {
private:          //   Defining contents.
	FogScopeMapOfRef _derivations;     //   Derived classes
public:
	FogScope_Derivations() {}
	bool add_derivation(FogScope& aScope) { return _derivations.add(aScope); }
	void changed_base_class(FogScope& thisScope, FogBaseClass& baseClass);
	void derive_from(FogScope& derivedScope, FogBaseClass& baseClass);
	void derive_from(FogFunction& baseFunction, FogPotentialDeclaration* potentialDeclaration);
	void derive_from(FogTypedef& baseTypedef, FogPotentialDeclaration* potentialDeclaration);
	void derive_from(FogVariable& baseVariable, FogPotentialDeclaration* potentialDeclaration);
	void destroy();
	std::ostream& print_depth(std::ostream& s, int aDepth) const;
	std::ostream& print_members(std::ostream& s, int aDepth) const;
};
#endif

