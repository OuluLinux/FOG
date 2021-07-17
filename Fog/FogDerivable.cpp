#include <Fog/FogIncludeAll.h>



























TYPEINFO_SINGLE(FogDerivable, Super)

FogDerivable::FogDerivable()
	:
	_primary_instantiation(0),
	_primary_specialisation(0),
	_referenced_entity(this),
	_is_active(false)
{}

FogDerivable::FogDerivable(FogMakerContext& makerContext, const FogLazyBool& isPure)
	:
	Super(makerContext, isPure),
	_primary_instantiation(0),
	_primary_specialisation(0),
	_referenced_entity(this),
	_is_active(false) {
	//    if (!aDeclaration.is_extern())
	//        set_utility(aScope.utility_mode());
	//    if (makerContext.has_context())
	//        add(makerContext);
}

FogDerivable::FogDerivable(FogMakeTemplateContext& makeTemplateContext, const FogLazyBool& isPure)
	:
	Super(makeTemplateContext, isPure),
	_primary_instantiation(0),
	_primary_specialisation(0),
	_referenced_entity(this),
	_is_active(false) {
	//    if (!aDeclaration.is_extern())
	//        set_utility(aScope.utility_mode());
}

FogDerivable::~FogDerivable() {}

const FogAccess& FogDerivable::access() const {
	return FogAccess::flyweight(specifier().decl_specifiers());
}

//
//    Merge the potential declarations from aDerivable into the actual declaration of this.
//
void FogDerivable::actualize(FogScopeContext& inScope, FogDerivable& aDerivable) {
	for (FogPotentialDeclarationConstListOfRefIterator p(aDerivable.potentials()); p; ++p)
		actualize(inScope, *p);
}

void FogDerivable::actualize(FogScopeContext& inScope, FogPotentialDeclaration& potentialDeclaration) {
	FogPotentialDeclarationMonitor aMonitor("Actualizing", potentialDeclaration);
	
	if (potentialDeclaration.actualize(inScope, _actual)) { //   If something got derived in
		if (&potentialDeclaration.entity() == this)         //   If it is not carried over by instantiation
			set_max_utility(potentialDeclaration.utility());
	}
}

const FogSpecifier& FogDerivable::actual_specifier() const {
	if (!done_derive())
		ERRMSG("BUG - should not access actual_declaration() before deriving for " << viz(*this));
		
	return _actual.specifier();
}

void FogDerivable::add(FogMakerContext& makerContext) {
	for (FogPotentialDeclarationConstListOfRefToConstIterator p(_potentials); p; ++p) {
		//   Same declaration may occur by using and derivation
		const FogPotentialDeclaration& potentialDeclaration = *p;
		
		if (&potentialDeclaration.specifier() == &makerContext.specifier())
			//         && (&potentialDeclaration.root_scope() == &makeContext.root_scope()))      //.bugbug need scope too
			return;
	}
	
	FogPotentialDeclaration::make_root(makerContext, *this);
	const FogSpecifier& aSpecifier = makerContext.specifier();
	
	if (!aSpecifier.is_null()) {
		const FogName& nameId = aSpecifier.name();
		PrimOstrstream s;
		nameId.print_this(s);
		scope().add_name(s.id(), *this);
	}
}

void FogDerivable::adopt(FogScopeContext& inScope, FogPotentialDeclaration& potentialDeclaration) {
	if (_actual) {
		const FogObjectSpecifier* oldObjectSpecifier = _actual.specifier().is_object_specifier();
		const FogObjectSpecifier* newObjectSpecifier = potentialDeclaration.specifier().is_object_specifier();
		
		if (oldObjectSpecifier && newObjectSpecifier && !newObjectSpecifier->decl_specifiers().is_using()) {
			FogMergeContext mergeContext(inScope);
			
			if (!oldObjectSpecifier->needs_header_merge_from(mergeContext, *newObjectSpecifier).left_is_valid())
				ERRMSG("Addition of " << viz(potentialDeclaration) << " occurs after usage of " << viz(*this));
		}
	}
	
	const FogSpecifier& newSpecifier = potentialDeclaration.specifier();
	
	for (FogPotentialDeclarationConstListOfRefIterator p1(potentials()); p1; ++p1) {
		const FogSpecifier& oldSpecifier = p1->specifier();
		
		if (&newSpecifier == &oldSpecifier) {
			//            ERRMSG("BUG - duplicate addition of " << viz(potentialDeclaration) << " to " << viz(*this) << " suppressed.");
			potentialDeclaration.annul();
			return;
		}
	}
	
	_potentials.adopt(potentialDeclaration);
	
	if (done_derive())
		actualize(inScope, potentialDeclaration);
}

