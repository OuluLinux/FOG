#include <Fog/FogIncludeAll.h>
















TYPEINFO_SINGLE(FogTemplatedName, Super)
FOGTOKEN_LEAF_IMPL(FogTemplatedName)

FogTemplatedName::FogTemplatedName(FogName& aName, FogListOfExpr& exprList)
	:
	Super(aName),
	_exprs(exprList)
{}

FogTemplatedName::FogTemplatedName(const This& aName)
	:
	Super(aName),
	_exprs(aName._exprs),
	_template_args(aName._template_args)
{}

FogTemplatedName::~FogTemplatedName() {}

void FogTemplatedName::create_usage(FogUsageContext& usageContext) const {
	_template_args->create_usage(usageContext);
	Super::create_usage(usageContext);
}

bool FogTemplatedName::emit(FogEmitContext& emitContext) const {
	if (!name().emit(emitContext))
		return false;
		
	if (_template_args)                             //   Defined during emit
		_template_args->emit(emitContext);
	else {                                          //   Undefined for function signatures
		FogSeparatedEmitContext separatedContext(emitContext, ",", 0);
		emitContext.emit_space_and_text(" <");
		
		for (FogExprConstListOfRefToConstIterator p(_exprs); p; ++p) {
			emitContext.emit_space_and_text(" ");
			p->emit(separatedContext);
			emitContext.emit_space_and_text(p.to_go() > 1 ? "," : " ");
		}
		
		emitContext.emit_space_and_text("> ");
	}
	
	return true;
}

FogEntity* FogTemplatedName::find_entity_in(FogScopeContext& inScope, FindStrategy aStrategy) const {
	FogTemplatedScopeContext templatedContext(inScope, *this);
	return name().find_entity_in(templatedContext, aStrategy);
}

const FogMetaSlot* FogTemplatedName::find_slot_in(FogScopeContext& scopeContext) const {
	//ERRMSG("INVESTIGATE - did not expect to FogTemplatedName::find_slot_in.");
	FogTemplatedScopeContext templatedContext(scopeContext, *this);
	const FogMetaSlot* slot = name().find_slot_in(templatedContext);
	
	if (slot)
		return slot;
		
	WRNMSG("untested hack running in FogTemplatedName::find_slot_in");
	FogMetaSlotFinding metaSlotFinding;
	
	if (find_slots_in(scopeContext, metaSlotFinding) && !metaSlotFinding.is_found())
		metaSlotFinding.add(FogMetaSlot::immutable_null());                     //   Propagate return truth.
		
	slot = metaSlotFinding.get_unambiguous_finding(*this, scopeContext);
	
	if (slot)
		return slot;
		
	return slot;
}

bool FogTemplatedName::get_object(FogTokenRef& returnValue, FogScopeContext& inScope) const {
	if (!inScope.at_meta())
		return Super::get_object(returnValue, inScope);
		
	return get_meta_entity_in(returnValue, inScope);
}

void FogTemplatedName::install(FogInstallContext& installContext) const {
	//    _template_args->install(installContext);
	Super::install(installContext);
}

bool FogTemplatedName::is_actual(const FogScopeContext& scopeContext) const {
	//    ERRMSG("INVESTIGATE use of FogTemplatedName::is_actual for " << viz(*this));
	return Super::is_actual(scopeContext) && !_template_args->is_null();
}

FogTemplatedName* FogTemplatedName::is_templated_name() { return this; }

void FogTemplatedName::make_actual_from(FogMakeActualContext& makeActualContext) {
	Super::make_actual_from(makeActualContext);
	_exprs->make_actual(_exprs.to_const(), makeActualContext);
	//    if (!makeActualContext.requires_resolution() || makeActualContext.is_parse_phase())
	//        return;
	//    FogToken *primaryToken = name().find_scope(makeActualContext, HAS_ARGS);
	//    if (!primaryToken)
	//        return;
	//    FogEntity *primaryEntity = primaryToken ? primaryToken->is_entity() : 0;
	//    if (!primaryEntity || !primaryEntity->is_templated())
	//    {
	//        ERRMSG("Template arguments applied to non template as " << viz(*this));
	//        return;
	//    }
	//    resolve_template(*primaryEntity, _template_args, makeActualContext);
}

