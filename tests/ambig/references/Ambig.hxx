#ifndef AMBIG_HXX
#define AMBIG_HXX

class Ambig
{
public:
#line 8 "tests/ambig/ambig.fog"
    void annul(const Ambig& anElement);
};

class Ambig1
{
    friend inline void annul(const Ambig1& anElement);
    
private:
#line 17
    static void(*_functions[])(const int& );
};

class Ambig2
{
    friend inline void annul(const Ambig2& anElement);
};

#endif
