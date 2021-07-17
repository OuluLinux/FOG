
#ifndef PRIMOFSTREAMMONITOR_HXX
#define PRIMOFSTREAMMONITOR_HXX






//
//  	A PrimOfstreamMonitor establishes a file creation context for a file name, with an optional fall-back
//  	to a stream (such as cout). Resources allocated to create the file are automatically released on
//  	destruction. The creation context is registered using the PrimMonitor protocol so that a message
//  	such as
//
//  		Creating "file.name".
//
//  	accompanies any error messages emitted during creation. The precise format of the message is
//
//  		<prefix> "<file-name>"<suffix>
//
//  	for which <prefix> and <suffix> are supplied directly from construction arguments. <file-name>
//  	is similarly supplied if non-0, but falls back to a default if 0. Thus
//
//  		PrimOfstreamMonitor aMonitor("Creating", fileName, ".", &cout, "<cout>");
//  		write_on(aMonitor.s());
//
//  	is a convenient way to create a file output stream to fileName, or cout according to whether
//  	fileName is 0 or not. The actual output stream is accessible as s().
//
//  	If a default stream is required but not supplied a stream to "/dev/null" is used.
//
class PrimOfstreamMonitor : public PrimMonitor {
	TYPEDECL_SINGLE(PrimTermMonitor, PrimMonitor)
	enum Locked {};           //   Useless type for pointer return.
private:
	const char* _prefix;
	PrimStringHandle _file_name;
	const char* _suffix;
	std::ostream* _s;        //   Stream used for output.
	PrimAdopted<std::ofstream> _file;    //   File buffer created for output.
	bool _ok;          //   True if output is sensible.
private:
	static std::ostream& null_stream();
	virtual std::ostream& print_message(std::ostream& s) const;
public:
	PrimOfstreamMonitor(const char* aPrefix, const PrimString& fileName, const char* aSuffix = "",
	                    std::ostream* sDefault = 0, const char* fDefault = 0);
	//
	//  	Return non-0 if output is sensible.
	//
	operator const Locked* () const { return (const Locked*)(_ok ? this : 0); }
	//
	//  	Return true if output is non sensible.
	//
	//
	bool operator!() const { return !_ok; }
	const PrimString& file_name() const { return *_file_name; }
	std::ostream& s() { return *_s; }
};
#endif

