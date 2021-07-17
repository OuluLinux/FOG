#include <Prim/PrimIncludeAll.h>



TYPEINFO_SINGLE(PrimCStringMonitor, PrimMonitor)

std::ostream& PrimCStringMonitor::print_message(std::ostream& s) const {
	if (_prefix)
		s << _prefix << " ";
		
	s << c_string(_string.str());
	
	if (_suffix)
		s << _suffix;
		
	return s;
}
