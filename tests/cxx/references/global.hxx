#ifndef GLOBAL_HXX
#define GLOBAL_HXX

class Cxx;
class ostream;
#line 39 "tests/cxx/cxx.fog"
inline ostream& operator<< (ostream& s, const Cxx& aCxx);
#line 47
inline Cxx& g1(unsigned int _i);
inline Cxx& g2(unsigned int _i);

#line 39
inline ostream& operator<< (ostream& s, const Cxx& aCxx)
{
#line 39
    s << "Cxx";
};

#line 47
inline Cxx& g1(unsigned int _i)
{
#line 47
    return *this;
};

#line 48
inline Cxx& g2(unsigned int _i)
{
#line 48
    return *this;
};

#endif
