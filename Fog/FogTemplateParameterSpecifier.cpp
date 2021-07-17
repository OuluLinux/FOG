#include <Fog/FogIncludeAll.h>



















TYPEINFO_SINGLE(FogTemplateParameterSpecifier, Super)
TYPEINFO_SINGLE(FogTemplatedParameterSpecifier, Super)
TYPEINFO_SINGLE(FogTypeParameterSpecifier, Super)
TYPEINFO_SINGLE(FogValueParameterSpecifier, Super)
PRIMREF_NULL_CLAS(FogTemplateParameterSpecifier)
FOGTOKEN_NULL_LEAF
virtual void emit_suffix(FogEmitContext& emitContext) const {}
};

PRIMREF_DERIVED_NULL_IMPL(FogTemplateParameterSpecifier)
PRIMREFS_IMPL(FogTemplateParameterSpecifier)
PRIMLISTOFREFS_IMPL(FogTemplateParameterSpecifier)
TMPL_HACK_FIX_DO(FogTemplateParameterSpecifier)

FOGTOKEN_LEAF_IMPL(FogTemplatedParameterSpecifier)
//TMPL_HACK_FIX_DO(FogTemplatedParameterSpecifier)

FOGTOKEN_LEAF_IMPL(FogTypeParameterSpecifier)
//TMPL_HACK_FIX_DO(FogTypeParameterSpecifier)

FOGTOKEN_LEAF_IMPL(FogValueParameterSpecifier)
//TMPL_HACK_FIX_DO(FogValueParameterSpecifier)

FogTemplateParameterSpecifier::FogTemplateParameterSpecifier(FogMakeSpecifierContext& makeSpecifierContext,
        FogName& nameName)
	:
	Super(makeSpecifierContext, default_name()),
	_unnormalised_name(nameName),
	_number(0) {}

void FogTemplateParameterSpecifier::create_usage(FogUsageContext& usageContext) const {
	Super::create_usage(usageContext);
	FogChangedUsageContext ofNameContext(usageContext, usageContext.of_use_by().of_name());
	_initializer->create_usage(ofNameContext);
}

FogName& FogTemplateParameterSpecifier::default_name() {
	static FogNameRef defaultName;
	
	if (defaultName->is_null()) {
		PrimOstrstream s;
		s << Fog::template_parameter_prefix();
		s << '0';
		defaultName = FogIdentifier::make(s.id());
	}
	
	return *defaultName;
}

bool FogTemplateParameterSpecifier::emit(FogEmitContext& emitContext) const {
	FogForUseAsEmitContext localContext(emitContext, FogForUseAs::LOCAL_INTERFACE);
	bool doneSomething = Super::emit(localContext);
	
	if (_initializer) {
		emitContext.emit_space_and_text(" = ");
		_initializer->emit(emitContext);
		doneSomething = true;
	}
	
	return doneSomething;
}

const FogTemplateParameterSpecifier* FogTemplateParameterSpecifier::find(FogScopeContext& scopeContext,
        const PrimId& anId) const {
	PrimIdHandle myId;
	return _unnormalised_name->resolve_id(myId, scopeContext) && (myId == anId) ? this : 0;
}

const FogTemplateParameterSpecifier* FogTemplateParameterSpecifier::find(
        const FogTemplateParameterSpecifierId& anId) const {
	return &name() == &anId ? this : 0;
}

void FogTemplateParameterSpecifier::install(FogInstallContext& installContext) const {
	Super::install(installContext);
	_initializer->install(installContext);
}

bool FogTemplateParameterSpecifier::is_actual(const FogScopeContext& scopeContext) const {
	return Super::is_actual(scopeContext) && _initializer->is_actual(scopeContext);
}

FogTemplateParameterSpecifier* FogTemplateParameterSpecifier::is_template_parameter_specifier() {
	return this;
}

FogTemplatedParameterSpecifier* FogTemplateParameterSpecifier::is_templated_parameter_specifier() {
	return 0;
}

FogTypeParameterSpecifier* FogTemplateParameterSpecifier::is_type_parameter_specifier() {
	return 0;
}

FogValueParameterSpecifier* FogTemplateParameterSpecifier::is_value_parameter_specifier() {
	return 0;
}

void FogTemplateParameterSpecifier::make_actual_from(FogMakeActualContext& makeActualContext) {
	Super::make_actual_from(makeActualContext);
	_initializer->make_actual(_initializer.to_const(), makeActualContext);
}

