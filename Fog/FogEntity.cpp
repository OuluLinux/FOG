#include <Fog/FogIncludeAll.h>



TYPEINFO_SINGLE(FogEntity, Super)
PRIMREF_NULL_CLAS(FogEntity)
virtual std::ostream& print_viz(std::ostream& s) const {
	return s << "\"null-entity:\"";
}
};

PRIMREF_DERIVED_NULL_IMPL(FogEntity)
PRIMREFS_IMPL(FogEntity)
PRIMLISTOFREFS_IMPL(FogEntity)
PRIMMAPOFREFS_IMPL(FogEntity)
TMPL_HACK_FIX_DO(FogEntity)

TMPL_HACK_FIX_TEMPLATE_EXPLICIT(FogEntitySetOfRefElement)
PRIMMAPOFREFS_IMPL(FogEntitySetOfRefElement)
PRIMSETELEMENT_IMPL(FogEntitySetOfRefElement)
TMPL_HACK_FIX_DO_TEMPLATE(FogEntitySetOfRefElement)

TMPL_HACK_FIX_TEMPLATE_EXPLICIT(FogEntitySetOfListOfRefElement)
PRIMMAPOFREFS_IMPL(FogEntitySetOfListOfRefElement)
PRIMSETELEMENT_IMPL(FogEntitySetOfListOfRefElement)
TMPL_HACK_FIX_DO_TEMPLATE(FogEntitySetOfListOfRefElement)

FogEntity::FogEntity()
		:
		_global_scope(FogRoot::null()),
		_scope(FogRoot::null()),
		_is_boundary(FogLazyBool::make_false()),
		_is_pure(FogLazyBool::make_false()),
		_done_compile(false),
		_done_derive(false) {}
		
//
//  	Construct the root entity named anId in the root scope aRoot.
//
FogEntity::FogEntity(FogScope& aScope, const PrimId& shortId, const PrimId& longId)
		:
		Super(shortId, longId),
		_global_scope(&aScope == this ? *(FogRoot *)this : aScope.global_scope()), //   Avoid recursion on global scope
		_scope(aScope),
		_is_boundary(FogLazyBool::make_undecided()),
		_is_pure(FogLazyBool::make_undecided()),
		_done_compile(false),
		_done_derive(false) {}
		
//
//  	Construct an entity named anId in aScope.
//
FogEntity::FogEntity(FogMakerContext& makerContext, const FogLazyBool& isPure)
		:
		Super(makerContext),
		_global_scope(&makerContext.dynamic_scope() == this ? *(FogRoot *)this : makerContext.dynamic_scope().global_scope()),
		_scope(makerContext.dynamic_scope()),
		_is_boundary(isPure),
		_is_pure(isPure),
		_done_compile(false),
		_done_derive(false) {}
		
//
//  	Construct an entity named anId in aScope.
//
FogEntity::FogEntity(FogMakeTemplateContext& makeTemplateContext, const FogLazyBool& isPure)
		:
		Super(makeTemplateContext),
		_global_scope(makeTemplateContext.dynamic_scope().global_scope()),
		_scope(makeTemplateContext.dynamic_scope()),
		_is_boundary(isPure),
		_is_pure(isPure),
		_done_compile(false),
		_done_derive(false) {}
		
FogEntity::~FogEntity() {}

const FogAccess& FogEntity::access() const {
	return FogAccess::invalid_access();
}

void FogEntity::add_type(FogEntity& aType) {
	ERRMSG("BUG -- should not attempt to add_type " << viz(aType) << " to " << viz(*this));
}

//
//  	Add a dependent usage of this entity by usageContext.
//
void FogEntity::add_use_by(FogUsageContext& usageContext) {
	add_use_by(usageContext.entity(), usageContext.of_use_by());
}