FogEntity* FogTemplatedName::make_entity(FogMakerContext& makerContext) const {
	FogTemplatedMakerContext templatedContext(makerContext, *this);
	FogEntity* anEntity = name().make_entity(templatedContext);
	return anEntity;
	//    FogEntity *primaryEntity = name().make_entity(makerContext);
	//    if (!primaryEntity)
	//        return 0;
	//    FogTemplateArgsRefToConst templateArgs;
	//    if (!resolve_template(*primaryEntity, templateArgs, makerContext))
	//        return 0;
	//    FogMakeTemplateContext makeTemplateContext(makerContext, *primaryEntity, *templateArgs);
	//    FogEntity *secondaryEntity = primaryEntity->find_template(makeTemplateContext);
	//    return secondaryEntity;
}

FogEntity* FogTemplatedName::make_name_entity(FogMakeEntityContext& makeEntityContext) const {
	//    FogTemplatedMakeEntityContext templatedContext(makeEntityContext, *this);
	//    FogEntity *anEntity = name().make_name_entity(templatedContext);
	//    return anEntity;
	FogEntity* primaryEntity = name().make_name_entity(makeEntityContext);
	
	if (!primaryEntity)
		return 0;
		
	FogTemplateArgsRefToConst templateArgs;
	
	if (!resolve_template(*primaryEntity, templateArgs, makeEntityContext))
		return 0;
		
	FogMakeTemplateContext makeTemplateContext(makeEntityContext, *primaryEntity, *templateArgs, IS_REFERENCE);
	FogEntity* secondaryEntity = primaryEntity->find_template(makeTemplateContext);
	return secondaryEntity;
}

bool FogTemplatedName::make_specifier(FogMakeSpecifierContext& makeSpecifierContext) {
	return makeSpecifierContext.make_specifier(*this) != 0;
}

FogEntity* FogTemplatedName::make_type_entity(FogMakeEntityContext& makeEntityContext) const {
	FogEntity* primaryEntity = name().make_type_entity(makeEntityContext);
	
	if (!primaryEntity)
		return 0;
		
	FogTemplateArgsRefToConst templateArgs;
	
	if (!resolve_template(*primaryEntity, templateArgs, makeEntityContext))
		return 0;
		
	FogMakeTemplateContext makeTemplateContext(makeEntityContext, *primaryEntity, *templateArgs, IS_REFERENCE);
	FogEntity* secondaryEntity = primaryEntity->find_template(makeTemplateContext);
	return secondaryEntity;
}

bool FogTemplatedName::make_type_specifier(FogMakeSpecifierContext& makeSpecifierContext) {
	FogObjectSpecifier* objectSpecifier = makeSpecifierContext.get_object_specifier(*this);
	
	if (!objectSpecifier)
		return false;
		
	FogDecoratedNameRef actualName;
	make_actual(actualName.to_const(), makeSpecifierContext);   //  .bugbug to_const
	objectSpecifier->set_type(*actualName);
	return true;
}

void FogTemplatedName::merge_from(FogMergeContext& mergeContext, const This& thatName) {
	Super::merge_from(mergeContext, thatName);
	thatName._exprs->merge_into(mergeContext, _exprs.to_const());
}

const FogMerge& FogTemplatedName::needs_merge_from(FogMergeContext& mergeContext, const This& thatName) const {
	FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, thatName));
	needsMerge |= _exprs->needs_merge(mergeContext, *thatName._exprs);
	return needsMerge;
}

std::ostream& FogTemplatedName::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	_exprs->print_deep(s, aDepth);
	return s;
}

std::ostream& FogTemplatedName::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	_exprs->print_on(s, aDepth);
	return s;
}

