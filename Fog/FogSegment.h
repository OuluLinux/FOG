
#ifndef FOGSEGMENT_HXX
#define FOGSEGMENT_HXX




struct FogSegment_Initializer;

class FogSegmentEnums {
public:
	enum Enum {
		ENTRY = 0,
		PRE,
		//  		CONSTRUCT,
		BODY,
		//  		DESTRUCT,
		POST,
		EXIT,
		INVALID, DEFAULT = INVALID,
		ZERO = ENTRY,      //   Initial value
		NUM_SEGMENTS = INVALID    //   Invalid value and count of useful segments
	};
public:
	enum Size { ARRAY_SIZE = INVALID + 1 };
};

struct FogSegment_Flyweights {
	friend class FogSegment;
private:
	static const FogSegment_Initializer _invalid_init;
	static FogSegment* _flyweights[FogSegmentEnums::ARRAY_SIZE];
	static FogSegment& invalid();
};

class FogSegmentValue : public FogSegmentEnums {
private:
	Enum _segment;
public:
	FogSegmentValue(Enum anEnum) : _segment(anEnum) {}
	//  	bool operator!() const { return _segment >= INVALID; }
	//  	bool ctor_order() const { return (_segment == CONSTRUCT) ||  (_segment == PRE); }
	bool ctor_order() const { return _segment == PRE; }
	bool is_body() const { return _segment == BODY; }
	bool is_exit() const { return _segment == EXIT; }
	bool is_valid() const { return _segment != INVALID; }
	Enum value() const { return _segment; }
public:
	friend bool operator==(const FogSegmentValue& firstPosition, const FogSegmentValue& secondPosition)
	{ return firstPosition._segment == secondPosition._segment; }
	friend Enum operator|(const FogSegmentValue& firstPosition, const FogSegmentValue& secondPosition)
	{ return Enum(firstPosition._segment | secondPosition._segment); }
};

class FogSegment : public FogIdentifier, public FogSegmentEnums {
	typedef FogSegment This;
	typedef FogIdentifier Super;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
private:
	FogSegmentValue _segment;
	const FogTokenType::TokenType _token_type;
	
private:
	FogSegment(const FogSegment& aSegment);
	FogSegment& operator=(const FogSegment& aSegment);
	
protected:
	friend struct FogSegment_Flyweights;
	friend void FogTokenType::initialise();
	FogSegment(const FogSegment_Initializer& segmentInit = FogSegment_Flyweights::_invalid_init);
	
public:
	bool ctor_order() const { return _segment.ctor_order(); }
	bool is_body() const { return _segment.is_body(); }
	bool is_exit() const { return _segment.is_exit(); }
	bool is_valid() const { return _segment.is_valid(); }
	const FogSegment& merge_segment(const FogSegment& aSegment) const;
	const FogMerge& needs_merge_segment(const FogSegment& aSegment) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	virtual FogTokenType::TokenType token_type_enum() const;
	Enum value() const { return _segment.value(); }
	
public:
	friend bool operator==(const FogSegment& firstSegment, const FogSegment& secondSegment)
	{ return firstSegment._segment == secondSegment._segment; }
	
public:
	static FogSegment& body() { return *FogSegment_Flyweights::_flyweights[BODY]; }
	//  	static FogSegment& construct() { return *FogSegment_Flyweights::_flyweights[CONSTRUCT]; }
	//  	static FogSegment& destruct() { return *FogSegment_Flyweights::_flyweights[DESTRUCT]; }
	static FogSegment& entry() { return *FogSegment_Flyweights::_flyweights[ENTRY]; }
	static FogSegment& exit() { return *FogSegment_Flyweights::_flyweights[EXIT]; }
	static FogSegment& flyweight(Enum anEnum = DEFAULT)
	{ return *FogSegment_Flyweights::_flyweights[int(anEnum) < int(ARRAY_SIZE) ? anEnum : INVALID]; }
	static FogSegment& invalid_segment() { return *FogSegment_Flyweights::_flyweights[INVALID]; }
	static FogSegment& post() { return *FogSegment_Flyweights::_flyweights[POST]; }
	static FogSegment& pre() { return *FogSegment_Flyweights::_flyweights[PRE]; }
};

class FogSegmentIterator {
private:
	enum IsValid {};         //   Useless type for pointer return.
private:
	int _i;
public:
	FogSegmentIterator() : _i(FogSegment::ZERO) {}
	void operator++() { _i = _i < FogSegment::NUM_SEGMENTS ? _i + 1 : _i; }
	operator const IsValid* () const { return (const IsValid*)(_i != FogSegment::INVALID ? this : 0); }
	bool operator!() const { return _i == FogSegment::INVALID; }
	const FogSegment& operator*() const { return FogSegment::flyweight(FogSegment::Enum(_i)); }
	const FogSegment* operator->() const { return &FogSegment::flyweight(FogSegment::Enum(_i)); }
	FogSegment::Enum value() const { return FogSegment::Enum(_i); }
};

struct FogSegment_Initializer {
	const char* _name;
	FogTokenType::TokenType _table_enum;  //   Index for storage in FogTokenType_id_table
	FogSegment::Enum _segment_enum;    //   Index into tags.
};

typedef FogEnumHandle<FogSegment> FogSegmentHandle;
#endif

