
#ifndef FOGTOKENMONITOR_HXX
#define FOGTOKENMONITOR_HXX




class FogTokenMonitor : public PrimMonitor {
	typedef FogTokenMonitor This;
	typedef PrimMonitor Super;
	TYPEDECL_SINGLE(This, Super)
private:
	const FogToken& _token;
private:
	virtual std::ostream& print_message(std::ostream& s) const;
public:
	FogTokenMonitor(const FogToken& aToken);
	virtual bool post_error(const PrimNotice_Error& aNotice) const;
	virtual bool post_warning(const PrimNotice_Warning& aNotice) const;
	virtual std::ostream& print_this(std::ostream& s) const;
};
#endif

