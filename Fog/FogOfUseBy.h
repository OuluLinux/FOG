
#ifndef FOGOFUSEBY_HXX
#define FOGOFUSEBY_HXX



struct FogOfUseBy_Flyweights {      //   dbxtool goes infinite if T has static array of T.
	static const FogOfUseBy _flyWeights[];
};

class FogOfUseBy {
private:
	enum Of { OF_NAME = 0, OF_TAIL = 1, OF_MASK = OF_NAME | OF_TAIL };
	enum By { BY_HEAD = 0, BY_IMPLEMENTATION = 2, BY_NAME = 4, BY_MASK = BY_HEAD | BY_IMPLEMENTATION | BY_NAME };
public:
	enum OfUseBy {
		OF_NAME_BY_HEAD = OF_NAME | BY_HEAD,
		OF_TAIL_BY_HEAD = OF_TAIL | BY_HEAD,
		OF_NAME_BY_IMPLEMENTATION = OF_NAME | BY_IMPLEMENTATION,
		OF_TAIL_BY_IMPLEMENTATION = OF_TAIL | BY_IMPLEMENTATION,
		OF_NAME_BY_NAME = OF_NAME | BY_NAME
	};
private:
	OfUseBy _of_use_by;
	
private:
	FogOfUseBy(const FogOfUseBy& ofUseBy);
	FogOfUseBy& operator=(const FogOfUseBy& ofUseBy);
	
private:
	friend struct FogOfUseBy_Flyweights;
	FogOfUseBy(OfUseBy ofUseByEnum) : _of_use_by(ofUseByEnum) {}
public:            //   egcs ignores friendship on static destructor
	~FogOfUseBy() {}
	
public:
	const FogOfUseBy& by_head() const
	{ return FogOfUseBy_Flyweights::_flyWeights[(_of_use_by & ~BY_MASK) | BY_HEAD]; }
	const PrimId& id() const;
	bool is_by_head() const { return (_of_use_by & BY_MASK) == BY_HEAD; }
	bool is_by_implementation() const { return (_of_use_by & BY_MASK) == BY_IMPLEMENTATION; }
	bool is_by_name() const { return (_of_use_by & BY_MASK) == BY_NAME; }
	bool is_of_name() const { return (_of_use_by & OF_MASK) == OF_NAME; }
	bool is_of_tail() const { return (_of_use_by & OF_MASK) == OF_TAIL; }
	const FogOfUseBy& of_name() const
	{ return FogOfUseBy_Flyweights::_flyWeights[(_of_use_by & ~OF_MASK) | OF_NAME]; }
	const FogOfUseBy& of_tail() const
	{ return FogOfUseBy_Flyweights::_flyWeights[(_of_use_by & ~OF_MASK) | OF_TAIL]; }
	
public:
	static const FogOfUseBy& of_name_by_implementation()
	{ return FogOfUseBy_Flyweights::_flyWeights[OF_NAME_BY_IMPLEMENTATION]; }
	static const FogOfUseBy& of_name_by_head()
	{ return FogOfUseBy_Flyweights::_flyWeights[OF_NAME_BY_HEAD]; }
	static const FogOfUseBy& of_name_by_name()
	{ return FogOfUseBy_Flyweights::_flyWeights[OF_NAME_BY_NAME]; }
	static const FogOfUseBy& of_tail_by_head()
	{ return FogOfUseBy_Flyweights::_flyWeights[OF_TAIL_BY_HEAD]; }
	static const FogOfUseBy& of_tail_by_implementation()
	{ return FogOfUseBy_Flyweights::_flyWeights[OF_TAIL_BY_IMPLEMENTATION]; }
	static const PrimEnum& values();
	
	friend bool operator==(const FogOfUseBy& firstOfUseBy, const FogOfUseBy& secondOfUseBy)
	{ return firstOfUseBy._of_use_by == secondOfUseBy._of_use_by; }
	friend std::ostream& operator<<(std::ostream& s, const FogOfUseBy& ofUseBy);
};
#endif

