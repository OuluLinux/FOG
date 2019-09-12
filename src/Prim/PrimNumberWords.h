
#ifndef PRIMNUMBERWORDS_HXX
#define PRIMNUMBERWORDS_HXX




//  
//  	A PrimNumberWords implements an arbitrary precision natural number and its arithmetic capabilities.
//  	It is intended primarily for use by PrimNumber.
//  
//  	Updating operations truncate within the size of the result. Creating operations create
//  	a value long enough to hold the result. Undersized inputs are automatically MS zero extended.
//  
class PrimNumberWords : public PrimPrint
{
 TYPEDECL_SINGLE(PrimNumberWords, PrimPrint)
private:
 enum
 {
  WORDS_PER_LONG = (sizeof(unsigned long) + sizeof(unsigned short) - 1) / sizeof(unsigned short),
  BITS_PER_WORD = 16,
  WORD_MASK = (1ul << BITS_PER_WORD) - 1
 };
private:
 PrimAdoptedArray<unsigned short> _contents;  //   Array of words, LS first.
 size_t _capacity;        //   Number of words allocated in array.
 size_t _tally;         //   Number of words used in array.
private:
 static void divide(PrimNumberWords& quotient, PrimNumberWords& dividend, const PrimNumberWords& divisor);
public:
//  
//  	Construct a default value comprising no contents and therefore zero value.
//  
 PrimNumberWords() : _contents(0), _capacity(0), _tally(0) {}
 explicit PrimNumberWords(const PrimSize& aCapacity);
 explicit PrimNumberWords(unsigned long aValue);
 PrimNumberWords(const PrimNumberWords& aValue);
 PrimNumberWords(const char *someChars, size_t numChars, int numberBase = 10);
 PrimNumberWords(const unsigned short *someValues, size_t numValues);
 virtual ~PrimNumberWords();
 PrimNumberWords& operator=(const PrimNumberWords& aValue);
 PrimNumberWords operator~() const;
 bool operator!() const;
 void assign(const unsigned short *someValues, size_t numValues);
 bool is_even() const;
//  
//  	Return true if conversion to small can be performed without overflow.
//  
 bool is_small() const { return non_zero_words() <= WORDS_PER_LONG; }
//  
//  	Return true if the number is zero.
//  
 bool is_zero() const { return non_zero_words() == 0; }
 PrimNumberWords maximum() const;
 size_t non_zero_words() const;
 virtual std::ostream& print_this(std::ostream& s) const;
 bool set_sizes(size_t aCapacity, size_t aTally);
 unsigned long short_add(unsigned long aCarry);
 unsigned long short_divide(unsigned short aFactor);
 unsigned long short_multiply(unsigned short aFactor, unsigned long aCarry = 0);
 double to_double() const;
 unsigned long to_small() const;
 static const PrimNumberWords& zero();
 friend PrimNumberWords operator+(const PrimNumberWords& firstValue, const PrimNumberWords& secondValue);
 friend PrimNumberWords operator-(const PrimNumberWords& firstValue, const PrimNumberWords& secondValue);
 friend PrimNumberWords operator*(const PrimNumberWords& firstValue, const PrimNumberWords& secondValue);
 friend PrimNumberWords operator/(const PrimNumberWords& firstValue, const PrimNumberWords& secondValue);
 friend PrimNumberWords operator%(const PrimNumberWords& firstValue, const PrimNumberWords& secondValue);
 friend PrimNumberWords operator&(const PrimNumberWords& firstValue, const PrimNumberWords& secondValue);
 friend PrimNumberWords operator|(const PrimNumberWords& firstValue, const PrimNumberWords& secondValue);
 friend PrimNumberWords operator^(const PrimNumberWords& firstValue, const PrimNumberWords& secondValue);
 friend PrimNumberWords operator<<(const PrimNumberWords& firstValue, const PrimNumberWords& secondValue);
 friend PrimNumberWords operator>>(const PrimNumberWords& firstValue, const PrimNumberWords& secondValue);
//  
//  	Return true if both firstValue and secondValue are non-zero.
//  
 friend bool operator&&(const PrimNumberWords& firstValue, const PrimNumberWords& secondValue)
  { return !(!firstValue || !secondValue); }
//  
//  	Return true if either firstValue or secondValue are non-zero.
//  
 friend bool operator||(const PrimNumberWords& firstValue, const PrimNumberWords& secondValue)
  { return !(!firstValue && !secondValue); }
 friend bool operator==(const PrimNumberWords& firstValue, const PrimNumberWords& secondValue);
//  
//  	Return true if firstValue differs from secondValue.
//  
 friend bool operator!=(const PrimNumberWords& firstValue, const PrimNumberWords& secondValue)
  { return !(firstValue == secondValue); }
 friend bool operator<(const PrimNumberWords& firstValue, const PrimNumberWords& secondValue);
//  
//  	Return true if firstValue is less than or equal to secondValue.
//  
 friend bool operator<=(const PrimNumberWords& firstValue, const PrimNumberWords& secondValue)
  { return !(secondValue < firstValue); }
//  
//  	Return true if firstValue is greater than or equal to secondValue.
//  
 friend bool operator>=(const PrimNumberWords& firstValue, const PrimNumberWords& secondValue)
  { return !(firstValue < secondValue); }
//  
//  	Return true if firstValue is greater than secondValue.
//  
 friend bool operator>(const PrimNumberWords& firstValue, const PrimNumberWords& secondValue)
  { return secondValue < firstValue; }
 friend std::ostream& operator<<(std::ostream& s, const PrimNumberWords& aValue);
};
#endif
 
