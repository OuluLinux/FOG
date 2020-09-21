/*!$@FOG@$!
 *	Generated at Mon Sep 21 07:02:34 2020
 *
 *	by fog 0.1.a of 06:37:39 Sep 21 2020
 *
 *	from
 *		InheritedTypedef.fog
 */

#ifndef EXAMPLE0_H
#define EXAMPLE0_H

class BaseClass0
{
private:
#line 40 "InheritedTypedef.fog"
    inline void method();
};

class IntermediateClass0 : public BaseClass0
{
private:
#line 45
    inline void method();
};

class DerivedClass0 : public IntermediateClass0
{
private:
#line 50
    inline void method();
};

#line 40
inline void BaseClass0::method() {};

#line 50
inline void DerivedClass0::method()
{
#line 50
    IntermediateClass0::method();
};

#line 45
inline void IntermediateClass0::method()
{
#line 45
    BaseClass0::method();
};

#endif
