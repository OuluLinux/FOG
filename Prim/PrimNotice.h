
#ifndef PRIMNOTICE_HXX
#define PRIMNOTICE_HXX


//
//  		A PrimNotice is the abstract class defining the behaviour of messages which subclasses of
//  		PrimNotifier may post upon themselves.
//  .note tutorial
//  		@file(PrimNotice.tutorial)
//
class PrimNotice : public PrimPrint {
	typedef PrimPrint Super;
	TYPEDECL_SINGLE(PrimNotice, Super)
protected:
	const PrimNotifier* const _posted_by;    //   The notifier that raised the message.
private:
	//
	//  	Creation of a copy is forbidden.
	//
	PrimNotice(const PrimNotice& aNotice);
	//
	//  	Assignment is forbidden.
	//
	PrimNotice& operator=(const PrimNotice& aNotice);
protected:
	//
	//  		Construct a new notice, optionally specifying theNotifier it was posted by.
	//
	PrimNotice(const PrimNotifier* theNotifier = 0) : _posted_by(theNotifier) {}
	virtual ~PrimNotice();
public:
	//
	//  		Report that two abstract notices match. Derived classes should reimplement or augment to
	//  		compare their own fields after verification of run-time type information.
	//  .details
	//  		This method was briefly made virtual with a comparison of run-time-types, derived classes
	//  		augmenting with their own field comparisons. However this proved to be stupid for the test
	//  		suite which multiply inherited the notice and so had an inconsistent type for it.
	//
	bool compare(const PrimNotice& /*aNotice*/) const { /*UNUSED(&aNotice);*/ return (true); }
	virtual bool not_posted() const;
	//
	//  		Report the notifier that posted this notice. The return may be 0 if unknown.
	//
	const PrimNotifier* posted_by() const { return (_posted_by); }
	virtual bool posted_on(PrimNotifier& aNoticeBoard) const;
	virtual const PrimNotice* post_on(const PrimNotifier* aNoticeBoard) const;
#ifndef NO_PRIM_STREAMS
	virtual std::ostream& print_members(std::ostream& s, int indentLevel = 0) const;
#endif
};


#endif

