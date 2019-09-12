#include <Prim/PrimIncludeAll.h>















#if defined(NEEDS_STD)

#elif !defined(__GNUC__)

#endif

#ifndef LONG_BIT
#if (ULONG_MAX == UCHAR_MAX)
#define LONG_BIT CHAR_BIT
#elif ((ULONG_MAX - (UCHAR_MAX << CHAR_BIT)) <= (UCHAR_MAX << CHAR_BIT))
#define LONG_BIT (CHAR_BIT << 1)
#elif ((ULONG_MAX - (UCHAR_MAX << (3*CHAR_BIT))) <= (UCHAR_MAX << (3*CHAR_BIT)))
#define LONG_BIT (CHAR_BIT << 2)
#else
#define LONG_BIT (CHAR_BIT << 3)
#endif
#endif

TYPEINFO_SINGLE(PrimNumber, Super)
PRIMREF_NULL_CLASS_AND_IMPL(PrimNumber)
PRIMSHARES_IMPL(PrimNumber)
PRIMREFS_IMPL(PrimNumber)
PRIMLISTOFREFS_IMPL(PrimNumber)
PRIMMAPOFREFS_IMPL(PrimNumber)


TMPL_HACK_FIX_DO(PrimNumber);

struct PrimNumber_info
{
 int _sign;       //   +1 +ve, 0 zero, -1 -ve.
 const char *_start;     //   First character of mantissa/exponent.
 int _size;       //   Number of characters in mantissa/exponent.
};

PrimNumber::PrimNumber()
:
 _id(PrimId::null()),
 _index(0)       //   Means null.
{}

PrimNumber::PrimNumber(const PrimId& anId)
:
 _id(anId),
 _index(1)       //   Means unordered.
{
 map().add(*this);
}

//  
//  	Destruction eliminates the references to the number.
//  .bugbug
//  		but never happens because destruction awaits no references.
//  
PrimNumber::~PrimNumber()
{
 map().remove(id());
}

const PrimNumber& PrimNumber::operator-() const
{
 const char *p = id().str();
 if (*p == 'z')
  return *this;
 PrimOstrstream s;
 s << (*p == 'm' ? 'p' : 'm') << (p+1) << std::ends;
 const double checkValue = -as_double();
 return PrimNumber::make(s.str(), &checkValue);
}

bool PrimNumber::operator!() const
{
 return is_zero();
}

const PrimNumber& PrimNumber::operator~() const
{
 ERRMSG("BUG -- Unimplemented PrimNumber::operator~");
 return PrimNumber::null();
}

//  
//  	Return a boolean representation interpreting any non-zero value as true.
//  
bool PrimNumber::as_bool() const { return !is_zero(); }

//  
//  	Return a decimal ASCII representation of the number.
//  
//  .limits
//  	Precision is limited by that available with a double precision floating point number.
//  
PrimStringHandle PrimNumber::as_decimal() const
{
 PrimOstrstream s;
 print_decimal(s);
 s << std::ends;
 return PrimStringHandle(s.str());
}

//  
//  	Return a conventional floating point representation of the number.
//  
//  .limits
//  	Precision is limited by that available with a double precision floating point number.
//  
double PrimNumber::as_double() const
{
 const char *p = _id.str();
 bool isNeg = *p++ == 'm';
 while (isxdigit(*p))
  p++;
 double mantissaValue = 0.0;
 for (const char *q = p; isxdigit(*(q-1)); q--, mantissaValue /= 16.0)
 {
  int aDigit = isdigit(*(q-1)) ? (*(q-1) - '0') : (tolower(*(q-1)) - 'a' + 10);
  mantissaValue += aDigit;
 }
 if (isNeg)
  mantissaValue = -mantissaValue;
 isNeg = *p++ == 'm';
 double exponentValue = 0.0;
 for (double exponentWeight = 1.0; isxdigit(*p); p++)
 {
  exponentValue *= 16.0;
  int aDigit = isdigit(*p) ? (*p - '0') : (tolower(*p) - 'a' + 10);
  exponentValue += aDigit;
 }
 if (isNeg)
  exponentValue = -exponentValue;
 double theValue =  mantissaValue * pow(16.0, exponentValue);
#if defined(NEEDS_STD)
 if (isinf(theValue))
  theValue = isNeg ? DBL_MIN : DBL_MAX;
#elif !defined(__GNUC__)   //   limits missing from egcs 1.0.2
 if (!std::numeric_limits<double>::has_infinity)
  ;
 else if (theValue >= std::numeric_limits<double>::infinity())
  theValue = std::numeric_limits<double>::max();
 else if (theValue <= -std::numeric_limits<double>::infinity())
  theValue = std::numeric_limits<double>::min();
#endif
 return theValue;
}

//  
//  	Return a hexadecimal ASCII representation of the number. The value retains the intrisicx normalisation
//  	and is compatible with make_hexadecimal.
//  
PrimStringHandle PrimNumber::as_hexadecimal() const
{
 PrimOstrstream s;
 print_hexadecimal(s);
 s << std::ends;
 return PrimStringHandle(s.str());
}

