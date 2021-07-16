
#ifndef PRIMIFSTREAMMONITOR_HXX
#define PRIMIFSTREAMMONITOR_HXX






//  
//  	A PrimIfstreamMonitor establishes a file open context for a file name, with an optional fall-back
//  	to a stream (such as cin). Resources allocated to open the file are automatically released on
//  	destruction. The open context is registered using the PrimMonitor protocol so that a message
//  	such as
//  
//  		Reading "file.name".
//  
//  	accompanies any error messages emitted during open. The precise format of the message is
//  
//  		<prefix> "<file-name>"<suffix>
//  
//  	for which <prefix> and <suffix> are supplied directly from construction arguments. <file-name>
//  	is similarly supplied if non-0, but falls back to a default if 0. Thus
//  
//  		PrimIfstreamMonitor aMonitor("Reading", fileName, ".", &cin, "<cin>");
//  		reda_from(aMonitor.s());
//  
//  	is a convenient way to open a file input stream to fileName, or cin according to whether
//  	fileName is 0 or not. The actual input stream is accessible as s().
//  
//  	If a default stream is required but not supplied a stream to "/dev/null" is used.
//  
class PrimIfstreamMonitor : public PrimMonitor
{
 TYPEDECL_SINGLE(PrimTermMonitor, PrimMonitor)
 enum Locked {};         //   Useless type for pointer return.
private:
 const char *_prefix;
 PrimStringHandle _file_name;
 const char *_suffix;
 std::istream *_s;        //   Stream used for input.
 PrimAdopted<std::ifstream> _file;    //   File buffer created for input.
 bool _ok;          //   True if input is sensible.
private:
 static std::istream& null_stream();
 virtual std::ostream& print_message(std::ostream& s) const;
public:
 PrimIfstreamMonitor(const char *aPrefix, const PrimString& fileName, const char *aSuffix = "",
  std::istream *sDefault = 0, const char *fDefault = 0);
//  
//  	Return non-0 if output is sensible.
//  
 operator const Locked *() const { return (const Locked *)(_ok ? this : 0); }
//  
//  	Return true if output is non sensible.
//  
//  
 bool operator!() const { return !_ok; }
 const PrimString& file_name() const { return *_file_name; }
 virtual std::ostream& print_this(std::ostream& s) const;
 std::istream& s() { return *_s; }
};
#endif
 
