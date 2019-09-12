
#ifndef FOGSTATIC_HXX
#define FOGSTATIC_HXX




struct FogStatic_Initializer;

class FogStaticEnums {

public:
	enum Enum {
		DEFAULT,
		NOT_STATIC = 1,
		STATIC = 2,
		INVALID        //   STATIC | NOT_STATIC
	};
	
public:
	enum Size { ARRAY_SIZE = INVALID+1 };
};

struct FogStatic_Flyweights {

	friend class FogStatic;
	
private:
	static FogStatic *_flyweights[FogStaticEnums::ARRAY_SIZE];
	static bool _initialised;
	
private:
	static bool initialise();
};

class FogStaticMask : public FogStaticEnums {

private:
	Enum _static;
	
public:
	FogStaticMask(Enum anEnum) : _static(anEnum) {}
	
	bool operator!() const {
		return _static >= INVALID;
	}
	
	bool is_not_static() const {
		return (_static & NOT_STATIC) != 0;
	}
	
	bool is_redundant() const {
		return _static != STATIC;
	}
	
	bool is_static() const {
		return (_static & STATIC) != 0;
	}
	
	bool is_valid() const {
		return _static < INVALID;
	}
	
	Enum value() const {
		return _static;
	}
	
public:
	friend bool operator==(const FogStaticMask& firstStatic, const FogStaticMask& secondStatic) {
		return firstStatic._static == secondStatic._static;
	}
	
	friend Enum operator|(const FogStaticMask& firstStatic, const FogStaticMask& secondStatic) {
		return Enum(firstStatic._static | secondStatic._static);
	}
};

class FogStatic : public FogDeclSpecifier, public FogStaticEnums {
	typedef FogStatic This;
	typedef FogDeclSpecifier Super;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	
private:
	enum IsValid {};         //   Useless type for pointer return.
	
private:
	const FogStaticMask _static;
	
private:
	FogStatic(const FogStatic& aStatic);
	FogStatic& operator=(const FogStatic& aStatic);
	
private:
	friend void FogTokenType::initialise();
	FogStatic(const FogStatic_Initializer& declSpecifier);
	
protected:
	friend  bool FogStatic_Flyweights::initialise();
	FogStatic(const char *aName = 0, Mask aMask = INVALID_MASK, Enum anEnum = INVALID);
	virtual ~FogStatic() {}
	
public:
	operator const IsValid *() const {
		return (const IsValid *)(_static.is_valid() ? this : 0);
	}
	
	bool operator!() const {
		return !_static.is_valid();
	}
	
	bool is_not_static() const {
		return _static.is_not_static();
	}
	
	bool is_redundant() const {
		return _static.is_redundant();
	}
	
	bool is_static() const {
		return _static.is_static();
	}
	
	bool is_valid() const {
		return _static.is_valid();
	}
	
	const FogStatic& merge_static(const FogStatic& aStatic) const;
	const FogMerge& needs_merge_static(const FogStatic& aStatic) const;
	Enum value() const {
		return _static.value();
	}
	
public:
	friend void operator==(const FogStatic& firstStatic, const FogStatic& secondStatic);
//  		{ return firstStatic._static == secondStatic._static; }
//  	friend std::ostream& operator<<(std::ostream& s, const FogStatic& aStatic);

public:
	static FogStatic& flyweight(const FogDeclSpecifierValue& aValue);
	static FogStatic& flyweight(Enum anEnum = DEFAULT) {
		return *FogStatic_Flyweights::_flyweights[int(anEnum) < int(ARRAY_SIZE) ? anEnum : INVALID];
	}
	
	static FogStatic& initial() {
		return *FogStatic_Flyweights::_flyweights[DEFAULT];
	}
	
	static FogStatic& invalid_static() {
		return *FogStatic_Flyweights::_flyweights[INVALID];
	}
	
	static FogStatic& not_static() {
		return *FogStatic_Flyweights::_flyweights[NOT_STATIC];
	}
	
	static FogStatic& yes_static() {
		return *FogStatic_Flyweights::_flyweights[STATIC];
	}
};

struct FogStatic_Initializer {
	FogDeclSpecifier_Initializer _decl_specifier;
	FogStaticMask::Enum _static_enum;
};

typedef FogEnumHandle<FogStatic> FogStaticHandle;
#endif