void FogDerivable::create_usages() {
	Super::create_usages();
	//    if (is_specialisation())                    // Instantiations have no interface.
	//    {
	//        FogUsage& nameUsage = name_usage();
	//        FogUsage& interfaceStart = interface_usage_start();
	//        FogUsage& interfaceFinish = interface_usage_finish();
	//        primary().name_usage().add_use_by(nameUsage);
	//        primary().interface_usage_finish().add_use_by(interfaceStart);
	//    }
	//    nameUsage.add_use_by(interfaceStart);
	//    non_virtual_scope().interface_usage_start().add_use_by(interfaceStart);
	//    interfaceStart.add_use_by(interfaceFinish);
	//    interfaceFinish.add_use_by(non_virtual_scope().interface_usage_finish());
}

const FogDeclSpecifierValue& FogDerivable::decl_specifiers(FogScopeContext& inScope)
{ return specifier(inScope).decl_specifiers(); }
const FogDeclSpecifierValue& FogDerivable::decl_specifiers() const { return specifier().decl_specifiers(); }

void FogDerivable::destroy() {
	for (FogPotentialDeclarationConstListOfRefIterator p(potentials()); p; ++p)
		p->destroy();
		
	_potentials.vacate();
	_actual.destroy();
	Super::destroy();
}

bool FogDerivable::do_compile(FogCompileContext& inScope) {
	if (!Super::do_compile(inScope))
		return false;
		
	if (!_actual)
		return true;
		
	//    const FogUtility& scopeUtility = scope().utility();             -- test in validate
	//    if ((scopeUtility > utility()) && !scope().is_name_space())
	//        ERRMSG("Attempted to extend utility " << viz(scope()) << " with " << viz(*this));
	if (decl_specifiers().is_extern())
		set_utility(global_scope().utility_mode());
		
	return true;
}

bool FogDerivable::do_derive(FogDeriveContext& inScope) {
	if (done_derive())                                                  //   Premature derivation supported for using's
		return true;
		
	if (!Super::do_derive(inScope))
		return false;
		
	if (_primary_instantiation) {
		actualize(inScope, *_primary_instantiation);                    //   Merge in primary instnace.
		_actual.instantiate();                                          //   Kill its initializers/body.
	}
	
	actualize(inScope, *this);                                          //   Merge in this instance.
	
	if (is_active())
		scope().set_max_utility(utility());
		
	FogStaticScopeContext installContext(*this);
	actual_specifier().install(installContext);
	return true;
}

void FogDerivable::emit_template_prefix(FogEmitContext& emitContext) const {
	if (specifier().is_explicit_instantiation())
		emitContext.emit_space_and_text("template ");
		
	Super::emit_template_prefix(emitContext);
}

void FogDerivable::find_entities(FogEntityFinder& theFinder) { inner_scope().find_entities(theFinder); }

bool FogDerivable::find_slots(FogMetaSlotFinder& theFinder) {
	const FogMetaSlot* aSlot = find_local_slot(theFinder.id());         //   Search this class
	
	if (!aSlot)
		return inner_scope().find_slots(theFinder);
		
	theFinder.add(*this, *aSlot);
	return true;
}

//  const FogStatic& FogDerivable::get_static() const { return specifier().get_static(); }

//
//    Make the type/non-type names of this entity known as part of the scope. This occurs when the
//    first potential declaration is adopted, not when the entity is created, since many entities
//    acquire derived objects which never materialise as actual class declarations.
//
void FogDerivable::install_names() {}

FogUsage& FogDerivable::interface_usage_finish() {
	if (_referenced_entity != this)
		return _referenced_entity ? _referenced_entity->interface_usage_finish() : FogUsage::mutable_null();
		
	if (specifier().is_null())
		return FogUsage::mutable_null();
		
	//    else if (_primary_instantiation)
	//        return _primary_instantiation->interface_usage_finish();
	//    else
	//    {
	return Super::interface_usage_finish();
	//    }
}

FogUsage& FogDerivable::interface_usage_start() {
	if (_referenced_entity != this)
		return _referenced_entity ? _referenced_entity->interface_usage_start() : FogUsage::mutable_null();
		
	if (specifier().is_null())
		return FogUsage::mutable_null();
		
	return Super::interface_usage_start();
}

