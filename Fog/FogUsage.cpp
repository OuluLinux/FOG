#include <Fog/FogIncludeAll.h>











const FogScope* FogUsage::filter_name;





class FogUsageBody : public FogUsage {
	typedef FogUsage Super;
	TYPEDECL_SINGLE(FogUsageBody, Super)
	
private:
	FogEntity& entity() {
		return *Super::entity();
	}
	
	const FogEntity& entity() const {
		return *Super::entity();
	}
	
	static PrimIdHandle make_id(FogEntity& anEntity) {
		PrimOstrstream s;
		s << anEntity.unique_id() << "'body";
		return PrimIdHandle(s.str());
	}
	
public:
	FogUsageBody(FogEntity& anEntity) : Super(*make_id(anEntity), IMPLEMENTATION, &anEntity) {}
	
	virtual void compile() {
		Super::compile();
		FogTargetFile *implFile = file();
		FogTargetFile *intFile = entity().interface_file_source();
		
		if (implFile && intFile && (implFile != intFile))
			implFile->add_include(*intFile);
	}
	
	virtual void emit(FogStream& s) {
		entity().emit_implementation(s);
	}
	
	virtual bool is_implementation() const {
		return true;
	}
	
	virtual void precompile(FogUsageManager& usageManager) {
		Super::precompile(usageManager, entity().implementation_file());
	}
};

class FogUsageFile : public FogUsage {
	typedef FogUsage Super;
	TYPEDECL_SINGLE(FogUsageFile, Super)
	
private:
	FogTargetFile& _file;
	
public:
	FogUsageFile(FogTargetFile& targetFile)
			: Super(targetFile.unique_id(), FILE, 0), _file(targetFile) {}
			
	virtual void emit(FogStream& s) {
		_file.emit_guarded_include(s);
	}
	
	virtual void precompile(FogUsageManager& usageManager) {
		Super::precompile(usageManager, &_file);
	}
	
	virtual const FogTargetFile *used_file() const {
		return &_file;
	}
};

class FogUsageFriend : public FogUsage
{
	typedef FogUsage Super;
	TYPEDECL_SINGLE(FogUsageFriend, Super)
private:
	FogScope& _of_scope;
private:
	FogEntity& entity() { return *Super::entity(); }
	const FogEntity& entity() const { return *Super::entity(); }
	static PrimIdHandle make_id(FogScope& ofScope, FogEntity& anEntity)
	{
		PrimOstrstream s;
		s << "friend " << ofScope.unique_id() << '\'' << anEntity.unique_id();
		return PrimIdHandle(s.str());
	}
public:
	FogUsageFriend(FogScope& ofScope, FogEntity& anEntity)
		: Super(*make_id(ofScope, anEntity), FRIEND, &anEntity), _of_scope(ofScope) {}
	virtual void emit(FogStream& s) { entity().emit_friend_interface(s); }
	virtual void precompile(FogUsageManager& usageManager)
		{ Super::precompile(usageManager, _of_scope.interface_file_source()); }
//	virtual std::ostream& print_viz(std::ostream& s) const;
};

class FogUsageHead : public FogUsage
{
	typedef FogUsage Super;
	TYPEDECL_SINGLE(FogUsageHead, Super)
private:
	FogEntity& entity() { return *Super::entity(); }
	const FogEntity& entity() const { return *Super::entity(); }
	static PrimIdHandle make_id(FogEntity& anEntity)
	{
		PrimOstrstream s;
		s << anEntity.unique_id() << "'head";
		return PrimIdHandle(s.str());
	}
public:
	FogUsageHead(FogEntity& anEntity) : Super(*make_id(anEntity), INTERFACE_START, &anEntity) {}
	virtual void emit(FogStream& s) { entity().emit_interface_start(s); }
	virtual bool is_interface() const { return true; }
	virtual void precompile(FogUsageManager& usageManager)
		{ Super::precompile(usageManager, entity().interface_file_sink()); }
};

