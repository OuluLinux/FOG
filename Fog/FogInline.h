
#ifndef FOGINLINE_HXX
#define FOGINLINE_HXX




struct FogInline_Initializer;

class FogInlineEnums {
public:
	enum Enum {
		//        IF_SHORT=0, DEFAULT = IF_SHORT,
		//        IF_SHORT=1, DEFAULT = IF_SHORT,
		IMPLICIT_IF_SHORT = 0, DEFAULT = 0,
		EXPLICIT_IF_SHORT = 1,
		IN_IMPLEMENTATION = 2,
		IN_INTERFACE = 4,
		WITH_FRIEND = 8,
		OUT_OF_LINE = 16,
		INVALID = IMPLICIT_IF_SHORT | EXPLICIT_IF_SHORT | IN_IMPLEMENTATION | IN_INTERFACE | WITH_FRIEND | OUT_OF_LINE
	};
public:
	enum Size { ARRAY_SIZE = INVALID + 1 };
};

struct FogInline_Flyweights {
	friend class FogInline;
private:
	static FogInline* _flyweights[FogInlineEnums::ARRAY_SIZE];
	static bool _initialised;
private:
	static bool initialise();
};

class FogInlineMask : public FogInlineEnums {
private:
	Enum _in_line;
public:
	FogInlineMask(Enum anEnum) : _in_line(anEnum) {}
	FogInlineMask& operator|=(const FogInlineMask& aInline) { _in_line = _in_line | aInline; return *this; }
	bool is_explicit_if_short() const { return _in_line == EXPLICIT_IF_SHORT; }
	bool is_if_short() const { return _in_line <= EXPLICIT_IF_SHORT; }
	bool is_implicit_if_short() const { return (_in_line == IMPLICIT_IF_SHORT) && is_valid(); }
	bool is_inline() const { return ((_in_line & (IN_IMPLEMENTATION | IN_INTERFACE)) != 0) && is_valid(); }
	bool is_in_implementation() const { return ((_in_line & IN_IMPLEMENTATION) != 0) && is_valid(); }
	bool is_in_interface() const { return ((_in_line & IN_INTERFACE) != 0) && is_valid(); }
	bool is_out_of_line() const { return (_in_line & OUT_OF_LINE) != 0; }
	bool is_redundant() const { return (_in_line & (IN_IMPLEMENTATION | IN_INTERFACE)) == 0; }
	bool is_valid() const { return _in_line != INVALID; }
	bool is_with_friend() const { return ((_in_line & WITH_FRIEND) != 0) && is_valid(); }
	Enum value() const { return _in_line; }
public:
	friend bool operator==(const FogInlineMask& firstInline, const FogInlineMask& secondInline)
	{ return firstInline._in_line == secondInline._in_line; }
	friend Enum operator|(const FogInlineMask& firstInline, const FogInlineMask& secondInline)
	{ return Enum(firstInline._in_line | secondInline._in_line); }
};

class FogInline : public FogDeclSpecifier, public FogInlineEnums {
	typedef FogInline This;
	typedef FogDeclSpecifier Super;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
private:
	enum IsValid {};                                    //   Useless type for pointer return.
private:
	const FogInlineMask _in_line;
	
private:
	FogInline(const FogInline& aInline);
	FogInline& operator=(const FogInline& aInline);
	
private:
	friend void FogTokenType::initialise();
	FogInline(const FogInline_Initializer& declSpecifier);
	
protected:
	friend  bool FogInline_Flyweights::initialise();
	FogInline(const char* aName = 0, Mask aMask = INVALID_MASK, Enum anEnum = INVALID);
	virtual ~FogInline() {}
public:
	bool is_explicit_if_short() const { return _in_line.is_explicit_if_short(); }
	bool is_if_short() const { return _in_line.is_if_short(); }
	bool is_implicit_if_short() const { return _in_line.is_implicit_if_short(); }
	bool is_inline() const { return _in_line.is_inline(); }
	bool is_in_implementation() const { return _in_line.is_in_implementation(); }
	bool is_in_interface() const { return _in_line.is_in_interface(); }
	bool is_out_of_line() const { return _in_line.is_out_of_line(); }
	bool is_redundant() const { return _in_line.is_redundant(); }
	//      bool is_valid() const { return _in_line.is_valid(); }
	bool is_with_friend() const { return _in_line.is_with_friend(); }
	const FogInlineMask& mask() const { return _in_line; }
	const FogInline& merge_inline(const FogInline& anInline) const;
	const FogMerge& needs_merge_inline(const FogInline& anInline) const;
	//    Enum value() const { return _in_line; }
	
public:
	friend void operator==(const FogInline& firstInline, const FogInline& secondInline);
	//        { return firstInline._in_line == secondInline._in_line; }
	//    friend std::ostream& operator<<(std::ostream& s, const FogInline& anInline);
	
public:
	static FogInline& flyweight(const FogDeclSpecifierValue& aValue);
	static FogInline& flyweight(Enum anEnum = DEFAULT)
	{ return *FogInline_Flyweights::_flyweights[int(anEnum) < int(ARRAY_SIZE) ? anEnum : INVALID]; }
	static FogInline& explicit_if_short() { return *FogInline_Flyweights::_flyweights[EXPLICIT_IF_SHORT]; }
	static FogInline& implicit_if_short() { return *FogInline_Flyweights::_flyweights[IMPLICIT_IF_SHORT]; }
	static FogInline& in_implementation() { return *FogInline_Flyweights::_flyweights[IN_IMPLEMENTATION]; }
	static FogInline& in_interface() { return *FogInline_Flyweights::_flyweights[IN_INTERFACE]; }
	static FogInline& invalid_inline() { return *FogInline_Flyweights::_flyweights[INVALID]; }
	static FogInline& out_of_line() { return *FogInline_Flyweights::_flyweights[OUT_OF_LINE]; }
	static FogInline& with_friend() { return *FogInline_Flyweights::_flyweights[WITH_FRIEND]; }
};

struct FogInline_Initializer {
	FogDeclSpecifier_Initializer _decl_specifier;
	FogInlineMask::Enum _inline_enum;
};

typedef FogEnumHandle<FogInline> FogInlineHandle;
#endif

