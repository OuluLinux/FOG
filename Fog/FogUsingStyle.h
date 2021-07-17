
#ifndef FOGUSINGSTYLE_HXX
#define FOGUSINGSTYLE_HXX



struct FogUsingStyle_Flyweights {     //   dbxtool goes infinite if T has static array of T.
	static const FogUsingStyle _flyWeights[];
};

class FogUsingStyle {
private:
	enum IsValid {};         //   Useless type for pointer return.
public:
	enum Enum {
		NORMAL,           //   No using
		NAME,           //   ANSI C++ using declaration
		SIGNATURE,          //   FOG use of declaration extension
		ACCESS,           //   ARM C++ access declaration
		INVALID, DEFAULT = NORMAL
	};
private:
	Enum _using;
	
private:
	FogUsingStyle(const FogUsingStyle& aUsing);
	FogUsingStyle& operator=(const FogUsingStyle& aUsing);
	
private:
	friend struct FogUsingStyle_Flyweights;
	FogUsingStyle(Enum anEnum) : _using(anEnum) {}
public:            //   egcs ignores friendship on static destructor
	~FogUsingStyle() {}
	
public:
	operator const IsValid* () const { return (const IsValid*)(_using != INVALID ? this : 0); }
	bool operator!() const { return _using == INVALID; }
	const PrimId& id() const;
	bool is_access() const { return _using == ACCESS; }
	bool is_name() const { return _using == NAME; }
	bool is_signature() const { return _using == SIGNATURE; }
	bool is_using() const { return _using != NORMAL; }
	//      bool is_valid() const { return _using != INVALID; }
	const FogUsingStyle& merge(const FogUsingStyle& aUsing) const;
	const FogMerge& needs_merge(const FogUsingStyle& aUsing) const;
	const char* str() const;
	Enum value() const { return _using; }
	
public:
	friend bool operator==(const FogUsingStyle& firstUsing, const FogUsingStyle& secondUsing)
	{ return firstUsing._using == secondUsing._using; }
	friend std::ostream& operator<<(std::ostream& s, const FogUsingStyle& aUsing);
	
public:
	static const FogUsingStyle& flyweight(Enum anEnum = DEFAULT)
	{ return FogUsingStyle_Flyweights::_flyWeights[anEnum <= INVALID ? anEnum : INVALID]; }
	static const FogUsingStyle& access() { return FogUsingStyle_Flyweights::_flyWeights[ACCESS]; }
	static const FogUsingStyle& invalid() { return FogUsingStyle_Flyweights::_flyWeights[INVALID]; }
	static const FogUsingStyle& name() { return FogUsingStyle_Flyweights::_flyWeights[NAME]; }
	static const FogUsingStyle& signature() { return FogUsingStyle_Flyweights::_flyWeights[SIGNATURE]; }
	static const PrimEnum& values();
};

typedef FogEnumHandle<FogUsingStyle> FogUsingStyleHandle;
#endif

