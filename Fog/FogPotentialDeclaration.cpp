#include <Fog/FogIncludeAll.h>











class FogRootPotentialDeclaration : public FogPotentialDeclaration {
	NEEDS_A_FRIEND_FOR_GNUC
	typedef FogPotentialDeclaration Super;
	typedef FogRootPotentialDeclaration This;
	TYPEDECL_SINGLE(This, Super)
private:
	FogSpecifierRefToConst _specifier;                  //   Specifier providing the information.
	const FogUtility& _utility;                         //   Status of declaration.
	FogTargetFile* _target_file;                        //   Pre-determined destination of declaration.
private:
	FogRootPotentialDeclaration(const This&);
	This& operator=(const This&);
private:
	virtual ~FogRootPotentialDeclaration() {}
public:
	FogRootPotentialDeclaration(FogMakerContext& makerContext, FogDerivable& dynamicScope);
	virtual void destroy();
	virtual const FogLazyBool& is_boundary() const;
	virtual const FogLazyBool& is_pure() const;
	virtual bool is_root() const { return true; }
	virtual bool is_root(FogScopeContext& inScope) const;
	virtual const FogScope& root_scope() const { return inner_scope(); }
	virtual const FogSpecifier& specifier() const { return *_specifier; }
	virtual FogTargetFile* target_file() const { return _target_file; }
	virtual const FogUtility& utility() const { return _utility; }
};

class FogDerivedPotentialDeclaration : public FogPotentialDeclaration {
	NEEDS_A_FRIEND_FOR_GNUC
	typedef FogPotentialDeclaration Super;
	typedef FogDerivedPotentialDeclaration This;
	TYPEDECL_SINGLE(This, Super)
private:
	const FogPotentialDeclaration& _base;
private:
	FogDerivedPotentialDeclaration(const This&);
	This& operator=(const This&);
private:
	virtual ~FogDerivedPotentialDeclaration() {}
public:
	FogDerivedPotentialDeclaration(FogDerivable& anEntity, const FogPotentialDeclaration& potentialDeclaration)
		: Super(anEntity), _base(potentialDeclaration) {}
	virtual const FogLazyBool& is_boundary() const;
	virtual const FogLazyBool& is_pure() const;
	virtual bool is_root() const { return false; }
	virtual bool is_root(FogScopeContext& inScope) const { return false; }
	virtual const FogScope& root_scope() const { return _base.root_scope(); }
	virtual const FogSpecifier& specifier() const { return _base.specifier(); }
	//    virtual FogTargetFile *target_file() const { return _base.target_file(); }
	virtual FogTargetFile* target_file() const { return 0; }
	virtual const FogUtility& utility() const { return FogUtility::pool_utility(); }
};

TYPEINFO_SINGLE(FogPotentialDeclaration, Super)
TYPEINFO_SINGLE(FogRootPotentialDeclaration, Super)
TYPEINFO_SINGLE(FogDerivedPotentialDeclaration, Super)

PRIMREF_NULL_CLAS(FogPotentialDeclaration)
virtual const FogLazyBool& is_boundary() const { return FogLazyBool::make_false(); }
virtual const FogLazyBool& is_pure() const { return FogLazyBool::make_false(); }
virtual bool is_root() const { return false; }
virtual bool is_root(FogScopeContext& inScope) const { return false; }
virtual const FogScope& root_scope() const { return FogScope::immutable_null(); }
virtual const FogSpecifier& specifier() const { return FogSpecifier::immutable_null(); }
virtual FogTargetFile* target_file() const { return 0; }
virtual const FogUtility& utility() const { return FogUtility::invalid_utility(); }
};
PRIMREF_DERIVED_NULL_IMPL(FogPotentialDeclaration)

PRIMREFS_IMPL(FogPotentialDeclaration)
PRIMLISTOFREFS_IMPL(FogPotentialDeclaration)
TMPL_HACK_FIX_DO(FogPotentialDeclaration)

