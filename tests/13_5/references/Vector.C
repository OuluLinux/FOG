#ifndef VECTOR_C
#define VECTOR_C

#ifndef VECTOR_H
#include <Vector.H>
#endif

#line 9 "tests/13_5/13_5.fog"
template < class _1 >
void Vector < _1 >::swap(Vector& a)
{
#line 83
    swap(v, a.v);
    swap(sz, a.sz);
};

#endif
