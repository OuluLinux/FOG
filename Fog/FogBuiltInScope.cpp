#include <Fog/FogIncludeAll.h>
















TYPEINFO_SINGLE(FogBuiltInScope, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogBuiltInScope)
PRIMREFS_IMPL(FogBuiltInScope)
TMPL_HACK_FIX_DO(FogBuiltInScope)

FogBuiltInScope::FogBuiltInScope()
	:
	Super(FogTag::built_in_tag(), FogRoot::mutable_null(), PrimId::null()) {
	CONDMSG(Fog::debug_make(), "Created " << *this);
	install_types();
}

FogBuiltInScope::FogBuiltInScope(FogRoot& globalScope, const PrimId& anId)
	:
	Super(FogTag::built_in_tag(), globalScope, anId) {
	CONDMSG(Fog::debug_make(), "Created " << *this);
	install_types();
}

FogBuiltInScope::~FogBuiltInScope() {}

void FogBuiltInScope::create_usages() {}

const FogAccess& FogBuiltInScope::default_access() const {
	return FogAccess::flyweight(FogAccess::AUTO);
}

const FogInline& FogBuiltInScope::default_inline() const {
	return FogInline::in_interface();
}

void FogBuiltInScope::emit_interface_finish(FogStream& s) const {}

void FogBuiltInScope::emit_interface_start(FogStream& s) const {}

void FogBuiltInScope::emit_scope_head(FogStream& s) const {}

void FogBuiltInScope::emit_scope_tail(FogStream& s) const {}

bool FogBuiltInScope::is_name_scope() const {
	return true;
}

FogUsage* FogBuiltInScope::make_interface_usage_finish() {
	return 0;
}

FogUsage* FogBuiltInScope::make_interface_usage_start() {
	return 0;
}

FogUsage* FogBuiltInScope::make_name_usage() {
	return 0;
}

const FogMetaType& FogBuiltInScope::meta_type() const {
	return FogMetaType::built_in_type();
}

FogScope& FogBuiltInScope::name_emit_scope() {
	return *this;
}

FogTargetFile* FogBuiltInScope::name_file() {
	return 0;
}

FogScope& FogBuiltInScope::name_scope() {
	return *this;
}

FogTokenType::TokenType FogBuiltInScope::pp_token_type_enum() const {
	return FogTokenType::Identifier;
}
//  FogTokenType::TokenType FogBuiltInScope::token_type_enum() const { return FogTokenType::BuiltIn; }
