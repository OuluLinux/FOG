#include <Prim/PrimIncludeAll.h>




TYPEINFO_SINGLE(PrimNotice_Diagnostic, PrimNotice_Text)

//
//  	Construct a diagnostic notice from aMessage.
//
PrimNotice_Diagnostic::PrimNotice_Diagnostic(const char* aMessage, const PrimNotifier* aNotifier)
	:
	PrimNotice_Text(aMessage, aNotifier)
{}

//
//  	Construct a diagnostic notice from aMessage, enforcing a null termination at (if not before) aSize.
//
PrimNotice_Diagnostic::PrimNotice_Diagnostic(char* aMessage, size_t aSize, const PrimNotifier* aNotifier)
	:
	PrimNotice_Text(aMessage, aSize, aNotifier)
{}

//
//  	The default destructor does nothing.
//
PrimNotice_Diagnostic::~PrimNotice_Diagnostic() {}

//
//  	Unposted diagnostics get relayed to the global notice board.
//
bool PrimNotice_Diagnostic::not_posted() const {
	return PrimNotifier::post_global_diagnostic(message());
}

//
//  	Post this diagnostic message up by instructing aNoticeBoard to post_diagnostic(), the
//  	return from which is returned to show whether the message was read.
//
bool PrimNotice_Diagnostic::posted_on(PrimNotifier& aNoticeBoard) const {
	return aNoticeBoard.post_diagnostic(*this);
}
