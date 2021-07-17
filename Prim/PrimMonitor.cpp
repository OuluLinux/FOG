#include <Prim/PrimIncludeAll.h>







TYPEINFO_SINGLE(PrimMonitor, PrimNotifier)

PrimMonitor* PrimMonitor::_installed_monitor = 0;
int PrimMonitor::_installed_depth = 0;

//
//  	The constructor caches the context message and currently installed monitor, installing this in its stead.
//
//  .note Use of this constructor is deprecated. Use PrimMonitor(Install) to make the side effect clear.
//
PrimMonitor::PrimMonitor()
	:
	_depth(0),
	_done(false),
	_monitor(0) {
	install();
}

//
//  	Construct a potential monitor. If doInstall is INSTALL, rather than NO_INSTALL, then the
//  	context message and currently installed monitor is cached, installing this in its stead.
//
PrimMonitor::PrimMonitor(Install doInstall)
	:
	_depth(0),
	_done(false),
	_monitor(0) {
	if (doInstall == INSTALL)
		install();
}

//
//  	The destructor reinstalls the less-nested monitor.
//
PrimMonitor::~PrimMonitor() {
	deinstall();
	//  	_installed_depth--;
	//  	_installed_monitor = _monitor;
	//  	set_global_notice_board(_installed_monitor);
}

//
//  	The deinstall this monitor and reinstall the less-nested monitor.
//
void PrimMonitor::deinstall() {
	if (global_notice_board() == this) {
		_installed_depth--;
		_installed_monitor = _monitor;
		set_global_notice_board(_installed_monitor);
		_monitor = 0;
	}
	else if (_monitor)
		ERRMSG("BUG - bad call to deinstall " << *this);
}

//
//  	Return true if this monitor has a message to contribute.
//
bool PrimMonitor::has_message() const { return true; }

//
//  	The install this monitor saving the context of the less-nested monitor.
//
void PrimMonitor::install() {
	if (!_monitor) {
		_depth = _installed_depth++;
		_done = false;
		_monitor = _installed_monitor;
		_installed_monitor = this;
		set_global_notice_board(_installed_monitor);
	}
	else
		ERRMSG("BUG - bad call to install " << *this);
}

//
//  	Post up an error notice, sending its text it to cerr prefixed by a bell then "ERROR -- ".
//
bool PrimMonitor::post_error(const PrimNotice_Error& aNotice) const {
	post_text("ERROR -- ", aNotice.message());
	prim_increment_error_count();
	return true;
}

//
//  	Post up the aMessage text, sending its text it to global_notice_stream() and global_notice_log() prefixed by
//  	indentation, aPrefix and suffixed  by aSuffix and endl.
//
void PrimMonitor::post_text(const char* aPrefix, const char* aMessage, const char* aSuffix) const {
	print_context();
	
	if (!aPrefix && !aMessage && !aSuffix)    //   Allow for a missing body without
		return;           //    a corresponding blank line
		
	std::ostream* s1 = global_notice_stream();
	
	if (s1) {
		*s1 << indent(_depth + 1);
		
		if (aPrefix) *s1 << aPrefix;
		
		if (aMessage) *s1 << aMessage;
		
		if (aSuffix) *s1 << aSuffix;
		
		*s1 << std::endl;
	}
	
	std::ostream* s2 = global_notice_log();
	
	if (s2) {
		*s2 << indent(_depth + 1);
		
		if (aPrefix) *s2 << aPrefix;
		
		if (aMessage) *s2 << aMessage;
		
		if (aSuffix) *s2 << aSuffix;
		
		*s2 << std::endl;
	}
}

//
//  	Post up a warning notice, sending its text it to cerr prefixed by a bell then "WARNING -- ".
//
bool PrimMonitor::post_warning(const PrimNotice_Warning& aNotice) const {
	post_text("WARNING -- ", aNotice.message());
	prim_increment_warning_count();
	return true;
}

//
//  	Output the context of an error or warning message.
//
void PrimMonitor::print_context() const {
	if (_done)
		return;
		
	mutate()._done = true;
	
	if (_monitor)
		_monitor->print_context();
		
	if (!has_message())
		return;
		
	if (global_notice_stream())
		print_message_line(*global_notice_stream(), _depth);
		
	if (global_notice_log())
		print_message_line(*global_notice_log(), _depth);
}

//
//  	Print the diagnostic message as a line fragment. Indentation is performed before and
//  	endl performed after. Derived classes must reimplement to define their message
//  	unless they reimplement print_message_line instead.
//
std::ostream& PrimMonitor::print_message(std::ostream& s) const {
	s << "Context of partially constructed/destructed PrimMonitor object at " << this;
	return s;
}

//
//  	Print the diagnostic message as a line, indented by aDepth and terminated by an endl.
//  	Derived classes may reimplement here to generate multiple lines.
//
std::ostream& PrimMonitor::print_message_line(std::ostream& s, int aDepth) const {
	s << indent(aDepth);
	print_message(s);
	s << std::endl;
	return s;
}

//
//  	Emit someText to s ensuring that each new-line terminated line of someText is indented by the
//  	moinitor depth plus the extraDepth. No new line is appended to someText.
//
std::ostream& PrimMonitor::print_message_text(std::ostream& s, const char* someText, int extraDepth) const {
	s << indent(_depth + extraDepth);
	
	if (someText) {
		const char* p = someText;
		
		while (*p) {
			const char* q = p;
			
			while (*q && (*q != '\n'))
				++q;
				
			s.write(p, q - p);
			p = q;
			
			if (*p) {
				s << '\n' << indent(_depth + extraDepth);
				++p;
			}
		}
	}
	
	return s;
}
