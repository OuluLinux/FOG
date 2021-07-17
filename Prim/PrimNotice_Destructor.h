
#ifndef PRIMNOTICE_DESTRUCTOR_HXX
#define PRIMNOTICE_DESTRUCTOR_HXX


//
//  	A PrimNotice_Destructor defines a notice identifying that a particular PrimNotifier
//  	has started destruction. Destruction notices provide an opportunity for readers to
//  	automatically destroy notice boards serving extinct application objects.
//
class PrimNotice_Destructor : public PrimNotice_Constructor {
	TYPEDECL_SINGLE(PrimNotice_Destructor, PrimNotice_Constructor)
public:
	PrimNotice_Destructor(const PrimNotifier& theNotifier) : PrimNotice_Constructor(theNotifier) {}
	virtual ~PrimNotice_Destructor();
	virtual bool posted_on(PrimNotifier& aNoticeBoard) const;
};
#endif