//
//	The inline usage node lies between the end of a class interface and the first function implementation.
//	It is used as a common node to hang the implentation dependencies on. It precompiles away if unused
//	and positions itself in interface or implementation file according to the presence of inline functions.
//
class FogUsageInline : public FogUsage
{
	typedef FogUsage Super;
	TYPEDECL_SINGLE(FogUsageInline, Super)
private:
	FogScope& scope() { return *(FogScope *)Super::entity(); }
	const FogScope& scope() const { return *(const FogScope *)Super::entity(); }
	static PrimIdHandle make_id(FogEntity& anEntity)
	{
		PrimOstrstream s;
		s << anEntity.unique_id() << "'inline";
		return PrimIdHandle(s.str());
	}
public:
	FogUsageInline(FogScope& aScope) : Super(*make_id(aScope), INLINE, &aScope) {}
	virtual void emit(FogStream& s) {}
	virtual void precompile(FogUsageManager& usageManager)
	{
		if (!is_used())
			Super::precompile(usageManager, 0);
		else if (scope().has_inline_functions())
			Super::precompile(usageManager, scope().interface_file_source());
		else
			Super::precompile(usageManager, scope().implementation_file());
	}
};

class FogUsageInterface : public FogUsage
{
	typedef FogUsage Super;
	TYPEDECL_SINGLE(FogUsageInterface, Super)
private:
	FogEntity& entity() { return *Super::entity(); }
	const FogEntity& entity() const { return *Super::entity(); }
public:
	FogUsageInterface(FogEntity& anEntity, TypeEnum aType) : Super(anEntity.unique_id(), aType, &anEntity) {}
	virtual void emit(FogStream& s) { entity().emit_interface(s); }
	virtual bool is_interface() const { return true; }
	virtual void precompile(FogUsageManager& usageManager)
		{ Super::precompile(usageManager, entity().interface_file_source()); }
};

class FogUsageName : public FogUsage
{
	typedef FogUsage Super;
	TYPEDECL_SINGLE(FogUsageName, Super)
private:
	bool _is_weak;			// True if a suppressible nested forward reference.
private:
	FogEntity& entity() { return *Super::entity(); }
	const FogEntity& entity() const { return *Super::entity(); }
	static PrimIdHandle make_id(FogEntity& anEntity)
	{
		PrimOstrstream s;
		s << anEntity.unique_id() << "'name";
		return PrimIdHandle(s.str());
	}
public:
	FogUsageName(FogEntity& anEntity) : Super(*make_id(anEntity), NAME, &anEntity), _is_weak(false) {}
	virtual void emit(FogStream& s) { entity().emit_forward_reference(s); }
	virtual bool is_weak() const { return _is_weak; }
	virtual void precompile(FogUsageManager& usageManager)
		{ Super::precompile(usageManager, entity().name_file()); }
	virtual void set_weak() { _is_weak = true; }
};

class FogUsageTail : public FogUsage
{
	typedef FogUsage Super;
	TYPEDECL_SINGLE(FogUsageTail, Super)
private:
	FogEntity& entity() { return *Super::entity(); }
	const FogEntity& entity() const { return *Super::entity(); }
	static PrimIdHandle make_id(FogEntity& anEntity)
	{
		PrimOstrstream s;
		s << anEntity.unique_id() << "'tail";
		return PrimIdHandle(s.str());
	}
public:
	FogUsageTail(FogEntity& anEntity) : Super(*make_id(anEntity), INTERFACE_FINISH, &anEntity) {}
	virtual void emit(FogStream& s) { entity().emit_interface_finish(s); }
//	This is an interface but it doesn't count as missing tails deriuved from dependencies or definitions.
//	Only heads are unequivocably definitions.
//	virtual bool is_interface() const { return true; }
	virtual void precompile(FogUsageManager& usageManager)
		{ Super::precompile(usageManager, entity().interface_file_source()); }
};

TYPEINFO_SINGLE(FogUsage, Super)
TYPEINFO_SINGLE(FogUsageBody, Super)
TYPEINFO_SINGLE(FogUsageFile, Super)
TYPEINFO_SINGLE(FogUsageFriend, Super)
TYPEINFO_SINGLE(FogUsageHead, Super)
TYPEINFO_SINGLE(FogUsageInline, Super)
TYPEINFO_SINGLE(FogUsageInterface, Super)
TYPEINFO_SINGLE(FogUsageName, Super)
TYPEINFO_SINGLE(FogUsageTail, Super)
PRIMREF_NULL_CLAS(FogUsage)
	virtual void add_use_by(FogUsage& aUsage) {}
	virtual void emit(FogStream& s) {}
	virtual void compile() {}
	virtual void precompile(FogUsageManager& usageManager) {}
	virtual void set_weak() {}
};
PRIMREF_NULL_IMPL(FogUsage)
PRIMSHARES_IMPL(FogUsage)
PRIMREFS_IMPL(FogUsage)
PRIMLISTOFREFS_IMPL(FogUsage)
PRIMMAPOFREFS_IMPL(FogUsage)
TMPL_HACK_FIX_DO(FogUsage)

