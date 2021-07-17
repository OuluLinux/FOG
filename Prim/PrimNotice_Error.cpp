#include <Prim/PrimIncludeAll.h>




TYPEINFO_SINGLE(PrimNotice_Error, PrimNotice_Text)

//
//  	Construct an error notice from aMessage.
//
PrimNotice_Error::PrimNotice_Error(const char* aMessage, const PrimNotifier* aNotifier)
	:
	PrimNotice_Text(aMessage, aNotifier)
{}

//
//  	Construct an error notice from aMessage, enforcing a null termination at (if not before) aSize.
//
PrimNotice_Error::PrimNotice_Error(char* aMessage, size_t aSize, const PrimNotifier* aNotifier)
	:
	PrimNotice_Text(aMessage, aSize, aNotifier)
{}

//
//  	The default destructor does nothing.
//
PrimNotice_Error::~PrimNotice_Error() {}

//
//  	Unposted errors get relayed to the global notice board.
//
bool PrimNotice_Error::not_posted() const {
	return PrimNotifier::post_global_error(message());
}

//
//  	Post this error message up by instructing aNoticeBoard to postErrorNotice(), the
//  	return from which is returned to show whether the message was read.
//
bool PrimNotice_Error::posted_on(PrimNotifier& aNoticeBoard) const {
	return aNoticeBoard.post_error(*this);
}
