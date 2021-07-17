#include <Fog/FogIncludeAll.h>









TYPEINFO_SINGLE(FogEnumeratorSpecifier, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogEnumeratorSpecifier)
PRIMREFS_IMPL(FogEnumeratorSpecifier)
FOGTOKEN_LEAF_IMPL(FogEnumeratorSpecifier)
TMPL_HACK_FIX_DO(FogEnumeratorSpecifier)

FogEnumeratorSpecifier::FogEnumeratorSpecifier() {}

FogEnumeratorSpecifier::FogEnumeratorSpecifier(FogName& anId, FogExpr* aValue)
	:
	Super(anId),
	_value(aValue) {
	CONDMSG(Fog::debug_make(), "Created " << *this);
}

FogEnumeratorSpecifier::~FogEnumeratorSpecifier() {}

void FogEnumeratorSpecifier::create_usage(FogUsageContext& aContext) const {
	Super::create_usage(aContext);
	_value->create_usage(aContext);
}

bool FogEnumeratorSpecifier::emit(FogEmitContext& emitContext) const {
	Super::emit(emitContext);
	
	if (_value) {
		emitContext.emit_space_and_text(" = ");
		_value->emit(emitContext);
	}
	
	return true;
}

void FogEnumeratorSpecifier::install(FogInstallContext& installContext) const {
	Super::install(installContext);
	_value->install(installContext);
}

bool FogEnumeratorSpecifier::is_actual(const FogScopeContext& scopeContext) const {
	return Super::is_actual(scopeContext) && _value->is_actual(scopeContext);
}

const FogEnumeratorSpecifier* FogEnumeratorSpecifier::is_raw_enumerator() const { return this; }

void FogEnumeratorSpecifier::make_actual_from(FogMakeActualContext& makeActualContext) {
	Super::make_actual_from(makeActualContext);
	_value->make_actual(_value.to_const(), makeActualContext);
}

void FogEnumeratorSpecifier::merge_from(FogMergeContext& mergeContext, const This& thatExpr) {
	Super::merge_from(mergeContext, thatExpr);
	thatExpr._value->merge_into(mergeContext, _value.to_const());
}

const FogMetaType& FogEnumeratorSpecifier::meta_type() const { return FogMetaType::enumerator_definition_type(); }

const FogMerge& FogEnumeratorSpecifier::needs_merge_from(FogMergeContext& mergeContext, const This& thatExpr) const {
	FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, thatExpr));
	needsMerge |= _value->needs_merge(mergeContext, *thatExpr._value);
	return needsMerge;
}

FogEnumerator* FogEnumeratorSpecifier::new_enumerator(FogScopeContext& inScope) const {
	PrimIdHandle anId;
	
	if (!resolve_id(anId, inScope))
		return 0;
		
	return new FogEnumerator(inScope.dynamic_scope(), *anId, *_value);
}

char FogEnumeratorSpecifier::print_named(std::ostream& s, const PrimId* fullId, char tailChar) const {
	tailChar = Super::print_named(s, fullId, tailChar);
	
	if (_value) {
		tailChar = FogStream::space_and_emit(s, tailChar, " = ");
		tailChar = _value->print_named(s, 0, tailChar);
	}
	
	return tailChar;
}

std::ostream& FogEnumeratorSpecifier::print_this(std::ostream& s) const {
	Super::print_this(s);
	
	if (_value) {
		s << " = ";
		_value->print_this(s);
	}
	
	return s;
}

std::ostream& FogEnumeratorSpecifier::print_viz(std::ostream& s) const {
	s << "\"enumerator-definition: ";
	Super::print_this(s);
	return s << '\"';
}

bool FogEnumeratorSpecifier::resolve_semantics(FogSemanticsContext& theSemantics) const {
	FogChildSemanticsContext nameSemantics(theSemantics);
	
	if (!name().resolve_semantics(nameSemantics))
		return false;
		
	FogChildSemanticsContext valueSemantics(theSemantics);
	
	if (!_value)
		valueSemantics.assign(FogSemantics::IS_EPSILON);
	else if (!_value->resolve_semantics(valueSemantics))
		return false;
		
	theSemantics.reset();
	
	if (nameSemantics.is_enumerator() && valueSemantics.is_constant_expression_opt())
		theSemantics.assign(FogSemantics::IS_ENUMERATOR_DEFINITION);
		
	return true;
}