FogUsage::FogUsage()
:
//	_entity(FogEntity::mutable_null()),
	_entity(0),
//	_type(NAME),
	_type(INVALID),
	_file(0),
	_usage_number(0)
{}

FogUsage::FogUsage(const PrimId& anId, TypeEnum aType, FogEntity *anEntity)
:
	_id(anId),
	_type(aType),
	_entity(anEntity),
	_file(0),
	_usage_number(0)
{}

FogUsage::~FogUsage() {}

void FogUsage::add_precursors_to(FogTargetFile& aFile)
{
	for (FogUsageConstMapOfRefIterator p(_precursors); p; p++)
	{
		if (&aFile == p->_file)
			;
		else if (!p->entity())
			;
		else if (!p->entity()->is_typedef() || !p->entity()->name_scope().is_name_space())
			aFile.add_external(*p);
		else
			p->add_precursors_to(aFile);
	}
}

VERBOSE(int counter = 0;)
#undef CONDMSG
#define CONDMSG(file, msg) VERBOSE(std::cout << counter << " " << msg << std::endl; /*if (counter == 26) {int* i = 0; *i = 0;}*/ counter++;)

//
//	Register the dependency of this usage resulting from use by aUsage.
//
void FogUsage::add_use_by(FogUsage& aUsage)
{
	
	/*if (filter_name && filter_name != &entity()->name_scope()) {
		CONDMSG(Fog::debug_file(), "SKIPPING use of " << viz(*this) << " by " << viz(aUsage));
		return;
	}*/
	
	if (&aUsage == this)
	{
		ERRMSG("BUG - should not add_use_by " << viz(aUsage) << " to itself.");
		return;
	}
	if (!aUsage.is_null() && _used_by.add_filtered(aUsage))
	{
		if (!aUsage._use_of.add_filtered(*this))
			ERRMSG("BUG - inconsistent prior usage of " << viz(*this) << " and " << viz(aUsage));
		CONDMSG(Fog::debug_file(), "Adding use of " << viz(*this) << " by " << viz(aUsage));
	}
}

int FogUsage::compare(const FogUsageRefToConst *p1, const FogUsageRefToConst *p2)
{
	int aDelta = (*p1)->type() - (*p2)->type();
	if (aDelta)
		return aDelta;
	const FogEntity *e1 = (*p1)->_entity;
	const FogEntity *e2 = (*p2)->_entity;
	if (!e1 && !e2)
		return 0;
	if (!e1)
		return -1;
	if (!e2)
		return 1;
	aDelta = FogScope::compare(e1->inner_scope(), e2->inner_scope());
	if (aDelta)
		return aDelta;
	return FogEntity::compare(*e1, *e2);
}

//
//	Return a sort ordering such that the highly dependent files sort late.
//
int FogUsage::compare_least_dependent_first(const FogUsageRefToConst *p1, const FogUsageRefToConst *p2)
{
	size_t firstIndependence = (*p1)->_usage_number;
	size_t secondIndependence = (*p2)->_usage_number;
	return int(firstIndependence) - int(secondIndependence);
}

//
//	Compilation of a usage installs the usage in the associated file.
//
void FogUsage::compile()
{
	if (_file)
	{
		_file->add_internal(*this);
		FogUsageListOfRef useList(_precursors);
		for (FogUsageConstListOfRefIterator p(useList); p; p++)
		{
//			if (_file != p->_file)
//				_file->add_external(*p);
			if (_file == p->_file)
				;
//			else if (!p->entity().is_transparent())
			else if (!p->entity())
				;
			else if (!p->entity()->is_typedef() || !p->entity()->name_scope().is_name_space())
				_file->add_external(*p);
			else
				p->add_precursors_to(*_file);
			_precursors |= p->_precursors;
		}
	}
}

//
//	Eliminate all references so that destruction is complete and static destruction well behaved.
//
void FogUsage::destroy()
{
	_use_of.vacate();
	_used_by.vacate();
	_precursors.vacate();
	_file = 0;
}

