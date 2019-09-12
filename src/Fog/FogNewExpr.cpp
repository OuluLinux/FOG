#include <Fog/FogIncludeAll.h>









TYPEINFO_SINGLE(FogNewExpr, Super)
FOGTOKEN_LEAF_IMPL(FogNewExpr)

FogNewExpr::FogNewExpr(FogListOfExpr *aPlace, FogExpr& typeId, FogExpr *anInit)
:
 _is_global(SCOPED_DELETE),
 _place(aPlace),
 _type(typeId),
 _init(anInit)
{}

FogNewExpr::FogNewExpr(FogListOfExpr& aPlace, FogListOfExpr *typeId, FogExpr *anInit)
:
 _is_global(SCOPED_DELETE),
 _place(aPlace),
 _type(typeId),
 _init(anInit)
{}

FogNewExpr::FogNewExpr(const This& aDecl)
:
 Super(aDecl),
 _is_global(aDecl._is_global),
 _place(aDecl._place),
 _type(aDecl._type),
 _init(aDecl._init)
{}

FogNewExpr::~FogNewExpr() {}

void FogNewExpr::create_usage(FogUsageContext& aContext) const
{
 Super::create_usage(aContext);
 _place->create_usage(aContext);
 _type->create_usage(aContext);
 _init->create_usage(aContext);
}

bool FogNewExpr::emit(FogEmitContext& emitContext) const
{
 if (_is_global)
  emitContext.emit_space_and_text(" ::");
 emitContext.emit_space_and_text("new ");
 if (_place)
 {
  emitContext.emit_space_and_text("(");
  _place->emit(emitContext);
  emitContext.emit_space_and_text(")");
 }
 _type->emit(emitContext);
 if (_init)
 {
  emitContext.emit_space_and_text("(");
  _init->emit(emitContext);
  emitContext.emit_space_and_text(")");
 }
 return true;
}

void FogNewExpr::install(FogInstallContext& installContext) const
{
 Super::install(installContext);
 _place->install(installContext);
 _type->install(installContext);
 _init->install(installContext);
}

bool FogNewExpr::is_actual(const FogScopeContext& scopeContext) const
{
 return Super::is_actual(scopeContext) && _place->is_actual(scopeContext)
  && _type->is_actual(scopeContext) && _init->is_actual(scopeContext);
}

void FogNewExpr::make_actual_from(FogMakeActualContext& makeActualContext)
{
 Super::make_actual_from(makeActualContext);
 _place->make_actual(_place.to_const(), makeActualContext);
 _type->make_actual(_type.to_const(), makeActualContext);
 _init->make_actual(_init.to_const(), makeActualContext);
}

void FogNewExpr::merge_from(FogMergeContext& mergeContext, const This& thatExpr)
{
 Super::merge_from(mergeContext, thatExpr);
 _is_global = merge_bool("global scope", _is_global != 0, thatExpr._is_global != 0) ? GLOBAL_DELETE : SCOPED_DELETE;
 thatExpr._place->merge_into(mergeContext, _place.to_const());
 thatExpr._type->merge_into(mergeContext, _type.to_const());
 thatExpr._init->merge_into(mergeContext, _init.to_const());
}

const FogMerge& FogNewExpr::needs_merge_from(FogMergeContext& mergeContext, const This& anExpr) const
{
 FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, anExpr));
 if (_is_global != anExpr._is_global)
  needsMerge |= FogMerge::incompatible_allocator();
 needsMerge |= _place->needs_merge(mergeContext, *anExpr._place);
 needsMerge |= _type->needs_merge(mergeContext, *anExpr._type);
 needsMerge |= _init->needs_merge(mergeContext, *anExpr._init);
 return needsMerge;
}

std::ostream& FogNewExpr::print_depth(std::ostream& s, int aDepth) const
{
 Super::print_depth(s, aDepth);
 _place->print_deep(s, aDepth);
 _type->print_deep(s, aDepth);
 _init->print_deep(s, aDepth);
 return s;
}

std::ostream& FogNewExpr::print_members(std::ostream& s, int aDepth) const
{
 Super::print_members(s, aDepth);
 _place->print_on(s, aDepth);
 _type->print_on(s, aDepth);
 _init->print_on(s, aDepth);
 return s;
}

char FogNewExpr::print_named(std::ostream& s, const PrimId *fullId, char tailChar) const
{
 if (_is_global)
  tailChar = FogStream::space_and_emit(s, tailChar, " ::");
 tailChar = FogStream::space_and_emit(s, tailChar, "new ");
 if (_place)
 {
  tailChar = FogStream::space_and_emit(s, tailChar, "(");
  tailChar = _place->print_named(s, 0, tailChar);
  tailChar = FogStream::space_and_emit(s, tailChar, ")");
 }
 tailChar = _type->print_named(s, 0, tailChar);
 if (_init)
 {
  tailChar = FogStream::space_and_emit(s, tailChar, "(");
  tailChar = _init->print_named(s, 0, tailChar);
  tailChar = FogStream::space_and_emit(s, tailChar, ")");
 }
 return tailChar;
}

std::ostream& FogNewExpr::print_viz(std::ostream& s) const
{
 s << "\"new-expression: ";
 print_this(s);
 return s << '\"';
}

bool FogNewExpr::resolve_semantics(FogSemanticsContext& theSemantics) const
{
 FogSemantics exprSemantics;
//  	if (!expr().resolve_semantics(exprSemantics, inScope))
//  		return false;
 theSemantics.reset();
//  	if (exprSemantics.is_basic_expression())
  theSemantics.set(FogSemantics::IS_BASIC_EXPRESSION);
 return true;
}