#include <Prim/PrimIncludeAll.h>


#ifdef HAS_DIRENT_H 


TYPEINFO_SINGLE(PrimDirectoryIterator, PrimPrint)

//  
//  	Open a directory stream to aFileName.
//  
PrimDirectoryIterator::PrimDirectoryIterator(const char *aFileName)
:
 _path(aFileName),
 _dir(aFileName ? opendir(aFileName) : 0),
 _error()
{
 if (_dir)
  operator++();
}

PrimDirectoryIterator::~PrimDirectoryIterator()
{
 if (_dir)
  closedir(_dir);
}

void PrimDirectoryIterator::operator++()
{
 if (_dir)
 {
  const struct dirent *anEntry = readdir(_dir);
  _error = errno;
  if (anEntry)
   _file = PrimStringHandle(anEntry->d_name, anEntry->d_reclen);
  else
  {
   closedir(_dir);
   _dir = 0;
  }
 }
}

std::ostream& PrimDirectoryIterator::print_this(std::ostream& s) const
{
 return s << path() << " / " << file();
}
#endif
