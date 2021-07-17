#include <Fog/FogIncludeAll.h>












class FogRealType : public FogBuiltInTypeId {
	typedef FogBuiltInTypeId Super;
	typedef FogRealType This;
	TYPEDECL_SINGLE(This, Super)
public:
	FogRealType(const PrimId& anId, bool isSimple) : Super(anId, isSimple) {}
	virtual IsType is_type() const { return IS_REAL; }
	virtual FogMetaType& number_type() const { return FogMetaType::double_type(); }
	virtual TypeNumber type_number() const { return REAL; }
public:
	virtual const PrimNumber& add(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const
	{ return firstNumber + secondNumber; }
	virtual const PrimNumber& div(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const
	{ return firstNumber / secondNumber; }
	virtual bool eq(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const
	{ return firstNumber == secondNumber; }
	virtual bool ge(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const
	{ return firstNumber >= secondNumber; }
	virtual bool gt(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const
	{ return firstNumber > secondNumber; }
	virtual bool le(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const
	{ return firstNumber <= secondNumber; }
	virtual bool log_and_(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const
	{ return firstNumber && secondNumber; }
	virtual bool log_neg(const PrimNumber& firstNumber) const
	{ return !firstNumber; }
	virtual bool log_or(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const
	{ return firstNumber || secondNumber; }
	virtual bool lt(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const
	{ return firstNumber < secondNumber; }
	virtual const PrimNumber& mul(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const
	{ return firstNumber * secondNumber; }
	virtual bool ne(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const
	{ return firstNumber != secondNumber; }
	virtual const PrimNumber& neg(const PrimNumber& firstNumber) const
	{ return -firstNumber; }
	virtual const PrimNumber& sub(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const
	{ return firstNumber - secondNumber; }
};

class FogIntegerType : public FogRealType {
	typedef FogRealType Super;
	typedef FogIntegerType This;
	TYPEDECL_SINGLE(This, Super)
public:
	FogIntegerType(const PrimId& anId, bool isSimple) : Super(anId, isSimple) {}
	virtual IsType is_type() const { return IS_INTEGER; }
	virtual FogMetaType& number_type() const { return FogMetaType::signed_type(); }
	virtual TypeNumber type_number() const { return INTEGER; }
public:
	virtual const PrimNumber& and_(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const
	{ return firstNumber & secondNumber; }
	virtual const PrimNumber& div(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const
	{ return idiv(firstNumber, secondNumber); }
	virtual const PrimNumber& mod(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const
	{ return firstNumber % secondNumber; }
	virtual const PrimNumber& or_(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const
	{ return firstNumber | secondNumber; }
	virtual const PrimNumber& shl(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const
	{ return firstNumber << secondNumber; }
	virtual const PrimNumber& shr(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const
	{ return firstNumber >> secondNumber; }
	virtual const PrimNumber& xor_(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const
	{ return firstNumber ^ secondNumber; }
};

class FogDefaultType : public FogIntegerType {
	typedef FogIntegerType Super;
	typedef FogDefaultType This;
	TYPEDECL_SINGLE(This, Super)
public:
	FogDefaultType(const PrimId& anId, bool isSimple) : Super(anId, isSimple) {}
	virtual IsType is_type() const { return IS_DEFAULT; }
public:
};

class FogNaturalType : public FogIntegerType {
	typedef FogIntegerType Super;
	typedef FogNaturalType This;
	TYPEDECL_SINGLE(This, Super)
public:
	FogNaturalType(const PrimId& anId, bool isSimple) : Super(anId, isSimple) {}
	virtual IsType is_type() const { return IS_NATURAL; }
	virtual FogMetaType& number_type() const { return FogMetaType::unsigned_type(); }
	virtual TypeNumber type_number() const { return NATURAL; }
public:
	virtual const PrimNumber& com(const PrimNumber& firstNumber) const
	{ return ~firstNumber; }
};

class FogLogicalType : public FogNaturalType {
	typedef FogNaturalType Super;
	typedef FogLogicalType This;
	TYPEDECL_SINGLE(This, Super)
public:
	FogLogicalType(const PrimId& anId, bool isSimple) : Super(anId, isSimple) {}
	virtual IsType is_type() const { return IS_LOGICAL; }
	virtual FogMetaType& number_type() const { return FogMetaType::bool_type(); }
	virtual TypeNumber type_number() const { return LOGICAL; }
public:
};

TYPEINFO_SINGLE(FogBuiltInTypeId, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogBuiltInTypeId)
PRIMREFS_IMPL(FogBuiltInTypeId)
PRIMMAPOFREFS_IMPL(FogBuiltInTypeId)
TMPL_HACK_FIX_DO(FogBuiltInTypeId)

TMPL_HACK_FIX_TEMPLATE_EXPLICIT(FogBuiltInTypeIdSetOfRefElement)
PRIMSETELEMENT_IMPL(FogBuiltInTypeIdSetOfRefElement)
PRIMMAPOFREFS_IMPL(FogBuiltInTypeIdSetOfRefElement)
TMPL_HACK_FIX_DO_TEMPLATE(FogBuiltInTypeIdSetOfRefElement)

TYPEINFO_SINGLE(FogDefaultType, Super)
TYPEINFO_SINGLE(FogIntegerType, Super)
TYPEINFO_SINGLE(FogLogicalType, Super)
TYPEINFO_SINGLE(FogNaturalType, Super)
TYPEINFO_SINGLE(FogRealType, Super)

FogBuiltInTypeId::FogBuiltInTypeId()
	:
	_is_simple(false)
{}

FogBuiltInTypeId::FogBuiltInTypeId(const PrimId& anId, bool isSimple)
	:
	Super(anId),
	_is_simple(isSimple)
{}

const PrimNumber& FogBuiltInTypeId::add(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const {
	ERRMSG("BUG - should not add() for " << viz(*this));
	return PrimNumber::null();
}

const PrimNumber& FogBuiltInTypeId::and_(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const {
	ERRMSG("BUG - should not and_() for " << viz(*this));
	return PrimNumber::null();
}

const PrimNumber& FogBuiltInTypeId::com(const PrimNumber& firstNumber) const {
	ERRMSG("BUG - should not com() for " << viz(*this));
	return PrimNumber::null();
}

const PrimNumber& FogBuiltInTypeId::div(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const {
	ERRMSG("BUG - should not div() for " << viz(*this));
	return PrimNumber::null();
}

bool FogBuiltInTypeId::eq(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const {
	ERRMSG("BUG - should not eq() for " << viz(*this));
	return false;
}

const FogBuiltInTypeId* FogBuiltInTypeId::find(const PrimId& anId) {
	const FogBuiltInTypeId* typeId = ids().find(anId);
	return typeId;
}

bool FogBuiltInTypeId::ge(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const {
	ERRMSG("BUG - should not ge() for " << viz(*this));
	return false;
}

FogBuiltInTypeId& FogBuiltInTypeId::get(const PrimId& anId) {
	FogBuiltInTypeId* typeId = ids().find(anId);
	
	if (!typeId)
		ERRMSG("BUG - should not use undefined type " << anId);
		
	return typeId ? *typeId : FogBuiltInTypeId::mutable_null();
}

bool FogBuiltInTypeId::gt(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const {
	ERRMSG("BUG - should not gt() for " << viz(*this));
	return false;
}

FogBuiltInTypeIdMapOfRef& FogBuiltInTypeId::ids() {
	static FogBuiltInTypeIdMapOfRef idMap(128);
	return idMap;
}

void FogBuiltInTypeId::install_singletons(FogKeywordMapOfRefToConst& tokenTypes) {
	tokenTypes.add(logical_type());
	tokenTypes.add(make("char", IS_DEFAULT, true));
	tokenTypes.add(real_type());
	tokenTypes.add(make("float", IS_REAL, true));
	tokenTypes.add(make("int", IS_DEFAULT, true));
	tokenTypes.add(make("long", IS_DEFAULT, true));
	tokenTypes.add(make("short", IS_DEFAULT, true));
	tokenTypes.add(integer_type());
	tokenTypes.add(natural_type());
	tokenTypes.add(void_type());
	tokenTypes.add(make("wchar_t", IS_NATURAL, true));
}

FogBuiltInTypeId& FogBuiltInTypeId::integer_type() {
	static FogBuiltInTypeId& theType = make("signed", IS_INTEGER, true);
	return theType;
}

//  const FogBuiltInTypeId *FogBuiltInTypeId::is_built_in() const { return this; }
FogBuiltInTypeId::IsType FogBuiltInTypeId::is_type() const { return IS_VOID; }

bool FogBuiltInTypeId::le(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const {
	ERRMSG("BUG - should not le() for " << viz(*this));
	return false;
}

bool FogBuiltInTypeId::log_and_(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const {
	ERRMSG("BUG - should not log_and_() for " << viz(*this));
	return false;
}

bool FogBuiltInTypeId::log_neg(const PrimNumber& firstNumber) const {
	ERRMSG("BUG - should not log_neg() for " << viz(*this));
	return false;
}

bool FogBuiltInTypeId::log_or(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const {
	ERRMSG("BUG - should not log_or() for " << viz(*this));
	return false;
}

FogBuiltInTypeId& FogBuiltInTypeId::logical_type() {
	static FogBuiltInTypeId& theType = make("bool", IS_LOGICAL, true);
	return theType;
}

bool FogBuiltInTypeId::lt(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const {
	ERRMSG("BUG - should not lt() for " << viz(*this));
	return false;
}

//
//    Return the singleton built-in type-id capturing the defining characteristics.
//
FogBuiltInTypeId& FogBuiltInTypeId::make(const char* anId, IsType isType, bool isSimple) {
	return make(*PrimIdHandle(anId), isType, isSimple);
}

//
//    Return the singleton built-in type-id capturing the defining characteristics.
//
FogBuiltInTypeId& FogBuiltInTypeId::make(const PrimId& anId, IsType isType, bool isSimple) {
	FogBuiltInTypeIdMapOfRef& idMap = ids();
	FogBuiltInTypeId* typeId = idMap.find(anId);
	
	if (typeId) {
		if (typeId->is_type() != isType)
			ERRMSG("BUG - should not use FogBuiltInTypeId::make to find a mismatched type " << anId);
	}
	else {
		if (isType & IS_VOID)
			typeId = new FogBuiltInTypeId(anId, isSimple);
		//        else if (isType & IS_COMPLEX)
		//            typeId = new FogComplexType(anId, isSimple);
		else if (isType & IS_REAL)
			typeId = new FogRealType(anId, isSimple);
		else if (isType & IS_INTEGER)
			typeId = new FogIntegerType(anId, isSimple);
		else if (isType & IS_NATURAL)
			typeId = new FogNaturalType(anId, isSimple);
		else if (isType & IS_LOGICAL)
			typeId = new FogLogicalType(anId, isSimple);
		else
			typeId = new FogDefaultType(anId, isSimple);
			
		if (typeId)
			idMap.adopt(*typeId);
	}
	
	return typeId ? *typeId : FogBuiltInTypeId::mutable_null();
}

FogBuiltInTypeId* FogBuiltInTypeId::make(const FogBuiltInTypeId& typeId) const {
	FogBuiltInTypeIdSetOfRefElement* anElement = _children.find(typeId.id());
	
	if (!anElement) {
		if (!_children.tally())
			mutate()._children.set_capacity(16);
			
		PrimOstrstream s;
		s << id() << ' ' << typeId.id();
		IsType oldIsType = is_type();
		IsType newIsType = typeId.is_type();
		
		if ((oldIsType != IS_DEFAULT) && (newIsType != IS_DEFAULT) && (oldIsType != newIsType))
			ERRMSG("BUG -- illegal IsType combinations for " << viz(*this) << " and " << viz(typeId));
			
		anElement = new FogBuiltInTypeIdSetOfRefElement(typeId.id(), make(s.str(), IsType(oldIsType | newIsType), false));
		mutate()._children.adopt(anElement);
	}
	
	return anElement ? anElement->value().pointer() : 0;
}

void FogBuiltInTypeId::make_built_in(FogNameRef& aName, const FogBuiltInTypeId& typeId) {
	aName = make(typeId);
}

const FogMetaType& FogBuiltInTypeId::meta_type() const { return FogMetaType::built_in_type_specifier_type(); }

const PrimNumber& FogBuiltInTypeId::mod(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const {
	ERRMSG("BUG - should not mod() for " << viz(*this));
	return PrimNumber::null();
}

const PrimNumber& FogBuiltInTypeId::mul(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const {
	ERRMSG("BUG - should not mul() for " << viz(*this));
	return PrimNumber::null();
}

FogBuiltInTypeId& FogBuiltInTypeId::natural_type() {
	static FogBuiltInTypeId& theType = make("unsigned", IS_NATURAL, true);
	return theType;
}

bool FogBuiltInTypeId::ne(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const {
	ERRMSG("BUG - should not ne() for " << viz(*this));
	return false;
}

const PrimNumber& FogBuiltInTypeId::neg(const PrimNumber& firstNumber) const {
	ERRMSG("BUG - should not neg() for " << viz(*this));
	return PrimNumber::null();
}

FogMetaType& FogBuiltInTypeId::number_type() const { return FogMetaType::void_type(); }

const PrimNumber& FogBuiltInTypeId::or_(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const {
	ERRMSG("BUG - should not or() for " << viz(*this));
	return PrimNumber::null();
}

FogTokenType::TokenType FogBuiltInTypeId::pp_token_type_enum() const { return FogTokenType::Identifier; }

std::ostream& FogBuiltInTypeId::print_viz(std::ostream& s) const {
	return s << "\"built-in-specifier: " << id() << '\"';
}

FogBuiltInTypeId& FogBuiltInTypeId::real_type() {
	static FogBuiltInTypeId& theType = make("double", IS_REAL, true);
	return theType;
}

bool FogBuiltInTypeId::resolve_semantics(FogSemanticsContext& theSemantics) const {
	theSemantics.assign(_is_simple ? FogSemantics::IS_SIMPLE_TYPE_SPECIFIER : FogSemantics::IS_TYPE_SPECIFIER_SEQ);
	theSemantics.set(FogSemantics::IS_META_TYPE_NAME);
	return true;
}

const PrimNumber& FogBuiltInTypeId::shl(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const {
	ERRMSG("BUG - should not shl() for " << viz(*this));
	return PrimNumber::null();
}

const PrimNumber& FogBuiltInTypeId::shr(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const {
	ERRMSG("BUG - should not shr() for " << viz(*this));
	return PrimNumber::null();
}

const PrimNumber& FogBuiltInTypeId::sub(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const {
	ERRMSG("BUG - should not sub() for " << viz(*this));
	return PrimNumber::null();
}

FogTokenType::TokenType FogBuiltInTypeId::token_type_enum() const { return FogTokenType::BuiltInTypeSpecifier; }

FogBuiltInTypeId::TypeNumber FogBuiltInTypeId::type_number() const { return VOID; }

FogBuiltInTypeId& FogBuiltInTypeId::void_type() {
	static FogBuiltInTypeId& theType = make("void", IS_VOID, true);
	return theType;
}

const PrimNumber& FogBuiltInTypeId::xor_(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const {
	ERRMSG("BUG - should not xor() for " << viz(*this));
	return PrimNumber::null();
}

const FogBuiltInTypeId& max(const FogBuiltInTypeId& firstType, const FogBuiltInTypeId& secondType) {
	FogBuiltInTypeId::TypeNumber firstTypeNumber = firstType.type_number();
	FogBuiltInTypeId::TypeNumber secondTypeNumber = secondType.type_number();
	return firstTypeNumber >= secondTypeNumber ? firstType : secondType;
}
