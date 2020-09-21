/*!$@FOG@$!
 *	Generated at Sat Sep 14 14:59:18 2019
 *
 *	by fog 0.1.a of 14:58:03 Sep 14 2019
 *
 *	from
 *		Patterns.fog
 */

#ifndef REFERENCECOUNT_CXX
#define REFERENCECOUNT_CXX

#ifndef REFERENCECOUNT_HXX
#include <ReferenceCount.hxx>
#endif

#line 52 "Patterns.fog"
bool ReferenceCount::Dec() const
{
#line 52
    if (shares == 1)
#line 52
        return false;
#line 52
    shares -- ;
#line 52
    return true;
};

#endif
