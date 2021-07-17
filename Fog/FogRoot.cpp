#include <Fog/FogIncludeAll.h>



















TYPEINFO_SINGLE(FogRoot, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogRoot)
PRIMREFS_IMPL(FogRoot)
TMPL_HACK_FIX_DO(FogRoot)

FogRoot::FogRoot()
	:
	Super(FogTag::namespace_tag(), *this, *PrimIdHandle("$null$")),
	_program("?"),
	_reader(*this),
	_root_context(*this) {}

FogRoot::FogRoot(const PrimId& anId)
	:
	Super(FogTag::namespace_tag(), *this, *PrimIdHandle(anId ? anId.str() : "<unnamed>")),
	_program("?"),
	_reader(*this),
	_meta_base(new FogAuto(*this), FogScopeRef::ADOPT),
	_std(new FogStd(*this), FogScopeRef::ADOPT),
	_root_context(*this) {
	CONDMSG(Fog::debug_make(), "Created " << *this);
	install_types();
	add_scope(meta_base());
	add_scope(*_std);
	
	if (!anId) {
		set_utility(FogUtility::utility_utility());
		set_interface_file(&FogTargetFile::mutable_null());
		set_implementation_file(&FogTargetFile::mutable_null());
	}
}

FogRoot::~FogRoot() {
	destroy();   //   Destroy inter-child references, so that destruction of the root destroys the tree.
}

void FogRoot::add_missing_interface_file(const PrimId& anId) {
	_missing_interface_file.add(anId);
}

void FogRoot::add_missing_implementation_file(const PrimId& anId) {
	_missing_implementation_file.add(anId);
}

void FogRoot::add_missing_implementation(const PrimId& anId) {
	_missing_implementation.add(anId);
}

bool FogRoot::compile() {
	//  	for (FogScopeConstMapOfRefIterator p(_linkages); p; ++p)
	//  		p->do_meta_construct();
	//  	do_meta_construct();
	static FogStaticDeriveContext deriveContext(*this);
	
	for (FogScopeConstMapOfRefIterator p2(_linkages); p2; ++p2)
		p2->do_derive(deriveContext);
		
	do_derive(deriveContext);
	//  	for (FogScopeConstMapOfRefIterator p(_linkages); p; ++p)
	//  		p->do_compile();
	static FogStaticCompileContext compileContext(*this);
	
	for (FogScopeConstListOfRefIterator p(_scopes); p; ++p)
		p->do_compile(compileContext);
		
	//  	for (FogScopeConstMapOfRefIterator p(_linkages); p; ++p)
	//  		p->create_usages();
	for (FogScopeConstListOfRefIterator p4(_scopes); p4; ++p4)
		p4->create_usages();
		
	_usage.compile();
	_files.compile();
	_source_list = _reader.sources();
	_source_list.sort(&PrimId::sort_compare);
	_missing_interface_file.sort(&PrimId::sort_compare);
	_missing_implementation.sort(&PrimId::sort_compare);
	_missing_implementation_file.sort(&PrimId::sort_compare);
	return true;
}

const FogInline& FogRoot::default_inline() const {
	const FogSourceFile* sourceFile = _reader.source_file();
	return sourceFile && !sourceFile->source_type().is_top()
	       ? FogInline::in_interface() : FogInline::out_of_line();
}

void FogRoot::destroy() {
	Super::destroy();
	_usage.destroy();
	_files.destroy();
	_linkages.vacate();
	_scopes.vacate();
}

void FogRoot::emit_banner(FogStream& s) const {
	if (!Fog::nobanner()) {
		s << "/*!$@FOG@$!\n *\tGenerated at " << _time << " *\n *\tby " << _program << "\n *\n *\tfrom\n";
		
		for (PrimIdConstListIterator p(_source_list); p; p++)
			s << " *\t\t" << *p << '\n';
			
		s << " */\n\n";
	}
}

void FogRoot::emit_dependencies(const PrimString& fileName) const {
	PrimOfstreamMonitor oFile("Creating", fileName, ".");
	
	if (!Fog::nobanner())
		oFile.s() << "#\n#\tGenerated at " << _time << "#\n#\tby " << _program << "\n#\n\n";
		
	oFile.s() << fileName << " :";
	
	for (PrimIdConstListIterator p(_source_list); p; p++)
		oFile.s() << " \\\n\t" << *p;
		
	oFile.s() << '\n';
	_files.emit_dependencies(oFile.s(), _source_list);
}