inline FogPotentialDeclaration::FogPotentialDeclaration()
	:
	_entity(FogVariable::mutable_null()),
	_is_active(FogLazyBool::make_false())
{}


inline FogPotentialDeclaration::FogPotentialDeclaration(FogDerivable& anEntity)
	:
	_entity(anEntity),
	_is_active(FogLazyBool::make_undecided())
{}

inline FogPotentialDeclaration::~FogPotentialDeclaration() {}

bool FogPotentialDeclaration::actualize(FogScopeContext& inScope, FogActualDeclaration& actualDeclaration) {
	const FogLazyBool& lazyBool = derive_at(inScope);
	
	if (!lazyBool.is_true()) {              //   If something could be derived in
		if (lazyBool.is_undecided())
			ERRMSG("BUG -- use of undecided derive_at for " << viz(*this));
			
		return false;
	}
	
	const FogSpecifier& aSpecifier = specifier();
	FogPotentialDeclarationScopeContext potentialDeclarationContext(inScope, *this, is_root(inScope));
	FogSpecifierRefToConst incrementalSpecifier(aSpecifier);
	aSpecifier.make_actual(incrementalSpecifier, potentialDeclarationContext);    //   Sort out the embedded $'s
	FogMergeContext mergeContext(inScope);
	actualDeclaration.merge(mergeContext, *incrementalSpecifier);
	FogSubspaceParseContext parseContext(potentialDeclarationContext, *this);
	aSpecifier.compile_subspace(parseContext, actualDeclaration.mutable_specifier());
	return true;
}

//
//    Return true if this potential declaration will be concrete if inScope is.
//    Return false if this potential declaration mandates purity through a root pure-virtual.
//
const FogLazyBool& FogPotentialDeclaration::can_be_concrete(FogScopeContext& inScope) {
	FogResolutionScopeContext weakContext(inScope, FogScopeContext::RESOLVE_LAZY);
	is_lazy_active(weakContext);
	
	if (_is_active->is_false())
		return FogLazyBool::make_undecided();
		
	FogPotentialDeclarationScopeContext potentialDeclarationContext(weakContext, *this, is_root(inScope));
	return specifier().can_be_concrete(potentialDeclarationContext);
}

FogToken* FogPotentialDeclaration::clone() const {
	DIAMSG("INVESTIGATE - did not expect to invoke FogPotentialDeclaration::clone()");
	share(); return (FogToken*)this;
}

void FogPotentialDeclaration::find_entities(FogEntityFinder& theFinder) { _entity.find_entities(theFinder); }

//  bool FogPotentialDeclaration::find_slots(FogMetaSlotFinder& theFinder)
//  {
//      const FogMetaSlot *aSlot = find_local_slot(theFinder.id()); // Search this class
//      if (!aSlot)
//          return _entity.find_slots(theFinder); // NO, NO, NO returns functions for wrong this
//      theFinder.add(*this, *aSlot);
//      return true;
//  }

bool FogPotentialDeclaration::get_identifier(PrimIdHandle& returnId, FogScopeContext& inScope) {
	return _entity.get_identifier(returnId, inScope);
}

FogScope& FogPotentialDeclaration::inner_scope() { return _entity.inner_scope(); }

bool FogPotentialDeclaration::is_active(FogScopeContext& inScope) {
	is_lazy_active(inScope);
	return is_active();
}

bool FogPotentialDeclaration::is_active() const {
	if (_is_active->is_false())
		return false;
		
	if (_is_active->is_undecided())
		ERRMSG("BUG -- should not use undecided activity of " << viz(*this));
		
	return true;
}

const FogLazyBool& FogPotentialDeclaration::is_lazy_active(FogScopeContext& inScope) {
	if (_is_active->is_undecided()) {
		FogPotentialDeclarationScopeContext potentialDeclarationContext(inScope, *this, is_root(inScope));
		const FogLazyBool& isActive = specifier().derive_at(potentialDeclarationContext);
		
		if (!isActive.is_undecided())
			set_is_active(isActive);
	}
	
	return *_is_active;
}

