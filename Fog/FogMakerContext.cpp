#include <Fog/FogIncludeAll.h>











TYPEINFO_SINGLE(FogMakerContext, Super)
TYPEINFO_SINGLE(FogEntityMakerContext, Super)
TYPEINFO_SINGLE(FogDecoratedMakerContext, Super)
TYPEINFO_SINGLE(FogFunctionMakerContext, Super)
TYPEINFO_SINGLE(FogNestedMakerContext, Super)
TYPEINFO_SINGLE(FogTemplatedMakerContext, Super)

FogEntity* FogMakerContext::find_entity(FindStrategy aStrategy) {
	FogEntityFinding theFinding(aStrategy);
	FogEntityFinder theFinder(theFinding, *this);
	find_entities(theFinder);
	return theFinding.get_unambiguous_finding(short_id(), dynamic_scope());
}

PrimIdHandle FogMakerContext::make_scoped_id(const PrimId& unscopedId) const {
	if (dynamic_scope().is_global_scope() || dynamic_scope().is_null())
		return unscopedId;
	else {
		PrimOstrstream s;
		s << dynamic_scope().global_signature_id() << "::" << unscopedId;
		size_t aSize = s.pcount();
		return PrimIdHandle(s.str(), aSize);
	}
}

PrimIdHandle FogMakerContext::make_templated_id(const PrimId& unscopedId) const {
	const FogTemplateParameterSpecifiers* templateParameterSpecifiers = template_parameters();
	
	if (!templateParameterSpecifiers)
		return unscopedId;
	else if (!templateParameterSpecifiers->parameters().tally())
		return unscopedId;
	else {
		PrimOstrstream s;
		char tailChar = FogStream::space_and_emit(s, 0, unscopedId);
		tailChar = templateParameterSpecifiers->print_suffix(s, tailChar);
		size_t aSize = s.pcount();
		return PrimIdHandle(s.str(), aSize);
	}
}

std::ostream& FogMakerContext::print_this(std::ostream& s) const {
	print_resolution(s);
	dynamic_token().print_long_id(s);
	s << " <- ";
	specifier().print_named(s, 0, 0);
	return s;
}

FogScopeContext::Resolution FogMakerContext::resolution() const { return RESOLVE_ATS; }

//
//    Resolve the template arguments within primaryEntity and assign to templateArgs, returning false if not possible
//    as is the case for resolution of a function name whose templae arguments must be deduced.
//
bool FogMakerContext::resolve_template(FogEntity& primaryEntity, FogTemplateArgsRefToConst& templateArgs) {
	return false;
}

const FogUtility& FogMakerContext::utility() const {
	const FogUtility& superUtility = Super::utility();
	const FogUtility& specifierUtility = specifier().utility();
	//      if (superUtility != specifierUtility)
	//          ERRMSG("INVESTIGATE -- inconsistent " << superUtility.str() << " and " << specifierUtility.str());
	const FogUtility& theUtility = min(specifierUtility, superUtility);
	return theUtility;
}

const PrimId& FogDecoratedMakerContext::global_id() const { return maker_context().global_id(); }
PrimIdHandle FogDecoratedMakerContext::global_signature_id() const { return maker_context().global_signature_id(); }
const PrimId& FogDecoratedMakerContext::local_id() const { return maker_context().local_id(); }
PrimIdHandle FogDecoratedMakerContext::local_signature_id() const { return maker_context().local_signature_id(); }
const PrimId& FogDecoratedMakerContext::long_id() const { return maker_context().long_id(); }
FogEntity* FogDecoratedMakerContext::make_entity(FogMakerContext& makerContext, const PrimId& anId)
{ return maker_context().make_entity(makerContext, anId); }
FogScopeContext::Resolution FogDecoratedMakerContext::resolution() const { return maker_context().resolution(); }
const PrimId& FogDecoratedMakerContext::short_id() const { return maker_context().short_id(); }
const FogSpecifier& FogDecoratedMakerContext::specifier() const { return maker_context().specifier(); }

//  ---------------------------------------------------------------------------------------------------------------------

FogEntityMakerContext::FogEntityMakerContext(FogMakeEntityContext& makeEntityContext,
        const FogSpecifier& aSpecifier, MakerFunction aMaker)
	:
	Super(makeEntityContext),
	_specifier(aSpecifier),
	_maker(aMaker)
{}