//
//	Return true if this should ve reported as a missed implementation usage in a non-emitted file. 
//
bool FogUsage::is_implementation() const { return false; }

//
//	Return true if this should ve reported as a missed interface usage in a non-emitted file. 
//
bool FogUsage::is_interface() const { return false; }

//
//	Return true if this usage should be suppressed, unless occurring as a precursor.
//
//	e.g
//	class X
//	{
//		union Y;			// Weak name can be suppressed unless needed to disentangle.
//		union Y {... };
//	};
//
bool FogUsage::is_weak() const { return false; }

FogUsage *FogUsage::new_enum_usage(FogEntity& anEntity, const FogDeclSpecifierValue& declSpecifiers)
{
	if (declSpecifiers.is_private())
		return new FogUsageInterface(anEntity, PRIVATE_ENUM);
	else if (declSpecifiers.is_protected())
		return new FogUsageInterface(anEntity, PROTECTED_ENUM);
	else
		return new FogUsageInterface(anEntity, PUBLIC_ENUM);
}

FogUsage *FogUsage::new_file_usage(FogTargetFile& targetFile)
{
	return new FogUsageFile(targetFile);
}

FogUsage *FogUsage::new_friend_usage(FogScope& ofScope, FogEntity& anEntity)
{
	return new FogUsageFriend(ofScope, anEntity);
}

FogUsage *FogUsage::new_function_usage(FogEntity& anEntity, const FogDeclSpecifierValue& declSpecifiers)
{
	bool isStatic = declSpecifiers.is_static();
	if (declSpecifiers.is_private())
		return new FogUsageInterface(anEntity, isStatic ? STATIC_PRIVATE_FUNCTION : PRIVATE_FUNCTION);
	else if (declSpecifiers.is_protected())
		return new FogUsageInterface(anEntity, isStatic ? STATIC_PROTECTED_FUNCTION : PROTECTED_FUNCTION);
	else
		return new FogUsageInterface(anEntity, isStatic ? STATIC_PUBLIC_FUNCTION : PUBLIC_FUNCTION);
}

FogUsage *FogUsage::new_implementation_usage(FogEntity& anEntity)
{
	return new FogUsageBody(anEntity);
}

FogUsage *FogUsage::new_inline_usage(FogScope& aScope)
{
	return new FogUsageInline(aScope);
}

FogUsage *FogUsage::new_interface_usage_finish(FogEntity& anEntity)
{
	return new FogUsageTail(anEntity);
}

FogUsage *FogUsage::new_interface_usage_start(FogEntity& anEntity)
{
	return new FogUsageHead(anEntity);
}

FogUsage *FogUsage::new_name_usage(FogEntity& anEntity)
{
	return new FogUsageName(anEntity);
}

FogUsage *FogUsage::new_typedef_usage(FogEntity& anEntity, const FogDeclSpecifierValue& declSpecifiers)
{
	if (declSpecifiers.is_private())
		return new FogUsageInterface(anEntity, PRIVATE_TYPEDEF);
	else if (declSpecifiers.is_protected())
		return new FogUsageInterface(anEntity, PROTECTED_TYPEDEF);
	else
		return new FogUsageInterface(anEntity, PUBLIC_TYPEDEF);
}

FogUsage *FogUsage::new_variable_usage(FogEntity& anEntity, const FogDeclSpecifierValue& declSpecifiers)
{
	bool isStatic = declSpecifiers.is_static();
	if (declSpecifiers.is_private())
		return new FogUsageInterface(anEntity, isStatic ? STATIC_PRIVATE_VARIABLE : PRIVATE_VARIABLE);
	else if (declSpecifiers.is_protected())
		return new FogUsageInterface(anEntity, isStatic ? STATIC_PROTECTED_VARIABLE : PROTECTED_VARIABLE);
	else
		return new FogUsageInterface(anEntity, isStatic ? STATIC_PUBLIC_VARIABLE : PUBLIC_VARIABLE);
}