void FogEntity::add_use_by(FogEntity& anEntity, const FogOfUseBy& ofUseBy) {
//  	if (is_null())							// Null entities signal null behaviour.
//  		return;
	FogEntity *ofEntity = this;
	FogScope *ofScope = ofEntity->outer_scope();
	
	
	if (!ofScope)       //   Use of the global scope is degenerate and certainly
		return;        //    does not merit an error message.
		
	for (; ofScope; ofEntity = ofScope, ofScope = ofScope->outer_scope()) {
		VERBOSE(std::cout << "NEW OF ENTITY " << ofEntity << "\n";
			std::strstream str;
			ofEntity->name_scope().print_viz(str);
			std::cout << "NAME SCOPE\n" << str.str() << std::endl;);
		
		FogScope& ofNameScope = ofScope->name_scope();
		FogEntity *byEntity = &anEntity;
		FogScope *byScope = byEntity->outer_scope();
		const FogOfUseBy *useBy = &ofUseBy;
		
		for (; byScope; byEntity = byScope, byScope = byScope->outer_scope(), useBy = &useBy->by_head()) {
			VERBOSE(std::cout << "NEW BY ... " << byEntity << "\n";);
			
			FogScope& byNameScope = byScope->name_scope();
//  			if ((&byNameScope == &ofNameScope) || useBy->is_by_implementation() || ofEntity->is_typedef())

			if ((&byNameScope == &ofNameScope) || useBy->is_by_implementation()) {
				if (ofEntity != byEntity) {  //   == for a member that mentions its scope.
					FogUsage& ofUsage = useBy->is_of_tail() ? ofEntity->interface_usage_finish() :
										ofEntity->name_usage();
					FogUsage& byUsage = useBy->is_by_head() ? byEntity->interface_usage_start() :
										useBy->is_by_name() ? byEntity->name_usage() :
										byEntity->implementation_usage();
					ofUsage.add_use_by(byUsage);
				}
				
				VERBOSE(std::cout << "RET\n";)
				
				return;
			}
		}
	}
	
	ERRMSG("BUG - failed to locate the "  << ofUseBy << " usage of " << viz(*this) << " by " << viz(anEntity));
}

const FogMetaSlot *FogEntity::adopt_slot(const FogMetaSlot *aSlot) {
	if (aSlot) {
		_slots.add_or_replace(*aSlot);   //   Slots can be replaced.
		aSlot->annul();
	}
	
	return aSlot;
}

const FogInline& FogEntity::default_inline() const {
	return _global_scope.default_inline();
}

//
//  	Eliminate all references so that destruction is complete and static destruction well behaved.
//
void FogEntity::destroy() {
	Super::destroy();
	_slots.vacate();
}

bool FogEntity::do_compile(FogCompileContext& inScope) {
	if (!_done_derive) {
		ERRMSG("BUG - should not do_compile without do_derive " << viz(*this));
		return false;
	}
	
	if (_done_compile) {
		ERRMSG("BUG - should not re-do_compile " << viz(*this));
		return false;
	}
	
	_done_compile = true;
	
	return true;
}

bool FogEntity::do_derive(FogDeriveContext& inScope) {
	if (_done_derive) {
		ERRMSG("BUG - should not re-do_derive " << viz(*this));
		return false;
	}
	
	_done_derive = true;
	
	return true;
}

bool FogEntity::emit(FogEmitContext& emitContext) const {
	return emitContext.emit_id(short_id(), *this);
}

void FogEntity::emit_forward_reference(FogStream& s) {
	ERRMSG("BUG -- should not attempt to emit_forward_reference to " << viz(*this));
}

void FogEntity::emit_friend_interface(FogStream& s) {
	ERRMSG("BUG -- should not attempt to declare friend interface of " << viz(*this));
}

void FogEntity::emit_template_prefix(FogEmitContext& emitContext) const {
	_scope.emit_template_prefix(emitContext);
	emitContext.start();
}

void FogEntity::find_entities_in(FogScopeContext& inScope, FogEntityFinding& theFinding) const {
	FogEntityFinder theFinder(theFinding, short_id(), inScope.in_scope());
	inScope.find_entities(theFinder);
}

const FogMetaSlot *FogEntity::find_local_slot(const PrimId& anId) const {
	const FogMetaSlot *aSlot = _slots.find(anId);
	return aSlot ? aSlot : Super::find_local_slot(anId);
}

FogEntity *FogEntity::find_template(FogMakeTemplateContext& makeTemplateContext) {
	ERRMSG("BUG - should not find_template " << viz(makeTemplateContext) << " of " << viz(*this));
	return this;
}

bool FogEntity::get_character(PrimIdHandle& returnId, FogScopeContext& inScope) {
	returnId = short_id();
	return true;
}