//  
//  	Return the conventional integer representation of the number.
//  
//  	is_int() should be used as a predicate to avoid errors from out of range numbers.
//  
long PrimNumber::as_int() const
{
 PrimNumber_info mantissaInfo;
 mantissa(mantissaInfo);
 if (mantissaInfo._sign == 0)
  return 0;
 PrimNumber_info exponentInfo;
 exponent(exponentInfo);
 if (exponentInfo._sign <= 0)
 {
  ERRMSG("BUG - cannot return as_int from fractional " << viz(*this));
  return 0;
 }
 if (exponentInfo._size > 1)
 {
  ERRMSG("BUG - cannot return as_int from over-range " << viz(*this));
  return mantissaInfo._sign ? LONG_MAX : LONG_MIN;
 }
 int expDigit = *exponentInfo._start - '0';     //   '0' to '9' will do.
 if (expDigit > (LONG_BIT / 4))        //  .bugbug assumes LONG_BIT is a multiple of 4.
 {
  ERRMSG("BUG - cannot return as_int from over-range " << viz(*this));
  return mantissaInfo._sign ? LONG_MAX : LONG_MIN;
 }
 if (mantissaInfo._size > expDigit)
 {
  ERRMSG("BUG - cannot return as_int from over-range " << viz(*this));
  return mantissaInfo._sign ? LONG_MAX : LONG_MIN;
 }
 unsigned long mantissaValue = 0;
 for (const char *p = mantissaInfo._start; isxdigit(*p); p++)
 {
  int aDigit = isdigit(*p) ? (*p - '0') : (tolower(*p) - 'a' + 10);
  mantissaValue = (mantissaValue << 4) + aDigit;
 }
 while (expDigit-- > mantissaInfo._size)
  mantissaValue <<= 4;
 if (mantissaInfo._sign > 0)
 {
  if (mantissaValue <= LONG_MAX)
   return long(mantissaValue);
  ERRMSG("BUG - cannot return as_int from over-range " << viz(*this));
  return LONG_MAX;
 }
 else
 {
  if (mantissaValue < (1uL+((unsigned long)-(LONG_MIN+1))))
   return -long(mantissaValue);
  if (mantissaValue > (1uL+((unsigned long)-(LONG_MIN+1))))
   ERRMSG("BUG - cannot return as_int from over-range " << viz(*this));
  return LONG_MIN;
 }
}

//  
//  	Return the conventional natural representation of the number.
//  
//  	is_nat() should be used as a predicate to avoid errors from out of range numbers.
//  
unsigned long PrimNumber::as_nat() const
{
 PrimNumber_info mantissaInfo;
 mantissa(mantissaInfo);
 if (mantissaInfo._sign < 0)
 {
  ERRMSG("BUG - cannot return as_nat from -ve " << viz(*this));
  return 0;
 }
 if (mantissaInfo._sign == 0)
  return 0;
 PrimNumber_info exponentInfo;
 exponent(exponentInfo);
 if (exponentInfo._sign <= 0)
 {
  ERRMSG("BUG - cannot return as_nat from fractional " << viz(*this));
  return 0;
 }
 if (exponentInfo._size > 1)
 {
  ERRMSG("BUG - cannot return as_nat from over-range " << viz(*this));
  return ULONG_MAX;
 }
 int expDigit = *exponentInfo._start - '0';     //   '0' to '9' will do.
 if (expDigit > (LONG_BIT / 4))        //  .bugbug assumes LONG_BIT is a multiple of 4.
 {
  ERRMSG("BUG - cannot return as_nat from over-range " << viz(*this));
  return ULONG_MAX;
 }
 if (mantissaInfo._size > expDigit)
 {
  ERRMSG("BUG - cannot return as_nat from over-range " << viz(*this));
  return ULONG_MAX;
 }
 unsigned long mantissaValue = 0;
 for (const char *p = mantissaInfo._start; isxdigit(*p); p++)
 {
  int aDigit = isdigit(*p) ? (*p - '0') : (tolower(*p) - 'a' + 10);
  mantissaValue = (mantissaValue << 4) + aDigit;
 }
 while (expDigit-- > mantissaInfo._size)
  mantissaValue <<= 4;
 return mantissaValue;
}

int PrimNumber::compare(const PrimNumberRefToConst *v1, const PrimNumberRefToConst *v2)
{
 int rawResult = compare_raw(v1, v2);
#ifdef SLOW_CONSISTENCY_CHECK
 double d1 = (*v1)->as_double();
 double d2 = (*v2)->as_double();
 double doubleResult = d1 - d2;
 if ((doubleResult < 0.0) && (rawResult < 0))
  return  -1;
 if ((doubleResult > 0.0) && (rawResult > 0))
  return  1;
 if ((doubleResult == 0.0) && (rawResult == 0))
  return  0;
 static bool doneMessage = false;
 if (!doneMessage)
 {
  doneMessage = true;
  ERRMSG("BUG - inconsistent comparison of " << viz(**v1) << " and " << viz(**v2));
 }
#endif
 return rawResult;
}

int PrimNumber::compare_raw(const PrimNumberRefToConst *v1, const PrimNumberRefToConst *v2)
{
 if (*v1 == *v2)
  return 0;
 PrimNumber_info v1Mantissa;
 PrimNumber_info v2Mantissa;
 (*v1)->mantissa(v1Mantissa);
 (*v2)->mantissa(v2Mantissa);
 if (v1Mantissa._sign != v2Mantissa._sign)
  return v1Mantissa._sign - v2Mantissa._sign;
 PrimNumber_info v1Exponent;
 PrimNumber_info v2Exponent;
 (*v1)->exponent(v1Exponent);
 (*v2)->exponent(v2Exponent);
 if (v1Exponent._sign != v2Exponent._sign)
  return (v1Exponent._sign - v2Exponent._sign) * v1Mantissa._sign;
 if (v1Exponent._size != v2Exponent._size)
  return (v1Exponent._size - v2Exponent._size) * v1Exponent._sign * v1Mantissa._sign;
 const char *p1 = v1Exponent._start;
 const char *p2 = v2Exponent._start;
 for (int i = v1Exponent._size; i-- > 0; p1++, p2++)
  if (*p1 != *p2)
   return (*p1 - *p2) * v1Exponent._sign * v1Mantissa._sign;
 p1 = v1Mantissa._start;
 p2 = v2Mantissa._start;
 for (int j = std::min(v1Mantissa._size, v2Mantissa._size); j-- > 0; p1++, p2++)
  if (*p1 != *p2)
   return (*p1 - *p2) * v1Mantissa._sign;
 if (v1Mantissa._size != v2Mantissa._size)
  return (v1Mantissa._size - v2Mantissa._size) * v1Mantissa._sign;
 ERRMSG("BUG - unexpected equality of " << viz(**v1) << " and " << viz(**v2));
 return v1 - v2;
}

//  
//  	Assign an analysis of the exponent field to exponentInfo.
//  
void PrimNumber::exponent(PrimNumber_info& exponentInfo) const
{
 const char *p = id().str();
 if (!*p++)
 {
  if (!is_null())
   ERRMSG("No mantissa sign in " << viz(*this));
  exponentInfo._sign = 0;
  exponentInfo._size = 0;
  return;
 }
 while (isxdigit(*p))
  p++;
 if (*p == 'p')
  exponentInfo._sign = 1;
 else if (*p == 'm')
  exponentInfo._sign = -1;
 else if (*p == 'z')
  exponentInfo._sign = 0;
 else
 {
  ERRMSG("Illegal exponent sign in " << viz(*this));
  exponentInfo._sign = 0;
  exponentInfo._size = 0;
  return;
 }
 exponentInfo._start = ++p;
 while (isxdigit(*p))
  p++;
 exponentInfo._size = p - exponentInfo._start;
}

