#include <Prim/PrimIncludeAll.h>









TYPEINFO_SINGLE(PrimNotifier, PrimPrint)

PrimNotifier *PrimNotifier::_global_notice_board = 0; //   Notice board used for global messages.
#ifndef NO_PRIM_STREAMS
std::ostream *PrimNotifier::_global_notice_stream = &std::cerr; //   stream used for default global messages.
std::ostream *PrimNotifier::_global_notice_log = 0;  //   secondary stream used to log default global notices.
#endif
int PrimNotifier::_error_count = 0;      //   Cumulative count of error messages.
int PrimNotifier::_warning_count = 0;     //   Cumulative count of warning messages.
int PrimNotifier::_maximum_error_count = 0;    //   Termination threshold on number of error messages.
int PrimNotifier::_maximum_warning_count = 0;   //   Termination threshold on number of warning messages.

//  
//  	Create a new PrimNotifier, optionally specifying aNotifier to which notices posted
//  	on this notice board are redirected.
//  
PrimNotifier::PrimNotifier(PrimNotifier *aNotifier)
:
 _redirect(aNotifier)
{}

//  
//  	When a notifier is destroyed, ensure that it is not a replacement global notice board.
//  	If it is the potential dangling pointer is 0ed, restoring the default global
//  	notice board behaviour.
//  
PrimNotifier::~PrimNotifier()
{
 if (_global_notice_board == this)     //   Avoid a reference to a destructed object
  _global_notice_board = 0;
/*  		Don't do destructor notices by default, otherwise any notice handler has to sort out
		all the dross below an interception point, and worse the dross doesn't know what kind
		of class it is. notify_destruction() must be used at a controlled time. */
//    if (_redirect != 0)
//  		PrimNotice_Destructor(*this).post_on(this);
}

//  
//  	Post a notice on aNotifier to notify any object that cares that this notifier has been constructed.
//  	This method must be explicitly invoked by application code once construction has completed.
//  	Typically it is invoked when there is no local notice board, so aNotifier is 0 to cause the notice
//  	to go to the global notice board, where other application layers can read it and automatically create
//  	appropriate local notice boards.
//  
void PrimNotifier::notify_construction(PrimNotifier *aNotifier)
{
 PrimNotice_Constructor(*this).post_on(aNotifier);
}

//  
//  	Post a notice to notify any object that cares that this notifier is about to be destructed.
//  	This method should be invoked by application code immediately prior to any destruction, so that
//  	notice boards can detach from this notifier in a tidy fashion.
//  
void PrimNotifier::notify_destruction()
{
 PrimNotice_Destructor(*this).post_on(this);
}

//  
//  	Post a notice on the notice board. If there is a parent notice board aNotice is passed
//  	on to that board. If not aNotice is notified that it has been posted_on() this board.
//  	This method is used to pass a notice up the notice board hierarchy. It should not be
//  	invoked directly since posting notices on self should be a logically const operation which this method
//  	is not. post_notice() is intended for other applications to invoke.
//  
bool PrimNotifier::post(const PrimNotice& aNotice)
{
 if ((_redirect != 0) && (_redirect != this))
  return _redirect->post(aNotice);
 else
  return aNotice.posted_on(*this);
}

//  
//  	Post up a constructor notice. The default implementation here just notifies
//  	aNotice that it has been not_posted() and propagates the corresponding return status.
//  
bool PrimNotifier::post_constructor(const PrimNotice_Constructor& aNotice)
{
 return aNotice.not_posted();
}

//  
//  	Post up a destructor notice. The default implementation here just notifies
//  	aNotice that it has been not_posted() and propagates the corresponding return status.
//  
bool PrimNotifier::post_destructor(const PrimNotice_Destructor& aNotice)
{
 return aNotice.not_posted();
}

//  
//  	Post up a diagnostic notice. The default implementation here just notifies
//  	aNotice that it has been not_posted() and propagates the corresponding return status.
//  
bool PrimNotifier::post_diagnostic(const PrimNotice_Diagnostic& aNotice) const
{
 return aNotice.not_posted();
}

//  
//  	Post up an error notice. The default implementation here just notifies
//  	aNotice that it has been not_posted() and propagates the corresponding return status.
//  
bool PrimNotifier::post_error(const PrimNotice_Error& aNotice) const
{
 return aNotice.not_posted();
}

