#ifndef GLOBAL_CXX
#define GLOBAL_CXX

#ifndef GLOBAL_HXX
#include <global.hxx>
#endif

#line 12 "tests/ansi_c/ansi_c.fog"
static int line1 = f(2 * (y + 1)) + f(2 * (f(2 * (z[0])))) % f(2 * (0)) + t(1);
static int line2 = f(2 * (2 + (3, 4) - 0, 1)) | f(2 * (~5)) & f(2 * (0, 1)) ^ m(0, 1);

#line 29
static void function()
{
    printf("x1= %d, x2= %s", x1, x2); 
    fputs("strncmp(\"abc\\0d\", \"abc\", '\\4') == 0: @\n", s); 
#line 34
    "hello"; 
    "hello, world";
};

#endif
