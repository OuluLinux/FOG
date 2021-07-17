
#ifndef FOGMERGE_HXX
#define FOGMERGE_HXX



struct FogMerge_Flyweights {      //   dbxtool goes infinite if T has static array of T.
	static const FogMerge _flyWeights[];
};

class FogMerge {
	friend class FogMergeHandle;
public:
	enum Enum {
		BOTH_VALID,     //   invocation of merge_from is redundant (already equivalent)
		RIGHT_INVALID,    //   left-hand object is a superset of right-hand
		LEFT_INVALID,    //   right-hand object is a superset of left-hand
		BOTH_INVALID,
		INCOMPATIBLE_ACCESS,
		INCOMPATIBLE_ALLOCATOR,
		INCOMPATIBLE_BIT_FIELD_SIZE,
		INCOMPATIBLE_CONST,
		INCOMPATIBLE_DECLARATOR,
		INCOMPATIBLE_DIMENSION,
		INCOMPATIBLE_ELEMENT_TYPE,
		INCOMPATIBLE_FRIEND,
		INCOMPATIBLE_IDENTIFIER,
		INCOMPATIBLE_INIT,
		INCOMPATIBLE_INLINE,
		INCOMPATIBLE_OVERLOAD,
		INCOMPATIBLE_SEGMENT,
		INCOMPATIBLE_SIGNATURE,
		INCOMPATIBLE_STATIC,
		INCOMPATIBLE_TAG,
		INCOMPATIBLE_TEMPLATE_ARGUMENT,
		INCOMPATIBLE_TEMPLATE_PARAMETER,
		INCOMPATIBLE_TEMPLATE_RANK,
		INCOMPATIBLE_TEMPLATING,
		INCOMPATIBLE_TYPEDEF,
		INCOMPATIBLE_TYPE_IDENTIFIER,
		INCOMPATIBLE_USING,
		INCOMPATIBLE_UTILITY,
		INCOMPATIBLE_VALUE,
		INCOMPATIBLE_VIRTUAL,
		INCOMPATIBLE_VOLATILE,
		INCOMPATIBLE,
		IMPOSSIBLE
	};
private:
	Enum _merge;
	
private:
	FogMerge(const FogMerge& aMerge);
	FogMerge& operator=(const FogMerge& aMerge);
	
private:
	friend struct FogMerge_Flyweights;
	FogMerge(Enum anEnum) : _merge(anEnum) {}
public:            //   egcs ignores friendship on static destructor
	~FogMerge() {}
	
public:
	bool both_are_valid() const { return _merge == BOTH_VALID; }
	const PrimId& id() const;
	bool is_impossible() const { return _merge > INCOMPATIBLE; }
	bool is_incompatible() const { return _merge > BOTH_INVALID; }
	bool left_is_valid() const { return (_merge == RIGHT_INVALID) || (_merge == BOTH_VALID); }
	bool right_is_valid() const { return (_merge == LEFT_INVALID) || (_merge == BOTH_VALID); }
	
public:
	friend const FogMerge& operator|(const FogMerge& firstMerge, const FogMerge& secondMerge);
	friend std::ostream& operator<<(std::ostream& s, const FogMerge& aMerge);
	
public:
	static const FogMerge& both_invalid() { return FogMerge_Flyweights::_flyWeights[BOTH_INVALID]; }
	static const FogMerge& both_valid() { return FogMerge_Flyweights::_flyWeights[BOTH_VALID]; }
	static const FogMerge& flyweight(Enum anEnum)
	{ return FogMerge_Flyweights::_flyWeights[anEnum <= IMPOSSIBLE ? anEnum : IMPOSSIBLE]; }
	static const FogMerge& incompatible() { return FogMerge_Flyweights::_flyWeights[INCOMPATIBLE]; }
	static const FogMerge& incompatible_access()
	{ return FogMerge_Flyweights::_flyWeights[INCOMPATIBLE_ACCESS]; }
	static const FogMerge& incompatible_allocator()
	{ return FogMerge_Flyweights::_flyWeights[INCOMPATIBLE_ALLOCATOR]; }
	static const FogMerge& incompatible_bit_field_size()
	{ return FogMerge_Flyweights::_flyWeights[INCOMPATIBLE_BIT_FIELD_SIZE]; }
	static const FogMerge& incompatible_const() { return FogMerge_Flyweights::_flyWeights[INCOMPATIBLE_CONST]; }
	static const FogMerge& incompatible_declarator()
	{ return FogMerge_Flyweights::_flyWeights[INCOMPATIBLE_DECLARATOR]; }
	static const FogMerge& incompatible_dimension()
	{ return FogMerge_Flyweights::_flyWeights[INCOMPATIBLE_DIMENSION]; }
	static const FogMerge& incompatible_element_type()
	{ return FogMerge_Flyweights::_flyWeights[INCOMPATIBLE_ELEMENT_TYPE]; }
	static const FogMerge& incompatible_friend()
	{ return FogMerge_Flyweights::_flyWeights[INCOMPATIBLE_FRIEND]; }
	static const FogMerge& incompatible_identifier()
	{ return FogMerge_Flyweights::_flyWeights[INCOMPATIBLE_IDENTIFIER]; }
	static const FogMerge& impossible_identifier()
	{ return FogMerge_Flyweights::_flyWeights[INCOMPATIBLE_IDENTIFIER]; }
	static const FogMerge& incompatible_init() { return FogMerge_Flyweights::_flyWeights[INCOMPATIBLE_INIT]; }
	static const FogMerge& incompatible_inline()
	{ return FogMerge_Flyweights::_flyWeights[INCOMPATIBLE_INLINE]; }
	static const FogMerge& incompatible_overload()
	{ return FogMerge_Flyweights::_flyWeights[INCOMPATIBLE_OVERLOAD]; }
	static const FogMerge& incompatible_segment()
	{ return FogMerge_Flyweights::_flyWeights[INCOMPATIBLE_SEGMENT]; }
	static const FogMerge& incompatible_signature()
	{ return FogMerge_Flyweights::_flyWeights[INCOMPATIBLE_SIGNATURE]; }
	static const FogMerge& incompatible_static()
	{ return FogMerge_Flyweights::_flyWeights[INCOMPATIBLE_STATIC]; }
	static const FogMerge& incompatible_tag() { return FogMerge_Flyweights::_flyWeights[INCOMPATIBLE_TAG]; }
	static const FogMerge& incompatible_template_argument()
	{ return FogMerge_Flyweights::_flyWeights[INCOMPATIBLE_TEMPLATE_ARGUMENT]; }
	static const FogMerge& incompatible_template_parameter()
	{ return FogMerge_Flyweights::_flyWeights[INCOMPATIBLE_TEMPLATE_PARAMETER]; }
	static const FogMerge& incompatible_template_rank()
	{ return FogMerge_Flyweights::_flyWeights[INCOMPATIBLE_TEMPLATE_RANK]; }
	static const FogMerge& incompatible_templating()
	{ return FogMerge_Flyweights::_flyWeights[INCOMPATIBLE_TEMPLATING]; }
	static const FogMerge& incompatible_typedef()
	{ return FogMerge_Flyweights::_flyWeights[INCOMPATIBLE_TYPEDEF]; }
	static const FogMerge& incompatible_type_identifier()
	{ return FogMerge_Flyweights::_flyWeights[INCOMPATIBLE_TYPE_IDENTIFIER]; }
	static const FogMerge& incompatible_using()
	{ return FogMerge_Flyweights::_flyWeights[INCOMPATIBLE_USING]; }
	static const FogMerge& incompatible_utility()
	{ return FogMerge_Flyweights::_flyWeights[INCOMPATIBLE_UTILITY]; }
	static const FogMerge& incompatible_value() { return FogMerge_Flyweights::_flyWeights[INCOMPATIBLE_VALUE]; }
	static const FogMerge& incompatible_virtual()
	{ return FogMerge_Flyweights::_flyWeights[INCOMPATIBLE_VIRTUAL]; }
	static const FogMerge& incompatible_volatile()
	{ return FogMerge_Flyweights::_flyWeights[INCOMPATIBLE_VOLATILE]; }
	static const FogMerge& left_invalid() { return FogMerge_Flyweights::_flyWeights[LEFT_INVALID]; }
	static const FogMerge& right_invalid() { return FogMerge_Flyweights::_flyWeights[RIGHT_INVALID]; }
	static const FogMerge& set_incompatible(const FogMerge& aMerge);
	static const PrimEnum& values();
};

class FogMergeHandle {
private:
	const FogMerge* _merge;        //   Never 0
	FogMerge::Enum _enum;        //   Just for debugging.
	
public:
	FogMergeHandle() : _merge(&FogMerge::both_valid()), _enum(FogMerge::BOTH_VALID) {}
	FogMergeHandle(const FogMerge& aMerge) : _merge(&aMerge), _enum(aMerge._merge) {}
	FogMergeHandle& operator=(const FogMerge& aMerge) { _merge = &aMerge; _enum = aMerge._merge; return *this; }
	operator const FogMerge& () const { return *_merge; }
	const FogMerge& operator*() const { return *_merge; }
	const FogMerge* operator->() const { return _merge; }
	FogMergeHandle& operator|=(const FogMerge& aMerge) { return operator=(*_merge | aMerge); }
	friend std::ostream& operator<<(std::ostream& s, const FogMergeHandle& aMerge);
	friend std::istream& operator>>(std::istream& s, FogMergeHandle& aMerge);
};
#endif