//  
//  	Return true if this is the number representation of a true or false value.
//  
bool PrimNumber::is_bool() const
{
 return (this == &make_false()) || (this == &make_true());
}

//  
//  	Return true if this is the number representation of a complex value.
//  
bool PrimNumber::is_complex() const
{
 return false;
}

//  
//  	Return true if this is the number representation of a long value, that can be returned by as_int().
//  
bool PrimNumber::is_int() const
{    //  .bugbug trips a numeric ordering.
 return is_integer() &&
 	(make_min_int() <= *this) &&
 	(*this <= make_max_int());
}

//  
//  	Return true if this is the number representation of an integer.
//  
bool PrimNumber::is_integer() const
{
 if (is_zero())
  return true;
 PrimNumber_info exponentInfo;
 exponent(exponentInfo);
 if (exponentInfo._sign < 1)
  return false;
 PrimNumber_info mantissaInfo;
 mantissa(mantissaInfo);
 long exponentValue = 0;
 for (const char *p = exponentInfo._start; isxdigit(*p); p++)
 {
  exponentValue = (exponentValue << 4) + (isdigit(*p) ? (*p - '0') : (tolower(*p) - ('a' - 10)));
  if (exponentValue >= mantissaInfo._size)
   return true;
 }
 return false;
}

//  
//  	Return true if this is the number representation of an unsigned long value, that can be returned by as_nat().
//  
bool PrimNumber::is_nat() const
{    //  .bugbug trips a numeric ordering.
 return is_natural() &&
 	(make_min_nat() <= *this) &&
 	(*this <= make_max_nat());
}

//  
//  	Return true if this is the number representation of a natural.
//  
bool PrimNumber::is_natural() const
{
 if (is_zero())
  return true;
 if (is_negative())
  return false;
 return is_integer();
}

//  
//  	Return true if this is the number representation of negative number (not 0).
//  
bool PrimNumber::is_negative() const
{
 const char *p = id().str();
 return *p == 'm';
}

//  
//  	Return true if this is the number representation of positive number (or 0).
//  
bool PrimNumber::is_positive() const
{
 const char *p = id().str();
 return *p != 'm';
}

//  
//  	Return true if this is the number representation of a real value.
//  
bool PrimNumber::is_real() const
{
 return true;
}

//  
//  	Return true if this is the number representation of 0.
//  
bool PrimNumber::is_zero() const
{
 const char *p = id().str();
 return *p == 'z';
}

//  
//  	Return the number representation for aString.
//  
const PrimNumber& PrimNumber::make(const char *aString, const double *checkValue)
{
 const PrimIdHandle anId(aString);
 PrimNumber *aNumber = map().find(*anId);
 if (aNumber)
  return *aNumber;
 aNumber = new PrimNumber(*anId);
 if (aNumber)
 {
  aNumber->annul();
  if (checkValue)
  {
   double d = aNumber->as_double();
   PrimNumber *theNumber = map().find(*anId);
   if (theNumber != aNumber)
    ERRMSG("BUG - " << viz(*theNumber) << " duplicates " << viz(*aNumber));
   else if (d != *checkValue)
   {
    char floatBuf[40];
    PrimOstrstream s(floatBuf, sizeof(floatBuf));
    s.precision(15);      //   Direct application to e.g. cerr would need restoration
    s << *checkValue << std::ends;
    ERRMSG("BUG - " << floatBuf << " inaccurately installed as " << viz(*aNumber));
   }
  }
  return *aNumber;
 }
 ERRMSG("Failed to allocate memory for PrimNumber " << anId);
 return immutable_null();
}

//  
//  	Construct a number from the loose string representation of a decimal number.
//  
//  		<mantissa-sign><mantissa-integer>.<mantissa-fraction><exponent-indicator><exponent-sign><exponent-value>
//  
//  	At least one digit must be supplied for <mantissa-integer> or <mantissa-fraction>.
//  	<mantissa-fraction> cannot be specified without a leading.
//  	<exponent-value> cannot be specified without an <exponent-indicator>.
//  
//  	<mantissa-sign> is completely optional and may be + or -.
//  	<exponent-sign> is completely optional and may be + or -.
//  	<exponent-indicator> is E or e.
//  	<mantissa-integer>, <mantissa-fraction> and <exponent-value> comprise 0 to 9.
//  
//  	The exponent is base 10 scale factor. Thus 1.4e1 is 14.0
//  
//  .limits
//  	Precision is limited by that of a double precision floating point number.
//  
const PrimNumber& PrimNumber::make_decimal(const char *aString)
{
 return make_decimal(aString, aString ? strlen(aString) : 0);
}
const PrimNumber& PrimNumber::make_decimal(const char *aBuffer, size_t aLength)
{
 std::istrstream is((char *)aBuffer, aLength);  //   MS DevStudio 5 needs cast
 double aValue;
 is >> std::ws >> aValue;
 if (is && !is.eof())
  is >> std::ws;
 if (is && is.eof())
  return make_real(aValue);
 ERRMSG("Failed to extract number from " << c_string(aBuffer, aLength));
 return make_zero();
}

