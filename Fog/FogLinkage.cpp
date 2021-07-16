#include <Fog/FogIncludeAll.h>










TYPEINFO_SINGLE(FogLinkage, Super)

FogLinkage::FogLinkage() {}

FogLinkage::FogLinkage(FogScope& aScope, const PrimId& anId, const PrimId& linkageString)
:
 Super(FogTag::linkage_tag(), aScope, anId),
 _string(linkageString)
{
 CONDMSG(Fog::debug_make(), "Created " << *this);
}

FogLinkage::~FogLinkage() {}

bool FogLinkage::do_compile(FogCompileContext& inScope)
{
 if (!raw_interface_file())
  set_utility(scope().utility());
 return Super::do_compile(inScope);
}

void FogLinkage::emit_scope(FogEmitContext& emitContext, const FogScope& inScope) const
{
 scope().emit_scope(emitContext, inScope);
}

void FogLinkage::emit_scope_head(FogStream& s) const
{
 next(s);
 s << "extern \"" << _string << "\"\n";
 s << "{\n";
 s.indent(1);
}

void FogLinkage::emit_scope_tail(FogStream& s) const
{
 s.indent(-1);
 s << "};\n";
}

void FogLinkage::find_entities(FogEntityFinder& theFinder) { scope().find_entities(theFinder); }

FogEntity *FogLinkage::find_entity(const PrimId& anId, InScope inScope, FindStrategy aStrategy)
{
 return scope().find_entity(anId, inScope, aStrategy);
}

FogTargetFile *FogLinkage::implementation_file()
{
 if (raw_implementation_file())
  return raw_implementation_file();
 return scope().implementation_file();
}

FogTargetFile *FogLinkage::interface_file_source() 
{
 if (raw_interface_file())
  return raw_interface_file();
 return scope().interface_file_source();
}

FogUsage& FogLinkage::interface_usage_finish() { return FogUsage::mutable_null(); }
FogUsage& FogLinkage::interface_usage_start() { return FogUsage::mutable_null(); }
const FogMetaType& FogLinkage::meta_type() const { return FogMetaType::linkage_type(); }
FogScope& FogLinkage::name_emit_scope() { return *this; }
FogScope& FogLinkage::name_emit_transient_scope() { return *this; }
FogUsage& FogLinkage::name_usage() { return FogUsage::mutable_null(); }
