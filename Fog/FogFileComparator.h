
#ifndef FOGFILECOMPARATOR_HXX
#define FOGFILECOMPARATOR_HXX



class FogFileComparator {
public:
	enum Status {
		NEW_TEXT,
		CHANGED_TEXT,
		SAME_TEXT,
		NON_FOG_TEXT
	};
private:
	const unsigned char* _p;
	long _return_count;
	bool _escaped : 1;
	bool _single_quoted : 1;
	bool _double_quoted : 1;
private:
	FogFileComparator(const char* p)
		:
		_p((const unsigned char*)p),
		_return_count(0),
		_escaped(false),
		_single_quoted(false),
		_double_quoted(false)
	{}
	int next();
public:
	static Status equivalent(const char* newText, const PrimId& oldFile);
};
#endif

