#include <Fog/FogIncludeAll.h>










void FogScope_Typedefs::create_usages() {
	for (FogTypedefConstListOfRefIterator p(_distinct_typedef_list); p; ++p)
		if (p->is_active())
			p->create_usages();
}

void FogScope_Typedefs::derive_from(FogScope& derivedScope, FogBaseClass& baseClass) {
	for (FogTypedefConstListOfRefIterator p(baseClass.base().distinct_typedef_list()); p; ++p)
		derive_from(derivedScope, *p, 0);
}

void FogScope_Typedefs::derive_from(FogScope& derivedScope, FogTypedef& baseTypedef,
                                    FogPotentialDeclaration* potentialDeclaration) {
	FogStaticScopeContext derivedScopeContext(derivedScope);
	FogDerivable* aDerivable = _distinct_typedef_map.find(baseTypedef.local_signature_id());
	
	if (!potentialDeclaration)
		aDerivable = baseTypedef.make_derived(derivedScopeContext, aDerivable);
	else if (!aDerivable)
		aDerivable = baseTypedef.make_derived(derivedScopeContext, aDerivable);
	else
		potentialDeclaration->make_derivation(derivedScopeContext, *aDerivable);
}

void FogScope_Typedefs::destroy() {
	for (FogTypedefConstListOfRefIterator p(_distinct_typedef_list); p; ++p)
		p->destroy();
		
	_distinct_typedef_map.vacate();
	_distinct_typedef_list.vacate();
}

void FogScope_Typedefs::do_compile(FogCompileContext& inScope) {
	_distinct_typedef_list.sort(&FogTypedef::compare);
	
	for (FogTypedefListOfRefIterator p(_distinct_typedef_list); p; ++p)
		p->do_compile(inScope);
}

void FogScope_Typedefs::do_derive(FogDeriveContext& inScope) {
	for (FogTypedefMapOfRefIterator p(_distinct_typedef_map); p; ++p)
		p->do_derive(inScope);
}

FogTypedef* FogScope_Typedefs::find_local_typedef(const FogTypedef& aTypedef) {
	const PrimId& anId = aTypedef.local_signature_id();
	FogTypedef* foundTypedef = _distinct_typedef_map.find(anId);
	return foundTypedef && foundTypedef->is_active() ? foundTypedef : 0;
}

//
//  	Create and return a typedef, returning an existing typedef if compatible.
//  	Note that multiple typedefs are permitted provided they all refer to the same entity.
//    In particular a class name or parameter name may be typedefed by its own name
//    (see FogTypdefSpecifier::compile_specifier).
//
FogTypedef* FogScope_Typedefs::make_typedef(FogMakerContext& makerContext) {
	FogScope& thisScope = makerContext.dynamic_scope();
	const FogTypedefSpecifier* typedefSpecifier = makerContext.specifier().get_typedef_specifier(thisScope);
	
	if (!typedefSpecifier)
		return 0;
		
	FogEntity* newValue = 0;
	const FogName* typedefName = typedefSpecifier->find_scalar_name();
	const FogName* typeName = typedefSpecifier->get_type_name();
	
	if (typedefName && typeName) {   //   If name is free from *, [], () - could be an alias
		FogResolutionScopeContext weakContext(makerContext, FogResolutionScopeContext::RESOLVE_LAZY);
		newValue = typeName->find_type_in(weakContext); //    find out what it could be an alias for.
	}
	
	//  	const PrimId& anId = makerContext.short_id();
	const PrimIdHandle anId = makerContext.local_signature_id();
	FogTypedef* aTypedef = _distinct_typedef_map.find(*anId);
	
	if (aTypedef) {       //   If a repeated typedef
		FogEntity* oldValue = aTypedef->resolve_alias();
		
		if (oldValue && newValue && (newValue != oldValue))
			ERRMSG("Cannot change typedef \"" << *anId << "\" from " << viz(*oldValue)
			       << " to " << viz(*newValue));
			       
		return aTypedef;
	}
	
	const FogEntity* anEntity = thisScope.find_local_type(*anId);
	
	if (anEntity && (newValue != anEntity)) {
		ERRMSG("Cannot create typedef \"" << *anId << "\" in conflict with " << viz(*anEntity));
		return 0;
	}
	
	if (thisScope.is_frozen())
		ERRMSG("Frozen class structure suppresses " << viz(makerContext));
	else if ((aTypedef = new FogTypedef(makerContext, newValue)) != 0) {
		_distinct_typedef_map.adopt(*aTypedef);
		_distinct_typedef_list.add(*aTypedef);
		thisScope.add_typedef(*aTypedef, 0);
	}
	
	return aTypedef;
}

bool FogScope_Typedefs::needs_emission() const {
	for (FogTypedefConstListOfRefToConstIterator p(_distinct_typedef_list); p; ++p)
		if (p->is_active())
			return true;
			
	return false;
}

std::ostream& FogScope_Typedefs::print_depth(std::ostream& s, int aDepth) const {
	if (_distinct_typedef_list.tally()) {
		s << indent(aDepth) << "typedefs\n";
		_distinct_typedef_list.print_depth(s, aDepth + 1);
	}
	
	return s;
}

std::ostream& FogScope_Typedefs::print_members(std::ostream& s, int aDepth) const {
	if (_distinct_typedef_list.tally()) {
		s << indent(aDepth) << "typedefs\n";
		_distinct_typedef_list.print_members(s, aDepth + 1);
	}
	
	return s;
}

void FogScope_Typedefs::validate() const {
	for (FogTypedefConstListOfRefToConstIterator p(_distinct_typedef_list); p; ++p)
		if (p->is_active())
			p->validate();
}
