#include <Fog/FogIncludeAll.h>










TYPEINFO_SINGLE(FogConversionId, Super)
FOGTOKEN_LEAF_IMPL(FogConversionId)

FogConversionId::FogConversionId(const FogExpr& aDeclarator)
:
 _declarator(aDeclarator)
{
//  	Following is suppressed until there is mechanism for verifying that the spelling will be scope-independent
//  	if (aDeclarator.is_actual(FogScopeContext::null()))
//  		_resolved_id = resolve_id(FogScopeContext::null());
}

FogConversionId::FogConversionId(const This& aDecl)
:
 Super(aDecl),
 _declarator(aDecl._declarator),
 _resolved_id(aDecl._resolved_id)
{}

FogConversionId::~FogConversionId() {}

void FogConversionId::create_usage(FogUsageContext& usageContext) const
{
 Super::create_usage(usageContext);
 FogChangedUsageContext localContext(usageContext, usageContext.of_use_by().of_name());
 _declarator->create_usage(localContext);
}

//  
//  	Override necessary for irregular scope-dependent spelling.
//  
bool FogConversionId::emit(FogEmitContext& emitContext) const
{
 emitContext.emit_scope();
 if (_resolved_id)
  return emitContext.emit_identifier(*_resolved_id);
 else
 {
  emitContext.emit_space_and_text("operator ");
  FogForUseAsEmitContext changedContext(emitContext, FogEmitContext::CONVERSION_INTERFACE);
  _declarator->emit(changedContext);
 }
 return true;
}

void FogConversionId::install(FogInstallContext& installContext) const
{
 Super::install(installContext);
 _declarator->install(installContext);
}

bool FogConversionId::is_actual(const FogScopeContext& scopeContext) const
{
 return Super::is_actual(scopeContext) && _declarator->is_actual(scopeContext);
}

const PrimId *FogConversionId::is_resolved() const { return _resolved_id ? _resolved_id.pointer() : 0; }

void FogConversionId::make_actual_from(FogMakeActualContext& makeActualContext)
{
 Super::make_actual_from(makeActualContext);
 _declarator->make_actual(_declarator, makeActualContext);
}

void FogConversionId::merge_from(FogMergeContext& mergeContext, const This& thatDecl)
{
 Super::merge_from(mergeContext, thatDecl);
 thatDecl._declarator->merge_into(mergeContext, _declarator);
 _resolved_id.reset();
}

const FogMerge& FogConversionId::needs_merge_from(FogMergeContext& mergeContext, const This& thatId) const
{
 FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, thatId));
 needsMerge |= _declarator->needs_merge(mergeContext, *thatId._declarator);
 return needsMerge;
}

char FogConversionId::print_named(std::ostream& s, const PrimId *scopeId, char tailChar) const
{
 if (scopeId)
 {
  tailChar = FogStream::space_and_emit(s, tailChar, scopeId->str());
  tailChar = FogStream::space_and_emit(s, tailChar, "::");  
 }
 if (_resolved_id)
  return FogStream::space_and_emit(s, tailChar, _resolved_id.str());
 tailChar = FogStream::space_and_emit(s, tailChar, "operator ");
 return _declarator->print_named(s, 0, tailChar);
}

std::ostream& FogConversionId::print_viz(std::ostream& s) const
{
 s << "\"conversion-function-id: ";
 print_this(s);
 return s << '\"';
}

bool FogConversionId::resolve_id(PrimIdHandle& returnId, FogScopeContext& inScope) const
{
 if (_resolved_id)
  returnId = _resolved_id;
 else
 {
  FogScope& aScope =  inScope.dynamic_scope();
  FogStream s(aScope);
  FogStaticEmitContext emitContext(s, aScope, FogEmitContext::CONVERSION_TYPE_ID);
  emitContext.emit_space_and_text("operator ");
  _declarator->emit(emitContext);
  size_t aSize = s.pcount();
  returnId = PrimIdHandle(s.str(), aSize);
 }
 return true;
}

bool FogConversionId::resolve_semantics(FogSemanticsContext& theSemantics) const
{
 FogChildSemanticsContext exprSemantics(theSemantics);
 if (!_declarator->resolve_semantics(exprSemantics))
  return false;
 theSemantics.reset();
 if (exprSemantics.is_conversion_type_id())
  theSemantics.assign(FogSemantics::IS_CONVERSION_FUNCTION_ID);
 return true;
}

FogToken::NameSort FogConversionId::sort_class() const { return CONVERSION_NAME; }