FogEntity* FogTemplateParameterSpecifier::make_name_entity(FogMakeEntityContext& makeEntityContext) const {
	ERRMSG("INVESTIGATE - FogTemplateParameterSpecifier::make_name_entity " << viz(*this));
	return 0;
	//  	FogEntityMakerContext makerContext(makeEntityContext, *this, &FogScope::make_template_parameter_entity);
	//  	return make_entity(makerContext);
}

void FogTemplateParameterSpecifier::merge_from(FogMergeContext& mergeContext, const This& templatePar) {
	Super::merge_from(mergeContext, templatePar);
	
	if (!_initializer)
		_initializer = templatePar._initializer;
	else if (templatePar._initializer)
		templatePar._initializer->merge_into(mergeContext, _initializer.to_const());
}

const FogMetaType& FogTemplateParameterSpecifier::meta_type() const {
	return FogMetaType::template_parameter_specifier_type();
}

const FogMerge& FogTemplateParameterSpecifier::needs_merge_from(FogMergeContext& mergeContext, const This& templatePar) const {
	if (_number != templatePar._number)
		return FogMerge::set_incompatible(FogMerge::incompatible_template_parameter());
		
	FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, templatePar));
	needsMerge |= _initializer->needs_merge(mergeContext, *templatePar._initializer);
	return needsMerge;
}

char FogTemplateParameterSpecifier::print_named(std::ostream& s, const PrimId* scopeId, char tailChar) const {
	//  	tailChar = name().print_named(s, 0, tailChar);
	//  	tailChar = Super::print_named(s, 0, tailChar);
	//  	tailChar = decl_specifiers().print_named(s, tailChar);
	if (!type().is_null())
		tailChar = type().print_named(s, 0, tailChar);
		
	//  	else
	//  		tailChar = FogStream::space_and_emit(s, tailChar, "--type--");
	for (size_t i = modifiers().tally(); i--;) {
		const FogModifier& iDecl = *modifiers()[i];
		tailChar = iDecl.print_prefix(s, tailChar, i > 0 ? modifiers()[i - 1]->is_indirect() : false);
	}
	
	if (!name().is_null())
		tailChar = name().print_named(s, scopeId, tailChar);
	else if (scopeId) {
		ERRMSG("INVESTIGATE - did not expect a scope for a null name in FogTemplateParameterSpecifier::print_named.");
		tailChar = FogStream::space_and_emit(s, tailChar, scopeId->str());
	}
	
	for (size_t j = 0; j < modifiers().tally(); j++) {
		const FogModifier& jDecl = *modifiers()[j];
		tailChar = jDecl.print_suffix(s, tailChar, j > 0 ? modifiers()[j - 1]->is_indirect() : false);
	}
	
	if (_initializer) {
		tailChar = FogStream::space_and_emit(s, tailChar, " = ");
		tailChar = _initializer->print_named(s, 0, tailChar);
	}
	
	return tailChar;
}

char FogTemplateParameterSpecifier::print_suffix(std::ostream& s, char tailChar) const {
	return name().print_named(s, 0, tailChar);
}

void FogTemplateParameterSpecifier::set_initializer(const FogExpr& anExpr) {
	_initializer = anExpr;
}

void FogTemplateParameterSpecifier::set_template_parameter_number(size_t aNumber,
        const FogTemplateParameterSpecifier* parentSpecifier) {
	_number = aNumber;
	PrimOstrstream s;
	
	if (!parentSpecifier)
		s << Fog::template_parameter_prefix();
	else {
		PrimIdHandle anId;
		parentSpecifier->resolve_id(anId, FogScopeContext::weak());
		s << *anId << '_';
	}
	
	s << aNumber;
	set_name(FogTemplateParameterSpecifierId::make(s.id(), *this));
}

//  FogTokenType::TokenType FogTemplateParameterSpecifier::token_type_enum() const { return FogTokenType::Token; }		//.bugbug

FogTemplatedParameterSpecifier::FogTemplatedParameterSpecifier(FogMakeSpecifierContext& makeSpecifierContext,
        FogSpecifier& nameName)
	:
	Super(makeSpecifierContext, nameName.name()) {}

FogTemplatedParameterSpecifier::~FogTemplatedParameterSpecifier() {}

void FogTemplatedParameterSpecifier::create_usage(FogUsageContext& usageContext) const {
	Super::create_usage(usageContext);
	
	for (FogTemplateParameterSpecifierConstListOfRefToConstIterator p(_parameters); p; ++p)
		p->create_usage(usageContext);
		
	//  	_template->create_usage(ofNameContext);
}

