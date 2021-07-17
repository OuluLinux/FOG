
#ifndef PRIMMONITOR_HXX
#define PRIMMONITOR_HXX




class PrimMonitor : public PrimNotifier {
	typedef PrimMonitor This;
	typedef PrimNotifier Super;
	TYPEDECL_SINGLE(This, Super)
public:
	enum Install { NO_INSTALL, INSTALL };
private:
	int _depth;           //   Nesting depth depth.
	mutable bool _done;         //   True once context message generated.
	PrimMonitor* _monitor;        //   The less nested monitor.
private:
	static PrimMonitor* _installed_monitor;    //   The current monitor.
	static int _installed_depth;      //   The current depth.
private:
	PrimMonitor(const This&);
	PrimMonitor& operator=(const This&);
	This& mutate() const { return *(This*)this; }
protected:
	PrimMonitor();
	PrimMonitor(Install doInstall);
	virtual ~PrimMonitor();
	void clear_done() { _done = false; }
	void deinstall();
	virtual bool has_message() const;
	void install();
	bool is_installed() const { return _monitor != 0; }
	PrimMonitor* monitor() { return _monitor; }
	void post_text(const char* aPrefix, const char* aMessage, const char* aSuffix = 0) const;
	void print_context() const;
	//
	//  	This function could usefully be defined as a pure virtual to mandate derived implementation, but
	//  	since error messages may occur during destruction, it is all too easy for the pure virtual to be
	//  	invoked at just the time when something useful is most needed.
	//
	virtual std::ostream& print_message(std::ostream& s) const;
	virtual std::ostream& print_message_line(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_message_text(std::ostream& s, const char* someText, int extraDepth) const;
public:
	const PrimMonitor* monitor() const { return _monitor; }
	virtual bool post_error(const PrimNotice_Error& aNotice) const;
	virtual bool post_warning(const PrimNotice_Warning& aNotice) const;
};
#endif

