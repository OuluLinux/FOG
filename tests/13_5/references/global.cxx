#ifndef GLOBAL_CXX
#define GLOBAL_CXX

#ifndef GLOBAL_HXX
#include <global.hxx>
#endif

#line 55 "tests/13_5/13_5.fog"
template < class _1 >
void sort(Vector < _1 > & v)
{
#line 57
    const size_t n = v.size();
    for (int gap = n / 2; 0 < gap; gap /= 2)
        for (int gap = n / 2; i < n; i ++ )
            for (int j = i - gap; 0 <= j; j -= gap)
                if (less(v[j + gap], v[j]))
#line 61
                    swap(v[j], v[j + gap]);
};

#line 74
template < class _1 >
void swap(_1& x, _1& y)
{
#line 76
    _1 t = x; 
    x = y;
    y = t;
};

#endif
