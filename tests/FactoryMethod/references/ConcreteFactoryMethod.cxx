#ifndef CONCRETEFACTORYMETHOD_CXX
#define CONCRETEFACTORYMETHOD_CXX

#ifndef CONCRETEFACTORYMETHOD_HXX
#include <ConcreteFactoryMethod.hxx>
#endif

#line 7 "tests/FactoryMethod/FactoryMethod.fog"
Product1 *MyCreator::make_product1()
{
#line 8
    return new MyProduct1;
};

#line 7
Product2 *MyCreator::make_product2(const char *aName)
{
#line 8
    return new MyProduct2(aName);
};

#line 7
Product3 *MyCreator::make_product3(Class& aClass)
{
#line 8
    return new MyProduct3(aClass);
};

#endif
