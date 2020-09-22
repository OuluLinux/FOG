#ifndef ANON_HXX
#define ANON_HXX

class Anon
{
private:
#line 12 "tests/anons/anons.fog"
    typedef int ptrdiff_t;
    
public:
    enum the_type
    {
        E5 = 5,
        E7 = 7
    };
    
protected:
    enum _anon_1
    {
        TEST,
        V3 = 2
    };
    
public:
#line 3
    int i5 : 5;
    
private:
#line 4
    int : 3;
#line 4
    int i7 : 7;
    int : 0;
    int i9 : 9;
    
public:
#line 7
    enum the_type first_var;
#line 7
    enum the_type second_var;
    
protected:
#line 8
    enum _anon_1 e;
#line 8
    enum _anon_1 f;
    
protected:
    union
    {
    public:
#line 9
        int c1;
#line 9
        int c2;
    };
    
private:
    typedef union
    {
    public:
#line 10
        int a;
#line 10
        int b;
    } _anon_3;
    
private:
#line 10
    union _anon_3 u;
    
protected:
    union
    {
    public:
#line 11
        int d1;
#line 11
        int d2;
    };
};

#endif
