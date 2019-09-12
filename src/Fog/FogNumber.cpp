#include <Fog/FogIncludeAll.h>















struct FogNumberValue {     //   Struct used to return a pair from FogNumber::decode.
	const PrimNumber *_number;
	const FogBuiltInTypeId *_type;
	
public:
	FogNumberValue() : _number(0), _type(0) {}
};

//
//  	Externally originating numbers with retained source spelling.
//

class FogAsciiNumber : public FogNumber {
	typedef FogNumber Super;
	typedef FogAsciiNumber This;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	
private:
	const PrimIdHandle _id;      //   The source text.
	
private:
	FogAsciiNumber(const This&);
	This& operator=(const This&);
	
private:
	const PrimNumber& initialise(const PrimId& anId);
	
protected:
	FogAsciiNumber() : Super(PrimNumber::null(), FogBuiltInTypeId::void_type()) {}
	
	virtual ~FogAsciiNumber() {}
	
public:
	FogAsciiNumber(const PrimNumber& aNumber, const FogBuiltInTypeId& numberType, const PrimId& anId)
			: Super(aNumber, numberType), _id(anId) {}
			
	const PrimId& id() const {
		return *_id;
	}
	
	virtual FogStream& print(FogStream& s) const {
		return s << *_id;
	}
	
	virtual std::ostream& print_this(std::ostream& s) const {
		return s << *_id;
	}
	
	virtual std::ostream& print_viz(std::ostream& s) const {
		return s << "\"number: " << *_id << '\"';
	}
	
	virtual size_t source_columns() const {
		return _id->length();
	}
};

//
//  	Internally originated numbers.
//

class FogBinaryNumber : public FogNumber {
	typedef FogNumber Super;
	typedef FogBinaryNumber This;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	
private:
	FogBinaryNumber(const This&);
	This& operator=(const This&);
	
protected:
	FogBinaryNumber() : Super(PrimNumber::null(), FogBuiltInTypeId::void_type()) {}
	
	virtual ~FogBinaryNumber() {}
	
public:
	FogBinaryNumber(const PrimNumber& aNumber, const FogBuiltInTypeId& numberType)
			: Super(aNumber, numberType) {}
			
	const PrimId& id() const {
		return number_value().id();
	}
	
	virtual FogStream& print(FogStream& s) const {
		return s << number_value();
	}
	
	virtual std::ostream& print_this(std::ostream& s) const {
		return number_value().print_viz(s);
	}
	
	virtual std::ostream& print_viz(std::ostream& s) const {
		return s << "\"number: " << viz(number_value()) << '\"';
	}
};

TYPEINFO_SINGLE(FogNumber, Super)
FOGTOKEN_SHAREDLEAF_IMPL(FogNumber)

PRIM_WITH_MAP(FogAsciiNumber)
TYPEINFO_SINGLE(FogAsciiNumber, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogAsciiNumber)
PRIMREFS_IMPL(FogAsciiNumber)
PRIMMAPOFREFS_IMPL(FogAsciiNumber)
TMPL_HACK_FIX_DO(FogAsciiNumber)

PRIM_WITH_MAP(FogBinaryNumber)
TYPEINFO_SINGLE(FogBinaryNumber, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogBinaryNumber)
PRIMREFS_IMPL(FogBinaryNumber)
PRIMMAPOFREFS_IMPL(FogBinaryNumber)
TMPL_HACK_FIX_DO(FogBinaryNumber)

FogNumber::FogNumber(const PrimNumber& aNumber, const FogBuiltInTypeId& numberType)
		:
		_number(aNumber),
		_type(numberType) {}
		
long FogNumber::as_int() const {
	return _number.as_int();
}