//  
//  	Post up a diagnostic notice on the global notice board. The message is printed
//  	on the global_notice_stream, provided the stream is not 0, in which case the message
//  	is discarded. Returns false in all cases to signal that the message was unread.
//  
bool PrimNotifier::post_global_diagnostic(const char *aMessage)
{
#ifndef NO_PRIM_STREAMS
 if (_global_notice_stream != 0)
  *_global_notice_stream << aMessage << std::endl;
 if (_global_notice_log != 0)
  *_global_notice_log << aMessage << std::endl;
#else
 UNUSED(aMessage);
#endif
 return false;
}

//  
//  	Post up an error notice on the global notice board. The message is printed with a "ERROR -- "
//  	prefix on the global_notice_stream, provided the stream is not 0, in which case the message
//  	is discarded. Returns false in all cases to signal that the message was unread.
//  
bool PrimNotifier::post_global_error(const char *aMessage)
{
#ifndef NO_PRIM_STREAMS
 if (_global_notice_stream != 0)
  *_global_notice_stream << "ERROR -- " << aMessage << std::endl;
 if (_global_notice_log != 0)
  *_global_notice_log << "ERROR -- " << aMessage << std::endl;
#else
 UNUSED(aMessage);
#endif
 prim_increment_error_count();
 return false;
}

//  
//  	Post a notice on the global notice board. If there is a global notice board aNotice is passed
//  	on to that board, otherwise aNotice is notified that it has been not_posted().
//  	Returns true if the notice was successfully posted and read.
//  
bool PrimNotifier::post_global_notice(const PrimNotice& aNotice)
{
 if (_global_notice_board != 0)
  return _global_notice_board->post(aNotice);
 else
  return aNotice.not_posted();
}

//  
//  	Post up a warning notice on the global notice board. The message is printed with a "WARNING -- "
//  	prefix on the global_notice_stream, provided the stream is not 0, in which case the message
//  	is discarded. Returns false in all cases to signal that the message was unread.
//  
bool PrimNotifier::post_global_warning(const char *aMessage)
{
#ifndef NO_PRIM_STREAMS
 if (_global_notice_stream != 0)
  *_global_notice_stream << "WARNING -- " << aMessage << std::endl;
 if (_global_notice_log != 0)
  *_global_notice_log << "WARNING -- " << aMessage << std::endl;
#else
 UNUSED(aMessage);
#endif
 prim_increment_warning_count();
 return false;
}

//  
//  	Post a notice on the notice board. If there is a parent notice board aNotice is passed
//  	on to that board, where post() may pass it up a hierarchy of notice boards. If not
//  	post_notice() is invoked to post it on the global notice board.
//  
bool PrimNotifier::post_notice(const PrimNotice& aNotice) const
{
 if (_redirect == 0)
  return post_global_notice(aNotice);
 else
  return _redirect->post(aNotice);
}

//  
//  	Post up a warning notice. The default implementation here just notifies
//  	aNotice that it has been not_posted() and propagates the corresponding return status.
//  
bool PrimNotifier::post_warning(const PrimNotice_Warning& aNotice) const
{
 return aNotice.not_posted();
}

//  
//  	Specify a new global notice board. Any previous global notice board is returned
//  	and should be disposed of by the caller.
//  
PrimNotifier *PrimNotifier::set_global_notice_board(PrimNotifier *aNotifier)
{
 PrimNotifier *oldNotifier = _global_notice_board;
 _global_notice_board = aNotifier;
 return oldNotifier;
}

