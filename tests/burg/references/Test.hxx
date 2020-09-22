#ifndef TEST_HXX
#define TEST_HXX

#ifndef BURGINSTANCE_HXX
#include <BurgInstance.hxx>
#endif
#ifndef BURG_INC
#include <burg.inc>
#endif

#line 21 "tests/burg/burg.fog"
typedef Burg < Test > BurgOfTest;

class Test
{
private:
#line 25
    BurgOfTest _a;
    
private:
#line 26
    inline Test();
    inline char c();
    void f();
    inline void m();
};

#line 26
inline Test::Test()
:
    _a(BurgInstance::y())
{};

#line 27
inline char Test::c()
{
#line 27
    return ':';
};

#line 29
inline void Test::m()
{
#line 29
    p -> number();
};

#endif
