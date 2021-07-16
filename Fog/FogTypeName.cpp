#include <Fog/FogIncludeAll.h>













TYPEINFO_SINGLE(FogTypeName, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogTypeName)
PRIMREFS_IMPL(FogTypeName)
PRIMMAPOFREFS_IMPL(FogTypeName)
TMPL_HACK_FIX_DO(FogTypeName)

FogTypeName::FogTypeName()
:
 _built_in(false)
{}

FogTypeName::FogTypeName(FogMakerContext& makerContext, bool builtIn)
:
 Super(makerContext),
 _built_in(builtIn)
{
 CONDMSG(Fog::debug_make(), "Created " << *this);
}

FogTypeName::~FogTypeName() {}

void FogTypeName::add_use_by(FogEntity& anEntity, const FogOfUseBy& ofUseBy)
{
 if (!is_null())       //   Null entities signal null behaviour.
  Super::add_use_by(anEntity, ofUseBy);
}

int FogTypeName::compare(const FogTypeNameRefToConst *p1, const FogTypeNameRefToConst *p2)
{
 return Super::compare(**p1, **p2);
}

void FogTypeName::emit_forward_reference(FogStream& s)
{
 FogProgressMonitor aMonitor("Emitting forward reference for", *this);
 if (!inner_scope().is_name_space())
 {
  ERRMSG("Forward reference to " << viz(*this) << " was not be emitted (must not be nested).");
  s << "// class " << global_signature_id() << ";\n";
 }
 else
 {
  WRNMSG("\"class\" assumed for forward reference to \"" << short_id() << '\"');
  s << "class " << short_id() << ";\n";
 }
}

FogUsage& FogTypeName::interface_usage_finish() { return name_usage(); }

bool FogTypeName::is_type() const { return true; }

FogUsage *FogTypeName::make_name_usage()
{
 return _built_in ? 0 : usage_manager().make_name_usage(*this);
}

const FogMetaType& FogTypeName::meta_type() const { return FogMetaType::typename_type(); }
FogTargetFile *FogTypeName::name_file() { return 0; }

std::ostream& FogTypeName::print_this(std::ostream& s) const
{
 s << global_signature_id();
 return s;
}

std::ostream& FogTypeName::print_viz(std::ostream& s) const
{
 s << "\"typename: ";
 print_this(s);
 return s << '\"';
}
