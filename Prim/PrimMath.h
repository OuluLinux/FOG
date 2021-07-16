// **************************** RACAL Confidential ****************************
// 
//     Title:            Simple mathematical decalarations
// 
//     File Name:        PrimMath.h
// 
//     Author:           E.D.Willink
// 
// END
// 

#ifndef PRIMMATH_H
#define PRIMMATH_H

#ifndef PRIM_H
#include <Prim/Prim.h>
#endif
#ifndef _CMATH_
#include <cmath>
#endif

#ifdef PI
#undef PI
#endif

/*  #define is used rather than const to encourage compile time expression evaluation */
#define PI  0.31415926535897932e+01
#define SQRT2 0.14142135623730950e+01
#define SQRT3 0.173205080757e+01
#define LOG2B10 0.30102999566398119e+00
#define LOG10BE 0.23025850929940456e+01
#define LOG10B2 0.33219280948873623e+01
#define LOGEB2 0.14426950408889634e+01

// 
// 	Return the absolute value of a number.
// .limits
// 	Maximum negative values are preserved.
// 
inline char abs(char aNumber) { return (aNumber >= 0 ? aNumber : -aNumber); }
//#ifndef __GNUC__
//inline double abs(double aNumber) { return (aNumber >= 0 ? aNumber : -aNumber); }
//inline float abs(float aNumber) { return (aNumber >= 0 ? aNumber : -aNumber); }
//#endif
// inline int abs(int aNumber) { return (aNumber >= 0 ? aNumber : -aNumber); }
extern "C" { int abs(int aNumber); }
//#ifndef __GNUC__
//inline long abs(long aNumber) { return (aNumber >= 0 ? aNumber : -aNumber); }
//#endif
inline short abs(short aNumber) { return (aNumber >= 0 ? aNumber : -aNumber); }
inline unsigned char abs(unsigned char aNumber) { return (aNumber); }
inline unsigned int abs(unsigned int aNumber) { return (aNumber); }
inline unsigned long abs(unsigned long aNumber) { return (aNumber); }
inline unsigned short abs(unsigned short aNumber) { return (aNumber); }

// 
// 	Return the larger of two numbers.
// 
#ifndef flagMSC
template <class T>
inline const T& max(const T& firstNumber, const T& secondNumber)
 { return firstNumber >= secondNumber ? firstNumber : secondNumber; }

// 
// 	Return the smaller of two numbers.
// 
template <class T>
inline const T& min(const T& firstNumber, const T& secondNumber)
 { return firstNumber <= secondNumber ? firstNumber : secondNumber; }
#endif

// 
// 	Implement the inequality operator from the equality operator.
// 
template <class T>
inline bool operator!=(const T& firstObject, const T& secondObject) { return !(firstObject == secondObject); }

// 
// 	Implement the less than or equal operator from the less than operator.
// 
// template <class T>
// inline bool operator<=(const T& firstObject, const T& secondObject) { return !(secondObject < firstObject); }

// 
// 	Implement the greater than or equal operator from the less than operator.
// 
// template <class T>
// inline bool operator>=(const T& firstObject, const T& secondObject) { return !(firstObject < secondObject); }

// 
// 	Implement the greater operator from the less than operator.
// 
// template <class T>
// inline bool operator>(const T& firstObject, const T& secondObject) { return secondObject < firstObject; }

#endif
