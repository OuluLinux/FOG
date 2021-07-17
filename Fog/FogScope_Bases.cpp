#include <Fog/FogIncludeAll.h>











FogScope_Bases::FogScope_Bases()
	:
	_virtual_bases(0)
{}

//
//  	Add baseClass to thisScope. Returns true if this is a new declaration,
//  	or false if a redundant repeated declaration.
//
bool FogScope_Bases::add_base(FogScope& thisScope, FogBaseClass& baseClass) {
	FogBaseClass* oldBaseClass = _base_map.find(baseClass.id());
	
	if (oldBaseClass) {
		FogStaticScopeContext staticScopeContext(thisScope);
		FogMergeContext mergeContext(staticScopeContext);
		const FogMerge& needsMerge = oldBaseClass->needs_merge(mergeContext, baseClass);
		
		if (needsMerge.is_incompatible())
			ERRMSG(needsMerge << " - cannot compose " << viz(*oldBaseClass) << " and " << viz(baseClass));
		else if (!needsMerge.both_are_valid()) {
			FogTokenRefToConst theToken(*oldBaseClass);
			baseClass.merge_into(mergeContext, theToken);
			thisScope.changed_base_class(baseClass);
		}
		
		return false;
	}
	else if (!baseClass.base().add_derivation(thisScope)) {
		ERRMSG("Cannot use derived class " << viz(baseClass) << " as a base of " << viz(thisScope));
		return false;
	}
	else {
		_direct_bases.add(baseClass);
		_base_map.add(baseClass);
		//  		thisScope.added_base_class(baseClass);		-- in caller so that base type gets in before a typedef.
		return true;
	}
}

const FogScope* FogScope_Bases::build_base_classes(FogScope& thisScope) {
	if (!_direct_bases.tally())
		return 0;
		
	_ctor_bases.vacate();
	_all_bases.vacate();
	PrimIdMap visitMap(64);
	visitMap.add(thisScope.unique_id());
	FogScopeListOfRef nonVirtualBases;
	const FogScope* multipleBase = find_bases(thisScope, visitMap, nonVirtualBases, _ctor_bases);
	_all_bases.add(_ctor_bases);
	_virtual_bases = _ctor_bases.tally();
	
	for (FogBaseClassConstListOfRefIterator p(_direct_bases); p; ++p)
		if (!p->is_virtual())
			_ctor_bases.add(p->base());
			
	_all_bases.add(nonVirtualBases);
	return multipleBase;
}

void FogScope_Bases::changed_base_class(FogScope& thisScope, FogBaseClass& baseClass) {
	//  	FogBaseClass *myBaseClass = _base_map.find(baseClass.id());
	//  	if (!myBaseClass && (myBaseClass->is_virtual() != baseClass.is_virtual()))
	build_base_classes(thisScope);
}

void FogScope_Bases::create_usages(FogScope& inScope) {
	for (FogBaseClassConstListOfRefIterator p(_direct_bases); p; ++p) {
		FogScope& aBase = p->base();
		
		if (!p->is_auto() && !aBase.is_auto())
			aBase.interface_usage_finish().add_use_by(inScope.interface_usage_start());
	}
}

void FogScope_Bases::derive_from(FogScope& derivedScope, FogBaseClass& baseClass) {
	//  	if (baseClass.is_virtual())
	build_base_classes(derivedScope);
	//  	else
	//  		_all_bases.add(baseClass.base());
}

void FogScope_Bases::destroy() {
	for (FogBaseClassConstListOfRefIterator p(_direct_bases); p; ++p)
		p->destroy();
		
	_direct_bases.vacate();
	_base_map.vacate();
	_ctor_bases.vacate();
	_all_bases.vacate();
}

void FogScope_Bases::do_compile(FogCompileContext& inScope) {
	//  	for (FogBaseClassConstListOfRefIterator p(_direct_bases); p; ++p)
	//  		p->do_compile(inScope);
}

void FogScope_Bases::do_derive(FogDeriveContext& inScope) {
	FogScope& aScope = inScope.dynamic_scope();
	//  	PrimIdMap visitMap(64);
	//  	visitMap.add(aScope.unique_id());
	//  	FogScopeListOfRef nonVirtualBases;
	//  	const FogScope *multipleBase = find_bases(aScope, visitMap, nonVirtualBases, _all_bases);
	//  	_non_virtual_bases = nonVirtualBases.tally();
	//  	_all_bases.add(nonVirtualBases);
	const FogScope* multipleBase = build_base_classes(aScope);
	
	if (multipleBase)
		ERRMSG("Base class " << viz(*multipleBase) << " occurs more than once in " << viz(inScope));
		
	for (FogBaseClassConstListOfRefIterator p4(_direct_bases); p4; ++p4) {
		FogScope& aBase = p4->base();
		
		if (!aBase.done_derive())
			aBase.do_derive(inScope);
	}
}

