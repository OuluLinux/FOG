#include <Prim/PrimIncludeAll.h>






PrimOstrstream::PrimOstrstream()
	:
	_fixed_buffer(false),
	_ends(false)
{}

PrimOstrstream::PrimOstrstream(char* fixedBuffer, int fixedSize, ios_base::openmode aMode)
	:
	Super(fixedBuffer, fixedSize, aMode),
	_fixed_buffer(true),
	_ends(false)
{}

PrimOstrstream::~PrimOstrstream() {
	if (!_fixed_buffer)
		delete[] Super::str();
}

PrimOstrstream& PrimOstrstream::ends() {
	std::ends(super());
	_ends = true;
	return *this;
}

const PrimId& PrimOstrstream::id() {
	if (!_id)
		_id = PrimIdHandle(str());
		
	return *_id;
}

const char* PrimOstrstream::str() {
	if (!_ends)
		ends();
		
	return Super::str();
}

const char* PrimOstrstream::str() const {
	return ((PrimOstrstream*)this)->str();
}

size_t PrimOstrstream::strlen() { return ::strlen(str()); }

const PrimString& PrimOstrstream::string() {
	if (!_string)
		_string = str();
		
	return *_string;
}

std::ostream& operator<<(std::ostream& s, PrimOstrstream& anObject) {
	s << anObject.str();
	return s;
}

std::ostream& operator<<(std::ostream& s, const PrimOstrstream& anObject) {
	s << anObject.str();
	return s;
}
