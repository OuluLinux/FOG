#include <Fog/FogIncludeAll.h>










TYPEINFO_SINGLE(FogTemplateArgs, Super)
PRIMREF_NULL_CLAS(FogTemplateArgs)
FOGTOKEN_NULL_LEAF
//    virtual void adopt(const FogTemplateArg& anArgument) {}
virtual void create_usage(FogUsageContext& usageContext) const {}
virtual void emit_suffix(FogEmitContext& emitContext) const {}
virtual const PrimId& id() const { return PrimId::null(); }
virtual bool is_actual(const FogScopeContext& scopeContext) const { return true; }
virtual FogTemplateArgsRefToConst merge(FogMergeContext& mergeContext,
                                        const FogTemplateArgs& templateArgs) const { return *this; }
virtual char print_suffix(std::ostream& s, char tailChar) const { return tailChar; }
};
PRIMREF_DERIVED_NULL_IMPL(FogTemplateArgs)
PRIMREFS_IMPL(FogTemplateArgs)
FOGTOKEN_LEAF_IMPL(FogTemplateArgs)
TMPL_HACK_FIX_DO(FogTemplateArgs)

FogTemplateArgs::FogTemplateArgs() {}

FogTemplateArgs::FogTemplateArgs(const FogTemplateArgs& templateArgs)
	:
	_arguments(templateArgs._arguments),
	_id(templateArgs._id)
{}

FogTemplateArgs::~FogTemplateArgs() {}

void FogTemplateArgs::add_argument(const FogTemplateParameterSpecifier& parameterSpecifier, FogExpr& anExpr) {
	if (parameterSpecifier.is_type_parameter_specifier())
		adopt(FogTemplateArg::new_template_type_argument(anExpr));
	else
		adopt(FogTemplateArg::new_template_value_argument(anExpr));
}

const FogTemplateArg* FogTemplateArgs::adopt(const FogTemplateArg* anArgument) {
	_id = 0;
	
	if (!anArgument)
		;
	else if (is_null()) {
		anArgument->annul();
		anArgument = 0;
	}
	else
		_arguments.adopt(*anArgument);
		
	return anArgument;
}

void FogTemplateArgs::create_usage(FogUsageContext& usageContext) const {
	Super::create_usage(usageContext);
	
	for (FogTemplateArgConstListOfRefToConstIterator p(_arguments); p; ++p)
		p->create_usage(usageContext);
}

bool FogTemplateArgs::emit(FogEmitContext& emitContext) const {
	emitContext.emit_space_and_text(" <");
	
	if (_arguments.tally()) {
		FogForUseAsEmitContext nestedContext(emitContext, FogEmitContext::ABSTRACT_SIGNATURE);
		
		for (FogTemplateArgConstListOfRefToConstIterator p(_arguments); p; ++p) {
			nestedContext.emit_space_and_text(" ");
			p->emit(nestedContext);
			nestedContext.emit_space_and_text(p.to_go() > 1 ? "," : " ");
		}
	}
	
	emitContext.emit_space_and_text("> ");
	return true;
}

const PrimId& FogTemplateArgs::id() const {
	if (!_id) {
		PrimOstrstream s;
		print_named(s, 0, 0);
		mutate()._id = s.id();
	}
	
	return *_id;
}

void FogTemplateArgs::install(FogInstallContext& installContext) const {
	Super::install(installContext);
	
	for (FogTemplateArgConstListOfRefToConstIterator p(_arguments); p; ++p)
		p->install(installContext);
}

bool FogTemplateArgs::is_actual(const FogScopeContext& scopeContext) const {
	for (FogTemplateArgConstListOfRefToConstIterator p(_arguments); p; ++p)
		if (!p->is_actual(scopeContext))
			return false;
			
	return Super::is_actual(scopeContext);
}

void FogTemplateArgs::make_actual_from(FogMakeActualContext& makeActualContext) {
	Super::make_actual_from(makeActualContext);
	
	for (FogTemplateArgListOfRefToConstIterator p(_arguments); p; ++p)
		p->make_actual(p.ref(), makeActualContext);
}

bool FogTemplateArgs::matches(FogMergeContext& mergeContext,
                              const FogTemplateParameterSpecifiers& templateParameters) const {
	FogTemplateArgConstListOfRefToConstIterator p1(_arguments);
	//      if (!p1.to_go())
	//          return true;                    // The empty template is an exact match.
	FogTemplateParameterSpecifierConstListOfRefToConstIterator p2(templateParameters.parameters());
	
	if (p1.to_go() != p2.to_go())
		return false;                   //  .bugbug should defaults be considered ?
		
	for ( ; p1; ++p1, ++p2)
		if (!p1->matches(mergeContext, *p2))
			return false;
			
	return true;
}

void FogTemplateArgs::merge_from(FogMergeContext& mergeContext, const This& templateArgs) {
	FogTemplateArgListOfRefToConstIterator p1(_arguments);
	FogTemplateArgConstListOfRefToConstIterator p2(templateArgs._arguments);
	
	if (p1.to_go() != p2.to_go()) {
		ERRMSG("BUG - should not merge incompatible " << viz(*this) << " and " << viz(templateArgs));
		return;
	}
	
	//    FogTemplateArgsRef mergedArguments(new FogTemplateArgs, FogTemplateArgsRef::ADOPT);
	for ( ; p1; ++p1, ++p2)
		p2->merge_into(mergeContext, p1.ref());
		
	//        mergedArguments->add(*p1->merge(mergeContext, *p2));
}

const FogMerge& FogTemplateArgs::needs_merge_from(FogMergeContext& mergeContext, const This& templateArgs) const {
	if (this == &templateArgs)
		return FogMerge::both_valid();
		
	if (is_null() != templateArgs.is_null())
		return FogMerge::set_incompatible(FogMerge::incompatible_templating());
		
	FogTemplateArgConstListOfRefToConstIterator p1(_arguments);
	FogTemplateArgConstListOfRefToConstIterator p2(templateArgs._arguments);
	
	if (p1.to_go() != p2.to_go())
		return FogMerge::set_incompatible(FogMerge::incompatible_template_rank());
		
	FogMergeHandle needsMerge;
	
	for ( ; p1 && !needsMerge->is_incompatible(); ++p1, ++p2)
		needsMerge |= p1->needs_merge(mergeContext, *p2);
		
	return needsMerge;
}

char FogTemplateArgs::print_named(std::ostream& s, const PrimId* scopeId, char tailChar) const {
	tailChar = FogStream::space_and_emit(s, tailChar, "<");
	
	for (FogTemplateArgConstListOfRefToConstIterator p(_arguments); p; ++p) {
		tailChar = FogStream::space_and_emit(s, tailChar, " ");
		tailChar = p->print_named(s, 0, tailChar);
		tailChar = FogStream::space_and_emit(s, tailChar, p.to_go() > 1 ? "," : " ");
	}
	
	return FogStream::space_and_emit(s, tailChar, ">");
}

std::ostream& FogTemplateArgs::print_this(std::ostream& s) const {
	print_named(s, 0, 0);
	return s;
}

std::ostream& FogTemplateArgs::print_viz(std::ostream& s) const {
	s << "\"template-arguments: ";
	print_this(s);
	return s << '\"';
}
