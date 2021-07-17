
#ifndef PRIMNOTIFIER_HXX
#define PRIMNOTIFIER_HXX


#ifndef NO_PRIM_STREAMS
	
#endif

//
//  	A PrimNotifier provides a base class for objects that need to communicate errors or
//  	other status information to indeterminate listeners.
//  	It implements a notice board mechanism with redirection so that notices posted on this object may be
//  	automatically relayed up a parent hierarchy to a notice board serving many objects.
//  .note tutorial
//  	@file("PrimNotifier.tutorial")
//
class PrimNotifier : public PrimPrint {
	TYPEDECL_SINGLE(PrimNotifier, PrimPrint)
private:
	static PrimNotifier* _global_notice_board; //   The notice board on which global messages are posted.
	//   0 results in notices being not_posted() globally.
#ifndef NO_PRIM_STREAMS
	static std::ostream* _global_notice_stream; //   The stream used to publish default global notices.
	//   0 suppresses default global notice board output.
	static std::ostream* _global_notice_log; //   The secondary stream used to log default global notices.
	//   0 suppresses logged global notice board output.
#endif
	PrimNotifier* _redirect;     //   The notice board to which to redirect notices posted
	//   on this object. 0 redirects to the global notice board.
	
private:
	//
	//  	Construction of a copy is forbidden.
	//
	PrimNotifier(const PrimNotifier& aNotifier);
	//
	//  	Assignment is forbidden.
	//
	PrimNotifier& operator=(const PrimNotifier& aNotifier);
	
protected:
	PrimNotifier(PrimNotifier* aNotifier = 0);   /*   Doesn't inline with cfront 2.1 */
	virtual bool post(const PrimNotice& aNotice);
	
public:
	static int _error_count;    //   The number of error messages posted on the global notice board.
	static int _warning_count;    //   The number of warning messages posted on the global notice board.
	static int _maximum_error_count;  //   Termination threshold on number of error messages.
	static int _maximum_warning_count;  //   Termination threshold on number of error messages.
	virtual ~PrimNotifier();
	void notify_construction(PrimNotifier* aNotifier = 0);
	void notify_destruction();
	//
	//  	Report the cimulative error count.
	//
	static int error_count() { return _error_count; }
	//
	//  	Report the notice board on which global notices should be posted.
	//
	static PrimNotifier* global_notice_board() { return _global_notice_board; }
#ifndef NO_PRIM_STREAMS
	//
	//  	Report the notice stream used to log notices posted on the global notice board.
	//
	static std::ostream* global_notice_log() { return _global_notice_log; }
	//
	//  	Report the notice stream used to publish notices posted on the global notice board.
	//
	static std::ostream* global_notice_stream() { return _global_notice_stream; }
	//
	//  	Report the notice stream used to publish notices posted on the global notice board,
	//  	or std::cerr if there is no global notice board.
	//
	static std::ostream& global_notice_stream_or_cerr()
	{ return *(_global_notice_stream != 0 ? _global_notice_stream : &std::cerr); }
#endif
	static int& maximum_error_count() { return _maximum_error_count; }
	static int& maximum_warning_count() { return _maximum_warning_count; }
	//
	//  	Report "this" as the notice board on which locally generated notices should be posted.
	//  	This method is used by the ERRMSG macros to automatically identify the target
	//  	notice board. When an ERRMSG occurs in class scope this method is used. When an
	//  	ERRMSG occurs outside class scope ::prim_notifier() is used.
	//
	const PrimNotifier* notifier() const { return this; }
	const PrimNotifier* prim_notifier() const { return this; }
	virtual bool post_constructor(const PrimNotice_Constructor& aNotice);
	virtual bool post_destructor(const PrimNotice_Destructor& aNotice);
	virtual bool post_diagnostic(const PrimNotice_Diagnostic& aNotice) const;
	virtual bool post_error(const PrimNotice_Error& aNotice) const;
	virtual bool post_notice(const PrimNotice& aNotice) const;
	virtual bool post_warning(const PrimNotice_Warning& aNotice) const;
	static bool post_global_diagnostic(const char* aMessage);
	static bool post_global_error(const char* aMessage);
	static bool post_global_notice(const PrimNotice& aNotice);
	static bool post_global_warning(const char* aMessage);
	//
	//  	Report the notice board to which this notice board redirects notices.
	//
	const PrimNotifier* redirect() const { return (_redirect); }
	PrimNotifier* redirect() { return (_redirect); }
	static PrimNotifier* set_global_notice_board(PrimNotifier* aNotifier);
#ifndef NO_PRIM_STREAMS
	static std::ostream* set_global_notice_log(std::ostream* aStream);
	static std::ostream* set_global_notice_log_file(const char* aFile);
	static std::ostream* set_global_notice_stream(std::ostream* aStream);
#endif
	virtual PrimNotifier* set_redirect(PrimNotifier* aNotifier);
	//
	//  	Report the cimulative warning count.
	//
	static int warning_count() { return _warning_count; }
};

void increment_error_count();
void increment_warning_count();

//
//  	Add one to the accumulated error message count.
//
inline void prim_increment_error_count() { increment_error_count(); }

//
//  	Add one to the accumulated warning message count.
//
inline void prim_increment_warning_count() { increment_warning_count(); }
#endif

