
#ifndef FOGENUMHANDLE_HXX
#define FOGENUMHANDLE_HXX



template <class T>
class FogEnumHandle
{
 typedef typename T::Enum T_Enum;
private:
 const T *_enum;        //   The flyweight, never ever 0.
public:
 FogEnumHandle() : _enum(&T::flyweight()) {}
 FogEnumHandle(const T& anEnum) : _enum(&anEnum) {}
 FogEnumHandle(T_Enum anEnum) : _enum(&T::flyweight(anEnum)) {}
 FogEnumHandle& operator=(const T& anEnum) { _enum = &anEnum; return *this; }
 FogEnumHandle& operator=(T_Enum anEnum) { _enum = &T::flyweight(anEnum); return *this; }
 const T& operator*() const { return *_enum; }
 const T *operator->() const { return _enum; }
};
#endif

