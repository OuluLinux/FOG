
#ifndef PRIMCSTRINGMONITOR_HXX
#define PRIMCSTRINGMONITOR_HXX



//  
//  	A PrimCStringMonitor refines a PrimMonitor to implement a context message defined by a prefix text string,
//  	a visible replesentation of a PrimString and then a suffix text string that defaults to a full stop.
//  
class PrimCStringMonitor : public PrimMonitor
{
 TYPEDECL_SINGLE(PrimTermMonitor, PrimMonitor)
private:
 const char *_prefix;
 const PrimString& _string;
 const char *_suffix;
private:
 virtual std::ostream& print_message(std::ostream& s) const;
public:
 PrimCStringMonitor(const char *aPrefix, const PrimString& aString, const char *aSuffix = ".")
  : _prefix(aPrefix), _string(aString), _suffix(aSuffix) {}
};
#endif

