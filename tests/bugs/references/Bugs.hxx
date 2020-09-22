#ifndef BUGS_HXX
#define BUGS_HXX

extern "C"
{
#line 26 "tests/bugs/bugs.fog"
    typedef unsigned int TangledExtern;
};
class tangledClass;

class Bugs
{
private:
#line 31
    TangledExtern _tangled_extern;
    tangledClass& _tangled_class;
    
private:
#line 17
    inline void f();
    
private:
#line 6
    static void destruct();
};

#ifndef X_HXX
#include <X.hxx>
#endif

#line 17
inline void Bugs::f() {};

#endif
