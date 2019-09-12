
#ifndef PRIMNOTICE_CONSTRUCTOR_HXX
#define PRIMNOTICE_CONSTRUCTOR_HXX


//  
//  	A PrimNotice_Constructor defines a notice identifying that a particular PrimNotifier
//  	has completed construction. Construction notices are normally posted on the global
//  	notice board, and provide an opportunity for readers to automatically establish
//  	more specific notice boards.
//  
class PrimNotice_Constructor : public PrimNotice
{
 TYPEDECL_SINGLE(PrimNotice_Constructor, PrimNotice)
public:
 PrimNotice_Constructor(const PrimNotifier& theNotifier) : PrimNotice(&theNotifier) {}
 virtual ~PrimNotice_Constructor();
 bool compare(const PrimNotice_Constructor& aNotice) const;
 virtual bool posted_on(PrimNotifier& aNoticeBoard) const;
#ifndef NO_PRIM_STREAMS
 virtual std::ostream& print_this(std::ostream& s) const;
#endif
};
#endif
 