#ifndef NO_PRIM_STREAMS
//  
//  	PrimNotifier_GlobalNoticeLogFile manages a singleton instance of PrimAdopted<std::ofstream> that
//  	owns the PrimNotifier::global_notice_log() allocated by PrimNotifier::set_global_notice_log_file().
//  	Existence as a distinct class enables the destructor to synchronise any static destruction of the
//  	log stream with a class to PrimNotifier::set_global_notice_log(0), thereby ensuring that log messages
//  	are not serviced by a freed notice board.
//  
class PrimNotifier_GlobalNoticeLogFile
{
 NEEDS_A_FRIEND_FOR_GNUC
private:
 PrimAdopted<std::ofstream> _stream;
private:
 PrimNotifier_GlobalNoticeLogFile() {}
 PrimNotifier_GlobalNoticeLogFile(const PrimNotifier_GlobalNoticeLogFile&);
 PrimNotifier_GlobalNoticeLogFile& operator=(const PrimNotifier_GlobalNoticeLogFile&);
public:          //  .bugbug MS DevStudio 5 needs this public not private
 ~PrimNotifier_GlobalNoticeLogFile() { PrimNotifier::set_global_notice_log_file(0); }
public:
 std::ofstream *release() { return _stream.release(0); }
 void reset() { _stream.reset(0); }
 static PrimNotifier_GlobalNoticeLogFile& singleton()
 {
  static PrimNotifier_GlobalNoticeLogFile anObject;
  return anObject;
 }
 std::ofstream *set(const char *fileName)
 {
#ifdef NEEDS_STD
  _stream.reset(new std::ofstream(fileName, std::ios::out, ofstream_open_mode()));
#else
  _stream.reset(new std::ofstream(fileName, std::ios::out));
#endif
  PrimError anError;
  if (!_stream.get())
   ERRMSG("Unable to create memory for \"" << fileName << "\"");
  else if (!*_stream.get())
   WRNMSG("Unable to create \"" << fileName << "\", " << anError);
  else
   return _stream.get();
  return 0;
 }
};

//  
//  	Specify a new global notice log stream. Any previous global notice log stream is returned
//  	and should be disposed of by the caller.
//  
//  	Be aware that any messages logged during static destruction may encounter a freed notice board. This
//  	problem may be circumvented by using set_global_notice_log_file(const char *fileName) to register
//  	a log file. 
//  
std::ostream *PrimNotifier::set_global_notice_log(std::ostream *aStream)
{
 std::ostream *oldStream = _global_notice_log;
 std::ostream *oldLogStream = PrimNotifier_GlobalNoticeLogFile::singleton().release();
 return oldStream;
}

//  
//  	Specify a new global notice log stream directed to fileName. Any previous global notice log stream
//  	resulting from set_global_notice_log() is returned and should be disposed of by the caller. Any
//  	previous global notice log stream resulting from set_global_notice_log_file() is deleted automatically
//  	and 0 is returned.
//  
std::ostream *PrimNotifier::set_global_notice_log_file(const char *fileName)
{
 std::ostream *oldStream = _global_notice_log;
 std::ostream *oldLogStream = PrimNotifier_GlobalNoticeLogFile::singleton().release();
 if (fileName)
  _global_notice_log = PrimNotifier_GlobalNoticeLogFile::singleton().set(fileName);
 else
 {
  PrimNotifier_GlobalNoticeLogFile::singleton().reset();
  _global_notice_log = 0;
 }
 if (oldLogStream)
 {
  delete oldLogStream;
  oldStream = 0;
 }
 return oldStream;
}

//  
//  	Specify a new global notice stream. Any previous global notice stream is returned
//  	and should be disposed of by the caller.
//  
std::ostream *PrimNotifier::set_global_notice_stream(std::ostream *aStream)
{
 std::ostream *oldStream = _global_notice_stream;
 _global_notice_stream = aStream;
 return oldStream;
}
#endif

//  
//  	Define the parent notifier of this PrimNotifier. Returns the identity of the previous parent.
//  
PrimNotifier *PrimNotifier::set_redirect(PrimNotifier *aNoticeBoard)
{
 PrimNotifier *const oldNotifier = _redirect;
 _redirect = aNoticeBoard;
 return oldNotifier;
}

//  
//  	Increment the overall error count. This routine is provided as a global method to simplify
//  	debugging. A break-point on this routine should catch all errors.
//  
void increment_error_count()
{
 PrimNotifier::_error_count++;
 if (PrimNotifier::_maximum_error_count && (PrimNotifier::_error_count > PrimNotifier::_maximum_error_count))
  abort();
}

//  
//  	Increment the overall warning count. This routine is provided as a global method to simplify
//  	debugging. A break-point on this routine should catch all warnings.
//  
void increment_warning_count()
{
 PrimNotifier::_warning_count++;
 if (PrimNotifier::_maximum_warning_count && (PrimNotifier::_warning_count > PrimNotifier::_maximum_warning_count))
  abort();
}
