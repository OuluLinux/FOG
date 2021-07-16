#include <Fog/FogIncludeAll.h>









TYPEINFO_SINGLE(FogFilespace, Super)

FogFilespace::FogFilespace() {}

FogFilespace::FogFilespace(FogMakerContext& makerContext)
:
 Super(FogTag::filespace_tag(), makerContext)
{
 CONDMSG(Fog::debug_make(), "Created " << *this);
 global_scope().add_file_space(*this);   //   Need to create an owner
}

FogFilespace::~FogFilespace() {}

const FogAccess& FogFilespace::default_access() const { return scope().default_access(); }
const FogInline& FogFilespace::default_inline() const { return scope().default_inline(); }

void FogFilespace::emit_scope(FogEmitContext& emitContext, const FogScope& inScope) const
{
 scope().emit_scope(emitContext, inScope);
}

void FogFilespace::find_entities(FogEntityFinder& theFinder) { scope().find_entities(theFinder); }

FogEntity *FogFilespace::find_entity(const PrimId& anId, InScope inScope, FindStrategy aStrategy)
{
 return scope().find_entity(anId, inScope, aStrategy);
}

const PrimId& FogFilespace::global_id() const { return scope().global_id(); }
const PrimId& FogFilespace::global_signature_id() const { return scope().global_signature_id(); }
bool FogFilespace::has_access() const { return scope().has_access(); }
FogTargetFile *FogFilespace::interface_file_source() { return scope().interface_file_source(); }
FogUsage& FogFilespace::interface_usage_finish() { return scope().interface_usage_finish(); }
FogUsage& FogFilespace::interface_usage_start() { return scope().interface_usage_start(); }
const PrimId& FogFilespace::local_id() const { return scope().local_id(); }
const PrimId& FogFilespace::local_signature_id() const { return scope().local_signature_id(); }
const PrimId& FogFilespace::long_id() const { return scope().long_id(); }
FogUsage& FogFilespace::name_usage() { return scope().name_usage(); }
FogScope& FogFilespace::parent_scope() { return scope().parent_scope(); }

void FogFilespace::set_interface_file(FogTargetFile *aFile)
{
 ERRMSG("Cannot specify an interface file for " << viz(*this));
}

const PrimId& FogFilespace::short_id() const { return scope().short_id(); }
