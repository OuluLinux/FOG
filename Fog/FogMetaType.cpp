#include <Fog/FogIncludeAll.h>
















TYPEINFO_SINGLE(FogMetaType, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogMetaType)
PRIMREFS_IMPL(FogMetaType)
PRIMLISTOFREFS_IMPL(FogMetaType)
PRIMMAPOFREFS_IMPL(FogMetaType)
TMPL_HACK_FIX_DO(FogMetaType)

#ifdef NEEDS_TYPENAME
	#undef typename
#endif
#ifdef NEEDS_USING
	#undef using
#endif

static FogMetaTypeMapOfRef* FogMetaType_map = 0;

FogMetaType::FogMetaType()
	:
	Super(*PrimIdHandle("-bad-")),
	_is_token(FogTokenType::ERROR),
	_my_bit3(0),
	_my_bit2(0),
	_my_bit1(0),
	_my_bit0(0),
	_is_bits3(0),
	_is_bits2(0),
	_is_bits1(0),
	_is_bits0(0),
	_semantics_checker(0),
	_semantics_value(&FogSemantics::void_value)
{}

inline FogMetaType::FogMetaType(const PrimId& anId, FogTokenType::TokenType isTerminal,
                                SemanticsChecker semanticsChecker, SemanticsValue semanticsValue)
	:
	Super(anId),
	_is_token(isTerminal),
	_my_bit3(next_bit(3, false)),
	_my_bit2(next_bit(2, false)),
	_my_bit1(next_bit(1, false)),
	_my_bit0(next_bit(0, true)),
	_is_bits3(_my_bit3),
	_is_bits2(_my_bit2),
	_is_bits1(_my_bit1),
	_is_bits0(_my_bit0),
	_semantics_checker(semanticsChecker),
	_semantics_value(semanticsValue)
{}

FogMetaType::~FogMetaType() {}

FogMetaEntity* FogMetaType::adopt_meta_entity(FogMetaEntity* metaEntity) {
	_meta_entities.adopt(metaEntity);
	return metaEntity;
}
//
//    Eliminate all references so that destruction is complete and static destruction well behaved.
//
void FogMetaType::destroy() {
	//    Super::destroy();
	_meta_entities.vacate();
}

FogMetaEntity* FogMetaType::find_meta_entity(const PrimId& anId) {
	FogMetaEntity* metaEntity = _meta_entities.find(anId);
	return metaEntity ? metaEntity : Super::find_meta_entity(anId);
}

const FogMetaSlot* FogMetaType::find_meta_slot(const FogToken& aToken, const PrimId& anId) const {
	const FogMetaSlot* aSlot = _slots.find(anId);
	return aSlot;
}

FogMetaType& FogMetaType::get(const PrimId& anId) {
	if (!FogMetaType_map)
		initialise();
		
	FogMetaType* metaType = FogMetaType_map->find(anId);
	
	if (metaType)
		return *metaType;
		
	//    const FogBuiltInTypeId *builtInTypeId = FogBuiltInTypeId::find(anId);
	//    if (builtInTypeId)
	//        return builtInTypeId->number_type();
	//    if (metaTypes.tally())      // No error message during re-entrant call during static construction.
	ERRMSG("There is no meta-type " << c_string(anId.str()));
	return mutable_null();
}

