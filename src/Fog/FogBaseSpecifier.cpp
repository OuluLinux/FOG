#include <Fog/FogIncludeAll.h>












TYPEINFO_SINGLE(FogBaseSpecifier, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogBaseSpecifier)
PRIMREFS_IMPL(FogBaseSpecifier)
PRIMLISTOFREFS_IMPL(FogBaseSpecifier)
FOGTOKEN_LEAF_IMPL(FogBaseSpecifier)
TMPL_HACK_FIX_DO(FogBaseSpecifier)

FogBaseSpecifier::FogBaseSpecifier() {}

FogBaseSpecifier::FogBaseSpecifier(FogName& aName)
		:
		Super(aName) {}
		
FogBaseSpecifier::~FogBaseSpecifier() {}

void FogBaseSpecifier::make_base(FogMakeEntityContext& makeEntityContext, FogScope& inScope) const {
	FogInScopeContext baseContext(makeEntityContext, IN_ANY_NOT_THIS_SCOPE);
	FogEntity *anEntity = find_type_in(baseContext);
	
	if (!anEntity) {
		ERRMSG("Failed to find an entity to resolve " << viz(name()) << " within " << viz(baseContext));
		return;
	}
	
	FogScope *aScope = anEntity->is_scope();
	
	if (!aScope) {
		ERRMSG("Expected " << viz(name()) << " to resolve to a scope within " << viz(baseContext));
		return;
	}
	
	const FogVirtual& aVirtual = FogVirtual::flyweight(decl_specifiers());
	
	FogBaseClass *baseClass = new FogBaseClass(*aScope, access(), aVirtual);
	
	if (baseClass) {
		inScope.add_base(*baseClass);
		baseClass->annul();
	}
}

const FogMetaType& FogBaseSpecifier::meta_type() const {
	return FogMetaType::base_specifier_type();
}

std::ostream& FogBaseSpecifier::print_this(std::ostream& s) const {
	const FogVirtual& aVirtual = FogVirtual::flyweight(decl_specifiers());
	s << access().str() << ' ' << aVirtual.str() << ' ';
	Super::print_this(s);
	return s;
}

std::ostream& FogBaseSpecifier::print_viz(std::ostream& s) const {
	s << "\"base-specifier: ";
	Super::print_this(s);
	return s << '\"';
}

void FogBaseSpecifier::set_access(const FogAccess& anAccess) {
	set_decl_specifier(anAccess);
}

void FogBaseSpecifier::set_virtual(const FogVirtual& aVirtual) {
	set_decl_specifier(aVirtual);
}