bool FogTemplatedParameterSpecifier::emit(FogEmitContext& emitContext) const {
	emitContext.emit_space_and_text("template <");
	
	for (FogTemplateParameterSpecifierConstListOfRefToConstIterator p(_parameters); p; ++p) {
		p->emit(emitContext);
		
		if (p.to_go() > 1)
			emitContext.emit_space_and_text(",");
	}
	
	emitContext.emit_space_and_text(">");
	return true;
}

void FogTemplatedParameterSpecifier::emit_suffix(FogEmitContext& emitContext) const {
	Super::emit(emitContext);
}

void FogTemplatedParameterSpecifier::install(FogInstallContext& installContext) const {
	Super::install(installContext);
	
	for (FogTemplateParameterSpecifierConstListOfRefToConstIterator p(_parameters); p; ++p)
		p->install(installContext);
		
	//  	_template->install(installContext);
}

bool FogTemplatedParameterSpecifier::is_actual(const FogScopeContext& scopeContext) const {
	for (FogTemplateParameterSpecifierConstListOfRefToConstIterator p(_parameters); p; ++p)
		if (!p->is_actual(scopeContext))
			return false;
			
	return Super::is_actual(scopeContext) /*   && _template->is_actual(scopeContext) */ ;
}

FogTemplatedParameterSpecifier* FogTemplatedParameterSpecifier::is_templated_parameter_specifier() {
	return this;
}

void FogTemplatedParameterSpecifier::make_actual_from(FogMakeActualContext& makeActualContext) {
	Super::make_actual_from(makeActualContext);
	
	for (FogTemplateParameterSpecifierListOfRefToConstIterator p(_parameters); p; ++p)
		p->make_actual(p.ref(), makeActualContext);
		
	//  	_template->make_actual(_template.to_const(), makeActualContext);
}

void FogTemplatedParameterSpecifier::merge_from(FogMergeContext& mergeContext, const This& templatePar) {
	Super::merge_from(mergeContext, templatePar);
	//  	Super::merge_long_into(mergeContext, _exprs, templatePar._exprs);
	FogTemplateParameterSpecifier::merge_long_into(mergeContext, _parameters, templatePar._parameters);
	//  	if (!_template)
	//  		_template = templatePar._template;
	//  	else if (templatePar._template)
	//  		templatePar._template->merge_into(mergeContext, _template.to_const());
}

const FogMetaType& FogTemplatedParameterSpecifier::meta_type() const {
	return FogMetaType::templated_parameter_specifier_type();
}

const FogMerge& FogTemplatedParameterSpecifier::needs_merge_from(FogMergeContext& mergeContext,
        const This& templatePar) const {
	if (this == &templatePar)
		return FogMerge::both_valid();
		
	if (is_null() != templatePar.is_null())
		return FogMerge::set_incompatible(FogMerge::incompatible_templating());
		
	FogTemplateParameterSpecifierConstListOfRefToConstIterator p1(_parameters);
	FogTemplateParameterSpecifierConstListOfRefToConstIterator p2(templatePar._parameters);
	
	if (p1.to_go() != p2.to_go())
		return FogMerge::set_incompatible(FogMerge::incompatible_template_rank());
		
	FogMergeHandle needsMerge;
	
	for (; p1 && !needsMerge->is_incompatible(); ++p1, ++p2)
		needsMerge |= p1->needs_merge(mergeContext, *p2);
		
	//  	needsMerge |= _template->needs_merge(mergeContext, *templatePar._template);
	return needsMerge;
}

char FogTemplatedParameterSpecifier::print_named(std::ostream& s, const PrimId* scopeId, char tailChar) const {
	tailChar = FogStream::space_and_emit(s, tailChar, "template <");
	
	for (FogTemplateParameterSpecifierConstListOfRefToConstIterator p(_parameters); p; ++p) {
		tailChar = p->print_named(s, 0,  tailChar);
		
		if (p.to_go() > 1)
			tailChar = FogStream::space_and_emit(s, tailChar, ",");
	}
	
	tailChar = FogStream::space_and_emit(s, tailChar, ">");
	return tailChar;
}

std::ostream& FogTemplatedParameterSpecifier::print_this(std::ostream& s) const {
	return Super::print_this(s);
}

std::ostream& FogTemplatedParameterSpecifier::print_viz(std::ostream& s) const {
	s << "\"templated-parameter: ";
	print_this(s);
	return s << '\"';
}

