#include <Prim/PrimIncludeAll.h>





TYPEINFO_SINGLE(PrimLineContext, PrimPrint)
PRIMREF_NULL_CLASS_AND_IMPL(PrimLineContext)
PRIMSHARES_IMPL(PrimLineContext)

PRIMREFS_IMPL(PrimLineContext)

TMPL_HACK_FIX_DO(PrimLineContext)

PrimLineContext::PrimLineContext(const char *fileName, size_t lineNumber)
:
 _file(fileName),
 _line(lineNumber)
{}

PrimLineContext::PrimLineContext(const PrimString& fileName, size_t lineNumber)
:
 _file(fileName),
 _line(lineNumber)
{}

std::ostream& PrimLineContext::print_this(std::ostream& s) const
{
 return s << _file << ":" << _line;
}

std::ostream& PrimLineContext::print_viz(std::ostream& s) const
{
 return s << c_string(_file.str()) << "." << _line;
}