void FogMetaType::initialise() {
	static FogMetaTypeMapOfRef metaTypes;
	FogMetaType_map = &metaTypes;
	
	if (metaTypes.capacity()) {
		ERRMSG("BUG - re-entrant call to FogMetaType::initialise.");
		return;
	}
	
	metaTypes.set_capacity(128);                //   Prevent double execution through re-entrant call.
#ifdef __GNUC__
#define FOGMETATYPE_FLYWEIGHTS_DECL(lower) \
	FogMetaType *name2(lower,_Type) = FogMetaType::make(metaTypes, name2(lower,_id)(), FogTokenType::TreeLiteral, \
	                                  SemanticsChecker(&FogSemantics::name2(is_,lower)), &FogSemantics::name2(lower, _value));
#else
#define FOGMETATYPE_FLYWEIGHTS_DECL(lower) \
	FogMetaType *name2(lower,_Type) = FogMetaType::make(metaTypes, name2(lower,_id)(), FogTokenType::TreeLiteral, \
	                                  &FogSemantics::name2(is_,lower), &FogSemantics::name2(lower, _value));
#endif
	FOGMETATYPE_FLYWEIGHTS_LIST(FOGMETATYPE_FLYWEIGHTS_DECL)
#undef FOGMETATYPE_FLYWEIGHTS_DECL
	//    The meta-type slots are populated after each meta-type has been constructed. A more modular
	//    approach in which each meta-type is configured by the constructor of a derived class fails because
	//    of chicken and egg conflicts between type definition and type reference.
	//
#define FOGMETATYPE_FLYWEIGHTS_DECL(lower) \
	if (name2(lower,_Type)) \
		name2(lower,_Type)->name3(add_,lower,_slots)();
	FOGMETATYPE_FLYWEIGHTS_LIST(FOGMETATYPE_FLYWEIGHTS_DECL)
#undef FOGMETATYPE_FLYWEIGHTS_DECL
	//    Inheritance must be defined from the root up (cheap but checked set_bases implementation). Use of
	//    Y.set_bases(X) indicates that a grammar production of Y may be used wherever an X is required.
	//
	set_bases_of(expression_Type, token_Type);
	set_bases_of(character_Type, expression_Type);
	set_bases_of(string_Type, expression_Type);
	set_bases_of(number_Type, expression_Type);
	set_bases_of(double_Type, number_Type);
	set_bases_of(signed_Type, double_Type);
	set_bases_of(unsigned_Type, signed_Type);
	set_bases_of(bool_Type, unsigned_Type);
	set_bases_of(name_Type, expression_Type);
	set_bases_of(keyword_Type, name_Type);
	set_bases_of(decl_specifier_Type, keyword_Type);
	set_bases_of(cv_qualifier_Type, decl_specifier_Type);
	set_bases_of(identifier_Type, keyword_Type);
	set_bases_of(meta_type_Type, keyword_Type);
	set_bases_of(class_key_Type, meta_type_Type);
	set_bases_of(reserved_Type, keyword_Type);
	set_bases_of(using_directive_Type, name_Type);
	set_synonym(initializer_clause_Type, expression_Type);
	set_bases_of(template_argument_Type, expression_Type);
	set_bases_of(tree_literal_Type, token_Type);
	set_bases_of(declaration_Type, token_Type);
	set_synonym(statement_Type, declaration_Type);
	set_bases_of(expression_statement_Type, statement_Type);
	set_bases_of(entity_Type, token_Type);
	set_bases_of(base_Type, entity_Type);
	set_bases_of(enumerator_Type, entity_Type);
	set_bases_of(namespace_alias_Type, entity_Type);
	set_bases_of(object_Type, entity_Type);
	set_bases_of(exception_Type, object_Type);
	set_bases_of(function_Type, object_Type);
	set_bases_of(meta_function_Type, object_Type);
	set_bases_of(meta_parameter_Type, object_Type);
	set_bases_of(meta_variable_Type, object_Type);
	set_bases_of(parameter_Type, object_Type);
	//                  set_bases_of(template_parameter_Type, object_Type);
	//                      set_bases_of(type_parameter_Type, template_parameter_Type);
	//                      set_bases_of(value_parameter_Type, template_parameter_Type);
	set_bases_of(typedef_Type, object_Type);
	set_bases_of(using_Type, object_Type);
	set_bases_of(variable_Type, object_Type);
	set_bases_of(scope_Type, entity_Type);
	set_bases_of(filespace_Type, scope_Type);
	set_bases_of(linkage_Type, scope_Type);
	set_bases_of(meta_class_Type, scope_Type);
	set_bases_of(namespace_Type, scope_Type);
	set_bases_of(type_Type, scope_Type);
	set_bases_of(built_in_Type, type_Type);
	set_bases_of(class_Type, type_Type);
	set_bases_of(enum_Type, type_Type);
	set_bases_of(struct_Type, type_Type);
	set_bases_of(typename_Type, type_Type);
	set_bases_of(union_Type, type_Type);
	set_bases_of(specifier_Type, declaration_Type);
	set_bases_of(asm_definition_Type, specifier_Type);
	set_bases_of(base_specifier_Type, specifier_Type);
	set_bases_of(enumerator_definition_Type, specifier_Type);
	set_bases_of(file_dependency_specifier_Type, specifier_Type);
	set_bases_of(file_placement_specifier_Type, specifier_Type);
	set_bases_of(namespace_alias_definition_Type, specifier_Type);
	set_bases_of(object_specifier_Type, specifier_Type);
	//                  set_bases_of(exception_declaration_Type, object_specifier_Type);
	set_bases_of(function_specifier_Type, object_specifier_Type);
	set_bases_of(meta_function_specifier_Type, object_specifier_Type);
	set_bases_of(meta_parameter_specifier_Type, object_specifier_Type);
	set_bases_of(meta_variable_specifier_Type, object_specifier_Type);
	set_bases_of(parameter_specifier_Type, object_specifier_Type);
	set_bases_of(template_parameter_specifier_Type, object_specifier_Type);
	set_bases_of(templated_parameter_specifier_Type, template_parameter_specifier_Type);
	set_bases_of(type_parameter_specifier_Type, template_parameter_specifier_Type);
	set_bases_of(value_parameter_specifier_Type, template_parameter_specifier_Type);
	set_bases_of(typedef_specifier_Type, object_specifier_Type);
	set_bases_of(using_declaration_Type, object_specifier_Type);
	set_bases_of(variable_specifier_Type, object_specifier_Type);
	set_bases_of(scope_specifier_Type, specifier_Type);
	set_bases_of(filespace_specifier_Type, scope_specifier_Type);
	set_bases_of(linkage_specification_Type, scope_specifier_Type);
	set_bases_of(meta_class_specifier_Type, scope_specifier_Type);
	set_bases_of(namespace_definition_Type, scope_specifier_Type);
	set_bases_of(type_specifier_Type, scope_specifier_Type);
	set_bases_of(built_in_type_specifier_Type, type_specifier_Type);
	set_bases_of(class_specifier_Type, type_specifier_Type);
	set_bases_of(elaborated_type_specifier_Type, type_specifier_Type);
	set_bases_of(enum_specifier_Type, type_specifier_Type);
	set_bases_of(object_statement_Type, token_Type);
	set_bases_of(exception_specification_Type, token_Type);
	set_bases_of(function_body_Type, token_Type);
	set_bases_of(handler_Type, token_Type);
	set_bases_of(iterator_Type, token_Type);
	set_bases_of(modifier_Type, token_Type);
	set_bases_of(array_modifier_Type, modifier_Type);
	set_bases_of(function_modifier_Type, modifier_Type);
	set_bases_of(pointer_modifier_Type, modifier_Type);
	set_bases_of(reference_modifier_Type, modifier_Type);
	set_bases_of(scoped_modifier_Type, modifier_Type);
	set_bases_of(void_Type, token_Type);
	set_bases_of(punctuation_Type, token_Type);
#ifdef __GNUC__
#define FOGMETATYPE_BUILT_IN_DECL(NAME, PFX, EQUIV) \
	FogMetaType *name2(PFX,_Type) = FogMetaType::make(metaTypes, *PrimIdHandle(NAME), FogTokenType::TreeLiteral, \
	                                SemanticsChecker(&FogSemantics::name2(is_,EQUIV)), &FogSemantics::name2(EQUIV, _value));
#else
#define FOGMETATYPE_BUILT_IN_DECL(NAME, PFX, EQUIV) \
	FogMetaType *name2(PFX,_Type) = FogMetaType::make(metaTypes, *PrimIdHandle(NAME), FogTokenType::TreeLiteral, \
	                                &FogSemantics::name2(is_,EQUIV), &FogSemantics::name2(EQUIV, _value));
#endif
	//    if (!Fog::no_bool_type())
	//        FOGMETATYPE_BUILT_IN_DECL("bool", bool, number)
	FOGMETATYPE_BUILT_IN_DECL("char", char, number)
	FOGMETATYPE_BUILT_IN_DECL("signed char", schar, number)
	FOGMETATYPE_BUILT_IN_DECL("unsigned char", uchar, number)
	//    FOGMETATYPE_BUILT_IN_DECL("double", double, number)
	FOGMETATYPE_BUILT_IN_DECL("long double", ldouble, number)
	FOGMETATYPE_BUILT_IN_DECL("float", float, number)
	FOGMETATYPE_BUILT_IN_DECL("int", int, number)
	FOGMETATYPE_BUILT_IN_DECL("unsigned int", uint, number)
	//    FOGMETATYPE_BUILT_IN_DECL("signed", signed, number)
	//    FOGMETATYPE_BUILT_IN_DECL("unsigned", unsigned, number)
	FOGMETATYPE_BUILT_IN_DECL("long", long, number)
	FOGMETATYPE_BUILT_IN_DECL("unsigned long", ulong, number)
	FOGMETATYPE_BUILT_IN_DECL("short", short, number)
	FOGMETATYPE_BUILT_IN_DECL("unsigned short", ushort, number)
	
	//      FOGMETATYPE_BUILT_IN_DECL("void", void, number)
	if (!Fog::no_wchar_t_type())
		FOGMETATYPE_BUILT_IN_DECL("wchar_t", wchar_t, number)
		if (Fog::long_long_type())
			FOGMETATYPE_BUILT_IN_DECL("long long", llong, number)
#undef FOGMETATYPE_BUILT_IN_DECL
		}

