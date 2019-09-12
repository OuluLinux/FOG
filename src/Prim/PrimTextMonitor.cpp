#include <Prim/PrimIncludeAll.h>




TYPEINFO_SINGLE(PrimTextMonitor, PrimMonitor)

bool PrimTextMonitor::has_message() const { return _message != 0; }

std::ostream& PrimTextMonitor::print_message(std::ostream& s) const
{
 if (_message)
  s << _message;
 return s;
}

std::ostream& PrimTextMonitor::print_this(std::ostream& s) const
{
 if (_message)
  s << _message;
 return s;
}
