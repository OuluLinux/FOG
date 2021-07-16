#include <Fog/FogIncludeAll.h>















TYPEINFO_SINGLE(FogStd, Super)

FogStd::FogStd(FogRoot& globalScope)
:
 Super(Fog::no_namespace() ? FogTag::class_tag() : FogTag::namespace_tag(), globalScope, Fog::std_id())
{
 CONDMSG(Fog::debug_make(), "Created " << *this);
 set_utility(FogUtility::utility_utility());
 install_types();
 FogMetaBuiltInFunction::install(*this);
}

FogStd::~FogStd() {}
void FogStd::create_usages() {}
const FogAccess& FogStd::default_access() const { return FogAccess::flyweight(FogAccess::PRIVATE); }
const FogInline& FogStd::default_inline() const { return FogInline::in_interface(); }
void FogStd::emit_interface_finish(FogStream& s) const {}
void FogStd::emit_interface_start(FogStream& s) const {}
void FogStd::emit_scope_head(FogStream& s) const {}
void FogStd::emit_scope_tail(FogStream& s) const {}
bool FogStd::is_name_scope() const { return true; }
bool FogStd::is_name_space() const { return true; }
bool FogStd::is_type() const { return true; }
FogUsage *FogStd::make_interface_usage_finish() { return 0; }
FogUsage *FogStd::make_interface_usage_start() { return 0; }
FogUsage *FogStd::make_name_usage() { return 0; }
FogScope& FogStd::name_emit_scope() { return *this; }
FogScope& FogStd::name_emit_transient_scope() { return *this; }
FogTargetFile *FogStd::name_file() { return 0; }
FogScope& FogStd::name_scope() { return *this; }
FogScope& FogStd::name_space() { return *this; }
