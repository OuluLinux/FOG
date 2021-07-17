
#ifndef FOGCV_HXX
#define FOGCV_HXX




struct FogCv_Initializer;

class FogCvEnums {

public:
	enum Enum {
		NONE, DEFAULT = NONE,
		CONST = 1,
		NOT_CONST = 2,
		VOLATILE = 4,
		NOT_VOLATILE = 8,
		CONST_VOLATILE = CONST | VOLATILE,
		CONST_NOT_VOLATILE = CONST | NOT_VOLATILE,
		NOT_CONST_VOLATILE = NOT_CONST | VOLATILE,
		NOT_CONST_NOT_VOLATILE = NOT_CONST | NOT_VOLATILE,
		INVALID = CONST | NOT_CONST | VOLATILE | NOT_VOLATILE
	};
	
public:
	enum Size { ARRAY_SIZE = INVALID + 1 };
};

struct FogCv_Flyweights {

	friend class FogCv;
	
private:
	static FogCv* _flyweights[FogCvEnums::ARRAY_SIZE];
	static bool _initialised;
	
private:
	static bool initialise();
};

class FogCvMask : public FogCvEnums {

private:
	Enum _cv;
	
public:
	FogCvMask(Enum anEnum) : _cv(anEnum) {}
	
	FogCvMask& operator|=(const FogCvMask& aCv) {
		_cv = _cv | aCv;
		return *this;
	}
	
	bool is_const() const {
		return (_cv & CONST) ? true : false;
	}
	
	bool is_cv() const {
		return (_cv & (CONST | VOLATILE)) ? true : false;
	}
	
	bool is_not_const() const {
		return (_cv & NOT_CONST) ? true : false;
	}
	
	bool is_not_volatile() const {
		return (_cv & NOT_VOLATILE) ? true : false;
	}
	
	bool is_valid() const {
		return _cv >= INVALID;
	}
	
	bool is_volatile() const {
		return (_cv & VOLATILE) ? true : false;
	}
	
	Enum value() const {
		return _cv;
	}
	
public:
	friend bool operator==(const FogCvMask& firstCv, const FogCvMask& secondCv) {
		return firstCv._cv == secondCv._cv;
	}
	
	friend Enum operator|(const FogCvMask& firstCv, const FogCvMask& secondCv) {
		return Enum(firstCv._cv | secondCv._cv);
	}
};

class FogCv : public FogDeclSpecifier, public FogCvEnums {
	typedef FogCv This;
	typedef FogDeclSpecifier Super;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	
private:
	enum IsValid {};         //   Useless type for pointer return.
	
private:
	const FogCvMask _cv;
	
private:
	FogCv(const FogCv& aCv);
	FogCv& operator=(const FogCv& aCv);
	
private:
	friend void FogTokenType::initialise();
	FogCv(const FogCv_Initializer& declSpecifier);
	
protected:
	friend  bool FogCv_Flyweights::initialise();
	FogCv(const char* aName = 0, Mask aMask = INVALID_MASK, Enum anEnum = INVALID);
	virtual ~FogCv() {}
	
public:
	//  	operator const IsValid *() const { return (const IsValid *)(_cv.is_valid() ? this : 0); }
	//  	bool operator!() const { return !_cv.is_valid(); }
	
public:
	FogCv& add(const FogCv& aCv) const {
		return flyweight(_cv | aCv.mask());
	}
	
	const FogCv& compile() const;
	bool is_const() const {
		return _cv.is_const();
	}
	
	bool is_cv() const {
		return _cv.is_cv();
	}
	
	bool is_not_const() const {
		return _cv.is_not_const();
	}
	
	bool is_not_volatile() const {
		return _cv.is_not_volatile();
	}
	
	bool is_valid() const {
		return _cv.is_valid();
	}
	
	bool is_volatile() const {
		return _cv.is_volatile();
	}
	
	const FogCvMask& mask() const {
		return _cv;
	}
	
	Enum merge_cv(const FogCv& aCv) const;
	Enum merge_cv(Enum aCv) const;
	virtual const FogMetaType& meta_type() const;
	const FogMerge& needs_merge_cv(const FogCv& aCv) const;
	//  	virtual bool resolve_semantics(FogSemantics& theSemantics, FogScopeContext& inScope) const;
	//      Enum value() const { return _cv; }
	
public:
	friend void operator==(const FogCv& firstCv, const FogCv& secondCv);
	//  		{ return firstCv._cv == secondCv._cv; }
	//  	friend std::ostream& operator<<(std::ostream& s, const FogCv& aCv);
	
public:
	static FogCv& flyweight(Enum anEnum = DEFAULT) {
		return *FogCv_Flyweights::_flyweights[int(anEnum) < int(ARRAY_SIZE) ? anEnum : INVALID];
	}
	
	static FogCv& initial() {
		return *FogCv_Flyweights::_flyweights[DEFAULT];
	}
	
	static FogCv& invalid_cv() {
		return *FogCv_Flyweights::_flyweights[INVALID];
	}
	
	static FogCv& not_const() {
		return *FogCv_Flyweights::_flyweights[NOT_CONST];
	}
	
	static FogCv& not_volatile() {
		return *FogCv_Flyweights::_flyweights[NOT_VOLATILE];
	}
	
	static FogCv& yes_const() {
		return *FogCv_Flyweights::_flyweights[CONST];
	}
	
	static FogCv& yes_volatile() {
		return *FogCv_Flyweights::_flyweights[VOLATILE];
	}
};

struct FogCv_Initializer {
	FogDeclSpecifier_Initializer _decl_specifier;
	FogCvMask::Enum _cv_enum;
};

typedef FogEnumHandle<FogCv> FogCvHandle;
#endif

