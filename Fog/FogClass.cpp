#include <Fog/FogIncludeAll.h>





















TYPEINFO_SINGLE(FogClass, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogClass)
PRIMREFS_IMPL(FogClass)
TMPL_HACK_FIX_DO(FogClass)

inline bool FogClass::inline_is_namespace() const {
	return tag().is_namespace_tag();
}

FogClass::FogClass()
		:
		_anonymity(NORMAL_TYPE) {}
		
FogClass::FogClass(const FogTag& aTag, FogMakerContext& makerContext, const FogAnonId *anonId)
		:
		Super(aTag, makerContext),
		_anonymity(anonymity(anonId)) {
	if (_anonymity != ANON_OBJECT)
		install_types();
}

//
//  	Make a specialisation of primaryClass.
//
FogClass::FogClass(This& primaryClass, FogMakeTemplateContext& makeTemplateContext)
		:
		Super(primaryClass, makeTemplateContext),
		_anonymity(primaryClass._anonymity) {
	if (strlen(makeTemplateContext.template_args().id().str()) < 2) {
		ERRMSG("BUG - instantiated very short arguments for " << viz(*this));
	}
	
	if (_anonymity != ANON_OBJECT)
		install_types();
}

FogClass::~FogClass() {}

FogClass::Anonymity FogClass::anonymity(const FogAnonId *anonId) {
	if (!anonId)
		return NORMAL_TYPE;
		
	if (anonId->is_abstract())
		return ANON_OBJECT;
	else
		if (anonId->is_named())
			return NAMED_ANON_TYPE;
		else
			return UNNAMED_ANON_TYPE;
}

int FogClass::compare(const FogClassRefToConst *p1, const FogClassRefToConst *p2) {
	return Super::compare(**p1, **p2);
}

const FogAccess& FogClass::default_access() const {
	return tag().is_class_tag() ? FogAccess::private_access() : FogAccess::public_access();
}

const FogInline& FogClass::default_inline() const {
	return FogInline::in_interface();
}

void FogClass::emit_forward_reference(FogStream& s) {
	if (_anonymity != NORMAL_TYPE)
		ERRMSG("BUG - should not emit forward reference to anonymous " << viz(*this));
		
	Super::emit_forward_reference(s);
}

void FogClass::emit_interface_finish(FogStream& s) const {
	if (!is_instantiation())
		s.change_to_emit_scope(scope());
}

void FogClass::emit_interface_start(FogStream& s) const {
	if (!is_instantiation()) {
		if (interface_file_source() == s.file())
			;
		else
			if (s.file())
				ERRMSG("BUG should not emit interface of " << viz(*this) << " to " << viz(*s.file()));
			else
				ERRMSG("BUG should not emit interface of " << viz(*this) << " to null file.");
				
		s.change_to_access(access());
		
		s.change_to_emit_scope(*this);
	}
}

void FogClass::emit_scope_head(FogStream& s) const {
	next(s);
	FogStaticEmitContext emitContext(s, s.outer_scope(), FogEmitContext::GLOBAL_INTERFACE);
	emit_template_prefix(emitContext);
	emitContext.start();
	
	switch (_anonymity) {
	
		case ANON_OBJECT:
			emitContext.emit_space_and_text(tag().str());
			break;
			
		case NAMED_ANON_TYPE:
		
		case UNNAMED_ANON_TYPE:
			emitContext.emit_space_and_text("typedef");
			emitContext.emit_space_and_text(tag().str());
			break;
			
		case NORMAL_TYPE:
		
		default:
			emitContext.emit_space_and_text(tag().str());
			emitContext.emit_space_and_text((has_own_interface() ? global_id() : local_id()).str());
			break;
	}
	
	emit_base_interface(emitContext);
	
	s << "\n{\n";
	dummy_blank_line(s);
	s.indent(1);
}

void FogClass::emit_scope_tail(FogStream& s) const {
	s.indent(-1);
	
	switch (_anonymity) {
	
		case ANON_OBJECT:
		
		case NORMAL_TYPE:
		
		default:
			s << "};\n";
			break;
			
		case NAMED_ANON_TYPE:
		
		case UNNAMED_ANON_TYPE:
			s << "} " << short_id() << ";\n";
			break;
	}
}

bool FogClass::has_access() const {
	return true;
}

FogClass *FogClass::is_class() {
	return this;
}

bool FogClass::is_name_scope() const {
	return true;
}

bool FogClass::is_name_space() const {
	return inline_is_namespace();
}

FogClass *FogClass::is_namespace() {
	return inline_is_namespace() ? this : 0;
}

bool FogClass::is_type() const {
	return true;
}

FogUsage *FogClass::make_interface_usage_finish() {
	return usage_manager().make_interface_usage_finish(*this);
}

FogUsage *FogClass::make_interface_usage_start() {
	return usage_manager().make_interface_usage_start(*this);
}

FogUsage *FogClass::make_name_usage() {
	return usage_manager().make_name_usage(*this);
}

FogScope& FogClass::name_emit_scope() {
	return *this;
}

FogScope& FogClass::name_emit_transient_scope() {
	if (inline_is_namespace())
		return *this;
	else
		return Super::name_emit_transient_scope();
}

FogTargetFile *FogClass::name_file() {
//  	if (scope().is_name_scope())
	if (scope().is_name_space())
		return 0;
	else
		return scope().interface_file_source();
}

FogScope& FogClass::name_scope() {
	return *this;
}

FogScope& FogClass::name_space() {
	if (inline_is_namespace())
		return *this;
	else
		return Super::name_space();
}

FogEntity *FogClass::new_template(FogMakeTemplateContext& makeTemplateContext) {
	return new FogClass(*this, makeTemplateContext);
}