//
//  	Decode the ASCII text anId, assigning the value and type to numberValue. Generates an error and returns
//  	false on failure.
//
bool FogNumber::decode(FogNumberValue& numberValue, bool unDiagnosed, const PrimId& anId) {
	const char *aBuffer = anId.str();
	size_t aLength = anId.length();
	bool isHex = (aLength >= 2) && (aBuffer[0] == '0') && ((aBuffer[1] == 'x') || (aBuffer[1] == 'X'));
	bool isF = false;
	bool isL = false;
	bool isU = false;
	
	for (bool keepGoing = true; (aLength > 0) && keepGoing; keepGoing ? --aLength : 0)
		switch (aBuffer[aLength-1]) {
		case 'u': case 'U': isU = true;
				break;
				
			case 'l':
			
			case 'L':
				isL = true;
				break;
				
			case 'f':
			
			case 'F':
			
				if (!isHex) {
					isF = true;
					break;
				}
				
			default:
			
				keepGoing = false;
				break;
		}
		
	if (aLength < 1) {
		numberValue._number = &PrimNumber::null();
		numberValue._type = &FogBuiltInTypeId::void_type();
		return false;
	}
	
	if (isHex) {
		numberValue._number = decode_hexadecimal(unDiagnosed, aBuffer, aLength);
		numberValue._type = isU ? &FogBuiltInTypeId::natural_type() : &FogBuiltInTypeId::integer_type();
		return numberValue._number != 0;
	}
	
	const char *pEnd = aBuffer + aLength;
	
	for (const char *p = aBuffer; p < pEnd; p++)
		if ((*p == '.') || (*p == 'e') || (*p == 'E')) {
			numberValue._number = decode_floating(unDiagnosed, aBuffer, aLength, isU);
			numberValue._type = &FogBuiltInTypeId::real_type();
			return numberValue._number != 0;
		}
		
	if (aBuffer[0] == '0')
		numberValue._number = decode_octal(unDiagnosed, aBuffer, aLength, isF);
	else
		numberValue._number = decode_decimal(unDiagnosed, aBuffer, aLength, isF);
		
	numberValue._type = isU ? &FogBuiltInTypeId::natural_type() : &FogBuiltInTypeId::integer_type();
	
	return numberValue._number != 0;
}

const PrimNumber *FogNumber::decode_decimal(bool unDiagnosed, const char *aBuffer, size_t aLength, bool hasF) {
	if (hasF) {
		if (!unDiagnosed)
			ERRMSG("Decimal number should not have floating-suffix.");
			
		return 0;
	}
	
	const char *p = aBuffer;
	
	const char *pEnd = aBuffer + aLength;
	
	for (; p < pEnd; p++)
		if (!isdigit(*p))
			break;
			
	if (p >= pEnd)
		return &PrimNumber::make_digits(10, false, aBuffer, p - aBuffer);
		
	if (!unDiagnosed)
		ERRMSG("Decimal number should not have non-decimal digits.");
		
	return 0;
}

const PrimNumber *FogNumber::decode_floating(bool unDiagnosed, const char *aBuffer, size_t aLength, bool hasU) {
	if (!hasU)
		return &PrimNumber::make_decimal(aBuffer, aLength);    //   Has internal diagnostic.
		
	if (!unDiagnosed)
		ERRMSG("Floating number should not have unsigned-suffix.");
		
	return 0;
}

const PrimNumber *FogNumber::decode_hexadecimal(bool unDiagnosed, const char *aBuffer, size_t aLength) {
	const char *p = aBuffer+2;
	const char *pEnd = aBuffer + aLength;
	
	for (; p < pEnd; p++)
		if (!isxdigit(*p))
			break;
			
	if (p >= pEnd)
		return &PrimNumber::make_hexadecimal(aBuffer+2, p - (aBuffer+2));
		
	if (!unDiagnosed)
		ERRMSG("Hexadecimal number should not have non-hexadecimal digits.");
		
	return 0;
}

const PrimNumber *FogNumber::decode_octal(bool unDiagnosed, const char *aBuffer, size_t aLength, bool hasF) {
	if (hasF) {
		if (!unDiagnosed)
			ERRMSG("Octal number should not have floating-suffix.");
			
		return 0;
	}
	
	const char *p = aBuffer;
	
	const char *pEnd = aBuffer + aLength;
	
	for (; p < pEnd; p++)
		if ((*p < '0') || ('7' < *p))
			break;
			
	if (p >= pEnd)
		return &PrimNumber::make_digits(8, false, aBuffer, p - aBuffer);
		
	if (!unDiagnosed)
		ERRMSG("Octal number should not have non-octal digits.");
		
	return 0;
}

