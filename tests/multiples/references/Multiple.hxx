#ifndef MULTIPLE_HXX
#define MULTIPLE_HXX

class Multiple
{
private:
#line 8 "tests/multiples/multiples.fog"
    const char **x2;
    
public:
#line 6
    static int x;
    
protected:
#line 7
    static char *const *const x3;
#line 7
    static char x4;
};

class Derived : public Multiple
{
#line 7
    static const char *x1;

protected:
#line 7
    static char *x2;
};

#endif
