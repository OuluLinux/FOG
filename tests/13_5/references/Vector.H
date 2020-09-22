#ifndef VECTOR_H
#define VECTOR_H

template < class _1 >
class Vector
{
private:
#line 2 "tests/13_5/13_5.fog"
    _1 *v;
    int sz;
    
public:
#line 5
    Vector();
    Vector(int);
#line 8
    _1& operator[](int i);
#line 7
    inline _1& elem(int i);
#line 9
   void swap(Vector& a);
};

template <>
class Vector < void * > 
{
private:
#line 23
    void **p;
    
private:
#line 24
    void *& operator[](int i);
};

template < class _1 >
class Vector < _1 * > : private Vector < void * > 
{
public:
    typedef Vector < void * > Base;
    
public:
#line 32
    inline Vector();
    explicit inline Vector(int i);
#line 35
    inline _1& operator[](int i);
#line 34
    inline _1& elem(int i);
};

#line 7
template < class _1 >
inline _1& Vector < _1 >::elem(int i)
{
#line 7
    return v[i];
};

#line 32
template < class _1 >
inline Vector < _1 * >::Vector()
:
    Base()
{};

#line 33
template < class _1 >
inline Vector < _1 * >::Vector(int i)
:
    Base(i)
{};

#line 35
template < class _1 >
inline _1& Vector < _1 * >::operator[](int i)
{
#line 35
    return static_cast< _1 * &>(Base::operator[](i));
};

#line 34
template < class _1 >
inline _1& Vector < _1 * >::elem(int i)
{
#line 34
    return static_cast< _1 * &>(Base::elem(i));
};

#endif