void FogScope_Bases::do_meta_construct(FogScopeContext& inScope) {
	for (FogBaseClassConstListOfRefIterator p(_direct_bases); p; ++p) {
		FogScope& aBase = p->base().primary();
		
		if (!aBase.done_meta_construct())
			aBase.do_meta_construct(inScope);
	}
}

void FogScope_Bases::emit_base_interface(FogEmitContext& emitContext) const {
	FogBaseClassConstListOfRefToConstIterator p(_direct_bases);
	
	if (p) {
		const char* prefixString = " : ";
		
		for ( ; p; ++p) {
			if (p->access().is_emitted()) {
				emitContext.emit_space_and_text(prefixString);
				p->emit_interface(emitContext);
				prefixString = ", ";
			}
		}
	}
}

//
//  	Participate in the hierarchical traversal initiated by FogScope::do_derive that establishes the
//  	construction ordered lists of non-virtual and virtual bases, with visitMap tracking progress
//  	to avoid infinite loops. Returns 0 if a valid set of bases established, returns the identity of some
//  	multiply occuring base otherwise.
//
const FogScope* FogScope_Bases::find_bases(const FogScope& inScope, PrimIdMap& visitMap,
        FogScopeListOfRef& nonVirtualBases, FogScopeListOfRef& virtualBases) const {
	const FogScope* multipleBase = 0;
	
	for (FogBaseClassConstListOfRefIterator p(_direct_bases); p; ++p) {
		const FogScope* nestedMultipleBase = p->find_bases(inScope, visitMap, nonVirtualBases, virtualBases);
		
		if (nestedMultipleBase && !multipleBase)
			multipleBase = nestedMultipleBase;
	}
	
	return multipleBase;
}

//
//  	Locate the name or type satisfying theFinder in the base classes.
//
bool FogScope_Bases::find_entity(FogEntityFinder& theFinder) {
	bool gotOne = false;
	
	for (FogBaseClassConstListOfRefIterator p(_direct_bases); p; ++p) {
		FogBaseClass& baseClass = *p;
		bool wasDominatable = theFinder.set_dominatable(baseClass.is_virtual());
		FogScope& aBase = baseClass.base();
		
		if (aBase.find_visible_entity(theFinder))
			gotOne = true;
			
		theFinder.set_dominatable(wasDominatable);
	}
	
	return gotOne;
}

//
//  	Locate the slot satisfying theFinder in the base classes.
//
bool FogScope_Bases::find_slot(FogScope& inScope, FogMetaSlotFinder& theFinder) {
	if (!_direct_bases.tally())
		//  		return inScope.global_scope().meta_base().find_local_slot(theFinder);
		return inScope.global_scope().meta_base().find_visible_slot(theFinder);
		
	bool gotOne = false;
	
	for (FogBaseClassConstListOfRefIterator p(_direct_bases); p; ++p) {
		FogBaseClass& baseClass = *p;
		//          bool wasDominatable = theFinder.set_dominatable(baseClass.is_virtual());
		FogScope& aBase = baseClass.base();
		
		if (aBase.find_visible_slot(theFinder))
			gotOne = true;
			
		//          theFinder.set_dominatable(wasDominatable);
	}
	
	return gotOne;
}

//
//  	Return true if any base class is pure.
//
bool FogScope_Bases::is_pure() const {
	for (FogScopeConstListOfRefToConstIterator p(_ctor_bases); p; ++p) {
		const FogScope& aBase = *p;
		const FogLazyBool& isPure = aBase.is_pure();
		
		if (isPure.is_true())
			return true;
			
		if (!isPure.is_false())
			ERRMSG("BUG -- use of undecided is_pure for " << viz(aBase));
	}
	
	return false;
}

bool FogScope_Bases::needs_emission() const {
	for (FogBaseClassConstListOfRefToConstIterator p(_direct_bases); p; ++p) {
		const FogBaseClass& baseClass = *p;
		
		if (baseClass.access().is_emitted())
			return true;
	}
	
	return false;
}

std::ostream& FogScope_Bases::print_depth(std::ostream& s, int aDepth) const {
	if (_direct_bases.tally()) {
		s << indent(aDepth) << "bases\n";
		_direct_bases.print_depth(s, aDepth + 1);
	}
	
	if (_all_bases.tally()) {
		s << indent(aDepth) << "all bases\n";
		_all_bases.print_members(s, aDepth);
	}
	
	return s;
}

std::ostream& FogScope_Bases::print_members(std::ostream& s, int aDepth) const {
	if (_direct_bases.tally()) {
		s << indent(aDepth) << "bases\n";
		_direct_bases.print_members(s, aDepth + 1);
	}
	
	if (_all_bases.tally()) {
		s << indent(aDepth) << "all bases\n";
		_all_bases.print_members(s, aDepth);
	}
	
	return s;
}
