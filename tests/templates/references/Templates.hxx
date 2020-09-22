#ifndef TEMPLATES_HXX
#define TEMPLATES_HXX

template < class _1 > class Z;

struct Templates
{
public:
#line 59 "tests/templates/templates.fog"
    static Z < char * > temp;
};

template < class _1 >
class X
{
private:
#line 11
    _1 _t;
    
public:
    inline X(_1 aT);
    
private:
#line 12
    static int _i;
};

template <>
class X < int >
{
private:
#line 33
    int _var;
};

#line 14
template < class _1 >
inline X < _1 >::X(_1 aT)
:
    _t(aT)
{};

#endif