bool FogEntity::get_dyadic(FogTokenRef& returnValue, FogScopeContext& inScope,
		FogTokenType::TokenType dyadicOp, const FogToken& rightValue) const {
	switch (dyadicOp) {
	
		case '.': {
			const FogName *rightName = rightValue.is_name();
			
			if (!rightName) {
				ERRMSG("Expected name as member name for " << viz(*this) << " in " << viz(inScope));
				returnValue = FogFailure::make();
				return false;
			}
			
			FogNestedScopeContext scopeContext(inScope, mutate());      //  .bugbug cast
			
			return rightName->get_meta_entity_in(returnValue, scopeContext);
		}
		
		default:
			return Super::get_dyadic(returnValue, inScope, dyadicOp, rightValue);
	}
}

bool FogEntity::get_identifier(PrimIdHandle& returnId, FogScopeContext& inScope) {
	returnId = short_id();
	return true;
}

bool FogEntity::get_string(PrimIdHandle& returnId, FogScopeContext& inScope) {
	returnId = short_id();
	return true;
}

FogRoot& FogEntity::global_scope() {
	return _global_scope;
}

FogTargetFile *FogEntity::implementation_file() {
	FogTargetFile *implementationFile = Super::implementation_file();
	return implementationFile ? implementationFile : _scope.implementation_file();
}

//
//  	Return the scope of this entity, which is this for a scope, or outer_scope for a non-scope.
//
FogScope& FogEntity::inner_scope() {
	return _scope;
}

FogEntity *FogEntity::instantiate(FogScopeContext& inScope, InstantiateOption ifRequired) {
	ERRMSG("BUG - should not instantiate " << viz(*this));
	return 0;
}

void FogEntity::instantiate_into(FogEntity& instantiatingEntity) {}

FogTargetFile *FogEntity::interface_file_sink() {
	FogTargetFile *interfaceFile = Super::interface_file_sink();
	
	if (interfaceFile)
		return interfaceFile;
		
	if (&_scope != this)
		return _scope.interface_file_sink();
		
	return raw_interface_file();
}

FogTargetFile *FogEntity::interface_file_source() {
	FogTargetFile *interfaceFile = Super::interface_file_source();
	
	if (interfaceFile)
		return interfaceFile;
		
	if (&_scope != this)
		return _scope.interface_file_source();
		
	return raw_interface_file();
}

//
//  	Return true unless this entity has been disabled.
//
bool FogEntity::is_active() const {
	ERRMSG("BUG -- unexpected FogEntity::is_active() for " << viz(*this));
	return true;
}

const FogLazyBool& FogEntity::is_boundary() const {
	if (_is_boundary->is_undecided()) {
		if (_scope.is_pure().is_true())
			return FogLazyBool::make_false();
	}
	
	return *_is_boundary;
}

FogEntity *FogEntity::is_entity() {
	return this;
}

bool FogEntity::is_global_scope() const {
	return false;
}

bool FogEntity::is_instantiation() const {
	return false;
}

bool FogEntity::is_name_scope() const {
	return false;    //   See name_scope() for description.
}

bool FogEntity::is_name_space() const {
	return false;    //   See name_space() for description.
}

bool FogEntity::is_read_only() const {
	return false;
}

bool FogEntity::is_templated() const {
	return false;
}

//
//  	Return true if the name of this entity is a type-name, arther than an object name.
//
bool FogEntity::is_type() const {
	return false;
}

bool FogEntity::make_specifier(FogMakeSpecifierContext& makeSpecifierContext) {
	return makeSpecifierContext.make_specifier(FogIdentifier::make(short_id())) != 0;
}

const FogMetaType& FogEntity::meta_type() const {
	return FogMetaType::entity_type();
}

//
//  	Return the scope in which the name of a child entity should be emitted.
//
//  	(That is a class/struct/union or namespace or extern "C", but not filespace).
//
FogScope& FogEntity::name_emit_scope() {
	return _scope.name_emit_scope();
}

//
//  	Return the scope in which the implementation of a child entity should be emitted.
//
//  	(That is a namespace or extern "C", but not class/struct/union or filespace).
//
FogScope& FogEntity::name_emit_transient_scope() {
	return _scope.name_emit_transient_scope();
}

//
//  	Return the file name in which the name of this entity is declared.
//  	The default implementation just returns the interface. Derived classes may return 0 for entities that
//  	can be forward declared, need no declaration or are generated pre-declared (templates)
//
FogTargetFile *FogEntity::name_file() {
	return interface_file_source();
}

