/*!$@FOG@$!
 *	Generated at Mon Sep 21 06:46:16 2020
 *
 *	by fog 0.1.a of 06:37:39 Sep 21 2020
 *
 *	from
 *		Clone.fog
 */

#ifndef CONCRETECLASS_CXX
#define CONCRETECLASS_CXX

#ifndef CONCRETECLASS_HXX
#include <ConcreteClass.hxx>
#endif

#line 84 "Clone.fog"
ConcreteClass::ConcreteClass(const ConcreteClass& src)
{
#line 67
#line 57
    IntermediateClass::operator= (src);
#line 60
    cc0 = src.cc0;
#line 60
    cc1 = src.cc1;;
};

#line 85
void ConcreteClass::operator= (const ConcreteClass& src)
{
#line 75
#line 57
    IntermediateClass::operator= (src);
#line 60
    cc0 = src.cc0;
#line 60
    cc1 = src.cc1;;
};

#line 86
RootClass *ConcreteClass::clone() const
{
#line 47
    return new ConcreteClass(*this);
};

#endif
