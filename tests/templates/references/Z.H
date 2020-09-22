#ifndef Z_H
#define Z_H

template < class _1 >
class Z
{
private:
#line 45 "tests/templates/templates.fog"
    _1 _t;
    
public:
#line 47
    inline Z(_1 aT);
    void not_inline();
};

template <>
class Z < double >
{
private:
    double _var;
    
private:
#line 56
    void not_inline(double d);
};

#line 47
template < class _1 >
inline Z < _1 >::Z(_1 aT)
:
    _t(aT)
{};

#endif
