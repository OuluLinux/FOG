
#ifndef PRIMNOTICE_ERROR_HXX
#define PRIMNOTICE_ERROR_HXX



//
//  	A PrimNotice_Error defines a notice containing a text error message.
//
//  	Error notices are normally created by the ERRMSG() or GLOBAL_ERRMSG() macros.
//
class PrimNotice_Error : public PrimNotice_Text {
	TYPEDECL_SINGLE(PrimNotice_Error, PrimNotice_Text)
public:
	PrimNotice_Error(const char* aMessage, const PrimNotifier* aNotifier = 0);
	PrimNotice_Error(char* aMessage, size_t aSize, const PrimNotifier* aNotifier = 0);
	virtual ~PrimNotice_Error();
	virtual bool not_posted() const;
	virtual bool posted_on(PrimNotifier& aNoticeBoard) const;
};
#endif

