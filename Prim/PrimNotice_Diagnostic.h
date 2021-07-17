
#ifndef PRIMNOTICE_DIAGNOSTIC_HXX
#define PRIMNOTICE_DIAGNOSTIC_HXX


//
//  	A PrimNotice_Diagnostic defines a notice containing a diagnostic text message.
//
//  	Diagnostic notices are normally
//  	created by the DIAMSG() CONDMSG(), GLOBAL_DIAMSG() or GLOBAL_CONDMSG() macros.
//
class PrimNotice_Diagnostic : public PrimNotice_Text {
	TYPEDECL_SINGLE(PrimNotice_Diagnostic, PrimNotice_Text)
public:
	PrimNotice_Diagnostic(const char* aMessage, const PrimNotifier* aNotifier = 0);
	PrimNotice_Diagnostic(char* aMessage, size_t aSize, const PrimNotifier* aNotifier = 0);
	virtual ~PrimNotice_Diagnostic();
	virtual bool not_posted() const;
	virtual bool posted_on(PrimNotifier& aNoticeBoard) const;
};
#endif

