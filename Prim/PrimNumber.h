
#ifndef PRIMNUMBER_HXX
#define PRIMNUMBER_HXX



#undef max
#undef min


struct PrimNumber_info;

class PrimNumber : public PrimPrint
{
 typedef PrimPrint Super;
 TYPEDECL_SINGLE(PrimNumber, Super)
 PRIMSHARES_DECLS(PrimNumber)
 PRIMREF_DECLS(PrimNumber)

private:
//  	static PrimNumberMapOfRef _map;					// Map of all numbers.
private:
 static int compare(const PrimNumberRefToConst *v1, const PrimNumberRefToConst *v2);
 static int compare_raw(const PrimNumberRefToConst *v1, const PrimNumberRefToConst *v2);
 static const PrimNumber& make(const char *anId, const double *checkValue = 0);
 static PrimNumberMapOfRef& map();    //   Map of all numbers.
 static void order();
 static std::ostream& print_exponent(std::ostream& s, long anExponent);
public:
 static const PrimNumber& make_bool(bool aBool) { return aBool ? make_true() : make_zero(); }
 static const PrimNumber& make_decimal(const char *aString);
 static const PrimNumber& make_decimal(const char *aBuffer, size_t aLength);
 static const PrimNumber& make_digits(int aBase, bool isNegated, const char *aString);
 static const PrimNumber& make_digits(int aBase, bool isNegated, const char *aBuffer, size_t aLength);
 static const PrimNumber& make_hexadecimal(const char *aString);
 static const PrimNumber& make_hexadecimal(const char *aBuffer, size_t aLength);
 static const PrimNumber& make_false() { return make_zero(); }
 static const PrimNumber& make_int(long anInt, bool checkNumber = true);
 static const PrimNumber& make_max_int();
 static const PrimNumber& make_max_integer(size_t someBits);
 static const PrimNumber& make_max_nat();
 static const PrimNumber& make_max_natural(size_t someBits);
 static const PrimNumber& make_min_int();
 static const PrimNumber& make_min_integer(size_t someBits);
 static const PrimNumber& make_min_nat() { return make_zero(); }
 static const PrimNumber& make_min_natural(size_t someBits) { return make_zero(); }
 static const PrimNumber& make_one();
 static const PrimNumber& make_nat(unsigned long anInt, bool checkNumber = true);
 static const PrimNumber& make_real(double aDouble, bool checkNumber = true);
 static const PrimNumber& make_true() { return make_one(); }
 static const PrimNumber& make_zero();
 static const PrimNumber& null() { return immutable_null(); }
 static std::ostream& print_all(std::ostream& s);

private:
 PrimNumber(const PrimNumber&);
 PrimNumber& operator=(const PrimNumber&);

private:
 const PrimIdHandle _id;
 unsigned int _index;
private:
 PrimNumber(const PrimId& anId);
 void exponent(PrimNumber_info& exponentInfo) const;
 void mantissa(PrimNumber_info& mantissaInfo) const;
protected:
 PrimNumber();
 virtual ~PrimNumber();
public:
 bool operator!() const;
 const PrimNumber& operator-() const;
 const PrimNumber& operator~() const;
 bool as_bool() const;
 PrimStringHandle as_decimal() const;
 double as_double() const;
 PrimStringHandle as_hexadecimal() const;
 long as_int() const;
 unsigned long as_nat() const;
 const PrimId& id() const { return *_id; }
 bool is_bool() const;
 bool is_complex() const;
 bool is_int() const;
 bool is_integer() const;
 bool is_nat() const;
 bool is_natural() const;
 bool is_negative() const;
 bool is_positive() const;
 bool is_real() const;
 bool is_zero() const;
 std::ostream& print_decimal(std::ostream& s) const;
 std::ostream& print_hexadecimal(std::ostream& s) const;
 virtual std::ostream& print_this(std::ostream& s) const;
 virtual std::ostream& print_viz(std::ostream& s) const;
 const PrimNumber& quantise_to_epsilon(const PrimNumber& anEpsilon) const;
 friend bool operator==(const PrimNumber& n1, const PrimNumber& n2) { return &n1 == &n2; }
 friend bool operator!=(const PrimNumber& n1, const PrimNumber& n2) { return &n1 != &n2; }
 friend bool operator<(const PrimNumber& n1, const PrimNumber& n2);
 friend bool operator<=(const PrimNumber& n1, const PrimNumber& n2) { return !(n2 < n1); }
 friend bool operator>=(const PrimNumber& n1, const PrimNumber& n2) { return !(n1 < n2); }
 friend bool operator>(const PrimNumber& n1, const PrimNumber& n2) { return n2 < n1; }
 friend const PrimNumber& operator+(const PrimNumber& n1, const PrimNumber& n2);
 friend const PrimNumber& operator-(const PrimNumber& n1, const PrimNumber& n2);
 friend const PrimNumber& operator*(const PrimNumber& n1, const PrimNumber& n2);
 friend const PrimNumber& operator/(const PrimNumber& n1, const PrimNumber& n2);
 friend const PrimNumber& operator%(const PrimNumber& n1, const PrimNumber& n2);
 friend const PrimNumber& operator<<(const PrimNumber& n1, const PrimNumber& n2);
 friend const PrimNumber& operator>>(const PrimNumber& n1, const PrimNumber& n2);
 friend const PrimNumber& operator&(const PrimNumber& n1, const PrimNumber& n2);
 friend const PrimNumber& operator|(const PrimNumber& n1, const PrimNumber& n2);
 friend const PrimNumber& operator^(const PrimNumber& n1, const PrimNumber& n2);
 friend bool operator&&(const PrimNumber& n1, const PrimNumber& n2);
 friend bool operator||(const PrimNumber& n1, const PrimNumber& n2);
 friend const PrimNumber& abs(const PrimNumber& n);
 friend const PrimNumber& acos(const PrimNumber& n);
 friend const PrimNumber& asin(const PrimNumber& n);
 friend const PrimNumber& atan(const PrimNumber& n);
 friend const PrimNumber& ceil(const PrimNumber& n);
 friend const PrimNumber& conj(const PrimNumber& n);
 friend const PrimNumber& cos(const PrimNumber& n);
 friend const PrimNumber& cosh(const PrimNumber& n);
 friend const PrimNumber& cplx(const PrimNumber& n1, const PrimNumber& n2);
 friend bool even(const PrimNumber& n);
 friend const PrimNumber& exp(const PrimNumber& n);
 friend const PrimNumber& floor(const PrimNumber& n);
 friend const PrimNumber& frac(const PrimNumber& n);
 friend const PrimNumber& idiv(const PrimNumber& n1, const PrimNumber& n2);
 friend const PrimNumber& imag(const PrimNumber& n);
 friend const PrimNumber& inv(const PrimNumber& n);
 friend const PrimNumber& ln(const PrimNumber& n);
 friend const PrimNumber& log(const PrimNumber& n1, const PrimNumber& n2);
 friend const PrimNumber& max(const PrimNumber& n1, const PrimNumber& n2);
 friend const PrimNumber& min(const PrimNumber& n1, const PrimNumber& n2);
 friend const PrimNumber& nat(const PrimNumber& n);
 friend bool odd(const PrimNumber& n);
 friend const PrimNumber& pow(const PrimNumber& n1, const PrimNumber& n2);
 friend const PrimNumber& real(const PrimNumber& n);
 friend const PrimNumber& round(const PrimNumber& n);
 friend const PrimNumber& sin(const PrimNumber& n);
 friend const PrimNumber& sinh(const PrimNumber& n);
 friend const PrimNumber& sqrt(const PrimNumber& n);
 friend const PrimNumber& tan(const PrimNumber& n);
 friend const PrimNumber& tanh(const PrimNumber& n);
 friend const PrimNumber& trunc(const PrimNumber& n);
};
#endif
 
