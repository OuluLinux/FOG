/*!$@FOG@$!
 *	Generated at Mon Sep 21 07:02:34 2020
 *
 *	by fog 0.1.a of 06:37:39 Sep 21 2020
 *
 *	from
 *		InheritedTypedef.fog
 */

#ifndef EXAMPLE1_H
#define EXAMPLE1_H

class BaseClass1
{
};

class IntermediateClass1 : public BaseClass1
{
public:
#line 63 "InheritedTypedef.fog"
    typedef BaseClass1 Inherited;
};

class DerivedClass1 : public IntermediateClass1
{
public:
#line 63
    typedef IntermediateClass1 Inherited;
};

#endif
