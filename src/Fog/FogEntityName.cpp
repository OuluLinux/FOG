#include <Fog/FogIncludeAll.h>







TYPEINFO_SINGLE(FogEntityName, Super)
PRIMREF_NULL_CLAS(FogEntityName)
 FOGTOKEN_NULL_LEAF
 virtual bool resolve_id(PrimIdHandle& returnId, FogScopeContext& inScope) const
  { returnId = PrimId::null(); return true; }
};
PRIMREF_DERIVED_NULL_IMPL(FogEntityName)
PRIMREFS_IMPL(FogEntityName)
TMPL_HACK_FIX_DO(FogEntityName)

bool FogEntityName::emit(FogEmitContext& emitContext) const
{
 if (is_null())
  return false;
 PrimIdHandle anId;
 resolve_id(anId, emitContext);
 emitContext.emit_scope();
 return emitContext.emit_identifier(*anId);
}

void FogEntityName::find_entities_in(FogScopeContext& inScope, FogEntityFinding& theFinding) const
{
 PrimIdHandle anId;
 if (resolve_id(anId, inScope))
 {
  FogEntityFinder theFinder(theFinding, *anId, inScope.in_scope());
  inScope.find_entities(theFinder);
 }
}

FogDestructorId *FogEntityName::is_destructor_id() { return 0; }
FogEntityName *FogEntityName::is_entity_name() { return this; }

FogEntity *FogEntityName::make_entity(FogMakerContext& makerContext) const
{
 PrimIdHandle anId;
 if (!resolve_id(anId, makerContext))
  return 0;
 return makerContext.make_entity(*anId);
}

bool FogEntityName::make_specifier(FogMakeSpecifierContext& makeSpecifierContext)
{
 return makeSpecifierContext.make_specifier(*this) != 0;
}

const FogMerge& FogEntityName::needs_merge(FogMergeContext& mergeContext, const FogToken& aToken) const
{              //   this->is_anon() part of algorithm in FogAnonId.
 if (this == &aToken)
  return FogMerge::both_valid();
 const FogName *thatName = aToken.is_name();
 const FogEntityName *thatEntityName = thatName ? thatName->is_entity_name() : 0;
 if (!thatEntityName)
  return Super::needs_merge(mergeContext, aToken);
 if (thatName->is_anon() || mergeContext.right_is_anon())
  return FogMerge::right_invalid();
 if (mergeContext.left_is_anon())
  return FogMerge::left_invalid();
 PrimIdHandle leftId;
 PrimIdHandle rightId;
 return resolve_id(leftId, mergeContext) && thatEntityName->resolve_id(rightId, mergeContext)
  && (leftId == rightId) ? FogMerge::both_valid() : impossible_merge();
}