//  
//  	Construct a number from a digit string in a specified base.
//  
//  	The base may be between 2 and 36 using digits and arbitrary case letters.
//  
const PrimNumber& PrimNumber::make_digits(int aBase, bool isNegated, const char *aString)
{
 return make_digits(aBase, isNegated, aString, aString ? strlen(aString) : 0);
}
const PrimNumber& PrimNumber::make_digits(int aBase, bool isNegated, const char *aBuffer, size_t aLength)
{
 if ((aBase <= 1) || (36 < aBase))
 {
  ERRMSG("Cannot parse number in base " << aBase);
  return null();
 }
 const unsigned long maxValue = ULONG_MAX / aBase;
 unsigned long value = 0;
 const char *p = aBuffer;
 size_t i = 0;
 for ( ; i < aLength; i++, p++)
 {
  if (value > maxValue)
   break;
  unsigned long newDigit = 0;
  if (isdigit(*p))
   newDigit = *p - '0';
  else if (isalpha(*p))
   newDigit = *p - (islower(*p) ? ('a'-10) : ('A'-10));
  else
  {
   value = 0;
   i = aLength;
   break;
  }
  const unsigned long oldValue = value;
  value = value * aBase + newDigit;
  if (value < oldValue)
   break;
 }
 if (i >= aLength)
 {
  const PrimNumber& aNumber = make_nat(value);
  return isNegated ? -aNumber : aNumber;
 }
 else
 {
  PrimNumberWords bigNumber(aBuffer, aLength, aBase);
  ERRMSG("Cannot encode arbitrary precisiion number");
  const PrimNumber& aNumber = make_zero();
  return isNegated ? -aNumber : aNumber;
 }
//  	if (p < pMax)
//  	{
//  		PrimOstrstream os;
//  		os.write(aBuffer, aLength);
//  		os << std::ends;
//  		ERRMSG("Excessive text supplied for make_hexadecimal(\"" << os.str() << "\")");
//  	}
}

//  
//  	Construct a number from the loose string representation of a hexadecimal number.
//  
//  		<mantissa-sign><mantissa-integer>.<mantissa-fraction><exponent-indicator><exponent-sign><exponent-value>
//  
//  	At least one digit must be supplied for <mantissa-integer> or <mantissa-fraction>.
//  	<mantissa-fraction> cannot be specified without a leading.
//  	<exponent-value> cannot be specified without an <exponent-indicator>.
//  
//  	<mantissa-sign> is completely optional and may be + or -.
//  	<exponent-sign> is completely optional and may be + or -.
//  	<exponent-indicator> is X or x.
//  	<mantissa-integer>, <mantissa-fraction> and <exponent-value> comprise 0 to 9, a to f or A to F.
//  
//  	The exponent is base 16 scale factor. Thus 1.4x1 is 20.0
//  
//  .limits
//  	The normalised base 16 exponent must be representable as a 32 bit number.
//  
const PrimNumber& PrimNumber::make_hexadecimal(const char *aString)
{
 return make_hexadecimal(aString, aString ? strlen(aString) : 0);
}
const PrimNumber& PrimNumber::make_hexadecimal(const char *aBuffer, size_t aLength)
{
 PrimOstrstream mantissa;
 long theExponent = 0;
 const char *p = aBuffer;
 const char *const pMax = &aBuffer[aLength];
 bool mantissaIsMinus = false;
 if ((p < pMax) && (*p == '-'))
 {
  mantissaIsMinus = true;
  p += 1;
 }
 else if ((p < pMax) && (*p == '+'))
  p += 1;
 while ((p < pMax) && (*p == '0'))     //   Step over leading zeroes.
  p++;
 while ((p < pMax) && isxdigit(*p))     //   Scan non-fractional mantissa
 {
  mantissa << *p++;
  theExponent++;
 }
 if ((p < pMax) && (*p == '.'))      //   If fraction required
 {
  p++;
  if (theExponent == 0)       //   If mantissa all zero
   for ( ; (p < pMax) && (*p == '0'); p++)  //   Step over fractional leading zeroes
    theExponent--;
  while ((p < pMax) && isxdigit(*p))    //   Scan fractional mantissa
   mantissa << *p++;
 }
 mantissa << std::ends;
 const char *const mStart = mantissa.str(); 
 char *mEnd = (char *)mStart + strlen(mStart); 
 while ((mEnd > mStart) && (*(mEnd-1) == '0')) 
  *--mEnd = 0;
 bool isZero = mEnd <= mStart;
 if ((p < pMax) && (tolower(*p) == 'x'))
 {
  bool exponentIsMinus = false;
  int exponentValue = 0;
  p++;
  if ((p < pMax) && (*p == '-'))
  {
   exponentIsMinus = true;
   p += 1;
  }
  else if ((p < pMax) && (*p == '+'))
   p += 1;
  for ( ; (p < pMax) && isxdigit(*p); p++)
  {
   int aDigit = isdigit(*p) ? (*p - '0') : (tolower(*p) - 'a' + 10);
   exponentValue = (exponentValue << 4) + aDigit;
  }
  if (exponentIsMinus)
   theExponent -= exponentValue;
  else
   theExponent += exponentValue;
 }
 PrimOstrstream s;
 if (isZero)
  s << "z0z0";
 else
 {
  s << (mantissaIsMinus ? 'm' : 'p');
  s << mStart;
  print_exponent(s, theExponent);
 }
 s << std::ends;
 if (p < pMax)
  ERRMSG("Excessive text supplied for make_hexadecimal(" << c_string(aBuffer, aLength) << ")");
 return make(s.str(), 0);
}

//  
//  	Return the number representation for anInt.
//  
const PrimNumber& PrimNumber::make_int(long anInt, bool checkNumber)
{
 if (anInt == 0)
  return make_false();
 char buf[43];
 char *p = &buf[40];
 char *q = p;
 unsigned int expDigit = 0;
 for (unsigned long aValue = (anInt >= 0) ? anInt : -anInt; aValue; aValue >>= 4, expDigit++)
 {
  unsigned int aDigit = aValue & 0xF;
  *--p = aDigit <= 9 ? char('0' + aDigit) : char('a' - 10 + aDigit);
 }
 *--p = (anInt >= 0) ? 'p' : 'm';
 while (*(q-1) == '0')
  q--;
 *q++ = 'p';
 if (expDigit >= 16) {
     int l = expDigit / 16;
     int r = expDigit % 16;
 	*q++ = l <= 9 ? char('0' + l) : char('a' - 10 + l);
 	*q++ = r <= 9 ? char('0' + r) : char('a' - 10 + r);
 }
 else
	 *q++ = expDigit <= 9 ? char('0' + expDigit) : char('a' - 10 + expDigit);
 *q = 0;
 const double checkValue = anInt;
 const PrimNumber& aNumber = make(p, checkNumber ? &checkValue : 0);
 if (checkNumber)
 {
  if (!aNumber.is_int())
   ERRMSG("BUG - make_int(" << anInt << ") fails is_int()");
  else if (aNumber.as_int() != anInt)
   ERRMSG("BUG - make_int(" << anInt << ") fails as_int()");
 }
 return aNumber;
}

