#include <Fog/FogIncludeAll.h>





TYPEINFO_SINGLE(FogSourceFile, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogSourceFile)
PRIMREFS_IMPL(FogSourceFile)
PRIMLISTOFREFS_IMPL(FogSourceFile)
PRIMMAPOFREFS_IMPL(FogSourceFile)
FOGTOKEN_ACTUAL_IMPL(FogSourceFile)
TMPL_HACK_FIX_DO(FogSourceFile)

FogSourceFile::FogSourceFile()
:
 _source_type(FogSourceFileType::invalid()),
 _has_been_read(false)
{}

FogSourceFile::FogSourceFile(const PrimId& pathedId, const PrimId& fileIdent,
  const FogSourceFileType& sourceType)
:
 _pathed_id(pathedId),
 _short_id(fileIdent),
 _source_type(sourceType),
 _has_been_read(false)
{}

//  const FogMetaType *FogSourceFile::meta_type() const { return &FogMetaType::source_file_type(); }

std::ostream& FogSourceFile::print_this(std::ostream& s) const
{
 s << _pathed_id << ", " << _source_type;
 return s;
}

std::ostream& FogSourceFile::print_viz(std::ostream& s) const
{
//  	return s << "\"file: " << c_string(_pathed_id.str()) << '"';
 return s << c_string(_pathed_id.str());
}