//
//  	Return the scope in which the name of a child entity should be defined.
//
//  	(That is a class/struct/union or namespace, but not extern "C", or filespace).
//
FogScope& FogEntity::name_scope() {
	return _scope.name_scope();
}

//
//  	Return the name-space in which the name of a child entity should be defined.
//
//  	(That is a namespace, but not class/struct/union, extern "C", or filespace).
//
FogScope& FogEntity::name_space() {
	return _scope.name_space();
}

//
//  	Return the specialisation of this for makeContext.
//
FogEntity *FogEntity::new_template(FogMakeTemplateContext& makeTemplateContext) {
	ERRMSG("BUG - should not new_template of " << viz(*this));
	return 0;
}

//
//  	Return the scope that contains this entity. Returns 0 if the root scope.
//
//  FogScope *FogEntity::outer_scope() { return &_scope != &_global_scope ? &_scope : 0; }

std::ostream& FogEntity::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	
	if (_slots.tally()) {
		s << indent(aDepth) << "slots\n";
		_slots.print_depth(s, aDepth+1);
	}
	
	return s;
}

std::ostream& FogEntity::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	
	if (_slots.tally()) {
		s << indent(aDepth) << "slots\n";
		_slots.print_members(s, aDepth+1);
	}
	
	return s;
}

char FogEntity::print_named(std::ostream& s, const PrimId *scopeId, char tailChar) const {
	if (*scopeId)
		tailChar = FogStream::space_and_emit(s, tailChar, scopeId->str());
	else
		tailChar = FogStream::space(s, tailChar, 'a');
		
	tailChar = FogStream::space_and_emit(s, tailChar, local_id());
	
	return tailChar;
}

FogReader& FogEntity::reader() {
	return _global_scope.reader();
}

bool FogEntity::resolve_semantics(FogSemanticsContext& theSemantics) const {
	const FogMetaType& metaType = meta_type();
	theSemantics.assign(metaType.is_semantics());
	return true;
}

void FogEntity::set_access(const FogAccess& anAccess) {
	ERRMSG("BUG - should not FogEntity::set_access for " << viz(*this));
}

void FogEntity::set_implementation(FogCode& someCode) {
	ERRMSG("BUG - should not FogEntity::set_implementation for " << viz(*this));
}

void FogEntity::set_initializer(const FogExpr& anExpr) {
	ERRMSG("BUG - should not FogEntity::set_initializer for " << viz(*this));
}

void FogEntity::set_interface(FogCode& someCode) {
	ERRMSG("BUG - should not FogEntity::set_interface for " << viz(*this));
}

void FogEntity::set_is_boundary(const FogLazyBool& isBoundary) {
	if (!_is_boundary->is_undecided()) {
		if (*_is_boundary != isBoundary)
			ERRMSG("BUG - should not set_is_boundary(" << isBoundary << ") for " << *_is_boundary << " " << viz(*this));
	}
	
	else {
		_is_boundary = isBoundary;
		CONDMSG(Fog::debug_purity(), "set_is_boundary(" << isBoundary << ") for " << viz(*this));
	}
}

void FogEntity::set_is_pure(const FogLazyBool& isPure) {
	if (!_is_pure->is_undecided()) {
		if (*_is_pure != isPure)
			ERRMSG("BUG - should not set_is_pure(" << isPure << ") for " << *_is_pure << " " << viz(*this));
	}
	
	else {
		_is_pure = isPure;
		CONDMSG(Fog::debug_purity(), "set_is_pure(" << isPure << ") for " << viz(*this));
		
		if (isPure.is_true())
			set_is_boundary(FogLazyBool::make_false());
	}
}

void FogEntity::set_using(const FogEntity& anEntity) {
	ERRMSG("BUG - should not FogEntity::set_using for " << viz(*this));
}

FogTemplateManager& FogEntity::template_manager() {
	ERRMSG("BUG - should not FogEntity::template_manager for " << viz(*this));
	return FogScope::mutable_null().template_manager();
}

const FogTemplateParameterSpecifiers& FogEntity::template_parameters() const {
	ERRMSG("BUG - should not FogEntity::template_parameters for " << viz(*this));
	return FogTemplateParameterSpecifiers::immutable_null();
}

FogUsageManager& FogEntity::usage_manager() {
	return _global_scope.usage_manager();
}

bool FogEntity::validate() const {
	return _scope.validate_utility(*this);
}
