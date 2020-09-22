#ifndef BURGINSTANCE_C
#define BURGINSTANCE_C

#ifndef BURG_INC
#include <burg.inc>
#endif

#line 10 "tests/burg/burg.inc"
template <>
void(*Burg < BurgInstance >::_functions)(const int& ) =
{
    (void(*)(const int & )) & Burg < BurgInstance > ::dummy
};

#line 12
template <>
void Burg < BurgInstance >::adopt(const BurgInstance& anElem)
{
#line 16 "tests/burg/burg.fog"
    instance_code();
};

#endif
