#ifndef MULTIDOLLAR_HXX
#define MULTIDOLLAR_HXX

class A
{
private:
    class B
    {
    };
};

class D
{
private:
    class E
    {
    private:
        class A
        {
        };
    };
};

class F
{
private:
    class G
    {
    private:
#line 25 "tests/multi_dollar/multi_dollar.fog"
        static const char *a0;
        static const char *a1;
        static const char *a2;
        static const char *a3;
        static const char *a4;
#line 16
        static const char *p0;
        static const char *p1;
        static const char *p2;
        static const char *p3;
        static const char *p4;
        static const char *p5;
        static const char *p6;
        static const char *p7;
        static const char *p8;
    };
};

class MultiDollar
{
};

#endif
