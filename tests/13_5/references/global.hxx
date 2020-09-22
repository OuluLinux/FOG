#ifndef GLOBAL_HXX
#define GLOBAL_HXX

#ifndef VECTOR_H
#include <Vector.H>
#endif
#ifndef LIST3_H
#include <List3.H>
#endif

#line 47 "tests/13_5/13_5.fog"
List3 < int * > li;

#line 16
Vector < int > vi;

#line 19
Vector < char * > vpc;

#line 20
Vector < Node * > vpn;

#line 38
Vector < int * * > vppi;

#line 17
Vector < Shape * > vps;

#line 18
Vector < string > vs;
#line 54
template < class _1 >
inline bool less(_1 a, _1 b);
#line 64
template <>
inline bool less < const char * > (const char *a, const char *b);
#line 69
template <>
inline bool less < const unsigned char * > (const unsigned char *a, const unsigned char *b);
#line 55
template < class _1 >
void sort(Vector < _1 > & v);
#line 74
template < class _1 >
void swap(_1& x, _1& y);
#line 88
template < class _1 >
inline void swap < Vector < _1 > > (Vector < _1 > & x, Vector < _1 > & y);
#line 27
Vector < void * > vpv;

#line 54
template < class _1 >
inline bool less(_1 a, _1 b)
{
#line 54
    return a < b;
};

#line 64
template <>
inline bool less < const char * > (const char *a, const char *b)
{
#line 66
    return strcmp(a, b) < 0;
};

template <>
inline bool less < const unsigned char * > (const unsigned char *a, const unsigned char *b)
{
#line 71
    return memcmp(a, b) < 0;
};

#line 88
template < class _1 >
inline void swap < Vector < _1 > > (Vector < _1 > & x, Vector < _1 > & y)
{
#line 90
    x.swap(y);
};

#endif
