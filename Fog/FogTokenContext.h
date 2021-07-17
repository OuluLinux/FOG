
#ifndef FOGTOKENCONTEXT_HXX
#define FOGTOKENCONTEXT_HXX




class FogTokenContext {
	typedef FogTokenContext This;
private:
	FogLineRef _line;
	size_t _column;
	FogTokenRef _value;
public:
	FogTokenContext();
	FogTokenContext(const This& tokenContext);
	FogTokenContext(FogLine& aLine, size_t aColumn);
	FogTokenContext(FogLine& aLine, size_t aColumn, FogToken& aToken);
	~FogTokenContext();
	FogTokenContext& operator=(const FogTokenContext& tokenContext);
	void assign(FogTokenType::TokenType tokenType) { assign(FogTokenType::get(tokenType)); }
	void assign(FogToken& aToken); //   { _value.assign(aToken); }
	void assign(FogLine& aLine, size_t aColumn, FogToken& aToken); //   { _value.assign(aToken); }
	size_t column() const { return _column; }
	FogLine& line() const { return *_line; }
	void reset();
	void set_line(FogLine& aLine, size_t aColumn) { _line = aLine; _column = aColumn; }
	FogToken& token() { return *_value; }
	const FogToken& token() const { return *_value; }
	FogTokenRef& value() { return _value; }
	const FogTokenRef& value() const { return _value; }
public:
	friend bool operator==(const FogTokenContext& aValue, const FogTokenContext& bValue);
	friend std::ostream& operator<<(std::ostream& s, const FogTokenContext& tokenContext);
};
#endif