//  
//  	Return the largest natural number representable as long.
//  
const PrimNumber& PrimNumber::make_max_int()
{
 static const PrimNumber& aNumber = make_int(LONG_MAX, false);
 return aNumber;
}

//  
//  	Return the largest integer number representable with someBits.
//  
const PrimNumber& PrimNumber::make_max_integer(size_t someBits)
{
 if (someBits <= 0)
  return make_zero();
 else
  return make_max_natural(someBits - 1);
}

//  
//  	Return the largest natural number representable as unsigned long.
//  
const PrimNumber& PrimNumber::make_max_nat()
{
 static const PrimNumber& aNumber = make_nat(ULONG_MAX, false);
 return aNumber;
}

//  
//  	Return the largest natural number representable with someBits.
//  
const PrimNumber& PrimNumber::make_max_natural(size_t someBits)
{
 if (someBits <= 0)
  return make_zero();
 double checkValue = pow(2.0, someBits) - 1.0;
 PrimOstrstream s;
 s << 'p';
 int theExponent = 0;
 if (someBits & 3)
 {
  s << char('0' + (1 << (someBits & 3)) - 1);
  someBits &= ~3;
  theExponent++;
 }
 for ( ; someBits; someBits -= 4, theExponent++)
  s << 'f';
 print_exponent(s, theExponent);
 s << std::ends;
 return make(s.str(), someBits <= DBL_MANT_DIG ? &checkValue : 0);
}

//  
//  	Return the largest natural number representable as long.
//  
const PrimNumber& PrimNumber::make_min_int()
{
 static const PrimNumber& aNumber = make_int(LONG_MIN, false);
 return aNumber;
}

//  
//  	Return the smallest integer number representable with someBits.
//  
const PrimNumber& PrimNumber::make_min_integer(size_t someBits)
{
 if (someBits <= 0)
  return make_zero();
 double checkValue = -0.5 * pow(2.0, someBits);
 someBits--;
 PrimOstrstream s;
 s << 'm';
 s << char('0' + (1 << (someBits & 3)));
 someBits &= ~3;
 print_exponent(s, 1 + (someBits >> 2));
 s << std::ends;
 return make(s.str(), &checkValue);
}

//  
//  	Return the number representation for aNat.
//  
const PrimNumber& PrimNumber::make_nat(unsigned long aNat, bool checkNumber)
{
 if (aNat == 0)
  return make_false();
 char buf[43];
 char *p = &buf[40];
 char *q = p;
 unsigned int expDigit = 0;
 for (unsigned long aValue = aNat; aValue; aValue >>= 4, expDigit++)
 {
  unsigned int aDigit = aValue & 0xF;
  *--p = aDigit <= 9 ? char('0' + aDigit) : char('a' - 10 + aDigit);
 }
 *--p = 'p';
 while (*(q-1) == '0')
  q--;
 *q++ = 'p';
 if (expDigit >= 16) {
     int l = expDigit / 16;
     int r = expDigit % 16;
 	*q++ = l <= 9 ? char('0' + l) : char('a' - 10 + l);
 	*q++ = r <= 9 ? char('0' + r) : char('a' - 10 + r);
 }
 else
 	*q++ = expDigit <= 9 ? char('0' + expDigit) : char('a' - 10 + expDigit);
 *q = 0;
 const double checkValue = aNat;
 const PrimNumber& aNumber = make(p, checkNumber ? &checkValue : 0);
 if (checkNumber)
 {
     //long l = aNumber.as_nat();
  if (!aNumber.is_nat())
   ERRMSG("BUG - make_nat(" << aNat << ") fails is_nat()");
  else if (aNumber.as_nat() != aNat)
   ERRMSG("BUG - make_nat(" << aNat << ") fails as_nat()");
 }
 return aNumber;
}

//  
//  	Return the number representation for one.
//  
const PrimNumber& PrimNumber::make_one()
{
 static double checkValue = 1.0;
 static const PrimNumber& aNumber = make("p1p1", &checkValue);
 return aNumber;
}

//  
//  	Return the number representation for aDouble.
//  
const PrimNumber& PrimNumber::make_real(double aDouble, bool checkNumber)
{
 int exponentValue;
 double mantissaValue = frexp(aDouble, &exponentValue);
 while (exponentValue & 3)
 {
  mantissaValue *= 0.5;
  exponentValue++;
 }
 exponentValue /= 4;
 if (mantissaValue == 0.0)
  return make_zero();
 PrimOstrstream s;
 if (mantissaValue < 0.0)
 {
  s << 'm';
  mantissaValue = -mantissaValue;
 }
 else
  s << 'p';
 while (mantissaValue != 0.0)
 {
  mantissaValue *= 16.0;
  long aDigit = long(floor(mantissaValue));
  s << char((aDigit <= 9) ? ('0' + aDigit) : ('a' - 10 + aDigit));
  mantissaValue -= aDigit;
 }
 print_exponent(s, exponentValue);
 s << std::ends;
 return make(s.str(), &aDouble);
}

//  
//  	Return the number representation for zero.
//  
const PrimNumber& PrimNumber::make_zero()
{
 static double checkValue = 0.0;
 static const PrimNumber& aNumber = make("z0z0", &checkValue);
 return aNumber;
}

//  
//  	Assign an analysis of the mantissa field to exponentInfo.
//  
void PrimNumber::mantissa(PrimNumber_info& mantissaInfo) const
{
 const char *p = id().str();
 if (*p == 'p')
  mantissaInfo._sign = 1;
 else if (*p == 'm')
  mantissaInfo._sign = -1;
 else if (*p == 'z')
  mantissaInfo._sign = 0;
 else
 {
  ERRMSG("Illegal sign in " << viz(*this));
  mantissaInfo._sign = 0;
  mantissaInfo._size = 0;
  return;
 }
 mantissaInfo._start = ++p;
 while (isxdigit(*p))
  p++;
 mantissaInfo._size = p - mantissaInfo._start;
}

