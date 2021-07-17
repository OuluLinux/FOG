
#ifndef FOGNUMBER_HXX
#define FOGNUMBER_HXX



struct FogNumberValue;
class FogNumber : public FogExpr {
	typedef FogExpr Super;
	typedef FogNumber This;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_SHAREDLEAF_DECLS
private:
	const PrimNumber& _number;     //   The number.
	const FogBuiltInTypeId& _type;    //    and its type.
private:
	FogNumber(const This&);
	This& operator=(const This&);
	This& mutate() const { return *(This*)this; }
protected:
	FogNumber(const PrimNumber& aNumber, const FogBuiltInTypeId& numberType);
	virtual ~FogNumber() {}
public:
	long as_int() const;
	virtual bool emit(FogEmitContext& emitContext) const;
	virtual bool get_character(PrimIdHandle& returnId, FogScopeContext& inScope);
	virtual bool get_dyadic(FogTokenRef& returnValue, FogScopeContext& inScope,
	                        FogTokenType::TokenType dyadicOp, const FogToken& rightValue) const;
	virtual bool get_identifier(PrimIdHandle& returnId, FogScopeContext& inScope);
	virtual bool get_monadic(FogTokenRef& returnValue, FogScopeContext& inScope,
	                         FogTokenType::TokenType monadicOp) const;
	virtual const PrimNumber& get_number(FogScopeContext& inScope) const;
	virtual bool get_number_token(FogTokenRef& returnValue, FogScopeContext& inScope) const;
	virtual bool get_string(PrimIdHandle& returnId, FogScopeContext& inScope);
	virtual FogNumber* is_number();
	virtual FogNumber& make(const FogMetaType& metaType, FogScopeContext& inScope) const;
	virtual const FogMetaType& meta_type() const;
	const PrimNumber& number_value() const { return _number; }
	virtual FogTokenType::TokenType pp_token_type_enum() const;
	virtual FogStream& print(FogStream& s) const = 0;
	virtual std::ostream& print_diagnostic(std::ostream& s, int aDepth) const;
	virtual char print_named(std::ostream& s, const PrimId* scopeId, char tailChar) const;
	virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
	virtual FogTokenType::TokenType token_type_enum() const;
private:
	static bool decode(FogNumberValue& numberValue, bool unDiagnosed, const PrimId& anId);
	static const PrimNumber* decode_decimal(bool unDiagnosed, const char* aBuffer, size_t aLength, bool hasF);
	static const PrimNumber* decode_floating(bool unDiagnosed, const char* aBuffer, size_t aLength, bool hasU);
	static const PrimNumber* decode_hexadecimal(bool unDiagnosed, const char* aBuffer, size_t aLength);
	static const PrimNumber* decode_octal(bool unDiagnosed, const char* aBuffer, size_t aLength, bool hasF);
	static FogNumber& make(const PrimNumber& aNumber, const FogBuiltInTypeId& numberType);
public:
	static FogNumber& make(const PrimId& anId, bool unDiagnosed = false);
	static FogNumber& make(const char* aBuffer, size_t aLength);
	//  	static FogNumber& make_complex(complex aValue);
	static FogNumber& make_false();
	static FogNumber& make_integer(long aValue);
	static FogNumber& make_logical(bool aValue);
	static FogNumber& make_natural(unsigned long aValue);
	static FogNumber& make_real(double aValue);
	static FogNumber& make_true();
	static FogNumber& make_zero();
	static FogNumber& null();
public:
	friend bool operator==(const FogNumber& firstNumber, const FogNumber& secondNumber)
	{ return &firstNumber._number == &secondNumber._number; }
	friend std::ostream& operator<<(std::ostream& s, const FogNumber& aNumber) { return aNumber.print_this(s); }
	friend FogStream& operator<<(FogStream& s, const FogNumber& aNumber) { return aNumber.print(s); }
};
#endif