bool FogNumber::emit(FogEmitContext& emitContext) const {
	emitContext.emit_number(*this);
	return true;
}

bool FogNumber::get_character(PrimIdHandle& returnId, FogScopeContext& inScope) {
	PrimOstrstream s;
	_number.print_decimal(s);
	size_t aSize = s.pcount();
	returnId = PrimIdHandle(s.str(), aSize);
	return true;
}

bool FogNumber::get_dyadic(FogTokenRef& returnValue, FogScopeContext& inScope,
		FogTokenType::TokenType dyadicOp, const FogToken& rightValue) const {
	FogTokenRef numberValue;
	rightValue.get_number_token(numberValue, inScope);
	const FogNumber& rightNumber = numberValue->number();
	const FogBuiltInTypeId& maxType = max(_type, rightNumber._type);
	
	switch (dyadicOp) {
	
		case '+':
		
		case FogTokenType::ASS_ADD:
			returnValue.assign(FogNumber::make(maxType.add(_number, rightNumber._number), maxType));
			return true;
			
		case '&':
		
		case FogTokenType::ASS_AND:
			returnValue.assign(FogNumber::make(maxType.and_(_number, rightNumber._number), maxType));
			return true;
			
		case '/':
		
		case FogTokenType::ASS_DIV:
			returnValue.assign(FogNumber::make(maxType.div(_number, rightNumber._number), maxType));
			return true;
			
		case FogTokenType::EQ:
			returnValue.assign(FogNumber::make_logical(maxType.eq(_number, rightNumber._number)));
			return true;
			
		case FogTokenType::GE:
			returnValue.assign(FogNumber::make_logical(maxType.ge(_number, rightNumber._number)));
			return true;
			
		case '>':
			returnValue.assign(FogNumber::make_logical(maxType.gt(_number, rightNumber._number)));
			return true;
			
		case FogTokenType::LE:
			returnValue.assign(FogNumber::make_logical(maxType.le(_number, rightNumber._number)));
			return true;
			
		case FogTokenType::LOG_AND:
			returnValue.assign(FogNumber::make_logical(maxType.log_and_(_number, rightNumber._number)));
			return true;
			
		case FogTokenType::LOG_OR:
			returnValue.assign(FogNumber::make_logical(maxType.log_or(_number, rightNumber._number)));
			return true;
			
		case '<':
			returnValue.assign(FogNumber::make_logical(maxType.lt(_number, rightNumber._number)));
			return true;
			
		case '%':
		
		case FogTokenType::ASS_MOD:
			returnValue.assign(FogNumber::make(maxType.mod(_number, rightNumber._number), maxType));
			return true;
			
		case '*':
		
		case FogTokenType::ASS_MUL:
			returnValue.assign(FogNumber::make(maxType.mul(_number, rightNumber._number), maxType));
			return true;
			
		case FogTokenType::NE:
			returnValue.assign(FogNumber::make_logical(maxType.ne(_number, rightNumber._number)));
			return true;
			
		case '|':
		
		case FogTokenType::ASS_OR:
			returnValue.assign(FogNumber::make(maxType.or_(_number, rightNumber._number), maxType));
			return true;
			
		case FogTokenType::SHL:
		
		case FogTokenType::ASS_SHL:
			returnValue.assign(FogNumber::make(_type.shl(_number, rightNumber._number), _type));
			return true;
			
		case FogTokenType::SHR:
		
		case FogTokenType::ASS_SHR:
			returnValue.assign(FogNumber::make(_type.shr(_number, rightNumber._number), _type));
			return true;
			
		case '-':
		
		case FogTokenType::ASS_SUB:
			returnValue.assign(FogNumber::make(maxType.sub(_number, rightNumber._number), maxType));
			return true;
			
		case '^':
		
		case FogTokenType::ASS_XOR:
			returnValue.assign(FogNumber::make(maxType.xor_(_number, rightNumber._number), maxType));
			return true;
			
		default:
			return Super::get_dyadic(returnValue, inScope, dyadicOp, rightValue);
	}
}

