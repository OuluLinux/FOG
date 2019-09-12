#include <Fog/FogIncludeAll.h>













TYPEINFO_SINGLE(FogEnumSpecifier, Super)
FOGTOKEN_LEAF_IMPL(FogEnumSpecifier)

FogEnumSpecifier::FogEnumSpecifier(FogName *aName, FogEnumeratorSpecifiers *theEnumerators)
:
 Super(FogTag::enum_tag(), aName ? *aName : FogName::make_anon()),
 _enumerators(theEnumerators)
{}

FogEnumSpecifier::FogEnumSpecifier(const This &thatSpecifier)
:
 Super(thatSpecifier),
 _enumerators(thatSpecifier._enumerators)
{}

FogEnumSpecifier::~FogEnumSpecifier() {}

void FogEnumSpecifier::create_usage(FogUsageContext& aContext) const
{
 Super::create_usage(aContext);
 _enumerators->create_usage(aContext);
}

bool FogEnumSpecifier::emit(FogEmitContext& emitContext) const
{
 emitContext.emit_space_and_text("enum ");
 name().emit(emitContext);
//  	emitContext.emit_space_and_text(" {");		//.bugbug ? test emitContext to see if used as type
//  	_enumerators->emit(emitContext);
//  	emitContext.emit_space_and_text("} ");
 return true;
}

void FogEnumSpecifier::install(FogInstallContext& installContext) const
{
 Super::install(installContext);
 _enumerators->install(installContext);
}

bool FogEnumSpecifier::is_actual(const FogScopeContext& scopeContext) const
{
 return Super::is_actual(scopeContext) && _enumerators->is_actual(scopeContext);
}

const FogEnumSpecifier *FogEnumSpecifier::is_raw_enum() const { return this; }

void FogEnumSpecifier::make_actual_from(FogMakeActualContext& makeActualContext)
{
 Super::make_actual_from(makeActualContext);
 _enumerators->make_actual(_enumerators.to_const(), makeActualContext);
}

FogEntity *FogEnumSpecifier::make_type_entity(FogMakeEntityContext& makeEntityContext) const
{
 FogEntityMakerContext makerContext(makeEntityContext, *this, &FogScope::make_enum);
//  	check_enum(*theDeclaration);
 if (makeEntityContext.template_parameters())
  ERRMSG("Template parameters ignored for " << viz(makerContext));
 FogEntity *theEntity = name().make_entity(makerContext);
 if (!theEntity)
  return 0;
 FogEnum *anEnum = theEntity->is_enum();
 if (!anEnum)
 {
  ERRMSG("BUG - made " << viz(*theEntity) << " when variable expected.");
  return 0;
 }
 anEnum->set_access(access());
//  	FogFile *sourceFile = theEntity->source_file();
//  	if (sourceFile)
//  		sourceFile->add_internal(theEntity->name_usage());
 for (FogEnumeratorSpecifierConstListOfRefToConstIterator p(_enumerators->enumerators()); p; ++p)
 {
  FogEnumerator *anEnumerator = p->new_enumerator(makerContext);
  if (anEnumerator)
  {
   anEnum->add(*anEnumerator);
   anEnumerator->annul();
  }
 }
 return theEntity;
}

void FogEnumSpecifier::merge_from(FogMergeContext& mergeContext, const This& thatExpr)
{
 Super::merge_from(mergeContext, thatExpr);
 thatExpr._enumerators->merge_into(mergeContext, _enumerators.to_const());
}

const FogMetaType& FogEnumSpecifier::meta_type() const { return FogMetaType::enum_specifier_type(); }

const FogMerge& FogEnumSpecifier::needs_merge_from(FogMergeContext& mergeContext, const This& thatExpr) const
{
 FogMergeHandle needsMerge(Super::needs_merge_from(mergeContext, thatExpr));
 needsMerge |= _enumerators->needs_merge(mergeContext, *thatExpr._enumerators);
 return needsMerge;
}

std::ostream& FogEnumSpecifier::print_depth(std::ostream& s, int aDepth) const
{
 Super::print_depth(s, aDepth);
 _enumerators->print_depth(s, aDepth);
 return s;
}

std::ostream& FogEnumSpecifier::print_members(std::ostream& s, int aDepth) const
{
 Super::print_members(s, aDepth);
 _enumerators->print_members(s, aDepth);
 return s;
}

char FogEnumSpecifier::print_named(std::ostream& s, const PrimId *fullId, char tailChar) const
{
//  	tailChar = FogStream::space_and_emit(s, tailChar, "enum ");	-- in Super
 tailChar = Super::print_named(s, fullId, tailChar);
 tailChar = FogStream::space_and_emit(s, tailChar, " {");
 tailChar = _enumerators->print_named(s, 0, tailChar);
 tailChar = FogStream::space_and_emit(s, tailChar, "}");
 return tailChar;
}

std::ostream& FogEnumSpecifier::print_viz(std::ostream& s) const
{
 s << "\"enum-specifier: ";
 print_this(s);
 return s << '\"';
}

bool FogEnumSpecifier::resolve_semantics(FogSemanticsContext& theSemantics) const
{
 FogChildSemanticsContext nameSemantics(theSemantics);
 if (!name().resolve_semantics(nameSemantics))
  return false;
 theSemantics.reset();
 if (nameSemantics.is_scoped_identifier_opt())
 {
  bool isEnumeratorList = true;
  for (FogEnumeratorSpecifierConstListOfRefToConstIterator p(_enumerators->enumerators()); p; ++p)
  {
   const FogEnumeratorSpecifier& anEnumerator = *p;
   FogChildSemanticsContext enumeratorSemantics(theSemantics);
   if (!anEnumerator.resolve_semantics(enumeratorSemantics))
    return false;
   if (!enumeratorSemantics.is_enumerator_definition())
    isEnumeratorList = false;
  }
  if (isEnumeratorList)
   theSemantics.assign(FogSemantics::IS_ENUM_SPECIFIER);
 }
 return true;
}
