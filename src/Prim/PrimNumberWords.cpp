#include <Prim/PrimIncludeAll.h>









//  %import math.h _MATH_H

TYPEINFO_SINGLE(PrimNumberWords, PrimPrint)

//  
//  	Construct aValue with aCapacity words, all defined as 0.
//  
PrimNumberWords::PrimNumberWords(const PrimSize& aCapacity)
:
 _contents(aCapacity.size() ? new unsigned short[aCapacity.size()] : 0),
 _capacity(_contents ? aCapacity.size() : 0),
 _tally(_capacity)
{
 unsigned short *q = _contents.get();
 for (size_t i = _tally; i-- > 0; )
  *q++ = 0;
}

//  
//  	Construct a two word value containing aValue.
//  
PrimNumberWords::PrimNumberWords(unsigned long aValue)
:
 _contents(new unsigned short[WORDS_PER_LONG]),
 _capacity(_contents ? WORDS_PER_LONG : 0),
 _tally(_capacity)
{
 unsigned short *q = _contents.get();
 for (size_t i = _tally; i-- > 0; )
 {
  *q++ = aValue & WORD_MASK;
  aValue >>= BITS_PER_WORD;
 }
}

//  
//  	Construct a natural number by reading from someChars[numChars] interpreting the standard
//  	ASCII representations of digits in numberBase. It is assumed that the someChars has been validated
//  	to eliminate any prefix, suffix or extraneous characters. Base 2 to 36 inclusive.
//  
PrimNumberWords::PrimNumberWords(const char *someChars, size_t numChars, int numberBase)
:
 _contents(0),
 _capacity(0),
 _tally(0)
{
 static const double divisor = 1.0 / log(1uL << BITS_PER_WORD);
 const size_t numWords = size_t(ceil(numChars * log(numberBase) * divisor));
 set_sizes(numWords, numWords);
 const char *p = someChars;
 for (size_t i = numChars; i-- > 0; p++)
 {
  unsigned long carry = 0;
  if (isdigit(*p))
   carry = *p - '0';
  else if (isalpha(*p))
   carry = *p - (islower(*p) ? ('a'-10) : ('A'-10));
  else
  {
   ERRMSG("Bad character in " << someChars);
   return;
  }
  carry = short_multiply(numberBase, carry);
  if (carry)
   ERRMSG("Overflow during string parsing of " << someChars);
 }
 set_sizes(_capacity, non_zero_words());
}

//  
//  	Construct a natural number initialised from the LSW first someValues[numValues].
//  
PrimNumberWords::PrimNumberWords(const unsigned short *someValues, size_t numValues)
:
 _contents(0),
 _capacity(0),
 _tally(0)
{
 assign(someValues, numValues);
}

//  
//  	Construct a copy of aValue.
//  
PrimNumberWords::PrimNumberWords(const PrimNumberWords& aValue)
:
 _contents(0),
 _capacity(0),
 _tally(0)
{
 assign(aValue._contents.get(), aValue._tally);
}
 
//  
//  	The destructor adds no functionality, but ensures that only one destructor is generated..
//  
PrimNumberWords::~PrimNumberWords() {}

//  
//  	Assign aValue to this value.
//  
PrimNumberWords& PrimNumberWords::operator=(const PrimNumberWords& aValue)
{
 if (this != &aValue)
  assign(aValue._contents.get(), aValue._tally);
 return *this;
}

//  
//  	Return a complemented instance of this value.
//  
PrimNumberWords PrimNumberWords::operator~() const
{
 PrimNumberWords returnValue(*this);
 unsigned short *q = returnValue._contents.get();
 for (size_t i = returnValue._tally; i-- > 0; q++)
  *q = ~*q & WORD_MASK; 
 return returnValue;
}

//  
//  	Return true if this value non-zero.
//  
bool PrimNumberWords::operator!() const
{
 const unsigned short *p = _contents.get();
 for (size_t i = _tally; i-- > 0; )
  if (*p++)
   return true;
 return false; 
}

//  
//  	Assign the LSW word first someValues[numValues].
//  
void PrimNumberWords::assign(const unsigned short *someValues, size_t numValues)
{
 set_sizes(numValues, 0);
 unsigned short *q = _contents.get();
 if (q)
 {
  const unsigned short *p = someValues;
  for (size_t i = numValues; i-- > 0; )
   *q++ = *p++;
  _tally = numValues;
 }
}

