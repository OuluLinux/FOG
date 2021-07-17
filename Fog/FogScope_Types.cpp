#include <Fog/FogIncludeAll.h>




#define TMPL_HACK_FIX_TEMPLATE_EXPLICIT(E)\
	template<> const E& E::immutable_null(); \
	template<> E& E::mutable_null();


TMPL_HACK_FIX_TEMPLATE_EXPLICIT(FogScope_TypeSetOfRefElement);

PRIMMAPOFREFS_IMPL(FogScope_TypeSetOfRefElement)
PRIMSETELEMENT_IMPL(FogScope_TypeSetOfRefElement)

TMPL_HACK_FIX_DO_TEMPLATE(FogScope_TypeSetOfRefElement)

typedef PrimConstMapOfRefToConstIterator<FogEntitySetOfListOfRefElement> FogEntityConstSetOfListOfRefToConstIterator;

void FogScope_Types::add_scope(FogEntity& typeValue) {
	if (!typeValue.is_name_space())       //   namespace names are not type names, but they are scopes,
		add_type(typeValue, typeValue);
	else {
		const PrimId& typeName = typeValue.short_id();
		FogScope_TypeSetOfRefElement* theType = _scopes.find(typeName);
		
		if (!theType) {
			theType = new FogScope_TypeSetOfRefElement(typeName);
			
			if (theType) {
				theType->value()._definition = typeValue;
				theType->value()._resolution = typeValue;
				_scopes.adopt(*theType);
			}
		}
		else
			ERRMSG("Cannot add scope-name " << typeName << " for " << viz(typeValue)
			       << "\n\tin conflict with " << viz(*theType->value()._definition));
	}
}

void FogScope_Types::add_type(const PrimId& typeName, FogEntity& typeValue) {
	FogScope_TypeSetOfRefElement* theType = _types.find(typeName);
	
	if (!theType) {
		theType = new FogScope_TypeSetOfRefElement(typeName);
		
		if (theType) {
			theType->value()._definition = typeValue;
			theType->value()._resolution = typeValue;
			_types.adopt(*theType);
		}
	}
	else
		ERRMSG("Cannot add type-name " << typeName << " for " << viz(typeValue)
		       << "\n\tin conflict with " << viz(*theType->value()._definition));
}

void FogScope_Types::add_type(FogEntity& typeName, FogEntity& typeValue) {
	const PrimId& shortId = typeName.short_id();
	FogScope_TypeSetOfRefElement* theType = _types.find(shortId);
	
	if (!theType) {
		theType = new FogScope_TypeSetOfRefElement(shortId);
		
		if (theType) {
			theType->value()._definition = typeName;
			theType->value()._resolution = typeValue;
			_types.adopt(*theType);
		}
	}
	else if ((&*theType->value()._definition == &typeValue)
	         && (&*theType->value()._resolution == &typeValue))
		theType->value()._definition = typeName;            //   Existing name got typedefed.
	else
		ERRMSG("Cannot add type-name " << viz(typeName) << " for " << viz(typeValue)
		       << "\n\tin conflict with " << viz(*theType->value()._definition));
		       
	const PrimId& localId = typeName.local_id();
	
	if (localId != shortId) {
		FogScope_TypeSetOfRefElement* theType = _types.find(localId);
		
		if (!theType) {
			theType = new FogScope_TypeSetOfRefElement(localId);
			
			if (theType) {
				theType->value()._definition = typeName;
				theType->value()._resolution = typeValue;
				_types.adopt(*theType);
			}
		}
		else if ((&*theType->value()._definition == &typeValue)
		         && (&*theType->value()._resolution == &typeValue))
			theType->value()._definition = typeName;            //   Existing name got typedefed.
		else
			ERRMSG("Cannot add type-name " << viz(typeName) << " for " << viz(typeValue)
			       << "\n\tin conflict with " << viz(*theType->value()._definition));
	}
}

void FogScope_Types::destroy() {
	_other_types.vacate();
	FogScope_TypeSetOfRef savedTypes(_types);
	_types.vacate();                                //   Break a potential recursion.
	
	for (FogScope_TypeSetOfRefIterator p(savedTypes); p; ++p) {
		FogEntity& anEntity = *p->value()._resolution;
		anEntity.destroy();
	}
}

FogEntity* FogScope_Types::find_local_scope(const PrimId& anId, FogScopeEnums::ResolveAlias resolveAlias) {
	FogScope_TypeSetOfRefElement* theType = _types.find(anId);
	
	if (!theType)
		theType = _scopes.find(anId);
		
	if (!theType)
		return 0;
		
	FogEntity& theEntity = *theType->value()._definition;
	
	if (!theEntity.is_active())
		return 0;
		
	if (resolveAlias)
		return theType->value()._resolution.pointer();
	else
		//        return theType->value()._definition.pointer();
		return &theEntity;
}

FogEntity* FogScope_Types::find_local_type(const PrimId& anId, FogScopeEnums::ResolveAlias resolveAlias) {
	FogScope_TypeSetOfRefElement* theType = _types.find(anId);
	
	if (!theType)
		return 0;
		
	FogEntity& theEntity = *theType->value()._definition;
	
	if (!theEntity.is_active())
		return 0;
		
	if (resolveAlias)
		return theType->value()._resolution.pointer();
	else
		//        return theType->value()._definition.pointer();
		return &theEntity;
}

FogTypeName* FogScope_Types::make_typename(FogMakerContext& makerContext, bool builtIn) {
	FogTypeName* typeName = _other_types.find(makerContext.short_id());
	
	if (!typeName) {
		typeName = new FogTypeName(makerContext, builtIn);
		
		if (typeName) {
			_other_types.adopt(*typeName);
			makerContext.dynamic_scope().add_type(*typeName);
		}
	}
	
	return typeName;
}

std::ostream& FogScope_Types::print_depth(std::ostream& s, int aDepth) const {
	if (_other_types.tally()) {
		s << indent(aDepth) << "other_types\n";
		_other_types.print_depth(s, aDepth + 1);
	}
	
	if (_types.tally()) {
		s << indent(aDepth) << "types\n";
		_types.print_members(s, aDepth + 1);
	}
	
	if (_scopes.tally()) {
		s << indent(aDepth) << "scopes\n";
		_scopes.print_members(s, aDepth + 1);
	}
	
	return s;
}

std::ostream& FogScope_Types::print_members(std::ostream& s, int aDepth) const {
	if (_other_types.tally()) {
		s << indent(aDepth) << "other_types\n";
		_other_types.print_members(s, aDepth + 1);
	}
	
	if (_types.tally()) {
		s << indent(aDepth) << "types\n";
		_types.print_members(s, aDepth + 1);
	}
	
	if (_scopes.tally()) {
		s << indent(aDepth) << "scopes\n";
		_scopes.print_members(s, aDepth + 1);
	}
	
	return s;
}
