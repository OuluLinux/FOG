#include <Fog/FogIncludeAll.h>










void FogScope_Enums::create_usages() {
	for (FogEnumMapOfRefIterator p(_enum_map); p; ++p)
		p->create_usages();
}

void FogScope_Enums::destroy() {
	_enum_map.vacate();
	_enum_list.vacate();
}

void FogScope_Enums::do_compile(FogCompileContext& inScope) {
	for (FogEnumListOfRefIterator p(_enum_list); p; ++p)
		p->do_compile(inScope);
}

void FogScope_Enums::do_derive(FogDeriveContext& inScope) {
	for (FogEnumListOfRefIterator p(_enum_list); p; ++p)
		p->do_derive(inScope);
}

FogEnum* FogScope_Enums::make_enum(FogMakerContext& makerContext) {
	const PrimId& anId = makerContext.short_id();
	FogScope& thisScope = makerContext.dynamic_scope();
	FogEntity* anEntity = thisScope.find_local_type(anId);
	
	if (anEntity) {
		FogEnum* anEnum = anEntity->is_enum();
		
		if (anEnum)
			return anEnum;
			
		ERRMSG("Cannot create enum \"" << anId << "\" in conflict with " << viz(*anEntity));
		return 0;
	}
	
	//  	const FogLocalDecl& nameId	= makerContext.name();
	const FogName& nameId = makerContext.specifier().name();
	FogEnum* anEnum = _enum_map.find(anId);
	
	if (!anEnum) {
		anEnum = new FogEnum(makerContext);
		
		if (anEnum) {
			const FogAnonId* anonId = nameId.is_anon();
			
			if (!anonId)
				;
			else if (anonId->is_named())
				anEnum->set_is_named_anon_type();
			else
				anEnum->set_is_unnamed_anon_type();
				
			//  			if (!nameId.is_anon())
			//  				;
			//  			else if (nameId.is_named_anon())
			//  				anEnum->set_is_named_anon_type();
			//  			else
			//  				anEnum->set_is_unnamed_anon_type();
			_enum_map.adopt(*anEnum);
			_enum_list.add(*anEnum);
			thisScope.add_type(*anEnum);
		}
	}
	
	return anEnum;
}

std::ostream& FogScope_Enums::print_depth(std::ostream& s, int aDepth) const {
	if (_enum_list.tally()) {
		s << indent(aDepth) << "enums\n";
		_enum_list.print_depth(s, aDepth + 1);
	}
	
	return s;
}

std::ostream& FogScope_Enums::print_members(std::ostream& s, int aDepth) const {
	if (_enum_list.tally()) {
		s << indent(aDepth) << "enums\n";
		_enum_list.print_members(s, aDepth + 1);
	}
	
	return s;
}
