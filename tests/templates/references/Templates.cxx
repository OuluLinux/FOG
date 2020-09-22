#ifndef TEMPLATES_CXX
#define TEMPLATES_CXX

#ifndef TEMPLATES_HXX
#include <Templates.hxx>
#endif
#ifndef Z_H
#include <Z.H>
#endif

#line 59 "tests/templates/templates.fog"
Z < char * > Templates::temp = 0;

#line 36
template <>
void X < double >::dummy() {};

#line 12
template <>
int X < float >::_i = 5;

#endif
