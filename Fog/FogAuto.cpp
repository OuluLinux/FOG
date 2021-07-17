#include <Fog/FogIncludeAll.h>














TYPEINFO_SINGLE(FogAuto, Super)

FogAuto::FogAuto(FogRoot& globalScope)
	:
	Super(FogTag::meta_tag(), globalScope, Fog::auto_id()) {
	CONDMSG(Fog::debug_make(), "Created " << *this);
	install_types();
}

FogAuto::~FogAuto() {}

void FogAuto::create_usages() {}

const FogAccess& FogAuto::default_access() const {
	return FogAccess::flyweight(FogAccess::AUTO);
}

const FogInline& FogAuto::default_inline() const {
	return FogInline::in_interface();
}

void FogAuto::emit_interface_finish(FogStream& s) const {}

void FogAuto::emit_interface_start(FogStream& s) const {}

void FogAuto::emit_scope_head(FogStream& s) const {}

void FogAuto::emit_scope_tail(FogStream& s) const {}

FogUsage* FogAuto::make_interface_usage_finish() {
	return 0;
}

FogUsage* FogAuto::make_interface_usage_start() {
	return 0;
}

FogUsage* FogAuto::make_name_usage() {
	return 0;
}

FogScope& FogAuto::name_emit_scope() {
	return *this;
}

FogTargetFile* FogAuto::name_file() {
	return 0;
}

FogScope& FogAuto::name_scope() {
	return *this;
}
