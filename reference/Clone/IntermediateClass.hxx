/*!$@FOG@$!
 *	Generated at Mon Sep 21 06:46:16 2020
 *
 *	by fog 0.1.a of 06:37:39 Sep 21 2020
 *
 *	from
 *		Clone.fog
 */

#ifndef INTERMEDIATECLASS_HXX
#define INTERMEDIATECLASS_HXX

#ifndef ROOTCLASS_HXX
#include <RootClass.hxx>
#endif

class IntermediateClass : public RootClass
{
private:
#line 90 "Clone.fog"
    int ic0;
#line 90
    int ic1;
    double ic2;
    
public:
#line 83
    inline IntermediateClass();
    IntermediateClass(const IntermediateClass& src);
    void operator= (const IntermediateClass& src);
    virtual RootClass *clone() const;
};

#line 83
inline IntermediateClass::IntermediateClass() {};

#endif