const FogLazyBool& FogPotentialDeclaration::is_leaf() const { return inner_scope().is_leaf(); }
FogPotentialDeclaration* FogPotentialDeclaration::is_potential_declaration() { return this; }

void FogPotentialDeclaration::make_derivation(FogDeriveContext& inScope, FogDerivable& aDerivable) const {
	FogPotentialDeclaration* derivedDeclaration = new FogDerivedPotentialDeclaration(aDerivable, *this);
	
	if (derivedDeclaration) {
		FogResolutionScopeContext lazyContext(inScope, FogScopeContext::RESOLVE_LAZY);
		derivedDeclaration->derive_at(lazyContext);
		aDerivable.adopt(inScope, *derivedDeclaration);
	}
}

void FogPotentialDeclaration::make_root(FogMakerContext& makerContext, FogDerivable& aDerivable) {
	FogPotentialDeclaration* rootDeclaration = new FogRootPotentialDeclaration(makerContext, aDerivable);
	
	if (rootDeclaration) {
		FogResolutionScopeContext lazyContext(makerContext, FogScopeContext::RESOLVE_LAZY);
		rootDeclaration->derive_at(lazyContext);
		aDerivable.adopt(makerContext, *rootDeclaration);
	}
}

//  void FogPotentialDeclaration::make_usage(FogDeriveContext& inScope, FogDerivable& aDerivable) const
//  {
//    FogSpecifierRefToConst strippedSpecifier(_specifier->stripped_clone());
//    FogPotentialDeclaration *usageDeclaration = new FogPotentialDeclaration(aDerivable, aDerivable, *this, *strippedSpecifier);
//    if (usageDeclaration)
//        aDerivable.adopt(inScope, *usageDeclaration);
//  }

void FogPotentialDeclaration::merge(FogMergeContext& mergeContext, const FogToken& aToken) {
	bool sameClass = &dynamic_type_info() == &aToken.dynamic_type_info();
	
	if (!sameClass) Super::merge(mergeContext, aToken);
}

const FogMetaType& FogPotentialDeclaration::meta_type() const { return FogMetaType::object_statement_type(); }

const FogMerge& FogPotentialDeclaration::needs_merge(FogMergeContext& mergeContext, const FogToken& aToken) const {
	//   Right-hand side of algorithm in base class.
	bool sameClass = &dynamic_type_info() == &aToken.dynamic_type_info();
	return impossible_merge();
}

std::ostream& FogPotentialDeclaration::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	const FogSpecifier& aSpecifier = specifier();
	
	if (!aSpecifier.is_null())
		aSpecifier.print_deep(s, aDepth + 1);
		
	return s;
}

std::ostream& FogPotentialDeclaration::print_long_id(std::ostream& s) const {
	return _entity.print_long_id(s);
}

std::ostream& FogPotentialDeclaration::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	const FogSpecifier& aSpecifier = specifier();
	
	if (!aSpecifier.is_null())
		aSpecifier.print_on(s, aDepth + 1);
		
	return s;
}

char FogPotentialDeclaration::print_named(std::ostream& s, const PrimId* scopeId, char tailChar) const {
	return specifier().print_named(s, scopeId, tailChar);
}

std::ostream& FogPotentialDeclaration::print_this(std::ostream& s) const {
	s << *_is_active << ", ";
	const FogSpecifier& aSpecifier = specifier();
	aSpecifier.print_named(s, &inner_scope().global_signature_id(), ' ');
	
	if (aSpecifier.decl_specifiers().is_pure_virtual())
		s << " = 0";
		
	return s;
}

std::ostream& FogPotentialDeclaration::print_viz(std::ostream& s) const {
	s << "\"potential-declaration: ";
	const FogSpecifier& aSpecifier = specifier();
	aSpecifier.print_named(s, &inner_scope().global_signature_id(), ' ');
	
	if (aSpecifier.decl_specifiers().is_pure_virtual())
		s << " = 0";
		
	return s << '\"';
}

