
#ifndef FOGLAZYBOOL_HXX
#define FOGLAZYBOOL_HXX




struct FogLazyBool_Flyweights {    //   dbxtool goes infinite if T has static array of T.
	static const FogLazyBool _flyWeights[];
};

class FogLazyBool {
public:
	//
	//  	Enum enumerates the verdicts.
	//
	enum Enum {
		IS_FALSE,                                 //   Definitely false
		IS_TRUE,                                  //   Definitely true
		IS_UNDECIDED,                             //   Undetermined
		DEFAULT = IS_UNDECIDED
	};
private:
	Enum _lazy_bool;
	
private:
	FogLazyBool(const FogLazyBool&);
	FogLazyBool& operator=(const FogLazyBool&);
	
private:
	friend struct FogLazyBool_Flyweights;
	FogLazyBool(Enum elementType) : _lazy_bool(elementType) {}
public:            //   egcs ignores friendship on static destructor
	~FogLazyBool() {}
	
public:
	bool is_false() const { return _lazy_bool == IS_FALSE; }
	bool is_true() const { return _lazy_bool == IS_TRUE; }
	bool is_undecided() const { return _lazy_bool == IS_UNDECIDED; }
	Enum value() const { return _lazy_bool; }
	
public:
	friend bool operator==(const FogLazyBool& firstType, const FogLazyBool& secondType)
	{ return firstType._lazy_bool == secondType._lazy_bool; }
	friend std::ostream& operator<<(std::ostream& s, const FogLazyBool& elementType);
	
public:
	static const FogLazyBool& flyweight(Enum anEnum)
	{ return FogLazyBool_Flyweights::_flyWeights[anEnum <= DEFAULT ? anEnum : DEFAULT]; }
	static const FogLazyBool& make_bool(bool isTrue)
	{ return FogLazyBool_Flyweights::_flyWeights[isTrue ? IS_TRUE : IS_FALSE]; }
	static const FogLazyBool& make_false() { return FogLazyBool_Flyweights::_flyWeights[IS_FALSE]; }
	static const FogLazyBool& make_true() { return FogLazyBool_Flyweights::_flyWeights[IS_TRUE]; }
	static const FogLazyBool& make_undecided() { return FogLazyBool_Flyweights::_flyWeights[IS_UNDECIDED]; }
	static const PrimEnum& values();
};

typedef FogEnumHandle<FogLazyBool> FogLazyBoolHandle;
#endif

