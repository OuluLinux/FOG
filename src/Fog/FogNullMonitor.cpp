#include <Fog/FogIncludeAll.h>




TYPEINFO_SINGLE(FogNullMonitor, Super)

FogNullMonitor::FogNullMonitor() {}
bool FogNullMonitor::has_message() const{ return false; }

