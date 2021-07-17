#include <Fog/FogIncludeAll.h>













TYPEINFO_SINGLE(FogUnique, Super)
FOGTOKEN_ACTUAL_IMPL(FogUnique)
PRIMREF_NULL_CLAS(FogUnique)
virtual void emit_forward_reference(FogStream& s) {}
virtual FogTargetFile* implementation_file() { return 0; }
virtual FogUsage& implementation_usage() { return FogUsage::mutable_null(); }
virtual FogTargetFile* interface_file_source() { return 0; }
virtual FogTargetFile* name_file() { return 0; }
virtual FogUsage& name_usage() { return FogUsage::mutable_null(); }
virtual FogUsage* new_implementation_usage() { return 0; }
virtual FogUsage* new_interface_usage() { return 0; }
virtual FogUsage* new_name_usage() { return 0; }
virtual std::ostream& print_viz(std::ostream& s) const { return s << "\"null-unique:\""; }
};
PRIMREF_DERIVED_NULL_IMPL(FogUnique)
PRIMREFS_IMPL(FogUnique)
PRIMMAPOFREFS_IMPL(FogUnique)
TMPL_HACK_FIX_DO(FogUnique)

//
//  	Construct a null object.
//
FogUnique::FogUnique()
	:
	_name_usage(0),
	_interface_usage_start(0),
	_interface_usage_finish(0),
	_implementation_usage(0),
	_implementation_file(0),
	_interface_file(0)
{}

//
//  	Construct a named object.
//
FogUnique::FogUnique(const PrimId& shortId, const PrimId& longId)
	:
	_short_id(shortId),
	_local_id(shortId),
	_local_signature_id(shortId),
	_long_id(longId),
	_global_id(longId),
	_global_signature_id(longId),
	_utility(FogUtility::emit_utility()),
	_name_usage(0),
	_interface_usage_start(0),
	_interface_usage_finish(0),
	_implementation_usage(0),
	_implementation_file(0),
	_interface_file(0)
{}

//
//  	Create a primary entity.
//
FogUnique::FogUnique(const FogMakerContext& makerContext)
	:
	_short_id(makerContext.short_id()),
	_local_id(makerContext.local_id()),
	_local_signature_id(makerContext.local_signature_id()),
	_long_id(makerContext.long_id()),
	_global_id(makerContext.global_id()),
	_global_signature_id(makerContext.global_signature_id()),
	_utility(makerContext.utility()),
	_name_usage(0),
	_interface_usage_start(0),
	_interface_usage_finish(0),
	_implementation_usage(0),
	_implementation_file(0),
	_interface_file(_utility->is_hash() ? makerContext.target_file() : 0)
{}

//
//  	Create a (partial) specification of the entity template <templateParameters> anId<templateSuffix>
//  	in aScope.
//
FogUnique::FogUnique(const FogMakeTemplateContext& makeTemplateContext)
	:
	_short_id(makeTemplateContext.short_id()),
	_local_id(makeTemplateContext.local_id()),
	_local_signature_id(makeTemplateContext.local_signature_id()),
	_long_id(makeTemplateContext.long_id()),
	_global_id(makeTemplateContext.global_id()),
	_global_signature_id(makeTemplateContext.global_signature_id()),
	_utility(makeTemplateContext.utility()),
	_name_usage(0),
	_interface_usage_start(0),
	_interface_usage_finish(0),
	_implementation_usage(0),
	_implementation_file(0),
	_interface_file(_utility->is_hash() ? makeTemplateContext.target_file() : 0)
{}

int FogUnique::compare(const FogUnique& p1, const FogUnique& p2) {
	int aDelta = p1.sort_class() - p2.sort_class();
	
	if (aDelta)
		return aDelta;
		
	aDelta = p1.global_id().compare(p2.global_id());
	
	if (aDelta)
		return aDelta;
		
	return p1.global_signature_id().compare(p2.global_signature_id());
}

int FogUnique::compare(const FogUniqueRefToConst* p1, const FogUniqueRefToConst* p2) {
	int aDelta = (*p1)->sort_class() - (*p2)->sort_class();
	
	if (aDelta)
		return aDelta;
		
	aDelta = (*p1)->global_id().compare((*p2)->global_id());
	
	if (aDelta)
		return aDelta;
		
	return (*p1)->global_signature_id().compare((*p2)->global_signature_id());
}

//
//  	Eliminate all references so that destruction is complete and static destruction well behaved.
//
void FogUnique::destroy() {
	Super::destroy();
	_implementation_usage = 0;
	_interface_usage_start = 0;
	_interface_usage_finish = 0;
	_name_usage = 0;
}

