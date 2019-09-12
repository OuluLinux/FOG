
#ifndef PRIMNOTICE_TEXT_HXX
#define PRIMNOTICE_TEXT_HXX


//  
//  	A PrimNotice_Text is an abstract base class defining a text notice.
//  	Derived classes support diagnostic, error and warning messages.
//  
class PrimNotice_Text : public PrimNotice
{
 TYPEDECL_SINGLE(PrimNotice_Text, PrimNotice)
private:
 const char *const _message;     //   The null terminated message text.
protected:
 PrimNotice_Text(const char *aMessage, const PrimNotifier *aNotifier = 0);
 PrimNotice_Text(char *aMessage, size_t aSize, const PrimNotifier *aNotifier = 0);
 virtual ~PrimNotice_Text();
public:
 bool compare(const PrimNotice_Text& aNotice) const;
//  
//  		Report the string contents of this notice.
//  
 const char *message() const { return _message; }
#ifndef NO_PRIM_STREAMS
 virtual std::ostream& print_this(std::ostream& s) const;
#endif
};
#endif
 