bool FogTemplatedParameterSpecifier::resolve_semantics(FogSemanticsContext& theSemantics) const {
	theSemantics.reset();
	theSemantics.set(FogSemantics::IS_TEMPLATED_PARAMETER);
	return true;
}

FogTypeParameterSpecifier::FogTypeParameterSpecifier(FogMakeSpecifierContext& makeSpecifierContext,
        FogScopeSpecifier& nameName)
	:
	Super(makeSpecifierContext, nameName.name()),
	_tag(nameName.tag()) {}

FogTypeParameterSpecifier::~FogTypeParameterSpecifier() {}

//  void FogTypeParameterSpecifier::create_usage(FogUsageContext& usageContext) const
//  {
//  	Super::create_usage(usageContext);
//  	FogChangedUsageContext ofNameContext(usageContext, usageContext.of_use_by().of_name());
//  }

bool FogTypeParameterSpecifier::emit(FogEmitContext& emitContext) const {
	emitContext.emit_space_and_text(_tag.str());
	return Super::emit(emitContext);
}

void FogTypeParameterSpecifier::emit_suffix(FogEmitContext& emitContext) const {
	Super::emit(emitContext);
}

//  void FogTypeParameterSpecifier::install(FogInstallContext& installContext) const
//  {
//  	Super::install(installContext);
//  }

//  bool FogTypeParameterSpecifier::is_actual(const FogScopeContext& scopeContext) const
//  	{ return Super::is_actual(scopeContext); }
FogTypeParameterSpecifier* FogTypeParameterSpecifier::is_type_parameter_specifier() {
	return this;
}

//  void FogTypeParameterSpecifier::make_actual_from(FogMakeActualContext& makeActualContext)
//  {
//  	Super::make_actual_from(makeActualContext);
//  }

//  void FogTypeParameterSpecifier::merge_from(FogMergeContext& mergeContext, const This& templatePar)
//  {
//  	Super::merge_from(mergeContext, templatePar);
//  }

const FogMetaType& FogTypeParameterSpecifier::meta_type() const {
	return FogMetaType::type_parameter_specifier_type();
}

const FogMerge& FogTypeParameterSpecifier::needs_merge_from(FogMergeContext& mergeContext,
        const This& templatePar) const {
	if (_tag != templatePar._tag)
		WRNMSG("Inconsistent template tags for " << viz(*this) << " and " << viz(templatePar));
		
	FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, templatePar));
	//  	needsMerge |= _template->needs_merge(mergeContext, *templatePar._template);
	return needsMerge;
}

char FogTypeParameterSpecifier::print_named(std::ostream& s, const PrimId* scopeId, char tailChar) const {
	tailChar = FogStream::space_and_emit(s, tailChar, _tag.str());
	return Super::print_named(s, 0, tailChar);
}

std::ostream& FogTypeParameterSpecifier::print_viz(std::ostream& s) const {
	s << "\"type-parameter: ";
	print_this(s);
	return s << '\"';
}

bool FogTypeParameterSpecifier::resolve_semantics(FogSemanticsContext& theSemantics) const {
	theSemantics.reset();
	theSemantics.set(FogSemantics::IS_TEMPLATE_TYPE_NAME);
	return true;
}

FogValueParameterSpecifier::FogValueParameterSpecifier(FogMakeSpecifierContext& makeSpecifierContext,
        FogName& nameName)
	:
	Super(makeSpecifierContext, nameName) {}

FogValueParameterSpecifier::~FogValueParameterSpecifier() {}

void FogValueParameterSpecifier::emit_suffix(FogEmitContext& emitContext) const {
	Super::emit(emitContext);
}

FogValueParameterSpecifier* FogValueParameterSpecifier::is_value_parameter_specifier() {
	return this;
}

const FogMetaType& FogValueParameterSpecifier::meta_type() const {
	return FogMetaType::value_parameter_specifier_type();
}

//  const FogMerge& FogValueParameterSpecifier::needs_merge_from(FogMergeContext& mergeContext,
//  		const This& templatePar) const
//  	{ return Super::needs_merge_from(mergeContext, templatePar); }

std::ostream& FogValueParameterSpecifier::print_viz(std::ostream& s) const {
	s << "\"value-parameter: ";
	print_this(s);
	return s << '\"';
}

bool FogValueParameterSpecifier::resolve_semantics(FogSemanticsContext& theSemantics) const {
	theSemantics.reset();
	theSemantics.set(FogSemantics::IS_TEMPLATE_VALUE_NAME);
	return true;
}