char FogTemplatedName::print_named(std::ostream& s, const PrimId* scopeId, char tailChar) const {
	tailChar = Super::print_named(s, 0, tailChar);
	
	if (_template_args)
		tailChar = _template_args->print_named(s, 0, tailChar);
	else {
		tailChar = FogStream::space_and_emit(s, tailChar, " <");
		
		for (FogExprConstListOfRefToConstIterator p(_exprs); p; ++p) {
			tailChar = FogStream::space_and_emit(s, tailChar, " ");
			tailChar = p->print_named(s, 0, tailChar);
			tailChar = FogStream::space_and_emit(s, tailChar, p.to_go() > 1 ? "," : " ");
		}
		
		tailChar = FogStream::space_and_emit(s, tailChar, "> ");
	}
	
	return tailChar;
}

bool FogTemplatedName::put_value(FogScopeContext& putScope, const FogToken& tokenValue,
                                 FogScopeContext& getScope) const {
	FogTemplatedScopeContext templatedContext(putScope, *this);
	return name().put_value(templatedContext, tokenValue, getScope);
}

bool FogTemplatedName::resolve_id(PrimIdHandle& returnId, FogScopeContext& inScope) const {
	//   This method is invoked to see if a function name is a constructor name
	PrimOstrstream s;
	print_named(s, 0, 0);
	size_t aSize = s.pcount();
	returnId = PrimIdHandle(s.str(), aSize);
	return true;
}

bool FogTemplatedName::resolve_semantics(FogSemanticsContext& theSemantics) const {
	FogChildSemanticsContext nameSemantics(theSemantics);
	FogChildSemanticsContext argsSemantics(theSemantics);
	
	if (!name().resolve_semantics(nameSemantics) || !exprs().resolve_semantics(argsSemantics))
		return false;
		
	theSemantics.reset();
	
	if (argsSemantics.is_template_argument_list()) {
		if (nameSemantics.is_template_name()) {
			theSemantics.set(FogSemantics::IS_TEMPLATE_ID);
			theSemantics.set(FogSemantics::IS_CLASS_NAME);
		}
		else if (theSemantics.is_resolve_type_weakly()) {
			theSemantics.set_diagnostic(FogSemanticsContext::BAD_TEMPLATE);
			theSemantics.set(FogSemantics::IS_TEMPLATE_ID);
			theSemantics.set(FogSemantics::IS_CLASS_NAME);
		}
		
		if (nameSemantics.is_template_type_name())
			theSemantics.set(FogSemantics::IS_TEMPLATE_TYPE_NAME);
			
		if (nameSemantics.is_template_value_name())
			theSemantics.set(FogSemantics::IS_TEMPLATE_VALUE_NAME);
	}
	
	return true;
}

//
//    Make actual version of the template arguments corresponding to the parameters of primaryEntity inScope.
//
bool FogTemplatedName::resolve_template(FogEntity& primaryEntity, FogTemplateArgsRefToConst& templateArgs,
                                        FogScopeContext& inScope) const {
	const FogTemplateParameterSpecifiers& templateParameters = primaryEntity.template_parameters();
	FogTemplateArgsRef templateArguments(new FogTemplateArgs, FogTemplateArgsRef::ADOPT);
	FogTemplateParameterSpecifierConstListOfRefToConstIterator q(templateParameters.parameters());
	FogExprConstListOfRefToConstIterator p(exprs().exprs());
	
	for ( ; p && q; ++p, ++q) {
		FogExprRef anExpr;
		p->make_actual(anExpr.to_const(), inScope);
		templateArguments->add_argument(*q, *anExpr);
	}
	
	if (p) {
		ERRMSG("Too many template arguments in " << viz(*this));
		return false;
	}
	
	for ( ; q; ++q) {
		const FogExpr& defaultExpr = q->initializer();
		
		if (defaultExpr.is_null()) {
			ERRMSG("No default available for missing template argument(s) in " << viz(*this));
			return false;
		}
		
		FogExprRef anExpr;
		defaultExpr.make_actual(anExpr.to_const(), inScope);
		templateArguments->add_argument(*q, *anExpr);
	}
	
	templateArgs = templateArguments;
	return true;
}

FogToken::NameSort FogTemplatedName::sort_class() const { return name().sort_class(); }