//  
//  	Return the result of dividend/divisor to quotient and the remainder to dividend.
//  
void PrimNumberWords::divide(PrimNumberWords& quotient, PrimNumberWords& dividend, const PrimNumberWords& divisor)
{
 int doubleWillOverflow = (divisor._contents.get()[divisor._capacity-1] & ~(WORD_MASK >> 1)) ? 1 : 0;
 PrimNumberWords doubleDivisor(PrimSize(divisor._capacity + doubleWillOverflow));
 doubleDivisor = divisor;
 doubleDivisor = doubleDivisor + divisor;
 if (dividend >= doubleDivisor)
 {
  divide(quotient, dividend, doubleDivisor);
  quotient = quotient + quotient;
 }
 if (dividend >= divisor)
 {
  dividend = dividend - divisor;
  static const PrimNumberWords one(1ul);
  quotient = quotient + one;
 }
}

//  
//  	Return true if the natural number is even.
//  
bool PrimNumberWords::is_even() const
{
 return (_tally == 0) || ((_contents.get()[0] & 1) == 0); 
}

//  
//  	Return the maximum representation of this value.
//  
PrimNumberWords PrimNumberWords::maximum() const
{
 PrimNumberWords returnValue(*this);
 unsigned short *q = returnValue._contents.get();
 for (size_t i = returnValue._tally; i-- > 0; q++)
  *q |= WORD_MASK; 
 return returnValue;
}

//  
//  	Return the number of words required to represent the actual numeric content.
//  
size_t PrimNumberWords::non_zero_words() const
{
 if (_tally == 0)
  return 0;
 const unsigned short *p = _contents.get() + _tally;
 for (size_t i = _tally; i-- > 0; )
  if (*--p)
   break;
 if (!*p)
  return 0;
 return (p + 1) - _contents.get();
}

//  
//  	Contribute the value to an inline diagnostic.
//  
std::ostream& PrimNumberWords::print_this(std::ostream& s) const
{
 PrimIosSave iosSave(s);
 s << std::hex << std::setfill('0');
 const unsigned short *p = _contents.get() + _tally;
 for (size_t i = _tally; i-- > 0; )
 {
  s << std::setw(4) << *--p;
  if (i > 0)
   s << ',';
 }
 return s;
}

//  
//  	Update the representation to support at least aCapacity words, with precisely aTally words in use.
//  	Additional memory is allocated if required to satisfy aCapacity. Additional MS words are zeroed
//  	if required to satify aTally. Any aCapacity less than aTally is treated as aTally.
//  
//  	Returns false if a memory allocation problem arose.
//  
bool PrimNumberWords::set_sizes(size_t aCapacity, size_t aTally)
{
 if (aCapacity < aTally)
  aCapacity = aTally;
 if (_capacity < aCapacity)
 {
  PrimAdoptedArray<unsigned short> newContents(new unsigned short[aCapacity]);
  if (!newContents)
  {
   ERRMSG(*this << " failed to allocate memory for " << aCapacity << " word array.");
   return false;
  }
  const unsigned short *p = _contents.get();
  unsigned short *q = newContents.get();
  for (size_t i = _tally; i-- > 0; )
   *q++ = *p++;
  _contents = newContents;   
  _capacity = aCapacity;
 }
 if (_tally < aTally)
 {
  unsigned short *q = _contents.get() + _tally;
  for (size_t i = aTally - _tally; i-- > 0; )
   *q++ = 0;
 }
 _tally = aTally;
 return true;
}

//  
//  	Add aCarry to this number without changing its size, returning any carry overflow.
//  
//  .limits
//  	This routine may malfunction if aCarry is greater than ULONG_MAX-USHRT_MAX.
//  
unsigned long PrimNumberWords::short_add(unsigned long aCarry)
{
 unsigned short *q = _contents.get();
 for (size_t j = _tally; j-- > 0; )
 {
  aCarry += *q;
  *q++ = aCarry & WORD_MASK;
  aCarry >>= BITS_PER_WORD;
 }
 return aCarry;
}

