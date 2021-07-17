#include <Prim/PrimIncludeAll.h>





#ifndef NO_PRIM_STREAMS
PrimVizBufBase::PrimVizBufBase(size_t maxCharacters, const char* dotDotDot)
	:
	_max_chars(maxCharacters + 1),
	_p(maxCharacters <= _max_chars ? _buf : new char[_max_chars]),
	_s(_p, _max_chars),
	_dot_dot_dot(dotDotDot)
{}

PrimVizBufBase::~PrimVizBufBase() {
	if (_p != _buf)
		delete[] _p;
}

std::ostream& operator<<(std::ostream& s, PrimVizBufBase& aVizBufBase) {
	size_t charsSoFar = aVizBufBase._s.pcount();
	aVizBufBase._s << std::ends;
	
	if (charsSoFar < aVizBufBase._max_chars)
		s.write(aVizBufBase._p, charsSoFar);
	else if (!aVizBufBase._dot_dot_dot)
		s.write(aVizBufBase._p, aVizBufBase._max_chars - 1);
	else {
		size_t dotDotDotLen = strlen(aVizBufBase._dot_dot_dot) + 1;
		size_t maxLen = (aVizBufBase._max_chars > dotDotDotLen) ? (aVizBufBase._max_chars - dotDotDotLen) : 0;
		s.write(aVizBufBase._p, maxLen);
		s << aVizBufBase._dot_dot_dot;
	}
	
	return s;
}
#endif
