#ifndef GLOBAL_CXX
#define GLOBAL_CXX

#ifndef GLOBAL_HXX
#include <global.hxx>
#endif

#line 2 "tests/lexical/lexical.fog"
static const char c = '\t';
#line 1
static const char *s1 = "\tu\r\a\000bc";

#line 5
void f()
{
    static const char * s1 = "\tu\r\a\000bc"; 
    s << '\t' << '\000' << '\a' << '\r';
};

extern "C"
{
#line 11
    unsigned long ulb = 6;
#line 13
    unsigned long ulb2 = 6;
};

extern "Cunsigned"
{
#line 12
    long lb = 6;
};

#endif