void FogRoot::emit_files() {
	if (!compiled())
		mutate().compile();
		
	_files.emit(*this);
	
	if (Super::short_id()) {
		if (_missing_interface_file.tally()) {
			PrimOstrstream s;
			s << "The following entities have no interface file:";
			
			for (PrimIdConstListIterator p(_missing_interface_file); p; ++p)
				s << "\n\t" << *p;
				
			WRNMSGZ(s.str());
		}
		
		if (_missing_implementation.tally()) {
			PrimOstrstream s;
			s << "The following entities have no implementation:";
			
			for (PrimIdConstListIterator p(_missing_implementation); p; ++p)
				s << "\n\t" << *p;
				
			WRNMSGZ(s.str());
		}
		
		if (_missing_implementation_file.tally()) {
			PrimOstrstream s;
			s << "The following entities have no implementation file:";
			
			for (PrimIdConstListIterator p(_missing_implementation_file); p; ++p)
				s << "\n\t" << *p;
				
			WRNMSGZ(s.str());
		}
	}
}

void FogRoot::emit_template_prefix(FogEmitContext& emitContext) const {}

void FogRoot::find_entities_in(FogScopeContext& inScope, FogEntityFinding& theFinding) const {
	switch (inScope.in_scope()) {
	case IN_THIS_SCOPE:
	case IN_BASE_SCOPE:
	case IN_ANY_SCOPE:
		theFinding.add(mutate());   //  .bugbug cast - cheap form of inScope.global_scope()
		break;
		
	case IN_BASE_NOT_THIS_SCOPE:
	case IN_ANY_NOT_THIS_SCOPE:
		break;
	}
}

const PrimId& FogRoot::global_id() const {
	return PrimId::null();
}

const PrimId& FogRoot::global_signature_id() const {
	return PrimId::null();
}

FogUsage& FogRoot::inline_usage() {
	return FogUsage::mutable_null();
}

FogBuiltInScope* FogRoot::install_built_in(FogBuiltInScope* theType, const char* p1) {
	PrimIdHandle typeId(p1);
	
	if (!theType) {
		theType = new FogBuiltInScope(*this, *typeId);
		
		if (theType) {
			add_type(*typeId, *theType);
			theType->annul();
		}
	}
	else
		add_type(*typeId, *theType);
		
	return theType;
}

FogBuiltInScope* FogRoot::install_built_in(FogBuiltInScope* theType, const char* p1, const char* p2) {
	if (!theType) {
		PrimOstrstream s;
		s << p1 << ' ' << p2;
		size_t aSize = s.pcount();
		PrimIdHandle typeId(s.str(), aSize);
		theType = new FogBuiltInScope(*this, *typeId);
		
		if (theType) {
			add_type(*typeId, *theType);
			theType->annul();
		}
	}
	
	for (int i = 1; i <= 3; i++) {
		const char* tmp;
		const char* ps[2] = { p1, p2 };
		
		if (i & 1)
			tmp = ps[0], ps[0] = ps[1], ps[1] = tmp;
			
		PrimOstrstream s;
		s << ps[0] << ' ' << ps[1];
		size_t aSize = s.pcount();
		PrimIdHandle typeId(s.str(), aSize);
		
		if (!find_local_type(*typeId))
			add_type(*typeId, *theType);
	}
	
	return theType;
}

FogBuiltInScope* FogRoot::install_built_in(FogBuiltInScope* theType,
        const char* p1, const char* p2, const char* p3) {
	if (!theType) {
		PrimOstrstream s;
		s << p1 << ' ' << p2 << ' ' << p3;
		size_t aSize = s.pcount();
		PrimIdHandle typeId(s.str(), aSize);
		theType = new FogBuiltInScope(*this, *typeId);
		
		if (theType) {
			add_type(*typeId, *theType);
			theType->annul();
		}
	}
	
	for (int i = 1; i <= 7; i++) {
		const char* tmp;
		const char* ps[3] = { p1, p2, p3 };
		
		if (i & 1)
			tmp = ps[0], ps[0] = ps[1], ps[1] = tmp;
			
		if (i & 2)
			tmp = ps[0], ps[0] = ps[2], ps[2] = tmp;
			
		if (i & 4)
			tmp = ps[1], ps[1] = ps[2], ps[2] = tmp;
			
		PrimOstrstream s;
		s << ps[0] << ' ' << ps[1] << ' ' << ps[2];
		size_t aSize = s.pcount();
		PrimIdHandle typeId(s.str(), aSize);
		
		if (!find_local_type(*typeId))
			add_type(*typeId, *theType);
	}
	
	return theType;
}

