#include <Fog/FogIncludeAll.h>













TYPEINFO_SINGLE(FogScopedModifier, Super)
FOGTOKEN_LEAF_IMPL(FogScopedModifier)

//
//  	Construct a nested name, representing 'scopeDecl'::'childDecl'.
//
FogScopedModifier::FogScopedModifier(FogName& scopeDecl, FogModifier& childDecl)
	:
	_modifier(childDecl),
	_scope(scopeDecl)
{}

//
//  	Construct a global name, representing ::'childDecl'.
//
FogScopedModifier::FogScopedModifier(FogModifier& childDecl)
	:
	_modifier(childDecl)
{}

FogScopedModifier::FogScopedModifier(const This& aDecl)
	:
	Super(aDecl),
	_modifier(aDecl._modifier),
	_scope(aDecl._scope)
{}

FogScopedModifier::~FogScopedModifier() {}

//  void FogScopedModifier::compile() const
//  {
//  	Super::compile();
//  	_modifier->compile();
//  }

void FogScopedModifier::create_usage(FogUsageContext& usageContext) const {
	FogToken* theScope = find_scope(usageContext);
	
	if (theScope) {
		FogNestedUsageContext nestedContext(usageContext, *theScope);
		Super::create_usage(nestedContext);
		_modifier->create_usage(nestedContext);
		//  		nestedContext->add_use_by(nestedContext.entity(), nestedContext.of_use_by().of_tail());
		nestedContext.entity().add_use_by(nestedContext.entity(), nestedContext.of_use_by().of_tail());
	}
}

bool FogScopedModifier::emit_prefix(FogEmitContext& emitContext, bool needsBrackets) const {
	if (emitContext.emit_expression()) {
		_scope->emit(emitContext);
		emitContext.emit_space_and_text("::");
		_modifier->emit_prefix(emitContext, false);
	}
	else {
		PrimIdHandle anId;
		const FogScope& aScope = emitContext.dynamic_scope();
		//  		const FogLocalDecl *scopeName = _scope->is_local_decl();
		//  		if (scopeName)
		//  			anId = scopeName->resolve_id(emitContext());
		_scope->resolve_id(anId, emitContext);
		
		if (aScope.short_id() != *anId) {     //  .bugbug Check template too?
			//   change from LOCAL_INTERFACE once done?
			_scope->emit(emitContext);
			emitContext.emit_space_and_text("::");
		}
		
		FogToken* theScope = find_scope(emitContext);
		
		if (theScope) {
			FogNestedEmitContext nestedContext(emitContext, *theScope);
			_modifier->emit_prefix(nestedContext, false);
		}
	}
	
	return true;
}

FogToken* FogScopedModifier::find_scope(FogScopeContext& inScope) const {
	FogScope& dynamicScope = inScope.dynamic_scope();
	
	if (dynamicScope.is_null())       //   If an any old scope form of resolve_semantics
		return 0;
		
	if (inScope.is_unresolvable())
		ERRMSG("INVESTIGATE -- Invoking FogScopedModifier::find_scope in unexpected phase");
		
	if (!_scope)
		return &inScope.global_scope();
		
	FogToken* theScope = _scope->find_entity_in(inScope, FIND_SCOPE);
	//  	if (newScope && !hasArgs && newScope->is_templated())
	//  	{
	//  		ERRMSG("ROUNDTUIT -- missing code to select default secondary template.");
	//  	}
	return theScope;
}

void FogScopedModifier::install(FogInstallContext& installContext) const {
	FogToken* theScope = find_scope(installContext);
	
	if (!theScope)
		return;
		
	FogNestedScopeContext nestedContext(installContext, *theScope);
	Super::install(nestedContext);
	_modifier->install(nestedContext);
}

bool FogScopedModifier::is_actual(const FogScopeContext& scopeContext) const {
	return Super::is_actual(scopeContext) && _modifier->is_actual(scopeContext) && _scope->is_actual(scopeContext);
}

bool FogScopedModifier::is_indirect() const { return _modifier->is_indirect(); }

void FogScopedModifier::make_actual_from(FogMakeActualContext& makeActualContext) {
	Super::make_actual_from(makeActualContext);
	FogToken* theScope = find_scope(makeActualContext);
	
	if (!theScope)
		return;
		
	FogNestedScopeContext nestedContext(makeActualContext, *theScope);
	_modifier->make_actual(_modifier, nestedContext);
	_scope->make_actual(_scope, nestedContext);
}

void FogScopedModifier::merge_from(FogMergeContext& mergeContext, const This& thatDecl) {
	Super::merge_from(mergeContext, thatDecl);
	thatDecl._modifier->merge_into(mergeContext, _modifier);
	thatDecl._scope->merge_into(mergeContext, _scope);
}

const FogMetaType& FogScopedModifier::meta_type() const { return FogMetaType::scoped_modifier_type(); }

const FogMerge& FogScopedModifier::needs_merge_from(FogMergeContext& mergeContext, const This& thatDecl) const {
	FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, thatDecl));
	needsMerge |= _modifier->needs_merge(mergeContext, *thatDecl._modifier);
	needsMerge |= _scope->needs_merge(mergeContext, *thatDecl._scope);
	return needsMerge;
}

std::ostream& FogScopedModifier::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	
	if (_modifier)
		_modifier->print_deep(s, aDepth);
		
	return s;
}

std::ostream& FogScopedModifier::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	
	if (_modifier)
		_modifier->print_on(s, aDepth);
		
	return s;
}

char FogScopedModifier::print_prefix(std::ostream& s, char tailChar, bool needsBrackets) const {
	tailChar = _scope->print_named(s, 0, tailChar);
	tailChar = FogStream::space_and_emit(s, tailChar, "::");
	return _modifier->print_prefix(s, 0, false);
}

void FogScopedModifier::set_cv(const FogCv& aCv) {
	ERRMSG("BUG -- should not set_cv for " << viz(*this));
}
