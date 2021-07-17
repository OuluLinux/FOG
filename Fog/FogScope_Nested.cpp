#include <Fog/FogIncludeAll.h>








FogScope* FogScope_Nested::add_scope(FogScope& toScope, FogScope& aScope) {
	const PrimId& anId = aScope.local_signature_id();
	
	if (_scope_map.find(anId))
		ERRMSG("BUG -- should not add duplicate " << viz(aScope) << " to " << viz(toScope));
	else if (aScope.nesting_depth() <= nesting_depth())
		ERRMSG("Cannot add more heavily nested " << viz(aScope) << " to " << viz(toScope));
	else {
		_scope_map.add(aScope);
		return &aScope;
	}
	
	return 0;
}

void FogScope_Nested::destroy() {
	for (FogScopeConstMapOfRefIterator p(_scope_map); p; ++p)
		p->destroy();
		
	_scope_map.vacate();
}

void FogScope_Nested::do_derive(FogDeriveContext& inScope) {
	for (FogScopeMapOfRefIterator p(_scope_map); p; ++p)
		if (!p->done_derive())                          //  .bugbug why is conditional needed
			p->do_derive(inScope);
}

void FogScope_Nested::do_meta_construct(FogScopeContext& inScope) {
	FogScopeListOfRef scopeList(_scope_map);
	
	while (true) {
		const size_t startTally = _scope_map.tally();
		
		for (FogScopeConstListOfRefIterator p(scopeList); p; ++p)
			if (!p->done_meta_construct())                  //  .bugbug why is conditional needed
				p->do_meta_construct(inScope);
				
		if (_scope_map.tally() == startTally)
			break;
			
		FogScopeMapOfRef newScopeMap(_scope_map);
		FogScopeMapOfRef oldScopeMap(scopeList.tally());
		oldScopeMap |= scopeList;
		newScopeMap -= oldScopeMap;
		scopeList = newScopeMap;
	}
}

std::ostream& FogScope_Nested::print_depth(std::ostream& s, int aDepth) const {
	if (_scope_map.tally()) {
		s << indent(aDepth) << "nested scopes\n";
		
		for (FogScopeConstMapOfRefToConstIterator p(_scope_map); p; ++p)
			p->print_deep(s, aDepth + 1);
	}
	
	return s;
}

std::ostream& FogScope_Nested::print_members(std::ostream& s, int aDepth) const {
	if (_scope_map.tally()) {
		s << indent(aDepth) << "nested scopes\n";
		
		for (FogScopeConstMapOfRefToConstIterator p(_scope_map); p; ++p)
			p->print_on(s, aDepth + 1);
	}
	
	return s;
}

void FogScope_Nested::validate() const {
	for (FogScopeConstMapOfRefToConstIterator p(_scope_map); p; ++p)
		p->validate();
}
