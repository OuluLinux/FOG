#include <Prim/PrimIncludeAll.h>




TYPEINFO_SINGLE(PrimNotice_Constructor, PrimNotice)

//  
//  	The default destructor does nothing.
//  
PrimNotice_Constructor::~PrimNotice_Constructor() {}

#ifndef NO_PRIM_STREAMS
//  
//  	Provide additional one-line diagnostic description of body of the constructor notice.
//  
std::ostream& PrimNotice_Constructor::print_this(std::ostream& s) const
{
 if (_posted_by != 0)
  return s << *_posted_by;
 else
  return s << "?";
}
#endif

//  
//  	Report whether this notice is identical to aNotice. Returns true if aNotice is an instance
//  	as the same class as this and if the notified object is identical.
//  
bool PrimNotice_Constructor::compare(const PrimNotice_Constructor& aNotice) const
{
 return _posted_by == aNotice._posted_by;
}

//  
//  	Post this constructor message up by instructing aNoticeBoard to post_constructor(), the
//  	return from which is returned to show whether the message was read.
//  
bool PrimNotice_Constructor::posted_on(PrimNotifier& aNoticeBoard) const
{
 return aNoticeBoard.post_constructor(*this);
}
