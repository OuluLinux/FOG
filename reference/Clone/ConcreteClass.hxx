/*!$@FOG@$!
 *	Generated at Mon Sep 21 06:46:16 2020
 *
 *	by fog 0.1.a of 06:37:39 Sep 21 2020
 *
 *	from
 *		Clone.fog
 */

#ifndef CONCRETECLASS_HXX
#define CONCRETECLASS_HXX

#ifndef INTERMEDIATECLASS_HXX
#include <IntermediateClass.hxx>
#endif

class ConcreteClass : public IntermediateClass
{
private:
#line 96 "Clone.fog"
    int cc0;
    double cc1;
    
public:
#line 83
    inline ConcreteClass();
    ConcreteClass(const ConcreteClass& src);
    void operator= (const ConcreteClass& src);
    virtual RootClass *clone() const;
};

#line 83
inline ConcreteClass::ConcreteClass() {};

#endif