PrimNumberMapOfRef& PrimNumber::map()
{
 static PrimNumberMapOfRef aMap(256);
 return aMap;
}

void PrimNumber::order()
{
 PrimNumberListOfRef aList(map());
 aList.sort(&PrimNumber::compare);
 size_t i = 2;        //   null is zero, unordered is 1.
 for (PrimNumberConstListOfRefIterator p(aList); p; p++, i++)
  p->_index = i;
}

//  
//  	Dump the number line for debug.
//  
std::ostream& PrimNumber::print_all(std::ostream& s)
{
 PrimNumberListOfRef aList(map());
 aList.sort(&PrimNumber::compare);
 aList.print_deep(s);
 return s;
}

//  
//  	Append a decimal ASCII representation of the number to a stream.
//  
//  .limits
//  	Precision is limited by that available with a double precision floating point number.
//  
std::ostream& PrimNumber::print_decimal(std::ostream& s) const
{
 PrimIosSave iosSave(s);
 s.precision(15);
 s << as_double();
 return s;
}

std::ostream& PrimNumber::print_exponent(std::ostream& s, long theExponent)
{
 if (theExponent == 0)
  s << "z0";
 else
 {
  if (theExponent < 0)
  {
   s << 'm';
   theExponent = -theExponent;
  }
  else
   s << 'p';
  char exponentBuf[41];
  char *q = &exponentBuf[40];
  for (*q = 0; theExponent; theExponent >>= 4)
  {
   int aDigit = theExponent & 0xf;
   *--q = (aDigit <= 9) ? char(aDigit + '0') : char(aDigit +'a' - 10);
  }
  s << q;
 }
 return s;
}

//  
//  	Append a hexadecimal ASCII representation of the number to a stream. The value retains the intrisic
//  	normalisation and is compatible with make_hexadecimal.
//  
std::ostream& PrimNumber::print_hexadecimal(std::ostream& s) const
{
 const char *p = _id.str();
 if (*p == 'm')
  s << '-';
 p++;
 s << "0.";
 while (isxdigit(*p))
  s << *p++;
 s << 'x';
 if (*p == 'm')
  s << '-';
 p++;
 while (isxdigit(*p))
  s << *p++;
 return s;
}

std::ostream& PrimNumber::print_this(std::ostream& s) const
{
 s << _id << ", ";
 if (is_null())
  return s << '?';
 else
  return print_decimal(s);
}

std::ostream& PrimNumber::print_viz(std::ostream& s) const
{
 if (is_null())
  return s << '?';
 else
  return print_decimal(s);
}

//  
//  	Return the number representation quantised to a multiple of anEpsilon.
//  
const PrimNumber& PrimNumber::quantise_to_epsilon(const PrimNumber& anEpsilon) const
{
 double epsilonValue = anEpsilon.as_double();
 double aMultiple = as_double() / epsilonValue;
 double roundedValue = aMultiple >= 0 ? floor(aMultiple + 0.5) : ceil(aMultiple - 0.5);
 return make_real(roundedValue * epsilonValue);
}

bool operator<(const PrimNumber& n1, const PrimNumber& n2)
{
 if ((n1._index == 1) || (n2._index == 1))
  PrimNumber::order();
 return n1._index < n2._index;
}

const PrimNumber& operator+(const PrimNumber& n1, const PrimNumber& n2) //  .bugbug inadequate
{
 return PrimNumber::make_real(n1.as_double() + n2.as_double());
}

const PrimNumber& operator-(const PrimNumber& n1, const PrimNumber& n2) //  .bugbug inadequate
{
 return PrimNumber::make_real(n1.as_double() - n2.as_double());
}

const PrimNumber& operator*(const PrimNumber& n1, const PrimNumber& n2) //  .bugbug inadequate
{
 return PrimNumber::make_real(n1.as_double() * n2.as_double());
}

const PrimNumber& operator/(const PrimNumber& n1, const PrimNumber& n2) //  .bugbug inadequate
{
 if (n2.is_zero())
 {
  ERRMSG("Attempted divide by zero for operator/ of " << viz(n1) << " and " << viz(n2));
  return PrimNumber::null();
 }
 return PrimNumber::make_real(n1.as_double() / n2.as_double());
}

const PrimNumber& operator%(const PrimNumber& n1, const PrimNumber& n2) //  .bugbug inadequate
{
 if (n2.is_zero())
 {
  ERRMSG("Attempted divide by zero for operator% of " << viz(n1) << " and " << viz(n2));
  return PrimNumber::null();
 }
 if (n1.is_int() && n2.is_int())
  return PrimNumber::make_int(n1.as_int() % n2.as_int());
 if (n1.is_nat() && n2.is_nat())
  return PrimNumber::make_int(n1.as_nat() % n2.as_nat());
 if (!n1.is_integer() || !n2.is_integer())
  ERRMSG("Cannot operator% non-integral " << viz(n1) << " and " << viz(n2));
 else
  ERRMSG("BUG -- Unsupported number range for operator% of " << viz(n1) << " and " << viz(n2));
 return PrimNumber::null();
}

const PrimNumber& operator<<(const PrimNumber& n1, const PrimNumber& n2) //  .bugbug inadequate
{
 return PrimNumber::make_real(n1.as_double() * pow(2.0, n2.as_double()), true);
}

const PrimNumber& operator>>(const PrimNumber& n1, const PrimNumber& n2) //  .bugbug inadequate
{
 return PrimNumber::make_real(floor(n1.as_double() * pow(0.5, n2.as_double())), true);
}

const PrimNumber& operator&(const PrimNumber& n1, const PrimNumber& n2) //  .bugbug inadequate
{
 if (n1.is_int() && n2.is_int())
  return PrimNumber::make_int(n1.as_int() & n2.as_int());
 if (n1.is_nat() && n2.is_nat())
  return PrimNumber::make_int(n1.as_nat() & n2.as_nat());
 if (!n1.is_integer() || !n2.is_integer())
  ERRMSG("Cannot operator& non-integral " << viz(n1) << " and " << viz(n2));
 else
  ERRMSG("BUG -- Unsupported number range for operator& of " << viz(n1) << " and " << viz(n2));
 return PrimNumber::null();
}