void FogRoot::install_types() {
	Super::install_types();
	
	if (!Fog::no_bool_type())
		install_built_in(0, "bool");
		
	install_built_in(0, "char");
	install_built_in(0, "signed", "char");
	install_built_in(0, "unsigned", "char");
	install_built_in(0, "double");
	install_built_in(0, "long", "double");
	install_built_in(0, "float");
	FogBuiltInScope* intType = install_built_in(0, "int");
	FogBuiltInScope* signedIntType = install_built_in(0, "signed", "int");
	FogBuiltInScope* unsignedIntType = install_built_in(0, "unsigned", "int");
	install_built_in(signedIntType, "signed");
	install_built_in(unsignedIntType, "unsigned");
	FogBuiltInScope* longType = install_built_in(0, "long");
	FogBuiltInScope* signedLongType = install_built_in(0, "signed", "long");
	FogBuiltInScope* unsignedLongType = install_built_in(0, "unsigned", "long");
	install_built_in(longType, "long", "int");
	install_built_in(signedLongType, "signed", "long", "int");
	install_built_in(unsignedLongType, "unsigned", "long", "int");
	FogBuiltInScope* shortType = install_built_in(0, "short");
	FogBuiltInScope* signedShortType = install_built_in(0, "signed", "short");
	FogBuiltInScope* unsignedShortType = install_built_in(0, "unsigned", "short");
	install_built_in(shortType, "short", "int");
	install_built_in(signedShortType, "signed", "short", "int");
	install_built_in(unsignedShortType, "unsigned", "short", "int");
	install_built_in(0, "void");
	
	if (!Fog::no_wchar_t_type())
		install_built_in(0, "wchar_t");
		
	if (Fog::long_long_type()) {
		install_built_in(0, "long", "long");
		install_built_in(0, "unsigned", "long", "long");
	}
}

FogUsage& FogRoot::interface_usage_finish() {
	return FogUsage::mutable_null();
}

FogUsage& FogRoot::interface_usage_start() {
	return FogUsage::mutable_null();
}

bool FogRoot::is_global_scope() const {
	return true;
}

bool FogRoot::is_name_scope() const {
	return true;
}

bool FogRoot::is_name_space() const {
	return true;
}

const PrimId& FogRoot::local_id() const {
	return PrimId::null();
}

const PrimId& FogRoot::local_signature_id() const {
	return PrimId::null();
}

const PrimId& FogRoot::long_id() const {
	return PrimId::null();
}

FogScope* FogRoot::make_linkage(const FogLinkageSpecifier& rawLinkage) {
	static unsigned long externCounter = 0;
	const PrimId& linkageName = rawLinkage.linkage();
	PrimOstrstream externId;
	externId << Fog::extern_prefix();
	const char* p = linkageName.str();
	
	if (p)
		for (; *p; p++)
			externId << ((isalnum(*p) || (*p == '_')) ? *p : '_');
			
	//  	externId << '_' << externCounter++;
	const PrimId& linkageId = externId.id();
	FogScope* aScope = _linkages.find(linkageId);
	
	if (!aScope) {
		aScope = new FogLinkage(*this, linkageId, linkageName);
		
		if (aScope)
			_linkages.adopt(*aScope);
	}
	
	return aScope;
}

FogScope& FogRoot::name_emit_scope() {
	return *this;
}

FogScope& FogRoot::name_emit_transient_scope() {
	return *this;
}

FogScope& FogRoot::name_scope() {
	return *this;
}

FogScope& FogRoot::name_space() {
	return *this;
}

FogUsage& FogRoot::name_usage() {
	return FogUsage::mutable_null();
}

std::ostream& FogRoot::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	_usage.print_depth(s, aDepth);
	_files.print_depth(s, aDepth);
	
	if (_linkages.tally()) {
		s << indent(aDepth) << "extern linkages\n";
		_linkages.print_depth(s, aDepth + 1);
	}
	
	if (_scopes.tally()) {
		s << indent(aDepth) << "construction ordering\n";
		_scopes.print_members(s, aDepth + 1);
	}
	
	return s;
}

std::ostream& FogRoot::print_long_id(std::ostream& s) const {
	return s << "/*global*/";
}

std::ostream& FogRoot::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	_usage.print_members(s, aDepth);
	_files.print_members(s, aDepth);
	
	if (_linkages.tally()) {
		s << indent(aDepth) << "extern linkages\n";
		_linkages.print_members(s, aDepth + 1);
	}
	
	if (_scopes.tally()) {
		s << indent(aDepth) << "construction ordering\n";
		_scopes.print_members(s, aDepth + 1);
	}
	
	return s;
}

std::ostream& FogRoot::print_this(std::ostream& s) const {
	if (is_null())
		s << "$null$";
	else
		s << "/*global*/";
		
	return s;
}

std::ostream& FogRoot::print_viz(std::ostream& s) const {
	return s << "\"the global namespace\"";
}

FogReader& FogRoot::reader() {
	return _reader;
}

void FogRoot::set_new_utility(const FogUtility& aUtility) {}

const PrimId& FogRoot::short_id() const {
	return PrimId::null();
}
