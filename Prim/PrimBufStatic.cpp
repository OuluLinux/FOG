#include <Prim/PrimIncludeAll.h>





//
//  	Construct a new buffer using the supplied buffer, which should be a quoted (null terminated) string
//  	or some other immutable text.
//
PrimBufStatic::PrimBufStatic(const char* aBuf)
	:
	PrimBuf(_static_rep),
	_static_rep(aBuf, strlen(aBuf), 0)
{}
