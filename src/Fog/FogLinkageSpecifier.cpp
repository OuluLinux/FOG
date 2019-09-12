#include <Fog/FogIncludeAll.h>










TYPEINFO_SINGLE(FogLinkageSpecifier, Super)
FOGTOKEN_READ_ONLY_IMPL(FogLinkageSpecifier)

FogLinkageSpecifier::FogLinkageSpecifier(const PrimId& anId, FogRaw *aDeclaration)
:
 Super(FogTag::linkage_tag(), FogIdentifier::make(anId), FogAccess::public_access(), aDeclaration),
 _linkage(*name().is_resolved())
{}

FogLinkageSpecifier::FogLinkageSpecifier(const This &thatSpecifier)
:
 Super(thatSpecifier),
 _linkage(thatSpecifier._linkage)
{}

FogLinkageSpecifier::~FogLinkageSpecifier() {}
FogLinkageSpecifier *FogLinkageSpecifier::is_linkage_specifier() { return this; }

FogEntity *FogLinkageSpecifier::make_type_entity(FogMakeEntityContext& makeEntityContext) const
{
 FogScope *madeScope = makeEntityContext.dynamic_scope().make_linkage(*this);
 if (!madeScope)
  return 0;
 FogEntityMakerContext makerContext(makeEntityContext, *this, &FogScope::make_linkage_entity);
 make_body(makerContext, *madeScope);
 return madeScope;
}

const FogMetaType& FogLinkageSpecifier::meta_type() const { return FogMetaType::linkage_specification_type(); }

std::ostream& FogLinkageSpecifier::print_this(std::ostream& s) const
{
 s << "extern \"" << linkage() << '\"';
 return s;
}

std::ostream& FogLinkageSpecifier::print_viz(std::ostream& s) const
{
 s << "\"linkage-specifier: \"" << _linkage.str() << "\"\"";;
 return s;
}

bool FogLinkageSpecifier::resolve_semantics(FogSemanticsContext& theSemantics) const
{
 theSemantics.assign(FogSemantics::IS_LINKAGE_SPECIFIER);
 return true;
}
