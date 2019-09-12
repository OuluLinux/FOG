#include <Fog/FogIncludeAll.h>




//  %import FogDecl.hxx














TYPEINFO_SINGLE(FogUsing, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogUsing)
PRIMREFS_IMPL(FogUsing)
PRIMMAPOFREFS_IMPL(FogUsing)
TMPL_HACK_FIX_DO(FogUsing)

FogUsing::FogUsing()
		:
		_access(FogAccess::invalid_access()),
		_entity(FogEntity::mutable_null()) {}
		
FogUsing::FogUsing(FogScope& aScope, const FogAccess& anAccess, FogEntity& usedEntity)
		:
		Super(aScope, usedEntity.long_id(), *make_long_id(aScope, usedEntity)),
		_access(anAccess),
		_entity(usedEntity) {
	scope().add_using(*this);
}

FogUsing::~FogUsing() {}

void FogUsing::create_usages() {
	FogProgressMonitor aMonitor("Establishing usage of ", *this);
	
	// hack fix: FogUsage::add_use_by was called for obviously bad values
	FogUsage::filter_name = &this->name_scope();
	
	Super::create_usages();
	scope().interface_usage_start().add_use_by(interface_usage_start());
	interface_usage_finish().add_use_by(scope().interface_usage_finish());
	
	FogUsage::filter_name = 0;
}

void FogUsing::emit_interface(FogStream& s) {
	s.change_to_access(_access);
	
	if (!Fog::no_using())
		s << "using ";
	else
		s << "/* using */ ";
		
	s << _entity.long_id() << ";\n";
}

FogUsage& FogUsing::interface_usage_start() {
	return interface_usage_finish();
}

FogUsage *FogUsing::make_interface_usage_finish() {
	const FogVariable *aVariable = _entity.is_variable();
	const FogFunction *aFunction = _entity.is_function();
	
	if (aVariable)
		return usage_manager().make_variable_usage(*this, aVariable->decl_specifiers());
	else
		if (aFunction)
			return usage_manager().make_function_usage(*this, aFunction->decl_specifiers());
		else
			if (_entity.is_enum())
				return usage_manager().make_enum_usage(*this, access().decl_specifier_value());
			else
				return usage_manager().make_typedef_usage(*this, access().decl_specifier_value());
}

PrimIdHandle FogUsing::make_long_id(FogScope& aScope, FogEntity& usedEntity) {
	PrimOstrstream s;
	s << aScope.global_signature_id() << "." << usedEntity.long_id();
	size_t aSize = s.pcount();
	return PrimIdHandle(s.str(), aSize);
}

const FogMetaType& FogUsing::meta_type() const {
	return FogMetaType::using_type();
}

std::ostream& FogUsing::print_this(std::ostream& s) const {
	s << unique_id();
	return s;
}

std::ostream& FogUsing::print_viz(std::ostream& s) const {
	return s << "\"using: " << unique_id() << '\"';
}

//  FogTokenType::TokenType FogUsing::token_type_enum() const { return FogTokenType::Using; }
