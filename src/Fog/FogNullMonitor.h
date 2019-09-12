
#ifndef FOGNULLMONITOR_HXX
#define FOGNULLMONITOR_HXX



//  
//  	A FogNullMonitor defines a null monitor - a monitor without a message. A null monitor is
//  	temporarily installed at the top of the notice board hierarchy to ensure that error
//  	messages are reported directly, rather than being buffered by a parse marker. The null
//  	functionality herre therefore ensures that default behaviour occurs, supplanting the
//  	redirection from FogTokenMonitor and its derivations.
//  
class FogNullMonitor : public PrimMonitor
{
 typedef PrimMonitor Super;
 TYPEDECL_SINGLE(FogNullMonitor, Super)
public:
 FogNullMonitor();
 virtual bool has_message() const;
};
#endif