//  
//  	Divide this number by aFactor, returning the remainder.
//  
//  .limits
//  	Do not divide by zero.
//  
unsigned long PrimNumberWords::short_divide(unsigned short aFactor)
{
 unsigned long aCarry = 0;
 if (aFactor)
 {
  unsigned short *q = _contents.get() + _tally;
  for (size_t j = _tally; j-- > 0; )
  {
   aCarry = (aCarry << BITS_PER_WORD) + *--q;
   unsigned long aDigit = aCarry / aFactor;
   aCarry = aCarry % aFactor;
   *q = aDigit;
  }
 }
 return aCarry;
}

//  
//  	Multiply this number by aFactor, and add aCarry without changing its size, returning any carry overflow.
//  
//  .limits
//  	This routine may malfunction if aCarry is greater than ULONG_MAX-(USHRT_MAX*aFactor).
//  
unsigned long PrimNumberWords::short_multiply(unsigned short aFactor, unsigned long aCarry)
{
 unsigned short *q = _contents.get();
 for (size_t j = _tally; j-- > 0; )
 {
  aCarry += *q * aFactor;
  *q++ = aCarry & WORD_MASK;
  aCarry >>= BITS_PER_WORD;
 }
 return aCarry;
}

//  
//  	Return the value as a double precision number.
//  
double PrimNumberWords::to_double() const
{
 double realValue = 0.0;
 const unsigned short *p = _contents.get() + _tally;
 for (size_t i = _tally; i-- > 0; )
  realValue = realValue * (1uL << BITS_PER_WORD) + *--p;
 return realValue;
}

//  
//  	Return the nearest natural number from the built-in number range.
//  
unsigned long PrimNumberWords::to_small() const
{
 if (_tally == 0)
  return 0;
 size_t nonZeroWords = non_zero_words();
 if (nonZeroWords > WORDS_PER_LONG)
  return ULONG_MAX;
 unsigned long returnValue = 0;
 for (unsigned short *p = &_contents.get()[nonZeroWords-1]; nonZeroWords > 0; nonZeroWords--, p--)
  returnValue = (returnValue << BITS_PER_WORD) + *p;
 return returnValue;
}

//  
//  	Return a zero instance.
//  
const PrimNumberWords& PrimNumberWords::zero()
{
 static const PrimNumberWords aZero;
 return aZero;
}

//  
//  	Return an instance containing the sum of firstValue and secondValue.
//  
PrimNumberWords operator+(const PrimNumberWords& firstValue, const PrimNumberWords& secondValue)
{
 const size_t maxTally = std::max(firstValue._tally, secondValue._tally);
 const PrimSize maxSize(maxTally);
 PrimNumberWords returnValue(maxSize);
 const unsigned short *p1 = firstValue._contents.get();
 const unsigned short *p2 = secondValue._contents.get();
 unsigned short *q = returnValue._contents.get();
 unsigned long carry = 0;
 for (size_t i = 0; i < returnValue._tally; i++)
 {
  if (i < firstValue._tally)
   carry += *p1++;
  if (i < secondValue._tally)
   carry += *p2++;
  *q++ = carry & PrimNumberWords::WORD_MASK;
  carry >>= PrimNumberWords::BITS_PER_WORD;
 }
 if (carry)
  ERRMSG("Overflow during " << firstValue << " + " << secondValue);
 return returnValue;
}

//  
//  	Return an instance containing the difference of firstValue and secondValue.
//  
PrimNumberWords operator-(const PrimNumberWords& firstValue, const PrimNumberWords& secondValue)
{
 typedef unsigned long UnsignedLong;
 const size_t maxTally = std::max(firstValue._tally, secondValue._tally);
 const PrimSize maxSize(maxTally);
 PrimNumberWords returnValue(maxSize);
 const unsigned short *p1 = firstValue._contents.get();
 const unsigned short *p2 = secondValue._contents.get();
 unsigned short *q = returnValue._contents.get();
 long carry = 0;
 for (size_t i = 0; i < returnValue._tally; i++)
 {
  if (i < firstValue._tally)
   carry += long(*p1++);
  if (i < secondValue._tally)
   carry -= long(*p2++);
  const unsigned long unsignedCarry = UnsignedLong(carry);
  const unsigned short unsignedTerm = unsignedCarry & PrimNumberWords::WORD_MASK;
  *q++ = unsignedTerm;
  carry -= long(unsignedTerm);
  if (carry >= 0)
   carry = long(UnsignedLong(carry) >> PrimNumberWords::BITS_PER_WORD);
  else
   carry = -long(UnsignedLong(-carry) >> PrimNumberWords::BITS_PER_WORD);
 }
 if (carry)
  ERRMSG("Overflow during " << firstValue << " - " << secondValue);
 return returnValue;
}

