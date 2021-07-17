
#ifndef FOGACCESS_HXX
#define FOGACCESS_HXX


#undef INITIAL

struct FogAccess_Initializer;

class FogAccessEnums {

public:
	enum Enum {
		AUTO,
		PRIVATE,
		PROTECTED,
		PUBLIC,
		INITIAL,
		INVALID, DEFAULT = INITIAL
	};
	
public:
	enum Size { ARRAY_SIZE = INVALID + 1 };
};

struct FogAccess_Flyweights {

	friend class FogAccess;
	
private:
	static FogAccess* _flyweights[FogAccessEnums::ARRAY_SIZE];
	static bool _initialised;
	
private:
	static bool initialise();
};

class FogAccessValue : public FogAccessEnums {

private:
	Enum _access;
	
public:
	FogAccessValue(Enum anEnum) : _access(anEnum) {}
	
	bool operator!() const {
		return _access >= INVALID;
	}
	
	bool is_auto() const {
		return _access == AUTO;
	}
	
	bool is_emitted() const {
		return _access > AUTO;
	}
	
	bool is_private() const {
		return _access == PRIVATE;
	}
	
	bool is_protected() const {
		return _access == PROTECTED;
	}
	
	bool is_public() const {
		return _access == PUBLIC;
	}
	
	bool is_valid() const {
		return _access < INITIAL;
	}
	
	Enum value() const {
		return _access;
	}
	
public:
	friend bool operator==(const FogAccessValue& firstAccess, const FogAccessValue& secondAccess) {
		return firstAccess._access == secondAccess._access;
	}
	
	friend Enum operator|(const FogAccessValue& firstAccess, const FogAccessValue& secondAccess) {
		return Enum(firstAccess._access | secondAccess._access);
	}
};

class FogAccess : public FogDeclSpecifier, public FogAccessEnums {
	typedef FogAccess This;
	typedef FogDeclSpecifier Super;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	
private:
	enum IsValid {};         //   Useless type for pointer return.
	
private:
	const FogAccessValue _access;
	
private:
	FogAccess(const FogAccess& aAccess);
	FogAccess& operator=(const FogAccess& aAccess);
	
private:
	friend void FogTokenType::initialise();
	FogAccess(const FogAccess_Initializer& declSpecifier);
	
protected:
	friend  bool FogAccess_Flyweights::initialise();
	FogAccess(const char* aName = 0, Mask aMask = INVALID_MASK, Enum anEnum = INVALID);
	virtual ~FogAccess() {}
	
public:
	operator const IsValid* () const {
		return (const IsValid*)(_access.is_valid() ? this : 0);
	}
	
	bool operator!() const {
		return _access.operator!();
	}
	
	bool is_auto() const {
		return _access.is_auto();
	}
	
	bool is_emitted() const {
		return _access.is_emitted();
	}
	
	bool is_private() const {
		return _access.is_private();
	}
	
	bool is_protected() const {
		return _access.is_protected();
	}
	
	bool is_public() const {
		return _access.is_public();
	}
	
	bool is_valid() const {
		return _access.is_valid();
	}
	
	const FogAccess& merge_access(const FogAccess& anAccess) const;
	const FogMerge& needs_merge_access(const FogAccess& anAccess) const;
	const FogAccessValue& value() const {
		return _access;
	}
	
public:
	friend bool operator==(const FogAccess& firstAccess, const FogAccess& secondAccess) {
		return firstAccess._access == secondAccess._access;
	}
	
	//  	friend bool operator==(const FogAccess& firstAccess, FogAccess::Enum secondAccess)
	//  		{ return firstAccess._access == secondAccess; }
	//  	friend std::ostream& operator<<(std::ostream& s, const FogAccess& anAccess);
	
public:
	static FogAccess& auto_access() {
		return *FogAccess_Flyweights::_flyweights[AUTO];
	}
	
	static FogAccess& flyweight(const FogDeclSpecifierValue& aValue);
	static FogAccess& flyweight(Enum anEnum = DEFAULT) {
		return *FogAccess_Flyweights::_flyweights[int(anEnum) < int(ARRAY_SIZE) ? anEnum : INVALID];
	}
	
	static FogAccess& initial_access() {
		return *FogAccess_Flyweights::_flyweights[INITIAL];
	}
	
	static FogAccess& invalid_access() {
		return *FogAccess_Flyweights::_flyweights[INVALID];
	}
	
	static FogAccess& private_access() {
		return *FogAccess_Flyweights::_flyweights[PRIVATE];
	}
	
	static FogAccess& protected_access() {
		return *FogAccess_Flyweights::_flyweights[PROTECTED];
	}
	
	static FogAccess& public_access() {
		return *FogAccess_Flyweights::_flyweights[PUBLIC];
	}
};

struct FogAccess_Initializer {
	FogDeclSpecifier_Initializer _decl_specifier;
	FogAccessValue::Enum _access_enum;
};

typedef FogEnumHandle<FogAccess> FogAccessHandle;
#endif