void FogPotentialDeclaration::set_is_active(const FogLazyBool& isActive) {
	if (!_is_active->is_undecided()) {
		ERRMSG("BUG -- should not re-FogPotentialDeclaration::set_is_active for " << viz(*this));
		return;
	}
	
	_is_active = isActive;
	
	if (!Fog::debug_activity())
		;
	else if (_is_active->is_true())
		DIAMSG("set_is_active for " << viz(*this));
	else if (_is_active->is_undecided())
		DIAMSG("set_is_dormant for " << viz(*this));
	else
		DIAMSG("set_is_passive for " << viz(*this));
		
	if (_is_active->is_true())
		_entity.set_is_active(*this);
}

const FogLazyBool& FogDerivedPotentialDeclaration::is_boundary() const {
	const FogLazyBool& isActive = is_lazy_active();
	
	if (isActive.is_false())
		return isActive;
		
	const FogLazyBool& isPure = inner_scope().is_pure();
	
	if (isPure.is_true())
		return FogLazyBool::make_false();
		
	if (isPure.is_undecided())
		return isActive;
		
	return _base.is_pure();
	//      return entity().is_boundary();
}

const FogLazyBool& FogDerivedPotentialDeclaration::is_pure() const {
	//      const FogLazyBool& isActive = is_lazy_active();
	//      if (isActive.is_false())
	//          return isActive;
	//      const FogLazyBool& isPure = inner_scope().is_pure();
	//      if (isPure.is_false())
	//          return FogLazyBool::make_false();
	//  //    if (isPure.is_undecided())
	//  //	    ERRMSG("BUG -- use of undecided is_pure for " << viz(*this));
	//      return _base.is_pure();
	return entity().is_pure();
}

//  ---------------------------------------------------------------------------------------------------------------------

FogRootPotentialDeclaration::FogRootPotentialDeclaration(FogMakerContext& makerContext, FogDerivable& dynamicScope)
	:
	Super(dynamicScope),
	_specifier(makerContext.specifier()),
	_utility(makerContext.utility()),
	_target_file(makerContext.target_file()) {
	if (_specifier->decl_specifiers().is_pure_virtual())
		entity().set_is_pure(FogLazyBool::make_true());
}

void FogRootPotentialDeclaration::destroy() {
	_specifier.reset();
	Super::destroy();
}

const FogLazyBool& FogRootPotentialDeclaration::is_boundary() const {
	const FogLazyBool& isActive = is_lazy_active();
	
	if (isActive.is_false())
		return isActive;
		
	if (specifier().decl_specifiers().is_pure_virtual())
		return FogLazyBool::make_false();
		
	//      if (isActive.is_undecided())
	//          return isActive;
	const FogLazyBool& isPure = inner_scope().is_pure();
	
	if (isPure.is_true())
		return FogLazyBool::make_false();
		
	if (isPure.is_undecided())
		return isActive;
		
	return FogLazyBool::make_true();
	//      return entity().is_boundary();
}

const FogLazyBool& FogRootPotentialDeclaration::is_pure() const {
	//      const FogLazyBool& isActive = is_lazy_active();
	//      if (isActive.is_false())
	//          return isActive;
	//      if (specifier().decl_specifiers().is_pure_virtual())
	//          return FogLazyBool::make_true();
	//      return FogLazyBool::make_false();
	return entity().is_pure();
}

//
//    Return true if the derivation of this PD inScope is a root PD.
//
bool FogRootPotentialDeclaration::is_root(FogScopeContext& inScope) const {
	const FogLazyBool& isActive = is_lazy_active();
	
	if (isActive.is_false()) {
		ERRMSG("INVESTIGATE -- is_passive in is_root for " << viz(*this));
		return false;
	}
	
	//      if (isActive.is_undecided())
	//  	    ERRMSG("BUG -- use of undecided is_active for " << viz(*this));
	const FogScope& derivedScope = inScope.dynamic_scope();
	const FogScope& rootScope = inner_scope();
	return &rootScope == &derivedScope;
}