//  
//  	Return an instance containing the product of firstValue and secondValue.
//  
PrimNumberWords operator*(const PrimNumberWords& firstValue, const PrimNumberWords& secondValue)
{
 const PrimNumberWords& longerValue = firstValue._tally > secondValue._tally ? firstValue : secondValue;
 const PrimNumberWords& shorterValue = firstValue._tally > secondValue._tally ? secondValue : firstValue;
 PrimNumberWords workingValue(PrimSize(firstValue._tally + secondValue._tally));
 unsigned short *q = workingValue._contents.get();
 const unsigned short *p1 = firstValue._contents.get();
 for (size_t i = firstValue._tally; i > 0; p1++, q++, i--)
 {
  unsigned long carry = 0;
  const unsigned short *p2 = secondValue._contents.get();
  unsigned short *qq = q;
  for (size_t j = secondValue._tally; j-- > 0; )
  { 
   carry += *qq;
   carry += *p1 * *p2++;
   *qq++ = carry & PrimNumberWords::WORD_MASK;
   carry >>= PrimNumberWords::BITS_PER_WORD;
  }
  for (size_t k = i; carry && (k-- > 0); )
  { 
   *qq++ = carry & PrimNumberWords::WORD_MASK;
   carry >>= PrimNumberWords::BITS_PER_WORD;
  }
  if (carry)
   ERRMSG("MSW overflow during " << firstValue << " * " << secondValue);
 }
 PrimNumberWords returnValue(PrimSize(longerValue._tally));
 const unsigned short *p = workingValue._contents.get() + workingValue._tally;
 bool overFlow = false;
 for (size_t l = shorterValue._tally; l-- > 0; )
  if (*--p)
  {
   ERRMSG("Overflow during " << firstValue << " * " << secondValue);
   overFlow = true;
   break;
  }
 q = returnValue._contents.get();
 if (overFlow)
 {
  for (size_t m = returnValue._tally; m-- > 0; )
   *q++ = PrimNumberWords::WORD_MASK;
 }
 else
 {
  const unsigned short *p = workingValue._contents.get();
  for (size_t m = returnValue._tally; m-- > 0; )
   *q++ = *p++;
 }
 return returnValue;
}

//  
//  	Return an instance containing the quotient of firstValue and secondValue.
//  
PrimNumberWords operator/(const PrimNumberWords& firstValue, const PrimNumberWords& secondValue)
{
 PrimNumberWords remainderValue(firstValue);
 PrimNumberWords returnValue(PrimSize(firstValue._tally));
 PrimNumberWords::divide(returnValue, remainderValue, secondValue);
 return returnValue;
}

//  
//  	Return an instance containing the result of firstValue modulo secondValue.
//  
PrimNumberWords operator%(const PrimNumberWords& firstValue, const PrimNumberWords& secondValue)
{
 PrimNumberWords remainderValue(firstValue);
 PrimNumberWords returnValue(PrimSize(firstValue._tally));
 PrimNumberWords::divide(returnValue, remainderValue, secondValue);
 return remainderValue;
}

//  
//  	Return an instance comprising the bit-wise and of firstValue and secondValue.
//  
PrimNumberWords operator&(const PrimNumberWords& firstValue, const PrimNumberWords& secondValue)
{
 const PrimNumberWords& longerValue = firstValue._tally > secondValue._tally ? firstValue : secondValue;
 const PrimNumberWords& shorterValue = firstValue._tally > secondValue._tally ? secondValue : firstValue;
 PrimNumberWords returnValue(PrimSize(longerValue._tally));
 const unsigned short *p1 = longerValue._contents.get();
 const unsigned short *p2 = shorterValue._contents.get();
 unsigned short *q = returnValue._contents.get();
 for (size_t i = shorterValue._tally; i-- > 0; )
  *q++ = *p1++ & *p2++;
 return returnValue;
}