const PrimNumber& operator|(const PrimNumber& n1, const PrimNumber& n2) //  .bugbug inadequate
{
 if (n1.is_int() && n2.is_int())
  return PrimNumber::make_int(n1.as_int() | n2.as_int());
 if (n1.is_nat() && n2.is_nat())
  return PrimNumber::make_int(n1.as_nat() | n2.as_nat());
 if (!n1.is_integer() || !n2.is_integer())
  ERRMSG("Cannot operator| non-integral " << viz(n1) << " and " << viz(n2));
 else
  ERRMSG("BUG -- Unsupported number range for operator| of " << viz(n1) << " and " << viz(n2));
 return PrimNumber::null();
}

const PrimNumber& operator^(const PrimNumber& n1, const PrimNumber& n2) //  .bugbug inadequate
{
 if (n1.is_int() && n2.is_int())
  return PrimNumber::make_int(n1.as_int() ^ n2.as_int());
 if (n1.is_nat() && n2.is_nat())
  return PrimNumber::make_int(n1.as_nat() ^ n2.as_nat());
 if (!n1.is_integer() || !n2.is_integer())
  ERRMSG("Cannot operator^ non-integral " << viz(n1) << " and " << viz(n2));
 else
  ERRMSG("BUG -- Unsupported number range for operator^ of " << viz(n1) << " and " << viz(n2));
 return PrimNumber::null();
}

bool operator&&(const PrimNumber& n1, const PrimNumber& n2)
{
 return !n1.is_zero() && !n2.is_zero();
}

bool operator||(const PrimNumber& n1, const PrimNumber& n2)
{
 return !n1.is_zero() || !n2.is_zero();
}

const PrimNumber& abs(const PrimNumber& n)
{
 const char *p = n.id().str();
 if (*p != 'm')
  return n;
 PrimOstrstream s;
 s << 'p' << (p+1) << std::ends;
 const double checkValue = -n.as_double();
 return PrimNumber::make(s.str(), &checkValue);
}

const PrimNumber& acos(const PrimNumber& n) //  .bugbug inadequate
{
 return PrimNumber::make_real(acos(n.as_double()), true);
}

const PrimNumber& asin(const PrimNumber& n) //  .bugbug inadequate
{
 return PrimNumber::make_real(asin(n.as_double()), true);
}

const PrimNumber& atan(const PrimNumber& n) //  .bugbug inadequate
{
 return PrimNumber::make_real(atan(n.as_double()), true);
}

const PrimNumber& ceil(const PrimNumber& n) //  .bugbug inadequate
{
 return PrimNumber::make_real(ceil(n.as_double()), true);
}

const PrimNumber& conj(const PrimNumber& n) //  .bugbug inadequate
{
 ERRMSG("BUG - use of complex numbers not yet supported.");
 return n;
}

const PrimNumber& cos(const PrimNumber& n) //  .bugbug inadequate
{
 return PrimNumber::make_real(cos(n.as_double()), true);
}

const PrimNumber& cosh(const PrimNumber& n) //  .bugbug inadequate
{
 return PrimNumber::make_real(cosh(n.as_double()), true);
}

const PrimNumber& cplx(const PrimNumber& n1, const PrimNumber& n2) //  .bugbug inadequate
{
 ERRMSG("BUG - construction of complex numbers not yet supported.");
 return n1;
}

//  
//  	Return true if this number is even. Generates an error if this number is not an integer.
//  
bool even(const PrimNumber& n)
{
 if (!n.is_integer())
 {
  ERRMSG("Cannot determine even-ness of non-integer " << viz(n));
  return false;
 }
 PrimNumber_info mantissaInfo;
 n.mantissa(mantissaInfo);
 const char *p = mantissaInfo._start + mantissaInfo._size - 1;
 return (*p & 1) == 0;
}

const PrimNumber& exp(const PrimNumber& n) //  .bugbug inadequate
{
 return PrimNumber::make_real(exp(n.as_double()), true);
}

const PrimNumber& floor(const PrimNumber& n) //  .bugbug inadequate
{
 return PrimNumber::make_real(floor(n.as_double()), true);
}

const PrimNumber& frac(const PrimNumber& n) //  .bugbug inadequate
{
 int exponentValue;
 return PrimNumber::make_real(frexp(n.as_double(), &exponentValue), true);
}

//  
//  	Return the integer quotient of this n1 and n2. Generates an error if either number is not
//  	an integer or if n2 is zero.
//  
const PrimNumber& idiv(const PrimNumber& n1, const PrimNumber& n2)
{
 if (n2.is_zero())
 {
  ERRMSG("Attempted integer divide by zero for " << viz(n1));
  return PrimNumber::make_zero();
 }
 if (!n1.is_integer() || !n2.is_integer())
 {
  ERRMSG("Attempted integer divide for non-integer " << viz(n1) << " or " << viz(n2));
  return PrimNumber::make_zero();
 }
 if (n1.is_int() && n2.is_int())
  return PrimNumber::make_int(n1.as_int() / n2.as_int());
 if (n1.is_nat() && n2.is_nat())
  return PrimNumber::make_nat(n1.as_nat() / n2.as_nat());
 ERRMSG("BUG - unsupported integer divide for " << viz(n1) << " / " << viz(n2));
 return PrimNumber::make_zero();
}

const PrimNumber& imag(const PrimNumber& n) //  .bugbug inadequate
{
 return PrimNumber::make_zero();
}

const PrimNumber& ln(const PrimNumber& n) //  .bugbug inadequate
{
 return PrimNumber::make_real(log(n.as_double()), true);
}

const PrimNumber& log(const PrimNumber& n1, const PrimNumber& n2) //  .bugbug inadequate
{
 return PrimNumber::make_real(log(n1.as_double()) / log(n2.as_double()), true);
}

const PrimNumber& max(const PrimNumber& n1, const PrimNumber& n2)
{
 if ((n1._index == 1) || (n2._index == 1))
  PrimNumber::order();
 return n1._index < n2._index ? n2 : n1;
}

const PrimNumber& min(const PrimNumber& n1, const PrimNumber& n2)
{
 if ((n1._index == 1) || (n2._index == 1))
  PrimNumber::order();
 return n1._index < n2._index ? n1 : n2;
}