//
//  	Emit any text required to define the implementation of this declaration.
//
void FogUnique::emit_implementation(FogStream& s) {}

//
//  	Emit any text required to define the interface of this declaration.
//
void FogUnique::emit_interface(FogStream& s) {}

//
//  	Emit any text required to define the finish of the interface of this declaration.
//
void FogUnique::emit_interface_finish(FogStream& s) const {}

//
//  	Emit any text required to define the start of the interface of this declaration.
//
void FogUnique::emit_interface_start(FogStream& s) const {}

const PrimId& FogUnique::file_id() const { return *_short_id; }
const PrimId& FogUnique::global_id() const { return *_global_id; }
const PrimId& FogUnique::global_signature_id() const { return *_global_signature_id; }

FogUsage& FogUnique::implementation_usage() {
	if (!_implementation_usage) {
		_implementation_usage = make_implementation_usage();
		
		if (!_implementation_usage)
			_implementation_usage = &FogUsage::mutable_null();
	}
	
	return *_implementation_usage;
}

FogUsage& FogUnique::interface_usage_finish() {
	if (!_interface_usage_finish) {
		_interface_usage_finish = make_interface_usage_finish();
		
		if (!_interface_usage_finish)
			_interface_usage_finish = &FogUsage::mutable_null();
	}
	
	return *_interface_usage_finish;
}

FogUsage& FogUnique::interface_usage_start() {
	if (!_interface_usage_start) {
		_interface_usage_start = make_interface_usage_start();
		
		if (!_interface_usage_start)
			_interface_usage_start = &FogUsage::mutable_null();
	}
	
	return *_interface_usage_start;
}

const PrimId& FogUnique::local_id() const { return *_local_id; }
const PrimId& FogUnique::local_signature_id() const { return *_local_signature_id; }
const PrimId& FogUnique::long_id() const { return *_long_id; }

FogUsage* FogUnique::make_implementation_usage() {
	ERRMSG("BUG - should not attempt to create an implementation_usage for " << viz(*this));
	return 0;
}

FogUsage* FogUnique::make_interface_usage_finish() {
	ERRMSG("BUG - should not attempt to create an interface_usage_finish for " << viz(*this));
	return 0;
}

FogUsage* FogUnique::make_interface_usage_start() {
	ERRMSG("BUG - should not attempt to create an interface_usage_start for " << viz(*this));
	return 0;
}

FogUsage* FogUnique::make_name_usage() {
	ERRMSG("BUG - should not attempt to create a name_usage for " << viz(*this));
	return 0;
}

FogUsage& FogUnique::name_usage() {
	if (!_name_usage) {
		_name_usage = make_name_usage();
		
		if (!_name_usage)
			_name_usage = &FogUsage::mutable_null();
	}
	
	return *_name_usage;
}

std::ostream& FogUnique::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	s << indent(aDepth) << "utility = " << utility() << '\n';
	return s;
}

std::ostream& FogUnique::print_long_id(std::ostream& s) const {
	return s << _long_id;
}

std::ostream& FogUnique::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	s << indent(aDepth) << "utility = " << utility() << '\n';
	return s;
}

std::ostream& FogUnique::print_this(std::ostream& s) const {
	return s << global_signature_id();
}

std::ostream& FogUnique::print_viz(std::ostream& s) const {
	s << "\"entity: ";
	print_this(s);
	return s << '\"';
}

void FogUnique::set_implementation_file(FogTargetFile* aFile) {
	_implementation_file = aFile;
	
	if (!Fog::debug_file())
		;
	else if (!aFile)
		DIAMSG(viz(*this) << " configured to emit no implementation.");
	else
		DIAMSG(viz(*this) << " configured to emit implementation to " << viz(*aFile));
}

void FogUnique::set_interface_file(FogTargetFile* aFile) {
	_interface_file = aFile;
	
	if (!Fog::debug_file())
		;
	else if (!aFile)
		DIAMSG(viz(*this) << " configured to emit no interface.");
	else
		DIAMSG(viz(*this) << " configured to emit interface to " << viz(*aFile));
}

void FogUnique::set_new_utility(const FogUtility& aUtility) {
	if (is_null())
		ERRMSG("BUG - should not set_new_utility for " << viz(*this));
	else {
		if (Fog::debug_utility())
			DIAMSG("Utility of " << viz(*this) << " changing from " << utility().str() << " to " << aUtility.str());
			
		_utility = aUtility;
	}
}

const PrimId& FogUnique::short_id() const { return *_short_id; }
int FogUnique::sort_class() const { return 0; }
const FogUtility& FogUnique::utility() const { return *_utility; }