//
//    Register the distinct meta-type names as lexical tokens. (Names such as class that duplicate
//    existing keywords are suppressed, they get resolved by id look-up for a meta-type production).
//
void FogMetaType::install_singletons(FogKeywordMapOfRefToConst& tokenTypes) {
	if (!FogMetaType_map)
		initialise();
		
	for (FogMetaTypeConstMapOfRefToConstIterator p(*FogMetaType_map); p; p++)
		if (!tokenTypes.find(p->id()))
			tokenTypes.add(*p);
}

FogMetaType* FogMetaType::is_meta_type() { return this; }

FogMetaType* FogMetaType::make(FogMetaTypeMapOfRefToConst& metaTypes, const PrimId& anId,
                               FogTokenType::TokenType isTerminal, SemanticsChecker semanticsChecker, SemanticsValue semanticsValue) {
	FogMetaType* metaType = new FogMetaType(anId, isTerminal, semanticsChecker, semanticsValue);
	metaTypes.adopt(metaType);
	return metaType;
}

const FogMetaType& FogMetaType::meta_type() const { return FogMetaType::meta_type_type(); }

#ifndef ULONG_BIT
	#if ULONG_MAX <= UCHAR_MAX
		#define ULONG_BIT CHAR_BIT
	#elif (ULONG_MAX >> CHAR_BIT) <= UCHAR_MAX
		#define ULONG_BIT 2 * CHAR_BIT
	#elif (ULONG_MAX >> (2 * CHAR_BIT)) <= UCHAR_MAX
		#define ULONG_BIT 3 * CHAR_BIT
	#elif (ULONG_MAX >> (3 * CHAR_BIT)) <= UCHAR_MAX
		#define ULONG_BIT 4 * CHAR_BIT
	#else
		#define ULONG_BIT 4 * CHAR_BIT
	#endif
