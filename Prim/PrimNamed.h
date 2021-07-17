
#ifndef PRIMNAMED_HXX
#define PRIMNAMED_HXX



//
//  		A PrimNamed defines a class with standard printing facilities and a name.
//
class PrimNamed : public PrimPrint {
	TYPEDECL_SINGLE(PrimNamed, PrimPrint)
private:
	PrimStringHandle _name;     //   The name.
public:
	//
	//  		Create a new unnamed instance.
	//
	PrimNamed() {}
	PrimNamed(const char* aName) : _name(aName) {}
	PrimNamed(const PrimString& aName) : _name(aName) {}
	virtual ~PrimNamed();
	//
	//  		Report whether this named instance matches the specified name.
	//
	bool operator==(const PrimString& aName) const { return _name == aName; }
#ifndef NO_PRIM_STREAMS
	virtual std::ostream& print_this(std::ostream& s) const;
#endif
	//
	//  		Report the name.
	//
	const PrimString& name() const { return *_name; }
	//
	//  		Set the name.
	//
	void set_name(const PrimString& aName) { _name = aName; }
};
#endif

