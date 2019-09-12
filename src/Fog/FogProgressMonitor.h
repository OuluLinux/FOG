
#ifndef FOGPROGRESSMONITOR_HXX
#define FOGPROGRESSMONITOR_HXX



//  
//  	A FogProgressMonitor refines a PrimMonitor to implement a context message defined by a prefix text string,
//  	the visible description of a FogToken and then a suffix text string that defaults to a full stop.
//  
class FogProgressMonitor : public PrimMonitor
{
 typedef PrimMonitor Super;
 TYPEDECL_SINGLE(FogProgressMonitor, Super)
private:
 const char *_prefix;
 const FogToken& _token;
 const char *_suffix;
private:
 virtual std::ostream& print_message_line(std::ostream& s, int aDepth) const;
public:
 FogProgressMonitor(const char *aPrefix, const FogToken& aToken, const char *aSuffix = ".");
 virtual bool post_error(const PrimNotice_Error& aNotice) const;
 virtual bool post_warning(const PrimNotice_Warning& aNotice) const;
 virtual std::ostream& print_this(std::ostream& s) const;
};
#endif
 