//  
//  	Return an instance comprising the bit-wise inclusive or of firstValue and secondValue.
//  
PrimNumberWords operator|(const PrimNumberWords& firstValue, const PrimNumberWords& secondValue)
{
 const PrimNumberWords& longerValue = firstValue._tally > secondValue._tally ? firstValue : secondValue;
 const PrimNumberWords& shorterValue = firstValue._tally > secondValue._tally ? secondValue : firstValue;
 PrimNumberWords returnValue(longerValue);
 const unsigned short *p = shorterValue._contents.get();
 unsigned short *q = returnValue._contents.get();
 for (size_t i = shorterValue._tally; i-- > 0; )
  *q++ |= *p++;
 return returnValue;
}

//  
//  	Return an instance comprising the bit-wise exclusive or of firstValue and secondValue.
//  
PrimNumberWords operator^(const PrimNumberWords& firstValue, const PrimNumberWords& secondValue)
{
 const PrimNumberWords& longerValue = firstValue._tally > secondValue._tally ? firstValue : secondValue;
 const PrimNumberWords& shorterValue = firstValue._tally > secondValue._tally ? secondValue : firstValue;
 PrimNumberWords returnValue(longerValue);
 const unsigned short *p = shorterValue._contents.get();
 unsigned short *q = returnValue._contents.get();
 for (size_t i = shorterValue._tally; i-- > 0; )
  *q++ ^= *p++;
 return returnValue;
}

//  
//  	Return an instance containing the firstValue left shifted secondValue.
//  .bugbug dreadful implementation
PrimNumberWords operator<<(const PrimNumberWords& firstValue, const PrimNumberWords& secondValue)
{
 PrimNumberWords loopCount(secondValue);
 PrimNumberWords returnValue(firstValue);
 const PrimNumberWords unitValue(1uL);
 while (!loopCount.is_zero())
 {
  returnValue = returnValue + returnValue;
  loopCount = loopCount - unitValue;
 }
 return returnValue;
}

//  
//  	Return an instance containing the firstValue right shifted secondValue.
//  .bugbug not implemented
PrimNumberWords operator>>(const PrimNumberWords& firstValue, const PrimNumberWords& secondValue)
{
 const PrimNumberWords& longerValue = firstValue._tally > secondValue._tally ? firstValue : secondValue;
 const PrimNumberWords& shorterValue = firstValue._tally > secondValue._tally ? secondValue : firstValue;
 PrimNumberWords returnValue(PrimSize(longerValue._tally));
 ERRMSG("Undefined " << firstValue << " >> " << secondValue);
 return returnValue;
}

//  
//  	Return true if firstValue is the same as secondValue.
//  
bool operator==(const PrimNumberWords& firstValue, const PrimNumberWords& secondValue)
{
 const PrimNumberWords& longerValue = firstValue._tally > secondValue._tally ? firstValue : secondValue;
 const PrimNumberWords& shorterValue = firstValue._tally > secondValue._tally ? secondValue : firstValue;
 const unsigned short *p1 = longerValue._contents.get();
 const unsigned short *p2 = shorterValue._contents.get();
 for (size_t i = shorterValue._tally; i-- > 0; )
  if (*p1++ != *p2++)
   return false;
 for (size_t j = longerValue._tally - shorterValue._tally; j-- > 0; )
  if (*p1++ != 0)
   return false;
 return true;
}

//  
//  	Return true if firstValue is less than secondValue.
//  
bool operator<(const PrimNumberWords& firstValue, const PrimNumberWords& secondValue)
{
 const bool firstLonger = firstValue._tally > secondValue._tally;
 const PrimNumberWords& longerValue = firstLonger ? firstValue : secondValue;
 const PrimNumberWords& shorterValue = firstLonger ? secondValue : firstValue;
 const unsigned short *p1 = longerValue._contents.get() + longerValue._tally;
 for (size_t j = longerValue._tally - shorterValue._tally; j-- > 0; )
  if (*--p1 != 0)
   return !firstLonger;
 p1 = firstValue._contents.get() + shorterValue._tally;
 const unsigned short *p2 = secondValue._contents.get() + shorterValue._tally;
 for (size_t i = shorterValue._tally; i-- > 0; )
  if (*--p1 != *--p2)
   return *p1 < *p2;
 return false;
}

