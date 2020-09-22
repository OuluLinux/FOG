#ifndef GLOBAL_CXX
#define GLOBAL_CXX

#ifndef GLOBAL_HXX
#include <global.hxx>
#endif

A < b < c, (d > e) > f2 = 0;
A < A < c, d > , e > f3 = 0;
A < g > f4 = 0;
A < A < c, d > (e), g > f5 = 0;

#endif
