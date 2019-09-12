#include <Prim/PrimIncludeAll.h>






TYPEINFO_SINGLE(PrimNotice, Super)

//  
//  	The default destructor does nothing.
//  
PrimNotice::~PrimNotice() {}

//  
//  	The default implementation of the notice has not been posted call back just returns false
//  	to signal that the message was not read. Derived classes may reimplement to provide some
//  	fall back policy to prevent important messages such as error diagnostics getting lost.
//  
bool PrimNotice::not_posted() const
{
 return false;
}

//  
//  	The default implementation of the notice has been posted call back just invokes
//  	not_posted() to return false
//  	and signal that the message was not read. Derived classes should validate that aNoticeBoard
//  	can respond to the message using the run-time type information mechanism and then use a message
//  	specific method to post the notice.
//  
bool PrimNotice::posted_on(PrimNotifier& aNoticeBoard) const
{
 UNUSED(&aNoticeBoard);
 return not_posted();
}

//  
//  	Post this notice on aNotifier, provided it is not-0 otherwise post it on the global
//  	notice board. The notice gets called back by posted_on() and/or not_posted(). "and" rather than
//  	"or" occurs if posted_on() decides it can't after all.
//  
const PrimNotice *PrimNotice::post_on(const PrimNotifier *aNotifier) const
{
 if (aNotifier != 0)
  aNotifier->post_notice(*this);
 else
  PrimNotifier::post_global_notice(*this);
 return this;
}

#ifndef NO_PRIM_STREAMS
//  
//  	Provide a diagnostic description of the contents of the notice. The default
//  	implementation just adds the posted by identity to the inherited print-out.
//  
std::ostream& PrimNotice::print_members(std::ostream& s, int indentLevel) const
{
 Super::print_members(s, indentLevel);
 if (_posted_by != 0)
  s << indent(indentLevel+1) << "posted by " << *_posted_by << "\n";
 return (s);
}
#endif
