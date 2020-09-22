#ifndef ABSTRACTFACTORYMETHOD_HXX
#define ABSTRACTFACTORYMETHOD_HXX

class Class;
class Product1;
class Product2;
class Product3;

class Creator
{
public:
#line 7 "tests/FactoryMethod/FactoryMethod.fog"
    /*pure*/virtual Product1 *make_product1() = 0;
#line 7
    /*pure*/virtual Product2 *make_product2(const char *aName) = 0;
#line 7
    /*pure*/virtual Product3 *make_product3(Class& aClass) = 0;
};

class Product1
{
};

class Product2
{
private:
#line 35
    const char *_name;
    
public:
#line 37
    inline Product2(const char *aName);
};

class Product3
{
};

#line 37
inline Product2::Product2(const char *aName)
:
    _name(aName)
{};

#endif
