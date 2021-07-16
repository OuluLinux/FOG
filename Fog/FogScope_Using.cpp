#include <Fog/FogIncludeAll.h>












void FogScope_Using::add_using(FogUsing& aUsing) {
	_actual_usings.add_filtered(aUsing);
}

//  void FogScope_Using::add_using_declaration(FogScopeImpl& scopeImpl, const FogUsingDeclaration& usingDirective)
//  {
//  	const PrimId& anId = usingDirective.id();
//  	if (_declarations.find(anId))
//  		return;
//  	_declarations.add(usingDirective);
//  	FogEntity *anEntity = usingDirective.make_declaration(scopeImpl.in_scope());
//  	if (anEntity->is_type())
//  		scopeImpl.add_type(usingDirective.short_id(), *anEntity);
//  	else
//  		scopeImpl.add_name(usingDirective.short_id(), *anEntity);
//  }

//  void FogScope_Using::add_using_directive(FogScopeImpl& scopeImpl, const FogUsingDeclaration& usingDirective)
//  {
//  	const PrimId& anId = usingDirective.id();
//  	if (_directives.find(anId))
//  		return;
//  	_directives.add(usingDirective);
//  	FogNamespace *nameSpace = usingDirective.make_directive(scopeImpl.in_scope());
//  	if (nameSpace && !_using.find(nameSpace->global_signature_id()))
//  		_using.add(*nameSpace);
//  }

void FogScope_Using::create_usages() {
	for (FogUsingConstMapOfRefIterator p(_actual_usings); p; ++p)
		p->create_usages();
}

void FogScope_Using::destroy() {
//  	_directives.vacate();
	_using_specifiers.vacate();
	_potential_usings.vacate();
	_actual_usings.vacate();
}

void FogScope_Using::do_derive(FogDeriveContext& inScope) {
	for (FogSpecifierConstListOfRefToConstIterator p1(_using_specifiers); p1; ++p1) {
		FogProgressMonitor aMonitor("Resolving " , *p1);
		const FogObjectSpecifier *theSpecifier = p1->is_object_specifier();
		
		if (!theSpecifier)
			ERRMSG("BUG - should not make_entity from " << viz(*p1));
		else {
//  			FogStaticMakeEntityContext makeEntityContext(inScope);
			const FogName& aliasName = theSpecifier->type();
			FogEntity *usedEntity = 0;
			
			if (!aliasName.is_null()) {
				usedEntity = aliasName.find_entity_in(inScope);
				FogTypedef *usedTypedef = usedEntity ? usedEntity->is_typedef() : 0;
				
				if (usedTypedef) {
					FogEntity *aliasEntity = usedTypedef->resolve_alias();
					
					if (aliasEntity)
						usedEntity = aliasEntity;
				}
			}
			
			else
				usedEntity = theSpecifier->find_entity_in(inScope, FogToken::FIND_ENTITY);
				
			FogDerivable *usedDerivable = usedEntity ? usedEntity->is_derivable() : 0;
			
			if (usedDerivable)
				FogDerivable *localDerivable = usedDerivable->make_used(inScope, *theSpecifier);
			else
				if (!usedEntity)
					ERRMSG("Failed to locate declaration used by " << viz(*theSpecifier));
				else
					ERRMSG("Located " << viz(*usedEntity) << " when member-declaration required for use by "
						   << viz(*theSpecifier));
		}
	}
	
	for (FogUsingSpecifierConstListOfRefToConstIterator p2(_potential_usings); p2; ++p2) {
		FogProgressMonitor aMonitor("Resolving" , *p2);
		const FogUsingSpecifier& rawUsing = *p2;
//  		rawUsing.make(thisScope);
	}
	
	_potential_usings.vacate();
}

FogUsing *FogScope_Using::make_using(FogMakerContext& makerContext) {
	_using_specifiers.add(makerContext.specifier());
	return &FogUsing::mutable_null(); //   Satisfy non-0 return.
}

std::ostream& FogScope_Using::print_depth(std::ostream& s, int aDepth) const {
	if (_using_specifiers.tally()) {
		s << indent(aDepth) << "using declaration specifiers\n";
		_using_specifiers.print_depth(s, aDepth+1);
	}
	
	if (_potential_usings.tally()) {
		s << indent(aDepth) << "using name declarations\n";
		_potential_usings.print_depth(s, aDepth+1);
	}
	
	if (_actual_usings.tally()) {
		s << indent(aDepth) << "using name declarations\n";
		_actual_usings.print_depth(s, aDepth+1);
	}
	
//  	if (_using.tally())
//  	{
//  		s << indent(aDepth) << "using directives\n";
//  		_using.print_depth(s, aDepth+1);
//  	}
	return s;
}

std::ostream& FogScope_Using::print_members(std::ostream& s, int aDepth) const {
	if (_using_specifiers.tally()) {
		s << indent(aDepth) << "using declaration specifiers\n";
		_using_specifiers.print_members(s, aDepth+1);
	}
	
	if (_potential_usings.tally()) {
		s << indent(aDepth) << "using name declarations\n";
		_potential_usings.print_members(s, aDepth+1);
	}
	
	if (_actual_usings.tally()) {
		s << indent(aDepth) << "using name declarations\n";
		_actual_usings.print_members(s, aDepth+1);
	}
	
//  	if (_using.tally())
//  	{
//  		s << indent(aDepth) << "using directives\n";
//  		_using.print_members(s, aDepth+1);
//  	}
	return s;
}
