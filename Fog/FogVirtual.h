
#ifndef FOGVIRTUAL_HXX
#define FOGVIRTUAL_HXX




struct FogVirtual_Initializer;

class FogVirtualEnums {
public:
	enum Enum {
		NONVIRTUAL = 0, DEFAULT = NONVIRTUAL,
		VIRTUAL = 1,
		PURE_NONVIRTUAL = 2,
		JUST_PURE = PURE_NONVIRTUAL,
		PURE_VIRTUAL = VIRTUAL | PURE_NONVIRTUAL, PURE = PURE_VIRTUAL,
		NOT_VIRTUAL = 4,
		INVALID = VIRTUAL | PURE_NONVIRTUAL | NOT_VIRTUAL
	};
public:
	enum Size { ARRAY_SIZE = INVALID + 1 };
};

struct FogVirtual_Flyweights {
	friend class FogVirtual;
private:
	static FogVirtual* _flyweights[FogVirtualEnums::ARRAY_SIZE];
	static bool _initialised;
private:
	static bool initialise();
};

class FogVirtualMask : public FogVirtualEnums {
private:
	Enum _virtual;
public:
	FogVirtualMask(Enum anEnum) : _virtual(anEnum) {}
	bool operator!() const { return _virtual >= INVALID; }
	bool is_not_virtual() const { return (_virtual & NOT_VIRTUAL) != 0; }
	bool is_pure() const { return (_virtual & PURE_NONVIRTUAL) != 0; }
	bool is_pure_nonvirtual() const { return _virtual == PURE_NONVIRTUAL; }
	bool is_pure_virtual() const { return _virtual == PURE_VIRTUAL; }
	bool is_redundant() const { return (_virtual & PURE_VIRTUAL) == 0; }
	bool is_valid() const { return _virtual != INVALID; }
	bool is_virtual() const { return (_virtual & VIRTUAL) != 0; }
	const FogVirtual& merge_virtual(const FogVirtual& aVirtual) const;
	const FogMerge& needs_merge_virtual(const FogVirtual& aVirtual) const;
	Enum strip_purity() const { return Enum(_virtual & ~JUST_PURE); }
	Enum value() const { return _virtual; }
public:
	friend bool operator==(const FogVirtualMask& firstVirtual, const FogVirtualMask& secondVirtual)
	{ return firstVirtual._virtual == secondVirtual._virtual; }
	friend Enum operator|(const FogVirtualMask& firstVirtual, const FogVirtualMask& secondVirtual)
	{ return Enum(firstVirtual._virtual | secondVirtual._virtual); }
};

class FogVirtual : public FogDeclSpecifier, public FogVirtualEnums {
	typedef FogVirtual This;
	typedef FogDeclSpecifier Super;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
private:
	enum IsValid {};         //   Useless type for pointer return.
private:
	const FogVirtualMask _virtual;
	
private:
	bool is_valid() const { return _virtual.is_valid(); } //   private because of syntactic/semantic ambiguity
	
private:
	FogVirtual(const FogVirtual& aVirtual);
	FogVirtual& operator=(const FogVirtual& aVirtual);
	
private:
	friend void FogTokenType::initialise();
	FogVirtual(const FogVirtual_Initializer& declSpecifier);
	
protected:
	friend  bool FogVirtual_Flyweights::initialise();
	FogVirtual(const char* aName = 0, Mask aMask = INVALID_MASK, Enum anEnum = INVALID);
	virtual ~FogVirtual() {}
	
public:
	operator const IsValid* () const { return (const IsValid*)(_virtual.is_valid() ? this : 0); }
	bool operator!() const { return !_virtual.is_valid(); }
	bool is_not_virtual() const { return _virtual.is_not_virtual(); }
	bool is_pure() const { return _virtual.is_pure(); }
	bool is_pure_nonvirtual() const { return _virtual.is_pure_nonvirtual(); }
	bool is_pure_virtual() const { return _virtual.is_pure_virtual(); }
	bool is_redundant() const { return _virtual.is_redundant(); }
	bool is_virtual() const { return _virtual.is_virtual(); }
	const FogVirtual& merge_virtual(const FogVirtual& aVirtual) const;
	const FogMerge& needs_merge_virtual(const FogVirtual& aVirtual) const;
	Enum strip_purity() const { return _virtual.strip_purity(); }
	
public:
	friend bool operator==(const FogVirtual& firstVirtual, const FogVirtual& secondVirtual)
	{ return firstVirtual._virtual == secondVirtual._virtual; }
	
public:
	static FogVirtual& flyweight(Enum anEnum = DEFAULT)
	{ return *FogVirtual_Flyweights::_flyweights[int(anEnum) < int(ARRAY_SIZE) ? anEnum : INVALID]; }
	static FogVirtual& flyweight(const FogDeclSpecifierValue& aValue);
	static FogVirtual& initial() { return *FogVirtual_Flyweights::_flyweights[DEFAULT]; }
	static FogVirtual& invalid_virtual() { return *FogVirtual_Flyweights::_flyweights[INVALID]; }
	//  	static FogVirtual& just_pure() { return FogVirtual_Flyweights::_flyweights[JUST_PURE]; }
	static FogVirtual& not_virtual() { return *FogVirtual_Flyweights::_flyweights[NOT_VIRTUAL]; }
	static FogVirtual& pure_virtual() { return *FogVirtual_Flyweights::_flyweights[PURE_VIRTUAL]; }
	static FogVirtual& yes_virtual() { return *FogVirtual_Flyweights::_flyweights[VIRTUAL]; }
};

struct FogVirtual_Initializer {
	FogDeclSpecifier_Initializer _decl_specifier;
	FogVirtualMask::Enum _virtual_enum;
};

typedef FogEnumHandle<FogVirtual> FogVirtualHandle;
#endif

