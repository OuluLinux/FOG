#ifndef Z_C
#define Z_C

#ifndef Z_H
#include <Z.H>
#endif

#line 48 "tests/templates/templates.fog"
template < class _1 >
void Z < _1 >::not_inline()
{
#line 48
    not_inline_code();
};

#line 56
template <>
void Z < double >::not_inline(double d)
{
#line 56
    not_inline_code(d);
};

#endif
