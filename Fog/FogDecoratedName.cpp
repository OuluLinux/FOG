#include <Fog/FogIncludeAll.h>









TYPEINFO_SINGLE(FogDecoratedName, Super)
PRIMREF_NULL_CLAS(FogDecoratedName)
FOGTOKEN_NULL_LEAF
virtual bool emit(FogEmitContext& emitContext) const { return false; }
};
PRIMREF_DERIVED_NULL_IMPL(FogDecoratedName)
PRIMREFS_IMPL(FogDecoratedName)
TMPL_HACK_FIX_DO(FogDecoratedName)

FogDecoratedName::~FogDecoratedName() {}

void FogDecoratedName::create_usage(FogUsageContext& usageContext) const {
	Super::create_usage(usageContext);
	_name->create_usage(usageContext);
}

size_t FogDecoratedName::executable_tokens() const { return _name->executable_tokens(); }

void FogDecoratedName::install(FogInstallContext& installContext) const {
	Super::install(installContext);
	_name->install(installContext);
}

bool FogDecoratedName::is_actual(const FogScopeContext& scopeContext) const {
	return Super::is_actual(scopeContext) && _name->is_actual(scopeContext);
}

void FogDecoratedName::make_actual_from(FogMakeActualContext& makeActualContext) {
	Super::make_actual_from(makeActualContext);
	_name->make_actual(_name.to_const(), makeActualContext);
}

void FogDecoratedName::merge_from(FogMergeContext& mergeContext, const This& thatDecl) {
	Super::merge_from(mergeContext, thatDecl);
	thatDecl._name->merge_into(mergeContext, _name.to_const());
}

const FogMerge& FogDecoratedName::needs_merge_from(FogMergeContext& mergeContext, const This& thatDecl) const {
	FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, thatDecl));
	needsMerge |= _name->needs_merge(mergeContext, *thatDecl._name);
	return needsMerge;
}

std::ostream& FogDecoratedName::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	
	if (_name)
		_name->print_deep(s, aDepth);
		
	return s;
}

std::ostream& FogDecoratedName::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	
	if (_name)
		_name->print_on(s, aDepth);
		
	return s;
}

char FogDecoratedName::print_named(std::ostream& s, const PrimId* scopeId, char tailChar) const {
	if (!is_null())
		return _name->print_named(s, scopeId, tailChar);
	else
		return tailChar;
}

void FogDecoratedName::set_decorated_destructor_name(FogNameRef& aName) {
	_name->set_destructor_name(_name);
	aName = *this;
}

void FogDecoratedName::set_line(FogLine& aLine) {
	Super::set_line(aLine);
	
	if (_name)          //   null for using/interface etc.
		_name->set_line(aLine);
}
