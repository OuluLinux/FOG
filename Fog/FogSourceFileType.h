
#ifndef FOGSOURCEFILETYPE_HXX
#define FOGSOURCEFILETYPE_HXX



struct FogSourceFileType_Flyweights {    //   dbxtool goes infinite if T has static array of T.
	static const FogSourceFileType _flyWeights[];
};

class FogSourceFileType {
public:
	//
	//  	Enum enumerates the nature of the soiurce file.
	//
	enum Enum {
		TOP_INPUT,       //   Source file specified from command line
		HASH_INPUT,       //   Source file #include'd
		USING_INPUT,      //   Source file using/include'd
		UNREAD_INPUT,      //   Source file not-read
		INVALID
	};
private:
	Enum _file_type;
	
private:
	FogSourceFileType(const FogSourceFileType& fileType);
	FogSourceFileType& operator=(const FogSourceFileType& fileType);
	
private:
	friend struct FogSourceFileType_Flyweights;
	FogSourceFileType(Enum fileType) : _file_type(fileType) {}
public:            //   egcs ignores friendship on static destructor
	~FogSourceFileType() {}
	
public:
	bool is_hash() const { return _file_type == HASH_INPUT; }
	bool is_read() const { return _file_type <= USING_INPUT; }
	bool is_top() const { return _file_type == TOP_INPUT; }
	bool is_unread() const { return _file_type == UNREAD_INPUT; }
	Enum value() const { return _file_type; }
	
public:
	friend bool operator==(const FogSourceFileType& firstType, const FogSourceFileType& secondType)
	{ return firstType._file_type == secondType._file_type; }
	friend bool operator!=(const FogSourceFileType& firstType, const FogSourceFileType& secondType)
	{ return firstType._file_type != secondType._file_type; }
	friend std::ostream& operator<<(std::ostream& s, const FogSourceFileType& fileType);
	
public:
	static const FogSourceFileType& hash_input()
	{ return FogSourceFileType_Flyweights::_flyWeights[HASH_INPUT]; }
	static const FogSourceFileType& invalid() { return FogSourceFileType_Flyweights::_flyWeights[INVALID]; }
	static const FogSourceFileType& top_input()
	{ return FogSourceFileType_Flyweights::_flyWeights[TOP_INPUT]; }
	static const FogSourceFileType& unread_input()
	{ return FogSourceFileType_Flyweights::_flyWeights[UNREAD_INPUT]; }
	static const FogSourceFileType& using_input()
	{ return FogSourceFileType_Flyweights::_flyWeights[USING_INPUT]; }
	static const PrimEnum& values();
};

typedef FogEnumHandle<FogSourceFileType> FogSourceFileTypeHandle;
#endif