#endif

//
//    Return the bit mask for the next bit in wordNumber.
//
unsigned long FogMetaType::next_bit(size_t wordNumber, bool postIncrement) {
	static size_t nextBit = 0;
	
	if (nextBit >= (BIT_WORDS * ULONG_BIT - 1))
		ERRMSG("BUG - cannot have more than " << (BIT_WORDS * ULONG_BIT - 1) << " meta-types.");
		
	unsigned long minBit = wordNumber * ULONG_BIT;
	unsigned long maxBit = minBit + ULONG_BIT;
	unsigned long theBit = ((nextBit < minBit) || (maxBit <= nextBit)) ? 0uL : (1uL << (nextBit - minBit));
	
	if (postIncrement)
		nextBit++;
		
	return theBit;
}

std::ostream& FogMetaType::print_depth(std::ostream& s, int aDepth) const {
	Super::print_depth(s, aDepth);
	
	if (_slots.tally()) {
		s << indent(aDepth) << "slots\n";
		_slots.print_depth(s, aDepth + 1);
	}
	
	return s;
}

std::ostream& FogMetaType::print_members(std::ostream& s, int aDepth) const {
	Super::print_members(s, aDepth);
	
	if (_slots.tally()) {
		s << indent(aDepth) << "slots\n";
		_slots.print_members(s, aDepth + 1);
	}
	
	return s;
}