//  
//  	Generate an ASCII representation of aValue to an output stream.
//  
std::ostream& operator<<(std::ostream& s, const PrimNumberWords& aValue)
{
 std::ostrstream os;
 PrimNumberWords residualValue(aValue);
 do {
  os << char('0' + residualValue.short_divide(10));
 } while (!residualValue.is_zero());
 os << std::ends;
 char *pStart = os.str();
 if (pStart)
 {
  for (const char *p = pStart + strlen(pStart); p > pStart; )
   s << *--p;
  delete[] pStart;
 }
 return s;
}
 




PRIMTEST_MODULE_DECL(PrimNumberWords_eq,"operator==()")
//  
//  	Perform tests of operator== returning true if ok.
//  
int PrimNumberWords_eq::do_test()
{
 static unsigned short in1[] = { 0xFFFF,0x7FFF,0x4123,0x0000,0x0000 };
 static unsigned short in2[] = { 0x0001,0x8000,0xBEDC };
 const PrimNumberWords val1(in1, PRIM_ARRAY_SIZE(in1));
 const PrimNumberWords val2(in2, PRIM_ARRAY_SIZE(in2));
 if (!(val1 == val1))
 {
  ERRMSG(val1 << " is not equal to itself.");
  return false;
 }
 if (val1 == val2)
 {
  ERRMSG(val1 << " is not equal to " << val2);
  return false;
 }
 return (true);
}

PRIMTEST_MODULE_DECL(PrimNumberWords_add,"operator+()")
//  
//  	Perform tests of operator+ returning true if ok.
//  
int PrimNumberWords_add::do_test()
{
 static unsigned short in1[] = { 0xFFFF,0x7FFF,0x4123,0x0000,0x0000 };
 static unsigned short in2[] = { 0x0001,0x8000,0xBEDC };
 static unsigned short res[] = { 0x0000,0x0000,0x0000,0x0001,0x0000 };
 const PrimNumberWords val1(in1, PRIM_ARRAY_SIZE(in1));
 const PrimNumberWords val2(in2, PRIM_ARRAY_SIZE(in2));
 const PrimNumberWords wantedRes(res, PRIM_ARRAY_SIZE(res));
 const PrimNumberWords actualRes(val1 + val2);
 if (actualRes != wantedRes)
 {
  ERRMSG(val1 << " + " << val2 << " should be " << wantedRes << " rather than " << actualRes);
  return false;
 }
 return (true);
}

PRIMTEST_MODULE_DECL(PrimNumberWords_mul,"operator*()")
//  
//  	Perform tests of operator* returning true if ok.
//  
int PrimNumberWords_mul::do_test()
{
 static unsigned short in1[] = { 0x1111,0x1111,0x0011,0x0000,0x0000 };
 static unsigned short in2[] = { 0x2222,0x2222,0x0022 };
 static unsigned short res[] = { 0x8642,0x0eca,0x1353,0x8acf,0x0246 };
 const PrimNumberWords val1(in1, PRIM_ARRAY_SIZE(in1));
 const PrimNumberWords val2(in2, PRIM_ARRAY_SIZE(in2));
 const PrimNumberWords wantedRes(res, PRIM_ARRAY_SIZE(res));
 const PrimNumberWords actualRes(val1 * val2);
 if (actualRes != wantedRes)
 {
  ERRMSG(val1 << " * " << val2 << " should be " << wantedRes << " rather than " << actualRes);
  return false;
 }
 return (true);
}

PRIMTEST_MODULE_DECL(PrimNumberWords_parse,"parse()")
//  
//  	Perform tests of read_from returning true if ok.
//  
int PrimNumberWords_parse::do_test()
{
 static const char numberString[] = "00000123abcDEF98760";
 static unsigned short res[] = { 0x8760,0xDEF9,0x3abc,0x0012,0x0000 };
 const PrimNumberWords wantedRes(res, PRIM_ARRAY_SIZE(res));
 const PrimNumberWords actualRes(numberString, sizeof(numberString)-1, 16);
 if (actualRes != wantedRes)
 {
  ERRMSG(actualRes << " should have parsed as " << wantedRes);
  return false;
 }
 return (true);
}
