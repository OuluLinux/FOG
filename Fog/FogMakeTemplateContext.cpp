#include <Fog/FogIncludeAll.h>






TYPEINFO_SINGLE(FogMakeTemplateContext, Super)

FogMakeTemplateContext::FogMakeTemplateContext(FogScopeContext& scopeContext,
        FogEntity& primaryEntity, const FogTemplateArgs& templateArguments, IsReference isReference)
	:
	_context(scopeContext),
	_primary(primaryEntity),
	_template_arguments(templateArguments),
	_is_reference(isReference)
{}

FogMakeTemplateContext::~FogMakeTemplateContext() {}

const FogDeclSpecifierValue& FogMakeTemplateContext::decl_specifiers() const {
	static const FogDeclSpecifierValue nullValue;
	return nullValue;
}

FogToken& FogMakeTemplateContext::dynamic_token() { return _context.dynamic_token(); }
const FogToken& FogMakeTemplateContext::dynamic_token() const { return scope_context().dynamic_token(); }
FogScopeContext* FogMakeTemplateContext::find_context(size_t dollarCount)
{ return dollarCount > 0 ? _context.find_context(dollarCount) : this; }

bool FogMakeTemplateContext::find_formal_slots(FogMetaSlotFinder& theFinder) {
	FogMakeEntityContext* makeEntityContext = _context.find_make_entity_context();
	return makeEntityContext && makeEntityContext->find_formal_slots(theFinder);
}

bool FogMakeTemplateContext::find_slots(FogMetaSlotFinder& theFinder) { return _context.find_slots(theFinder); }
const FogTemplateParameterSpecifier* FogMakeTemplateContext::find_template_parameter_specifier
(const PrimId& anId) { return _context.find_template_parameter_specifier(anId); }
const FogTemplateParameterSpecifier* FogMakeTemplateContext::find_template_parameter_specifier
(const FogTemplateParameterSpecifierId& anId) { return _context.find_template_parameter_specifier(anId); }

const PrimId& FogMakeTemplateContext::global_id() const {
	if (!_global_id)
		make_templated_id(mutate()._global_id, long_id());
		
	return *_global_id;
}

const PrimId& FogMakeTemplateContext::global_signature_id() const { return global_id(); }
FogScopeContext::InScope FogMakeTemplateContext::in_scope() const { return _context.in_scope(); }

const PrimId& FogMakeTemplateContext::local_id() const {
	if (!_local_id)
		make_templated_id(mutate()._local_id, short_id());
		
	return *_local_id;
}

const PrimId& FogMakeTemplateContext::local_signature_id() const { return local_id(); }
const PrimId& FogMakeTemplateContext::long_id() const { return _primary.long_id(); }

void FogMakeTemplateContext::make_templated_id(PrimIdHandle& madeId, const PrimId& unscopedId) const {
	if (template_args().is_null())
		madeId = unscopedId;
	else {
		PrimOstrstream s;
		s << unscopedId;
		template_args().print_named(s, 0, 'a');
		madeId = s.str();
	}
}

const FogScopeContext* FogMakeTemplateContext::meta_context() const { return _context.meta_context(); }

std::ostream& FogMakeTemplateContext::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	_context.print_deep(s, aDepth);
	return s;
}

std::ostream& FogMakeTemplateContext::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	_context.print_on(s, aDepth);
	return s;
}

std::ostream& FogMakeTemplateContext::print_resolution(std::ostream& s) const {
	s << (is_reference() ? "REFERENCE, " : "DEFINITION, ");
	Super::print_resolution(s);
	template_args().print_named(s, 0, 0);
	s << ", ";
	return s;
}

FogScopeContext::Resolution FogMakeTemplateContext::resolution() const { return _context.resolution(); }
const PrimId& FogMakeTemplateContext::short_id() const { return _primary.short_id(); }
FogToken& FogMakeTemplateContext::static_token() { return _context.static_token(); }
const FogToken& FogMakeTemplateContext::static_token() const { return scope_context().static_token(); }

FogTargetFile* FogMakeTemplateContext::target_file() const {
	//      const FogTemplateParameterSpecifiers *templateParameters = template_parameters();
	//      if (templateParameters && templateParameters->is_using_template())
	//          return (FogTargetFile *)dynamic_scope().interface_file_source();        //.bugbug cast
	//      else
	return _primary.inner_scope().interface_file_source();
}

const FogTemplateParameterSpecifiers* FogMakeTemplateContext::template_parameters() const
{ return _context.template_parameters(); }

const FogUtility& FogMakeTemplateContext::utility() const {
	const FogTemplateParameterSpecifiers* templateParameters = template_parameters();
	
	if (templateParameters && templateParameters->is_using_template())
		return dynamic_scope().utility();
	else
		return _primary.inner_scope().utility();
}