bool FogNumber::get_identifier(PrimIdHandle& returnId, FogScopeContext& inScope) {
	PrimOstrstream s;
	_number.print_decimal(s);
	size_t aSize = s.pcount();
	returnId = PrimIdHandle(s.str(), aSize);
	return true;
}

bool FogNumber::get_monadic(FogTokenRef& returnValue, FogScopeContext& inScope,
		FogTokenType::TokenType monadicOp) const {
	switch (monadicOp) {
	
		case '-':
			returnValue.assign(FogNumber::make(_type.neg(_number), _type));
			return true;
			
		case '!':
			returnValue.assign(FogNumber::make_logical(_type.log_neg(_number)));
			return true;
			
		case '~':
			returnValue.assign(FogNumber::make(_type.com(_number), _type));
			return true;
			
		case FogTokenType::DEC:
			returnValue.assign(FogNumber::make(_type.sub(_number, PrimNumber::make_one()), _type));
			return true;
			
		case FogTokenType::INC:
			returnValue.assign(FogNumber::make(_type.add(_number, PrimNumber::make_one()), _type));
			return true;
			
		default:
			return Super::get_monadic(returnValue, inScope, monadicOp);
	}
}

const PrimNumber& FogNumber::get_number(FogScopeContext& inScope) const {
	return number_value();
}

bool FogNumber::get_number_token(FogTokenRef& returnValue, FogScopeContext& inScope) const {
	returnValue = mutate();
	return true;
}

bool FogNumber::get_string(PrimIdHandle& returnId, FogScopeContext& inScope) {
	PrimOstrstream s;
	_number.print_decimal(s);
	size_t aSize = s.pcount();
	returnId = PrimIdHandle(s.str(), aSize);
	return true;
}

FogNumber *FogNumber::is_number() {
	return this;
}

FogNumber& FogNumber::make(const char *aBuffer, size_t aLength) {
	return make(*PrimIdHandle(aBuffer, aLength));
}

//
//  	Make a number from ASCII.
//
FogNumber& FogNumber::make(const PrimId& anId, bool unDiagnosed) {
	static FogAsciiNumberMapOfRef numberMap;
	
	if (!anId)
		return FogNumber::null();
		
	FogNumber *aNumber = numberMap.find(anId);
	
	if (!aNumber) {
		FogNumberValue numberValue;
		
		if (decode(numberValue, unDiagnosed, anId)) {
			FogAsciiNumber *asciiNumber = new FogAsciiNumber(*numberValue._number, *numberValue._type, anId);
			
			if (asciiNumber)
				numberMap.adopt(asciiNumber);
				
			aNumber = asciiNumber;
		}
		
		else
			if (!unDiagnosed)
				aNumber = &make_zero();
	}
	
	return aNumber ? *aNumber : FogNumber::null();
}

//
//  	Make a number from value.
//
FogNumber& FogNumber::make(const PrimNumber& theNumber, const FogBuiltInTypeId& numberType) {
	static FogBinaryNumberMapOfRef numberMaps[FogBuiltInTypeId::ARRAY_SIZE];
	const FogBuiltInTypeId::TypeNumber typeNumber = numberType.type_number();
	FogBinaryNumberMapOfRef& numberMap = numberMaps[typeNumber];
	FogBinaryNumber *aNumber = numberMap.find(theNumber.id());
	
	if (!aNumber) {
		aNumber = new FogBinaryNumber(theNumber, numberType);
		
		if (aNumber)
			numberMap.adopt(aNumber);
	}
	
	return aNumber ? *aNumber : FogBinaryNumber::mutable_null();
}

