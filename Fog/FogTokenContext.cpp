#include <Fog/FogIncludeAll.h>





FogTokenContext::FogTokenContext() {}

FogTokenContext::FogTokenContext(const This& tokenContext)
	:
	_line(tokenContext._line),
	_column(tokenContext._column),
	_value(tokenContext._value)
{}


FogTokenContext::FogTokenContext(FogLine& aLine, size_t aColumn)
	:
	_line(aLine),
	_column(aColumn)
{}

FogTokenContext::FogTokenContext(FogLine& aLine, size_t aColumn, FogToken& aToken)
	:
	_line(aLine),
	_column(aColumn),
	_value(aToken)
{}

FogTokenContext::~FogTokenContext() {}

FogTokenContext& FogTokenContext::operator=(const FogTokenContext& tokenContext) {
	if (&tokenContext != this) {
		_line = tokenContext._line;
		_column = tokenContext._column;
		_value = tokenContext._value;
	}
	
	return *this;
}

void FogTokenContext::assign(FogToken& aToken) {
	FogLine* aLine = aToken.is_line();
	_line = aLine ? *aLine : FogLine::mutable_null();
	_column = 0;
	_value.assign(aToken);
}

void FogTokenContext::assign(FogLine& aLine, size_t aColumn, FogToken& aToken) {
	_line = aLine;
	_column = aColumn;
	_value.assign(aToken);
}

void FogTokenContext::reset() {
	_line.reset();
	_column = 0;
	_value.reset();
}

std::ostream& operator<<(std::ostream& s, const FogTokenContext& tokenContext) {
	return s << viz(*tokenContext._line) << '.' << tokenContext._column << ": " << tokenContext.token();
}
