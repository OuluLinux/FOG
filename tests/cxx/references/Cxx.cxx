#ifndef CXX_CXX
#define CXX_CXX

#ifndef CXX_HXX
#include <Cxx.hxx>
#endif
#ifndef NOTDEPENDENT_HXX
#include <NotDependent.hxx>
#endif

#line 35 "tests/cxx/cxx.fog"
NotDependent Cxx::_v1 = 0;

#line 33
inline void Cxx::clear(int i)
{
#line 43
    STREAM_RMUTEX_LOCK(this, lckp); 
    unsafe_ios::clear(i);
};

#line 24
Cxx& Cxx::mixed_decl(unsigned int i)
{
#line 49
    return i;
};

#line 31
Cxx *Cxx::tie()
{
#line 51
    return this;
};

#line 30
inline Cxx *Cxx::tie(Cxx *s)
{
#line 52
    return this;
};

#line 32
const PrimEnum& Cxx::values()
{
#line 56
    return theValues;
};

#endif