//
//  	Return this number after conversion to metaType.
//
FogNumber& FogNumber::make(const FogMetaType& metaType, FogScopeContext& inScope) const {
	if (&metaType == &_type.number_type())
		return mutate();
	else
		if (metaType.is(FogMetaType::bool_type()))
			return make_logical(!_number.is_zero());
		else
			if (metaType.is(FogMetaType::unsigned_type())) {
				if (_number.is_nat())
					return make(_number, FogBuiltInTypeId::natural_type());
				else
					if (_number.is_negative()) {
						ERRMSG("Negative number truncated to 0.");
						return make(PrimNumber::make_zero(), FogBuiltInTypeId::natural_type());
					}
					
					else
						return make(trunc(_number), FogBuiltInTypeId::natural_type());
			}
			
			else
				if (metaType.is(FogMetaType::signed_type())) {
					if (_number.is_int())
						return make(_number, FogBuiltInTypeId::integer_type());
					else
						return make(trunc(_number), FogBuiltInTypeId::integer_type());
				}
				
				else
					if (metaType.is(FogMetaType::double_type()))
						return make(_number, FogBuiltInTypeId::real_type());
					else
						if (metaType.is(FogMetaType::number_type())) {
//  		ERRMSG("Should not use meta-type number for storage of values.");
							return mutate();
						}
						
						else {
							ERRMSG("BUG unexpected morph_to " << viz(metaType));
							return make_zero();
						}
}

FogNumber& FogNumber::make_false() {
	static const FogAsciiNumberRef falseValue(new FogAsciiNumber(PrimNumber::make_false(),
			FogBuiltInTypeId::logical_type(), *PrimIdHandle("false")), FogAsciiNumberRef::ADOPT);
	return *falseValue;
}

FogNumber& FogNumber::make_integer(long aValue) {
	return make(PrimNumber::make_int(aValue), FogBuiltInTypeId::integer_type());
}

FogNumber& FogNumber::make_logical(bool aValue) {
	return make(PrimNumber::make_bool(aValue), FogBuiltInTypeId::logical_type());
}

FogNumber& FogNumber::make_natural(unsigned long aValue) {
	return make(PrimNumber::make_nat(aValue), FogBuiltInTypeId::natural_type());
}

FogNumber& FogNumber::make_real(double aValue) {
	return make(PrimNumber::make_real(aValue), FogBuiltInTypeId::real_type());
}

FogNumber& FogNumber::make_true() {
	static const FogAsciiNumberRef trueValue(new FogAsciiNumber(PrimNumber::make_true(),
			FogBuiltInTypeId::logical_type(), *PrimIdHandle("true")), FogAsciiNumberRef::ADOPT);
	return *trueValue;
}

FogNumber& FogNumber::make_zero() {
//  	static const FogAsciiNumberRef zeroValue(new FogAsciiNumber(PrimNumber::make_zero(),
//  		FogBuiltInTypeId::logical_type(), *PrimIdHandle("0")), FogAsciiNumberRef::ADOPT);
	static const FogNumberRef zeroValue(make(*PrimIdHandle("0")));
	return *zeroValue;
}

const FogMetaType& FogNumber::meta_type() const {
	return _type.number_type();
}

FogNumber& FogNumber::null() {
	return FogBinaryNumber::mutable_null();
}

FogTokenType::TokenType FogNumber::pp_token_type_enum() const {
	return FogTokenType::NumberLiteral;
}

std::ostream& FogNumber::print_diagnostic(std::ostream& s, int aDepth) const {
	return s << "Number " << viz(*this);
}

char FogNumber::print_named(std::ostream& s, const PrimId *scopeId, char tailChar) const {
	FogStream::space(s, tailChar, '0');
	s << viz(_number);
	return '0';
}

bool FogNumber::resolve_semantics(FogSemanticsContext& theSemantics) const {
	theSemantics.assign(FogSemantics::IS_NUMBER);
	
	if (this == &make_zero())
		theSemantics.set(FogSemantics::IS_ZERO);
		
	return true;
}

FogTokenType::TokenType FogNumber::token_type_enum() const {
	return FogTokenType::NumberLiteral;
}
