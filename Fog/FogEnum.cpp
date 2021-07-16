#include <Fog/FogIncludeAll.h>
















TYPEINFO_SINGLE(FogEnum, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogEnum)
PRIMREFS_IMPL(FogEnum)
PRIMLISTOFREFS_IMPL(FogEnum)
PRIMMAPOFREFS_IMPL(FogEnum)
TMPL_HACK_FIX_DO(FogEnum)

FogEnum::FogEnum()
:
 _is_named_anon_type(false),
 _is_unnamed_anon_type(false),
 _current(0)
{}

FogEnum::FogEnum(FogMakerContext& makerContext)
:
 Super(makerContext),
 _is_named_anon_type(false),
 _is_unnamed_anon_type(false),
 _current(0)
{}

FogEnum::~FogEnum() {}

const FogAccess& FogEnum::access() const { return *_access; }

void FogEnum::add(FogEnumerator& anEnumerator)
{
 anEnumerator.set_previous(_current); 
 FogEnumerator *oldEnumerator = _map.find(anEnumerator.local_signature_id());
 if (!oldEnumerator)
 {
  _map.add(anEnumerator);
  _list.add(anEnumerator);
  _current = &anEnumerator;
 }
 else if (oldEnumerator->matches(anEnumerator))
  _current = oldEnumerator;
 else
  WRNMSG("Redeclaration of " << viz(*oldEnumerator) << " ignored.");
}

int FogEnum::compare(const FogEnumRefToConst *p1, const FogEnumRefToConst *p2)
{
 return Super::compare(**p1, **p2);
}

void FogEnum::create_usages()
{
 FogUsage& aUsage = interface_usage_finish();
 scope().interface_usage_start().add_use_by(aUsage);
 aUsage.add_use_by(scope().interface_usage_finish());
}

bool FogEnum::do_compile(FogCompileContext& inScope)
{
 FogProgressMonitor aMonitor("Compling", *this);
 return Super::do_compile(inScope);
}

void FogEnum::emit_forward_reference(FogStream& s)
{
 FogProgressMonitor aMonitor("Emitting forward reference for", *this);
 if (!name_scope().is_name_space() || _is_named_anon_type)
 {
  ERRMSG("Forward reference required for " << viz(*this));
  s << "// enum " << global_signature_id() << ";\n";
 }
 else
 {
  WRNMSG("Forward reference emitted for " << viz(*this));
  s << "enum " << short_id() << ";\n";
 }
}

void FogEnum::emit_interface(FogStream& s)
{
 FogProgressMonitor aMonitor("Emitting interface for", *this);
 if (!compiled())
  ERRMSG("BUG -- should not invoke emit_interface before compilation of " << viz(*this));
//  	if (!derivation().here())
//  		return false;
 FogStreamEntityInterface nestedStream(s, *this);
//  	s.change_to_context(*this);
//  	s.change_to_access(_access);
//  	FogStreamIndent extraIndent(s);
//  	if (_is_named_anon_type || _is_unnamed_anon_type)
//  		s << "typedef enum \n";
//  	else
  s << "enum " << short_id() << '\n';
 s << "{\n";
 for (FogEnumeratorConstListOfRefIterator p(_list); p; p++)
 {
  FogStreamIndent extraIndent(s);
  p->emit_interface(s);
  if (p.to_go() > 1)
   s << ',';
  s << '\n';
 }
//  	if (_is_named_anon_type || _is_unnamed_anon_type)
//  		s << "} " << short_id() << ";\n";
//  	else
  s << "};\n";
}

FogUsage& FogEnum::interface_usage_start() { return interface_usage_finish(); }
bool FogEnum::is_active() const { return true; }
FogEnum *FogEnum::is_enum() { return this; }
bool FogEnum::is_type() const { return true; }
FogUsage *FogEnum::make_interface_usage_finish() { return usage_manager().make_enum_usage(*this, access().decl_specifier_value()); }
const FogMetaType& FogEnum::meta_type() const { return FogMetaType::enum_type(); }
FogUsage& FogEnum::name_usage() { return interface_usage_finish(); }

std::ostream& FogEnum::print_depth(std::ostream& s, int aDepth) const
{
 Super::print_depth(s, aDepth);
 s << indent(aDepth) << "access = " << *_access<< '\n';
 if (_list.tally())
  _list.print_depth(s, aDepth+1);
 return s;
}

std::ostream& FogEnum::print_members(std::ostream& s, int aDepth) const
{
 Super::print_members(s, aDepth);
 s << indent(aDepth) << "access = " << *_access<< '\n';
 if (_list.tally())
  _list.print_members(s, aDepth+1);
 return s;
}

std::ostream& FogEnum::print_viz(std::ostream& s) const
{
 s << "\"enum: ";
 print_this(s);
 return s << '\"';
}

void FogEnum::set_access(const FogAccess& anAccess) { _access = _access->merge_access(anAccess); }

void FogEnum::set_default_access(const FogAccess& anAccess)
{ 
 if (!*_access)
  _access = anAccess;
}
