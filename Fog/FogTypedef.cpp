#include <Fog/FogIncludeAll.h>















TYPEINFO_SINGLE(FogTypedef, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogTypedef)
PRIMREFS_IMPL(FogTypedef)
PRIMLISTOFREFS_IMPL(FogTypedef)
PRIMMAPOFREFS_IMPL(FogTypedef)
TMPL_HACK_FIX_DO(FogTypedef)

FogTypedef::FogTypedef()
	:
	_entity(0),
	_of(INVALID) {}

FogTypedef::FogTypedef(FogMakerContext& makerContext, FogEntity* anEntity)
	:
	Super(makerContext),
	_entity(anEntity),
	_of(anEntity ? TYPE : of(makerContext)) {
	if (scope().is_instantiation()) {
		FogScope& primaryScope = scope().primary_instantiation();
		FogTypedef* primaryTypedef = primaryScope.find_local_typedef(*this);
		
		if (primaryTypedef)
			set_primary_instantiation(*primaryTypedef);
		else
			ERRMSG("Failed to find primary typedef for specialisation by " << viz(makerContext));
	}
}

FogTypedef::~FogTypedef() {}

void FogTypedef::adopt(FogScopeContext& inScope, FogPotentialDeclaration& potentialDeclaration) {
	Super::adopt(inScope, potentialDeclaration);
	inner_scope().add_typedef(*this, &potentialDeclaration);
}

int FogTypedef::compare(const FogTypedefRefToConst* p1, const FogTypedefRefToConst* p2) {
	return Super::compare(**p1, **p2);
}

void FogTypedef::create_usages() {
	if (specifier().is_null())
		return;
		
	FogStaticUsageContext nameUsageContext(scope(), *this, FogOfUseBy::of_name_by_name());
	FogStaticUsageContext interfaceUsageContext(scope(), *this, FogOfUseBy::of_tail_by_head());
	specifier().create_usage(nameUsageContext);
	specifier().create_usage(interfaceUsageContext);
	
	if (_entity) {      //   create_usages() cannot be relied upon to create dependence
		FogStaticUsageContext entityUsageContext(scope(), *this, FogOfUseBy::of_name_by_head());
		_entity->add_use_by(entityUsageContext);  //    if typedef hides an external type.
	}
	
	FogUsage& nameUsage = name_usage();
	FogUsage& startUsage = interface_usage_start();
	//  	FogUsage& finishUsage = interface_usage_finish();
	nameUsage.add_use_by(startUsage);
	
	//  	startUsage.add_use_by(finishUsage);
	if (!is_free()) {
		scope().interface_usage_start().add_use_by(nameUsage);
		nameUsage.add_use_by(scope().interface_usage_finish());
	}
}

bool FogTypedef::do_compile(FogCompileContext& inScope) {
	FogProgressMonitor aMonitor("Compiling", *this);
	
	if (!Super::do_compile(inScope))
		return false;
		
	if (!specifier().is_null())         //   If this is an actual entity
		return true;           //    great, it stays
		
	FogInScopeContext inContext(inScope, IN_BASE_NOT_THIS_SCOPE); //   If not we have to find out what
	set_referenced_entity(find_type_in(inContext));    //    actual entity gets is referenced
	return false;
}

bool FogTypedef::do_derive(FogDeriveContext& inScope) {
	FogProgressMonitor aMonitor("Deriving into", *this);
	return Super::do_derive(inScope);
}

void FogTypedef::emit_forward_reference(FogStream& s) {
	FogProgressMonitor aMonitor("Emitting forward reference for", *this);
	
	if (!compiled())
		ERRMSG("BUG -- should not invoke emit_forward_reference before compilation of " << viz(*this));
		
	FogStreamEntityInterface nestedStream(s, *this);
	FogStaticEmitContext emitContext(s, scope(), FogEmitContext::LOCAL_INTERFACE);
	specifier().emit(emitContext);
}


void FogTypedef::install_names() {
	if (_entity)
		scope().add_type(*this, *_entity);
	else
		scope().add_type(*this, *this);
}

FogTargetFile* FogTypedef::interface_file_source() {
	if (is_free())
		return 0;       //   Name-scope typedefs are not constrained.
	else
		return Super::interface_file_source();
}

FogUsage& FogTypedef::interface_usage_finish() { return interface_usage_start(); }
inline bool FogTypedef::is_free() const { return name_scope().is_name_space(); }
bool FogTypedef::is_type() const { return true; }
FogTypedef* FogTypedef::is_typedef() { return this; }

FogUsage* FogTypedef::make_interface_usage_start() {
	return usage_manager().make_typedef_usage(*this, decl_specifiers());
}

FogUsage* FogTypedef::make_name_usage() {
	return usage_manager().make_name_usage(*this);
}

const FogMetaType& FogTypedef::meta_type() const { return FogMetaType::typedef_type(); }

FogTargetFile* FogTypedef::name_file() {
	if (is_free())
		return 0;       //   Name-scope typedefs can be generated on demand.
	else
		return Super::name_file();
}

FogTypedef::TypedefOf FogTypedef::of(FogMakerContext& makerContext) {
	const FogSpecifier& aSpecifier = makerContext.specifier();
	const FogTypedefSpecifier* typedefSpecifier = aSpecifier.get_typedef_specifier(makerContext.dynamic_token());
	
	if (!typedefSpecifier)
		return INVALID;
		
	return typedefSpecifier->has_function_signature() ? FUNCTION : NAME;
}

std::ostream& FogTypedef::print_viz(std::ostream& s) const {
	s << "\"typedef: ";
	print_this(s);
	return s << '\"';
}

bool FogTypedef::resolve_semantics(FogSemanticsContext& theSemantics) const {
	switch (_of) {
	case FUNCTION: theSemantics.assign(FogSemantics::IS_FUNCTION_DEFINITION); break;
	
	case NAME: theSemantics.assign(FogSemantics::IS_OBJECT_DECLARATION); break;
	
	case TYPE: theSemantics.assign(FogSemantics::IS_TYPEDEF_DECLARATION); break;
	
	default: theSemantics.reset(); break;
	}
	
	return true;
}