const PrimId& FogEntityMakerContext::global_id() const { return long_id(); }
PrimIdHandle FogEntityMakerContext::global_signature_id() const { return make_templated_id(global_id()); }
const PrimId& FogEntityMakerContext::local_id() const { return short_id(); }
PrimIdHandle FogEntityMakerContext::local_signature_id() const { return make_templated_id(local_id()); }

const PrimId& FogEntityMakerContext::long_id() const {
	if (!_long_id)
		mutate()._long_id = make_scoped_id(short_id());
		
	return *_long_id;
}

FogEntity* FogEntityMakerContext::make_entity(FogMakerContext& makerContext, const PrimId& anId) {
	_short_id = anId;
	FogScope& theScope = makerContext.dynamic_scope();
	return (theScope.*_maker)(makerContext);
}

const PrimId& FogEntityMakerContext::short_id() const { return *_short_id; }
const FogSpecifier& FogEntityMakerContext::specifier() const { return *_specifier; }

FogFunctionMakerContext::FogFunctionMakerContext(FogMakerContext& makerContext,
        const PrimId& localSignatureId, const PrimId& globalSignatureId)
	:
	Super(makerContext),
	_local_signature_id(localSignatureId),
	_global_signature_id(globalSignatureId)
{}

PrimIdHandle FogFunctionMakerContext::global_signature_id() const { return *_global_signature_id; }
PrimIdHandle FogFunctionMakerContext::local_signature_id() const { return *_local_signature_id; }

//  ---------------------------------------------------------------------------------------------------------------------

FogNestedMakerContext::FogNestedMakerContext(FogMakerContext& makerContext, FogToken& inToken)
	:
	Super(makerContext),
	_dynamic_token(inToken)
{}

FogToken& FogNestedMakerContext::dynamic_token() { return _dynamic_token; }
const FogToken& FogNestedMakerContext::dynamic_token() const { return _dynamic_token; }
bool FogNestedMakerContext::find_formal_slots(FogMetaSlotFinder& theFinder) { return false; }
bool FogNestedMakerContext::find_slots(FogMetaSlotFinder& theFinder) { return _dynamic_token.find_slots(theFinder); }

const PrimId& FogNestedMakerContext::global_id() const {
	if (!_global_id)
		mutate()._global_id = make_scoped_id(local_id());
		
	return *_global_id;
}

PrimIdHandle FogNestedMakerContext::global_signature_id() const { return global_id(); }
FogScopeContext::InScope FogNestedMakerContext::in_scope() const { return IN_THIS_SCOPE; }

const PrimId& FogNestedMakerContext::long_id() const {
	if (!_long_id)
		mutate()._long_id = make_scoped_id(short_id());
		
	return *_long_id;
}

FogTemplatedMakerContext::FogTemplatedMakerContext(FogMakerContext& makerContext, const FogTemplatedName& templatedName)
	:
	Super(makerContext),
	_templated_name(templatedName)
{}

void FogTemplatedMakerContext::find_entities(FogEntityFinder& theFinder) {
	FogEntityFinding theFinding(theFinder.strategy());
	FogEntityFinder nestedFinder(theFinding, theFinder);
	Super::find_entities(nestedFinder);
	FogEntity* primaryEntity = theFinding.get_unambiguous_finding(dynamic_token(), *this);
	
	if (primaryEntity) {
		FogTemplateArgsRefToConst templateArgs;
		
		if (_templated_name.resolve_template(*primaryEntity, templateArgs, *this)) {
			FogMakeTemplateContext makeContext(*this, *primaryEntity, *templateArgs, IS_DEFINITION);
			FogEntity* secondaryEntity = primaryEntity->find_template(makeContext);
			
			if (secondaryEntity)
				theFinder.add_find(*secondaryEntity);
		}
	}
}

std::ostream& FogTemplatedMakerContext::print_resolution(std::ostream& s) const {
	Super::print_resolution(s);
	s << " < ";
	_templated_name.exprs().print_named(s, 0, 0);
	s << " >, ";
	return s;
}

bool FogTemplatedMakerContext::resolve_template(FogEntity& primaryEntity, FogTemplateArgsRefToConst& templateArgs) {
	return _templated_name.resolve_template(primaryEntity, templateArgs, *this);
}