const PrimNumber& nat(const PrimNumber& n)
{
 const char *p = n.id().str();
 if (*p == 'm')
  return PrimNumber::make_zero();
 else
  return PrimNumber::make_real(floor(n.as_double()), true);
}

bool odd(const PrimNumber& n)
{
 return !even(n);
}

const PrimNumber& pow(const PrimNumber& n1, const PrimNumber& n2) //  .bugbug inadequate
{
 return PrimNumber::make_real(pow(n1.as_double(), n2.as_double()), true);
}

const PrimNumber& real(const PrimNumber& n) //  .bugbug inadequate
{
 return n;
}

const PrimNumber& round(const PrimNumber& n) //  .bugbug inadequate
{
 double aValue = n.as_double();
 return PrimNumber::make_real(aValue >= 0.0 ? floor(aValue + 0.5) : ceil(aValue - 0.5), true);
}

const PrimNumber& sin(const PrimNumber& n) //  .bugbug inadequate
{
 return PrimNumber::make_real(sin(n.as_double()), true);
}

const PrimNumber& sinh(const PrimNumber& n) //  .bugbug inadequate
{
 return PrimNumber::make_real(sinh(n.as_double()), true);
}

const PrimNumber& sqrt(const PrimNumber& n) //  .bugbug inadequate
{
 return PrimNumber::make_real(sqrt(n.as_double()), true);
}

const PrimNumber& tan(const PrimNumber& n) //  .bugbug inadequate
{
 return PrimNumber::make_real(tan(n.as_double()), true);
}

const PrimNumber& tanh(const PrimNumber& n) //  .bugbug inadequate
{
 return PrimNumber::make_real(tanh(n.as_double()), true);
}

const PrimNumber& trunc(const PrimNumber& n) //  .bugbug inadequate
{
 double aValue = n.as_double();
 return PrimNumber::make_real(aValue >= 0.0 ? floor(aValue) : ceil(aValue), true);
}
 








#ifndef LONG_BIT
#if (ULONG_MAX == UCHAR_MAX)
#define LONG_BIT CHAR_BIT
#elif ((ULONG_MAX - (UCHAR_MAX << CHAR_BIT)) <= (UCHAR_MAX << CHAR_BIT))
#define LONG_BIT (CHAR_BIT << 1)
#elif ((ULONG_MAX - (UCHAR_MAX << (3*CHAR_BIT))) <= (UCHAR_MAX << (3*CHAR_BIT)))
#define LONG_BIT (CHAR_BIT << 2)
#else
#define LONG_BIT (CHAR_BIT << 3)
#endif
#endif

PRIMTEST_MODULE_DECL(PrimNumber_sort,"sort()")
//  
//  	Perform tests of misc number creation and sorting.
//  
int PrimNumber_sort::do_test()
{
 int savedErrorCount = PrimNotifier::error_count();
 PrimNumber::make_nat(123456789);
 PrimNumber::make_int(-123456789);
 PrimNumber::make_nat(3);
 PrimNumber::make_hexadecimal("-1.1x-1");
 PrimNumber::make_bool(true);
 if (PrimNumber::make_max_natural(LONG_BIT) != PrimNumber::make_max_nat())
  ERRMSG("Inconsistent make_max_natural(" << LONG_BIT << ')');
 if (PrimNumber::make_min_natural(LONG_BIT) != PrimNumber::make_min_nat())
  ERRMSG("Inconsistent make_min_natural(" << LONG_BIT << ')');
 if (PrimNumber::make_max_integer(LONG_BIT) != PrimNumber::make_max_int())
  ERRMSG("Inconsistent make_max_integer(" << LONG_BIT << ')');
 if (PrimNumber::make_min_integer(LONG_BIT) != PrimNumber::make_min_int())
  ERRMSG("Inconsistent make_min_integer(" << LONG_BIT << ')');
 if (PrimNumber::make_max_natural(1) != PrimNumber::make_nat(1))
  ERRMSG("Inconsistent make_max_natural(1)");
 if (PrimNumber::make_max_integer(1) != PrimNumber::make_zero())
  ERRMSG("Inconsistent make_max_integer(1)");
 if (PrimNumber::make_max_integer(2) != PrimNumber::make_nat(1))
  ERRMSG("Inconsistent make_max_integer(2)");
 if (PrimNumber::make_max_integer(3) != PrimNumber::make_nat(3))
  ERRMSG("Inconsistent make_max_integer(3)");
 if (PrimNumber::make_max_integer(4) != PrimNumber::make_nat(7))
  ERRMSG("Inconsistent make_max_integer(4)");
 if (PrimNumber::make_min_integer(1) != PrimNumber::make_int(-1))
  ERRMSG("Inconsistent make_min_integer(1)");
 if (PrimNumber::make_min_integer(2) != PrimNumber::make_int(-2))
  ERRMSG("Inconsistent make_min_integer(2)");
 if (PrimNumber::make_min_integer(3) != PrimNumber::make_int(-4))
  ERRMSG("Inconsistent make_min_integer(3)");
 if (PrimNumber::make_min_integer(4) != PrimNumber::make_int(-8))
  ERRMSG("Inconsistent make_min_integer(4)");
 if (PrimNumber::make_int(-31) >= PrimNumber::make_int(-16))
  ERRMSG("Inconsistent make_int(-31) >= make_int(-16)");
 if (PrimNumber::make_nat(31) <= PrimNumber::make_nat(16))
  ERRMSG("Inconsistent make_nat(31) <= make_nat(16)");
 if (PrimNumber::make_real(-57.9567423678015e72) != PrimNumber::make_decimal("-57.9567423678015e72"))
  ERRMSG("Inconsistent make_real(-57.9567423678015e72), make_decimal(\"-57.9567423678015e72\")");
 if (PrimNumber::make_hexadecimal("0.0002x") <= PrimNumber::make_real(0.000030517578124))
  ERRMSG("Inconsistent make_hexadecimal(\"0.0002x\"), make_real(0.000030517578124)");
 if (PrimNumber::make_hexadecimal("0.0002x") >= PrimNumber::make_real(0.000030517578126))
  ERRMSG("Inconsistent make_hexadecimal(\"0.0002x\"), make_real(0.000030517578126)");
 return savedErrorCount == PrimNotifier::error_count();
}