std::ostream& FogMetaType::print_viz(std::ostream& s) const {
	s << "\"meta-type: ";
	print_this(s);
	return s << '\"';
}

bool FogMetaType::resolve_semantics(FogSemanticsContext& theSemantics) const {
	if (!Super::resolve_semantics(theSemantics))
		return false;
		
	theSemantics.set(FogSemantics::IS_META_TYPE_NAME);
	return true;
}

bool FogMetaType::satisfied_by(const FogToken& aToken, FogScopeContext& inScope) const {
	if (_semantics_checker == 0)
		return false;
		
	FogStrongSemanticsContext theSemantics(inScope);
	
	if (!aToken.resolve_semantics(theSemantics))
		return false;
		
	return (theSemantics.*_semantics_checker)();
}

void FogMetaType::set_bases_of(FogMetaType* thisType, const FogMetaType* firstBaseType, const FogMetaType* secondBaseType) {
	if (!thisType)
		return;
		
	if (firstBaseType) {
		if (!firstBaseType->is(token_type()))
			ERRMSG("BUG - out of order set_bases_of(" << viz(*thisType) << ", " << viz(*firstBaseType) << ").");
			
		thisType->_is_bits3 |= firstBaseType->_is_bits3;
		thisType->_is_bits2 |= firstBaseType->_is_bits2;
		thisType->_is_bits1 |= firstBaseType->_is_bits1;
		thisType->_is_bits0 |= firstBaseType->_is_bits0;
		thisType->_slots |= firstBaseType->_slots;
	}
	
	if (secondBaseType) {
		if (!secondBaseType->is(token_type()))
			ERRMSG("BUG - out of order set_bases_of(" << viz(*thisType) << ", " << viz(*secondBaseType) << ").");
			
		thisType->_is_bits3 |= secondBaseType->_is_bits3;
		thisType->_is_bits2 |= secondBaseType->_is_bits2;
		thisType->_is_bits1 |= secondBaseType->_is_bits1;
		thisType->_is_bits0 |= secondBaseType->_is_bits0;
		thisType->_slots |= secondBaseType->_slots;
	}
}

void FogMetaType::set_synonym(FogMetaType* thisType, FogMetaType* thatType) {
	thisType->_is_bits3 |= thatType->_is_bits3;
	thisType->_is_bits2 |= thatType->_is_bits2;
	thisType->_is_bits1 |= thatType->_is_bits1;
	thisType->_is_bits0 |= thatType->_is_bits0;
	thisType->_slots |= thatType->_slots;
	thatType->_is_bits3 |= thisType->_is_bits3;
	thatType->_is_bits2 |= thisType->_is_bits2;
	thatType->_is_bits1 |= thisType->_is_bits1;
	thatType->_is_bits0 |= thisType->_is_bits0;
	thatType->_slots |= thisType->_slots;
}

FogTokenType::TokenType FogMetaType::token_type_enum() const { return FogTokenType::MetaType; }

#define FOGMETATYPE_FLYWEIGHTS_IMPL(lower) \
	const PrimId& FogMetaType::name2(lower,_id)() \
	{ \
		static const PrimIdHandle anId(#lower); \
		return *anId; \
	} \
	FogMetaType& FogMetaType::name2(lower,_type)() \
	{ \
		static FogMetaType& aType = get(name2(lower,_id)()); \
		return aType; \
	}
FOGMETATYPE_FLYWEIGHTS_LIST(FOGMETATYPE_FLYWEIGHTS_IMPL)
#undef FOGMETATYPE_FLYWEIGHTS_IMPL

#ifdef NEEDS_TYPENAME
	#define typename
#endif
#ifdef NEEDS_USING
	#define using
#endif
