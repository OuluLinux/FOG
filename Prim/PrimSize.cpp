#include <Prim/PrimIncludeAll.h>




//
//  	Construct an empty array of text strings.
//
const PrimSize& PrimSize::zero() {
	static const PrimSize zeroSize(0);
	return zeroSize;
}
