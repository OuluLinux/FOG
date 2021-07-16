#include <Prim/PrimIncludeAll.h>




TYPEINFO_SINGLE(PrimNotice_Destructor, PrimNotice_Constructor)

//  
//  	The default destructor does nothing.
//  
PrimNotice_Destructor::~PrimNotice_Destructor() {}

//  
//  	Post this destructor message up by instructing aNoticeBoard to post_destructor(), the
//  	return from which is returned to show whether the message was read.
//  
bool PrimNotice_Destructor::posted_on(PrimNotifier& aNoticeBoard) const
{
 return aNoticeBoard.post_destructor(*this);
}
