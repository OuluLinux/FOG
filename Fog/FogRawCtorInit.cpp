#include <Fog/FogIncludeAll.h>








TYPEINFO_SINGLE(FogRawCtorInit, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogRawCtorInit)
PRIMREFS_IMPL(FogRawCtorInit)
FOGTOKEN_UNIQUE_IMPL(FogRawCtorInit)
TMPL_HACK_FIX_DO(FogRawCtorInit)

FogRawCtorInit::FogRawCtorInit() {}

FogRawCtorInit::FogRawCtorInit(FogName& aName, const FogExpr* initList)
	:
	_name(aName),
	_inits(initList)
{}

FogRawCtorInit::~FogRawCtorInit() {}

void FogRawCtorInit::compile_ctor(FogMakeEntityContext& makeEntityContext, FogCtorInitMapOfRefToConst& ctorInits) const {
	FogResolutionScopeContext requiredContext(makeEntityContext, FogScopeContext::RESOLVE_ATS);
	FogEntity* anEntity = _name->find_entity_in(requiredContext);
	
	if (!anEntity) {
		ERRMSG("Failed to find entity to satisfy " << viz(*this) << " in " << viz(makeEntityContext));
		return;
	}
	
	const PrimId& anId = anEntity->unique_id();
	FogExprRefToConst actualInits(_inits);
	
	if (!actualInits->is_actual(makeEntityContext))
		actualInits->make_actual(actualInits, makeEntityContext);
		
	const FogCtorInit* ctorInit = ctorInits.find(anId);
	
	if (!ctorInit)
		ctorInits.adopt(new FogCtorInit(*anEntity, *_inits));
	else {
		FogMergeContext mergeContext(makeEntityContext);
		
		if (!actualInits->needs_merge(mergeContext, ctorInit->inits()).both_are_valid())
			ERRMSG("Conflicting construction arguments for " << viz(*this) << " and " << viz(*ctorInit));
	}
}

std::ostream& FogRawCtorInit::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	return s;
}

std::ostream& FogRawCtorInit::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	return s;
}

std::ostream& FogRawCtorInit::print_this(std::ostream& s) const {
	_name->print_this(s);
	s << '(';
	
	if (_inits)
		_inits->print_this(s);
		
	return s << ')';
}

std::ostream& FogRawCtorInit::print_viz(std::ostream& s) const {
	s << "\"ctor-init: ";
	print_this(s);
	return s << '\"';
}
