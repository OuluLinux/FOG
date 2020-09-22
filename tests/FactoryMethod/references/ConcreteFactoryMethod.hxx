#ifndef CONCRETEFACTORYMETHOD_HXX
#define CONCRETEFACTORYMETHOD_HXX

#ifndef ABSTRACTFACTORYMETHOD_HXX
#include <AbstractFactoryMethod.hxx>
#endif

class MyCreator : public Creator
{
public:
#line 7 "tests/FactoryMethod/FactoryMethod.fog"
    virtual Product1 *make_product1();
#line 7
    virtual Product2 *make_product2(const char *aName);
#line 7
    virtual Product3 *make_product3(Class& aClass);
};

class MyProduct1 : public Product1
{
};

class MyProduct2 : public Product2
{
public:
#line 39
    inline MyProduct2(const char *aName);
};

class MyProduct3 : public Product3
{
private:
#line 92
    Class& _class;
    
public:
#line 52
    inline MyProduct3(Class& aClass);
};

#line 39
inline MyProduct2::MyProduct2(const char *aName)
:
    Product2(aName)
{};

#line 52
inline MyProduct3::MyProduct3(Class& aClass)
:
    _class(aClass)
{};

#endif
