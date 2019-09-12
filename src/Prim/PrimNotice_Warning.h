
#ifndef PRIMNOTICE_WARNING_HXX
#define PRIMNOTICE_WARNING_HXX



//  
//  	A PrimNotice_Warning defines a notice containing a text warning message.
//  
//  	Warning notices are normally created by the WRNMSG() or GLOBAL_WRNMSG() macros.
//  
class PrimNotice_Warning : public PrimNotice_Text
{
 TYPEDECL_SINGLE(PrimNotice_Warning, PrimNotice_Text)
public:
 PrimNotice_Warning(const char *aMessage, const PrimNotifier *aNotifier = 0);
 PrimNotice_Warning(char *aMessage, size_t aSize, const PrimNotifier *aNotifier = 0);
 virtual ~PrimNotice_Warning();
 virtual bool not_posted() const;
 virtual bool posted_on(PrimNotifier& aNoticeBoard) const;
};
#endif
 
