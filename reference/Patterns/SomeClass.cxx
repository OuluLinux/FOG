/*!$@FOG@$!
 *	Generated at Sat Sep 14 14:59:18 2019
 *
 *	by fog 0.1.a of 14:58:03 Sep 14 2019
 *
 *	from
 *		Patterns.fog
 */

#ifndef SOMECLASS_CXX
#define SOMECLASS_CXX

#ifndef SOMECLASS_HXX
#include <SomeClass.hxx>
#endif

#line 25 "Patterns.fog"
const int& SomeClass::Get() const
{
#line 26
    return some_var1;
#line 26
    return some_var2;
};

#line 22
void SomeClass::Set(const int& value)
{
#line 23
    this -> some_var1 = value;
#line 23
    this -> some_var2 = value;
};

#endif