//
//	Pick an arbitrary use-of as part of usageManager's identification of a detected cycle.
//	Returns that use-of. In the impossible event that none is available, an error has occurred
//	and this usage is changed to independent breaking a cycle. 0 is then returned.
//
FogUsage *FogUsage::pick_cyclic_use_of(FogUsageManager& usageManager)
{
	do
	{
		for (FogUsageConstMapOfRefIterator p(_use_of); p; p++)
		{
			if (p->_used_by.find(unique_id()))
				return &*p;
			WRNMSG("BUG - " << viz(*this) << " uses but was not registered as a user of " << viz(*p) << ".");
			_use_of -= *p;
		}
	} while (_use_of.tally());							// Repeated loop since map domain unstable.
	WRNMSG("BUG - " << viz(*this) << " was independent.");
	usageManager.promote_to_independent(*this);
	return 0;
}

//
//	Pre-compilation of a usage installs the usage in the appropriate independent/dependent in the usageManager,
//	and establioshes the file in which this usage appears.
//
void FogUsage::precompile(FogUsageManager& usageManager, FogTargetFile *aFile)
{
	_file = aFile;
	if (_use_of.tally())
		usageManager.add_dependent(*this);
	else
		usageManager.add_independent(*this);
	_precursors = _use_of;
}

std::ostream& FogUsage::print_depth(std::ostream& s, int aDepth) const
{
	Super::print_depth(s, aDepth);
	if (is_weak())
		s << indent(aDepth) << "weak\n";
	if (_entity)
		s << indent(aDepth) << "entity: " << *_entity << "\n";
	if (_file)
		s << indent(aDepth) << "file: " << *_file << "\n";
	if (_precursors.tally())
	{
		s << indent(aDepth) << "precursors\n";
		_precursors.print_members(s, aDepth+1);
	}
	if (_use_of.tally())
	{
		s << indent(aDepth) << "uses\n";
		_use_of.print_members(s, aDepth+1);
	}
	if (_used_by.tally())
	{
		s << indent(aDepth) << "used by\n";
		_used_by.print_members(s, aDepth+1);
	}
	return s;
}

std::ostream& FogUsage::print_members(std::ostream& s, int aDepth) const
{
	Super::print_members(s, aDepth);
	if (_use_of.tally())
	{
		s << indent(aDepth) << "uses\n";
		_use_of.print_members(s, aDepth+1);
	}
	if (_used_by.tally())
	{
		s << indent(aDepth) << "used by\n";
		_used_by.print_members(s, aDepth+1);
	}
	return s;
}

std::ostream& FogUsage::print_this(std::ostream& s) const
{
	return s << _usage_number << ", " << type() << ", " << id();
}

std::ostream& FogUsage::print_viz(std::ostream& s) const
{
	return s << '"' << type() << ": " << id() << '"';
}

//
//	Eliminate all uses of the name, once this usage has been allocated a usage independence number.
//
void FogUsage::remove_all_uses(FogUsageManager& usageManager, size_t usageNumber)
{
	_usage_number = usageNumber;
	CONDMSG(Fog::debug_file(), *this << " assigned usage independence number " << _usage_number);
	for (FogUsageConstMapOfRefIterator p(_used_by); p; p++)
		p->remove_just_use_of(*this, usageManager);
	_used_by.vacate();
}

//
//	Eliminate the usage of the name of aUsage by _entity, once _entity has a usage independence number.
//
void FogUsage::remove_just_use_of(FogUsage& aUsage, FogUsageManager& usageManager)
{
	if (!_use_of.remove(aUsage.id()))
		ERRMSG("BUG - should not remove un-use-of " << viz(aUsage) << " from " << viz(*this));
	CONDMSG(Fog::debug_file(), "Removed use of " << aUsage << " by " << *this);
	if (!_use_of.tally())
		usageManager.promote_to_independent(*this);
}

//
//	Eliminate the use-of and used-by relationship for the use of aUsage._file by _entity, once
//	_entity has been allocated a usage independence number, removing an impediment on
//	allocating a usage independence number to _file.
//
void FogUsage::remove_use_of(FogUsage& aUsage, FogUsageManager& usageManager)
{
	if (!aUsage._used_by.remove(id()))
		ERRMSG("BUG - should not remove un-used-by unused " << viz(*this) << " from " << viz(aUsage));
	remove_just_use_of(aUsage, usageManager);
}

const FogScope *FogUsage::scope() const { return _entity ? _entity->is_scope() : 0; }

void FogUsage::set_weak()
{ 
	ERRMSG("BUG - should not FogUsage::set_weak for " << viz(*this));
}
