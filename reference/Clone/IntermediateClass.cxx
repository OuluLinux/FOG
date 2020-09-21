/*!$@FOG@$!
 *	Generated at Mon Sep 21 06:46:16 2020
 *
 *	by fog 0.1.a of 06:37:39 Sep 21 2020
 *
 *	from
 *		Clone.fog
 */

#ifndef INTERMEDIATECLASS_CXX
#define INTERMEDIATECLASS_CXX

#ifndef INTERMEDIATECLASS_HXX
#include <IntermediateClass.hxx>
#endif

#line 84 "Clone.fog"
IntermediateClass::IntermediateClass(const IntermediateClass& src)
{
#line 67
    
#line 60
    ic0 = src.ic0;
#line 60
    ic1 = src.ic1;
#line 60
    ic2 = src.ic2;;
};

#line 85
void IntermediateClass::operator= (const IntermediateClass& src)
{
#line 75
    
#line 60
    ic0 = src.ic0;
#line 60
    ic1 = src.ic1;
#line 60
    ic2 = src.ic2;;
};

#line 86
RootClass *IntermediateClass::clone() const
{
#line 47
    return new IntermediateClass(*this);
};

#endif
