#include <Fog/FogIncludeAll.h>





TYPEINFO_SINGLE(FogUsageContext, Super)
TYPEINFO_SINGLE(FogChangedUsageContext, Super)
TYPEINFO_SINGLE(FogDecoratedUsageContext, Super)
TYPEINFO_SINGLE(FogStaticUsageContext, Super)
TYPEINFO_SINGLE(FogNestedUsageContext, Super)

//  -------------------------------------------------------------------------------------------------------------------------

FogChangedUsageContext::FogChangedUsageContext(FogUsageContext& usageContext, const FogOfUseBy& ofUseBy)
	:
	Super(usageContext),
	_of_use_by(ofUseBy)
{}

FogChangedUsageContext::~FogChangedUsageContext() {}
const FogOfUseBy& FogChangedUsageContext::of_use_by() const { return _of_use_by; }

//  -------------------------------------------------------------------------------------------------------------------------

FogDecoratedUsageContext::FogDecoratedUsageContext(FogUsageContext& usageContext)
	:
	_context(usageContext)
{}

FogToken& FogDecoratedUsageContext::dynamic_token() { return _context.dynamic_token(); }
const FogToken& FogDecoratedUsageContext::dynamic_token() const { return _context.dynamic_token(); }
FogEntity& FogDecoratedUsageContext::entity() { return _context.entity(); }
FogScopeContext* FogDecoratedUsageContext::find_context(size_t dollarCount)
{ return dollarCount > 0 ? _context.find_context(dollarCount) : this; }
bool FogDecoratedUsageContext::find_slots(FogMetaSlotFinder& theFinder) { return _context.find_slots(theFinder); }
const FogTemplateParameterSpecifier* FogDecoratedUsageContext::find_template_parameter_specifier
(const PrimId& anId) { return _context.find_template_parameter_specifier(anId); }
const FogTemplateParameterSpecifier* FogDecoratedUsageContext::find_template_parameter_specifier
(const FogTemplateParameterSpecifierId& anId) { return _context.find_template_parameter_specifier(anId); }
const FogScopeContext* FogDecoratedUsageContext::meta_context() const { return _context.meta_context(); }
const FogOfUseBy& FogDecoratedUsageContext::of_use_by() const { return _context.of_use_by(); }

std::ostream& FogDecoratedUsageContext::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	_context.print_deep(s, aDepth);
	return s;
}

std::ostream& FogDecoratedUsageContext::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	_context.print_on(s, aDepth);
	return s;
}

FogScopeContext::Resolution FogDecoratedUsageContext::resolution() const { return _context.resolution(); }
FogToken& FogDecoratedUsageContext::static_token() { return _context.static_token(); }
const FogToken& FogDecoratedUsageContext::static_token() const { return _context.static_token(); }
const FogTemplateParameterSpecifiers* FogDecoratedUsageContext::template_parameters() const
{ return _context.template_parameters(); }

FogStaticUsageContext::FogStaticUsageContext(FogScope& aScope, FogEntity& anEntity, const FogOfUseBy& ofUseBy)
	:
	_static_token(aScope),
	_entity(anEntity),
	_of_use_by(ofUseBy)
{}

FogStaticUsageContext::~FogStaticUsageContext() {}
FogToken& FogStaticUsageContext::dynamic_token() { return _static_token; }
const FogToken& FogStaticUsageContext::dynamic_token() const { return _static_token; }
FogEntity& FogStaticUsageContext::entity() { return _entity; }
bool FogStaticUsageContext::find_slots(FogMetaSlotFinder& theFinder) { return _static_token.find_slots(theFinder); }

const FogTemplateParameterSpecifier* FogStaticUsageContext::find_template_parameter_specifier
(const FogTemplateParameterSpecifierId& anId) {
	const FogTemplateParameterSpecifiers& templateParameters = dynamic_scope().template_parameters();
	return templateParameters.find(anId, true);
}

const FogOfUseBy& FogStaticUsageContext::of_use_by() const { return _of_use_by; }
FogToken& FogStaticUsageContext::static_token() { return _static_token; }
const FogToken& FogStaticUsageContext::static_token() const { return _static_token; }

//  -------------------------------------------------------------------------------------------------------------------------

FogNestedUsageContext::FogNestedUsageContext(FogUsageContext& usageContext, FogToken& inToken)
	:
	Super(usageContext),
	_dynamic_token(inToken)
{}

FogNestedUsageContext::~FogNestedUsageContext() {}
FogToken& FogNestedUsageContext::dynamic_token() { return _dynamic_token; }
const FogToken& FogNestedUsageContext::dynamic_token() const { return _dynamic_token; }
bool FogNestedUsageContext::find_slots(FogMetaSlotFinder& theFinder) { return _dynamic_token.find_slots(theFinder); }
FogScopeContext::InScope FogNestedUsageContext::in_scope() const { return IN_BASE_SCOPE; }
