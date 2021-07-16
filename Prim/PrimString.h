
#ifndef PRIMSTRING_HXX
#define PRIMSTRING_HXX





//  .bugbug This is a C-style extending class, and as such is only valid C++ if it is a POD, which
//   it isn't although it could be by flattening the share count macro to eliminate internal access
//   specifiers, and moving all constructors, operators, destructors, assignments to the handle.
//   Note that the null string is zero valued and so requires no initialisation.

class PrimString {

	friend class PrimStringHandle;
	
private:
	enum Locked {};
	
private:
	static char _null[];        //   The null instance.
	
public:
//
//  	Report the class name for debug purposes.
//
	static const char *class_name() {
		return "PrimString";
	}
	
	static const PrimString *create(const char *someText);
//  	static const PrimString *create(const char *firstText, const char *secondText);
	static const PrimString *create(const char *aBuffer, size_t aLength);
	static const PrimString& null() {
		return (*(const PrimString *)_null);
	}
	
	static int sort_compare(const PrimStringHandle *s1, const PrimStringHandle *s2);
//
//  	Report whether firstString is the same as secondString.
//
	friend bool operator==(const PrimString& firstString, const PrimString& secondString) {
		return firstString.is_equal(secondString);
	}
	
	friend bool operator==(const PrimString& firstString, const char *secondString) {
		return firstString.is_equal(secondString);
	}
	
	friend bool operator==(const char *firstString, const PrimString& secondString) {
		return secondString.is_equal(firstString);
	}
	
//
//  	Report whether firstString is different to secondString.
//
	friend bool operator!=(const PrimString& firstString, const PrimString& secondString) {
		return !firstString.is_equal(secondString);
	}
	
	friend bool operator!=(const PrimString& firstString, const char *secondString) {
		return !firstString.is_equal(secondString);
	}
	
	friend bool operator!=(const char *firstString, const PrimString& secondString) {
		return !secondString.is_equal(firstString);
	}
	
	friend std::ostream& operator<<(std::ostream& s, const PrimString& aString);
	
private:
	PRIMSHARES_DECLS(PrimString)
	PRIMREF_DERIVED_DECLS(PrimString)      //   *DERIVED* to exclude virtual is_null()
	mutable char _text /*  [1]*/;        //   First byte of extendable text area.
	
private:
	PrimString(const PrimString& aString);     //   Copy by creation is forbidden.
	PrimString& operator=(const PrimString& aString);  //   Copy by assignment is forbidden.
	
private:
	inline PrimString(const char *aBuffer, size_t aLength);
	~PrimString() {}
	
public:
//
//  	Return non-0 if a non-null id.
//
	operator const Locked *() const {
		return (const Locked *)((const char *)this != _null ? this : 0);
	}
	
	bool check(bool fullCheck = false) const {
		return true;
	}
	
	int compare(const PrimString& aString) const;
	unsigned long hash() const;
	bool is_equal(const PrimString& aString) const;
	bool is_equal(const char *someText) const;
//
//  	Report whether this is the null string.
//
	bool is_null() const {
		return (const char *)this == _null;
	}
	
//
//  	Report whether this string is valid. That is whether it has a non-0 representation.
//  	This method is used to distinguish invalid keys, when a PrimStringHandle is a dictionary key.
//
	bool is_valid() const {
		return (const char *)this != _null;
	}
	
//
//  	There is no additional information to form part of a print-out.
//
	std::ostream& print_deep(std::ostream& s, int aDepth = 0) const {
		UNUSED(aDepth);
		return s;
	}
	
//
//  	Display the text as part of an inline print-out.
//
	std::ostream& print_instance(std::ostream& s) const {
		return s << *this;
	}
	
//
//  	Report the text of the representation (never 0).
//
	const char *str() const {
		return &_text;
	}
};

inline const PrimString& PrimString::immutable_null() {
	return null();
}

//
//  	PrimStringHandle provides a reference counted handle for a PrimString. The class offers all the facilities
//  	of its public base class PrimRefToConst<PrimString> together with a friendlier name and
//  	some useful construction methods.
//
//  .note The plethora of operator==, operator!= are necessary to defeat attempts by PrimMath.h to do
//  	inherited pointer comparison in conjuction with PrimStringHandle(const PrimString& aString).

class PrimStringHandle : public PrimRefToConst<PrimString> {
	typedef PrimRefToConst<PrimString> Super;
	
private:
	static const PrimString *_null;      //   The null instance.
	
public:
//
//  	Return the 0 instance.
//
	static const PrimStringHandle& null() {
		return (*(const PrimStringHandle *)_null);
	}
	
public:
	PrimStringHandle() {}
	
	PrimStringHandle(const char *aName) : Super(PrimString::create(aName), ADOPT) {}
	
	PrimStringHandle(const char *aBuffer, size_t aLength) : Super(PrimString::create(aBuffer, aLength), ADOPT) {}
	
	PrimStringHandle(const PrimString& aString) : Super(aString) {}
	
	const char *str() const {
		return element().str();
	}
	
	friend std::ostream& operator<<(std::ostream& s, const PrimStringHandle& aHandle) {
		return operator<<(s, aHandle.element());
	}
	
	friend std::istream& operator>>(std::istream& s, PrimStringHandle& idHandle);
//
//  	Report whether firstString is the same as secondString.
//
	friend bool operator==(const PrimStringHandle& firstString, const PrimStringHandle& secondString) {
		return firstString->is_equal(*secondString);
	}
	
	friend bool operator==(const PrimStringHandle& firstString, const PrimString& secondString) {
		return firstString->is_equal(secondString);
	}
	
	friend bool operator==(const PrimStringHandle& firstString, const char *secondString) {
		return firstString->is_equal(secondString);
	}
	
	friend bool operator==(const PrimString& firstString, const PrimStringHandle& secondString) {
		return firstString.is_equal(*secondString);
	}
	
	friend bool operator==(const char *firstString, const PrimStringHandle& secondString) {
		return secondString->is_equal(firstString);
	}
	
//
//  	Report whether firstString is different to secondString.
//
	friend bool operator!=(const PrimStringHandle& firstString, const PrimStringHandle& secondString) {
		return !firstString->is_equal(*secondString);
	}
	
	friend bool operator!=(const PrimStringHandle& firstString, const PrimString& secondString) {
		return !firstString->is_equal(secondString);
	}
	
	friend bool operator!=(const PrimStringHandle& firstString, const char *secondString) {
		return !firstString->is_equal(secondString);
	}
	
	friend bool operator!=(const PrimString& firstString, const PrimStringHandle& secondString) {
		return !firstString.is_equal(*secondString);
	}
	
	friend bool operator!=(const char *firstString, const PrimStringHandle& secondString) {
		return !secondString->is_equal(firstString);
	}
};

#endif

