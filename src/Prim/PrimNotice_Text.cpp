#include <Prim/PrimIncludeAll.h>





TYPEINFO_SINGLE(PrimNotice_Text, PrimNotice)

//  
//  	Construct a new notice using the null terminated text from aMessage.
//  
PrimNotice_Text::PrimNotice_Text(const char *aMessage, const PrimNotifier *aNotifier)
:
 PrimNotice(aNotifier),
 _message(aMessage)
{}

//  
//  	Construct a new notice using the text from aMessage, and ensuring that there is a null termination
//  	at aSize characters.
//  
PrimNotice_Text::PrimNotice_Text(char *aMessage, size_t aSize, const PrimNotifier *aNotifier)
:
 PrimNotice(aNotifier),
 _message(aMessage)
{
 aMessage[aSize-1] = EOS;
}

//  
//  	The destructor does nothing.
//  
PrimNotice_Text::~PrimNotice_Text() {}

//  
//  	Report whether this notice is identical to aNotice. Returns true if aNotice is an instance
//  	as the same class as this and if the text message is identical.
//  
bool PrimNotice_Text::compare(const PrimNotice_Text& aNotice) const
{
 return strcmp(message(), aNotice.message()) == 0;
}

#ifndef NO_PRIM_STREAMS
//  
//  	Provide additional one-line diagnostic description of body of the stream notice.
//  
std::ostream& PrimNotice_Text::print_this(std::ostream& s) const
{
 return s << "\"" << message() << "\"";
}
#endif
