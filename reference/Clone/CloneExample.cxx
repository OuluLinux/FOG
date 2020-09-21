/*!$@FOG@$!
 *	Generated at Mon Sep 21 06:46:16 2020
 *
 *	by fog 0.1.a of 06:37:39 Sep 21 2020
 *
 *	from
 *		Clone.fog
 */

#ifndef CLONEEXAMPLE_CXX
#define CLONEEXAMPLE_CXX

#ifndef CLONEEXAMPLE_HXX
#include <CloneExample.hxx>
#endif

#line 118 "Clone.fog"
int CloneExample::Main()
{
#line 119
    ConcreteClass src;
    ConcreteClass * dst = new ConcreteClass(src);
    src = *dst;
    delete dst;
    return 0;
};

#endif
