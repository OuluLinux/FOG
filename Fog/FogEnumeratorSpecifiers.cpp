#include <Fog/FogIncludeAll.h>








TYPEINFO_SINGLE(FogEnumeratorSpecifiers, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogEnumeratorSpecifiers)
PRIMREFS_IMPL(FogEnumeratorSpecifiers)
FOGTOKEN_LEAF_IMPL(FogEnumeratorSpecifiers)
TMPL_HACK_FIX_DO(FogEnumeratorSpecifiers)

FogEnumeratorSpecifiers::FogEnumeratorSpecifiers() {}

FogEnumeratorSpecifiers::FogEnumeratorSpecifiers(const This& anExpr)
:
 Super(anExpr),
 _enumerators(anExpr._enumerators)   //   ? .bugbug should these be cloned
{}

FogEnumeratorSpecifiers::~FogEnumeratorSpecifiers() {}

void FogEnumeratorSpecifiers::add(FogEnumeratorSpecifier& anEnumerator)
{
 _enumerators.add(anEnumerator);
}

void FogEnumeratorSpecifiers::create_usage(FogUsageContext& aContext) const
{
 Super::create_usage(aContext);
 for (FogEnumeratorSpecifierConstListOfRefToConstIterator p(_enumerators); p; ++p)
  p->create_usage(aContext);
}

bool FogEnumeratorSpecifiers::emit(FogEmitContext& emitContext) const
{
 if (!_enumerators.tally())
  return false;
 for (FogEnumeratorSpecifierConstListOfRefToConstIterator p(_enumerators); p; ++p)
 {
  emitContext.emit_space_and_text(" ");
  p->emit(emitContext);
  emitContext.emit_space_and_text(p.to_go() > 1 ? "," : " ");
 }
 return true;
}

void FogEnumeratorSpecifiers::install(FogInstallContext& installContext) const
{
 Super::install(installContext);
 for (FogEnumeratorSpecifierConstListOfRefToConstIterator p(_enumerators); p; ++p)
  p->install(installContext);
}

bool FogEnumeratorSpecifiers::is_actual(const FogScopeContext& scopeContext) const
{
 for (FogEnumeratorSpecifierConstListOfRefToConstIterator p(_enumerators); p; ++p)
  if (!p->is_actual(scopeContext))
   return false;
 return Super::is_actual(scopeContext);
}

void FogEnumeratorSpecifiers::make_actual_from(FogMakeActualContext& makeActualContext)
{
 Super::make_actual_from(makeActualContext);
 for (FogEnumeratorSpecifierListOfRefToConstIterator p(_enumerators); p; ++p)
  p->make_actual(p.ref(), makeActualContext);
}

void FogEnumeratorSpecifiers::merge_from(FogMergeContext& mergeContext, const This& thatExpr)
{
 Super::merge_from(mergeContext, thatExpr);
//  	Super::merge_long_into(mergeContext, _exprs, thatExpr._enumerators);
 FogEnumeratorSpecifier::merge_long_into(mergeContext, _enumerators, thatExpr._enumerators);
}

const FogMerge& FogEnumeratorSpecifiers::needs_merge_from(FogMergeContext& mergeContext, const This& anExpr) const
{
 FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, anExpr));
 FogEnumeratorSpecifierConstListOfRefToConstIterator p1(_enumerators);
 FogEnumeratorSpecifierConstListOfRefToConstIterator p2(anExpr._enumerators);
 for ( ; p1 && p2; ++p1, ++p2)
 {
  if (p1->is_null())
  {
   if (!p2->is_null())
    needsMerge |= FogMerge::left_invalid();
  }
  else
  {
   if (!p2->is_null())
    needsMerge |= p1->needs_merge(mergeContext, *p2);
   else
    needsMerge |= FogMerge::right_invalid();
  }
 }
 if (p1)
  needsMerge |= FogMerge::right_invalid();
 else if (p2)
  needsMerge |= FogMerge::left_invalid();
 return needsMerge;
}

std::ostream& FogEnumeratorSpecifiers::print_depth(std::ostream& s, int aDepth) const
{
 Super::print_depth(s, aDepth);
 if (_enumerators.tally())
 {
  s << indent(aDepth) << "raw-enumerators\n";
  _enumerators.print_depth(s, aDepth+1);
 }
 return s;
}

std::ostream& FogEnumeratorSpecifiers::print_members(std::ostream& s, int aDepth) const
{
 Super::print_members(s, aDepth);
 if (_enumerators.tally())
 {
  s << indent(aDepth) << "raw-enumerators\n";
  _enumerators.print_members(s, aDepth+1);
 }
 return s;
}

char FogEnumeratorSpecifiers::print_named(std::ostream& s, const PrimId *fullId, char tailChar) const
{
 for (FogEnumeratorSpecifierConstListOfRefToConstIterator p(_enumerators); p; ++p)
 {
  tailChar = FogStream::space_and_emit(s, tailChar, " ");
  tailChar = p->print_named(s, 0, tailChar);
  tailChar = FogStream::space_and_emit(s, tailChar, p.to_go() > 1 ? "," : " ");
 }
 return tailChar;
}

std::ostream& FogEnumeratorSpecifiers::print_this(std::ostream& s) const { return s; }

std::ostream& FogEnumeratorSpecifiers::print_viz(std::ostream& s) const
{
 s << "\"raw-enumerators: ";
 print_this(s);
 return s << '\"';
}
