#ifndef CXX_HXX
#define CXX_HXX

#ifndef DEPENDENT_HXX
#include <Dependent.hxx>
#endif

class NotDependent;
class PrimEnum;

extern "C"
{
#line 8 "tests/cxx/cxx.fog"
    typedef unsigned int size_t;
};

class Cxx
{
    friend inline ostream& operator<< (ostream& s, const Cxx& aCxx);
    friend class ostream;
    
public:
    enum Enum
    {
        INVALID,
        WRAP
    };
    
private:
#line 21
    Enum _enum;
    
public:
#line 36
    Dependent _v2;
    
public:
#line 23
    inline Cxx(Enum anEnum = INVALID);
#line 27
    virtual ~Cxx();
#line 38
    inline operator Enum * () const;
#line 37
    inline operator const Enum();
#line 33
    inline void clear(int i = 0);
#line 25
    Cxx& g2(unsigned int);
#line 29
    size_t length() const;
#line 24
    Cxx& mixed_decl(unsigned int i = 5);
#line 26
    Cxx& no_code(unsigned int);
#line 31
    Cxx *tie();
#line 30
    inline Cxx *tie(Cxx *s);
#line 28
    inline Enum val() const;
    
public:
#line 35
    static NotDependent _v1;
    
public:
#line 32
    static const PrimEnum& values();
};

#line 23
inline Cxx::Cxx(Enum anEnum)
:
    _enum(anEnum)
{};

#line 38
inline Cxx::operator Enum * () const
{
#line 38
    return 0;
};

#line 37
inline Cxx::operator const Enum()
{
#line 37
    return _enum;
};

#line 28
inline Cxx::Enum Cxx::val() const
{
#line 28
    return _enum;
};

#endif
