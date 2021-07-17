
#ifndef PRIMTEXTMONITOR_HXX
#define PRIMTEXTMONITOR_HXX


//
//  	A PrimTextMonitor refines a PrimMonitor to implement a context message defined by a single text string.
//
class PrimTextMonitor : public PrimMonitor {
	typedef PrimTextMonitor This;
	typedef PrimMonitor Super;
	TYPEDECL_SINGLE(This, Super)
private:
	const char* _message;
private:
	virtual bool has_message() const;
	virtual std::ostream& print_message(std::ostream& s) const;
public:
	PrimTextMonitor(const char* aMessage) : _message(aMessage) {}
	virtual std::ostream& print_this(std::ostream& s) const;
};
#endif

