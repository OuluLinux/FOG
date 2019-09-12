#include <Prim/PrimIncludeAll.h>




TYPEINFO_SINGLE(PrimNotice_Warning, PrimNotice_Text)

//  
//  	Construct a warning notice from aMessage, enforcing a null termination at (if not before) aSize.
//  
PrimNotice_Warning::PrimNotice_Warning(const char *aMessage, const PrimNotifier *aNotifier)
:
 PrimNotice_Text(aMessage, aNotifier)
{}

//  
//  	Construct a warning notice from aMessage, enforcing a null termination at (if not before) aSize.
//  
PrimNotice_Warning::PrimNotice_Warning(char *aMessage, size_t aSize, const PrimNotifier *aNotifier)
:
 PrimNotice_Text(aMessage, aSize, aNotifier)
{}

//  
//  	The default destructor does nothing.
//  
PrimNotice_Warning::~PrimNotice_Warning() {}

//  
//  	Unposted warnings get relayed to the global notice board.
//  
bool PrimNotice_Warning::not_posted() const
{
 return PrimNotifier::post_global_warning(message());
}

//  
//  	Post this warning message up by instructing aNoticeBoard to post_warning(), the
//  	return from which is returned to show whether the message was read.
//  
bool PrimNotice_Warning::posted_on(PrimNotifier& aNoticeBoard) const
{
 return aNoticeBoard.post_warning(*this);
}