bool FogDerivable::is_active() const { return _is_active; }
//  bool FogDerivable::is_const() const { return specifier().has_const(); }
FogDerivable* FogDerivable::is_derivable() { return this; }
//  bool FogDerivable::is_extern() const { return specifier().has_extern(); }
//  bool FogDerivable::is_instantiation() const { return scope().is_instantiation(); }
//  bool FogDerivable::is_static() const { return get_static().is_static(); }
//  bool FogDerivable::is_volatile() const { return specifier().has_volatile(); }

//
//    Create/extend and return the derived entity initially identified as aDerivable within inScope.
//
FogDerivable* FogDerivable::make_derived(FogDeriveContext& inScope, FogDerivable* aDerivable) {
	//    It is unfortunately necessary to create the derived entity and each derived potential declaration
	//    in order to be able to evaluate meta-functions such as is_root() with a correct context.
	//    Unwanted derived potential declarations are destroyed here.
	//    Unwanted derived entities must be detected by an absence of potential declarations.
	if (!aDerivable)
		for (FogPotentialDeclarationConstListOfRefIterator p1(potentials()); p1; ++p1) {
			FogPotentialDeclaration& potentialDeclaration = *p1;
			FogRecompileMakeEntityContext explicitContext(inScope, potentialDeclaration, NO_CONTEXT);
			FogEntity* anEntity = potentialDeclaration.specifier().make_name_entity(explicitContext);
			aDerivable = anEntity ? anEntity->is_derivable() : 0;
			
			if (aDerivable)
				break;
		}
		
	if (aDerivable)
		for (FogPotentialDeclarationConstListOfRefIterator p2(potentials()); p2; ++p2)
			p2->make_derivation(inScope, *aDerivable);
			
	return aDerivable;
}

//
//    Create/extend and return a used version of this entity for use in deriveContext in response to theSpecifier.
//
FogDerivable* FogDerivable::make_used(FogDeriveContext& deriveContext, const FogObjectSpecifier& usingSpecifier) {
	FogStaticParseContext parseContext(deriveContext.dynamic_token(), usingSpecifier,
	                                   FogAccess::initial_access(), deriveContext.dynamic_scope().utility());
	FogStrongSemanticsContext theSemantics(parseContext);
	
	if (!resolve_semantics(theSemantics) || !theSemantics.is_declaration()) {
		ERRMSG("BUG -- bad resolve_semantics for " << viz(*this));
		return 0;
	}
	
	FogDeclSpecifierValue declSpecifiers(usingSpecifier.decl_specifiers());
	declSpecifiers.reset(FogDeclSpecifierValue::USING_MASK);
	FogBaseMakeSpecifierContext makeSpecifierContext(parseContext, theSemantics, declSpecifiers,
	        FogMakeSemantics::DECLARATION);
	FogNameRef theName(usingSpecifier.name().derived_clone(), FogNameRef::ADOPT);
	
	if (!theName || !theName->make_specifier(makeSpecifierContext)) {
		ERRMSG("Failed to make_specifier for " << viz(*this));
		return 0;
	}
	
	FogObjectSpecifier* objectSpecifier = makeSpecifierContext.get_object_specifier(deriveContext.dynamic_token());
	
	if (objectSpecifier) {
		objectSpecifier->set_used(specifier(deriveContext));
		objectSpecifier->set_using(usingSpecifier);
	}
	
	makeSpecifierContext.compile_specifier();
	makeSpecifierContext.compile_type_specifier();
	FogCompileMakeEntityContext compileContext(makeSpecifierContext);
	FogEntity* typeEntity = makeSpecifierContext.specifier().make_type_entity(compileContext);
	FogEntity* anEntity = makeSpecifierContext.specifier().make_name_entity(compileContext);
	FogDerivable* aDerivable = anEntity ? anEntity->is_derivable() : 0;
	
	if (!aDerivable) {
		ERRMSG("Failed to make_entity for " << viz(*this));
		return 0;
	}
	
	//    for (FogPotentialDeclarationConstListOfRefToConstIterator p(potentials()); p; ++p)
	//    {
	//        const FogPotentialDeclaration& potentialDeclaration = *p;
	//        if (potentialDeclaration.is_root())
	//            potentialDeclaration.make_usage(compileContext, *aDerivable);
	//    }
	return aDerivable;
}

const FogMetaType& FogDerivable::meta_type() const { return FogMetaType::object_type(); }

FogSpecifier& FogDerivable::mutable_specifier() {
	if (!done_derive())
		ERRMSG("BUG - should not access mutable_declaration() before deriving for " << viz(*this));
		
	return _actual.mutable_specifier();
}

