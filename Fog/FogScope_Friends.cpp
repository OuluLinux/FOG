#include <Fog/FogIncludeAll.h>








void FogScope_Friends::add(const FogScope& aScope, FogEntity& anEntity) {
	if (_friends.find(anEntity.unique_id()))
		WRNMSG("Duplicate " << viz(anEntity) << " ignored for " << viz(aScope));
	else
		_friends.add(anEntity);
}

void FogScope_Friends::create_usages(FogScope& inScope) {
	for (FogUniqueConstMapOfRefIterator p(_friends); p; p++) {
		FogUsage* aUsage = inScope.usage_manager().make_friend_usage(inScope, (FogEntity&) * p);
		
		if (aUsage) {
			inScope.interface_usage_start().add_use_by(*aUsage);
			aUsage->add_use_by(inScope.interface_usage_finish());
		}
	}
}

void FogScope_Friends::destroy() {
	_friends.vacate();
}

void FogScope_Friends::do_derive(FogDeriveContext& inScope) {
	//  .bugbug no code
}

std::ostream& FogScope_Friends::print_depth(std::ostream& s, int aDepth) const {
	if (_friends.tally()) {
		s << indent(aDepth) << "friends\n";
		_friends.print_depth(s, aDepth + 1);
	}
	
	return s;
}

std::ostream& FogScope_Friends::print_members(std::ostream& s, int aDepth) const {
	if (_friends.tally()) {
		s << indent(aDepth) << "friends\n";
		_friends.print_members(s, aDepth + 1);
	}
	
	return s;
}