FogUsage& FogDerivable::name_usage() {
	if (_referenced_entity != this)
		return _referenced_entity ? _referenced_entity->name_usage() : FogUsage::mutable_null();
		
	//    if (_primary_instantiation)
	//        return _primary_instantiation->name_usage();
	//    else
	return Super::name_usage();
}

const FogSpecifier* FogDerivable::potential_specifier() const {
	if (!done_derive())
		ERRMSG("BUG - should not access potential_specifier() before deriving for " << viz(*this));
		
	if (!_potentials.tally())
		return 0;
		
	return &_potentials[0]->specifier();
}

std::ostream& FogDerivable::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	
	if (_potentials.tally()) {
		s << indent(aDepth) << "potential declarations\n";
		_potentials.print_deep(s, aDepth + 1);
	}
	
	if (!_actual)
		;
	//    else if (&*_actual == &*_potential)
	//        s << indent(aDepth) << "actual == potential\n";
	else {
		s << indent(aDepth) << "actual declaration\n";
		_actual.print_deep(s, aDepth + 1);
	}
	
	if (_primary_instantiation)
		s << indent(aDepth) << "primary_instantiation " << *_primary_instantiation << '\n';
		
	return s;
}

std::ostream& FogDerivable::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	
	if (_potentials.tally()) {
		s << indent(aDepth) << "potential declarations\n";
		_potentials.print_members(s, aDepth + 1);
	}
	
	if (!_actual)
		;
	//    else if (&*_actual == &*_potential)
	//        s << indent(aDepth) << "actual == potential\n";
	else {
		s << indent(aDepth) << "actual declaration\n";
		_actual.print_on(s, aDepth + 1);
	}
	
	if (_primary_instantiation)
		s << indent(aDepth) << "primary_instantiation " << *_primary_instantiation << '\n';
		
	return s;
}

std::ostream& FogDerivable::print_this(std::ostream& s) const {
	//    const FogSpecifier *theSpecifier = &_actual.specifier();
	//    if (theSpecifier->is_null())
	//        theSpecifier = _potentials.tally() ? &_potentials[0]->specifier() : 0;
	//    if (theSpecifier)
	//        theSpecifier->print_named(s, &scope().global_id(), 0);
	if (!is_active())
		s << "DISABLED, ";
		
	return s << global_signature_id();
}

void FogDerivable::set_interface_file(FogTargetFile* aFile) {
	ERRMSG("BUG - should not FogDerivable::set_interface_file for " << viz(*this));
	Super::set_interface_file(aFile);
}

void FogDerivable::set_is_active(FogPotentialDeclaration& potentialDeclaration) {
	if (!_is_active) {
		_is_active = true;
		install_names();
		CONDMSG(Fog::debug_activity(), "set_is_active for " << viz(*this) << " from " << viz(potentialDeclaration));
	}
}

void FogDerivable::set_primary_instantiation(FogDerivable& aDerivable) {
	if (_primary_instantiation && (_primary_instantiation != &aDerivable))
		ERRMSG("BUG -- should not re-invoke set_primary_instantiation for instantiation " << viz(*this));
		
	_primary_instantiation = &aDerivable;
}

void FogDerivable::set_primary_specialisation(FogDerivable& aDerivable) {
	if (_primary_specialisation && (_primary_specialisation != &aDerivable))
		ERRMSG("BUG -- should not re-invoke set_primary_specialisation for specialisation " << viz(*this));
		
	_primary_specialisation = &aDerivable;
}

const FogObjectSpecifier& FogDerivable::specifier() const {
	if (!done_derive()) {
		ERRMSG("BUG - premature invocation of specifier() for " << viz(*this));
		return FogObjectSpecifier::immutable_null();
	}
	
	const FogObjectSpecifier* objectSpecifier = _actual.specifier().is_object_specifier();
	
	if (objectSpecifier)
		return *objectSpecifier;
		
	//    if (!theSpecifier->is_null())
	//        ERRMSG("BUG -- FogObjectSpecifier expected for " << viz(*this));
	return FogObjectSpecifier::immutable_null();
}

const FogObjectSpecifier& FogDerivable::specifier(FogScopeContext& inScope) {
	if (!done_derive() && !do_derive(inScope)) {
		ERRMSG("BUG - failed to do_derive FogDerivable::specifier for " << viz(*this));
		return FogObjectSpecifier::immutable_null();
	}
	
	return specifier();
}

bool FogDerivable::validate() const {
	if (!Super::validate())
		return false;
		
	if (scope().is_instantiation() && !_primary_instantiation)
		ERRMSG("Template instantiation should not contain specialisation " << viz(*this) << ".");
		
	return true;
}
