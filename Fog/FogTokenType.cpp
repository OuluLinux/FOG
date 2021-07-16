#include <Fog/FogIncludeAll.h>

























static FogKeywordMapOfRef *FogTokenType_id_map = 0;
static FogToken *FogTokenType_id_table[FogTokenType::REDUCE_HERE+1] = { 0 };

void FogTokenType::add_built_in(const PrimId& anId)
{
 if (!FogTokenType_id_map)
  initialise();
 FogTokenType_id_map->add(FogBuiltInTypeId::get(anId));  //  .bugbug prehistoric policy
}

FogToken& FogTokenType::error()
{
 static FogToken& errorToken = get(FogTokenType::ERROR);
 return errorToken;
}

FogToken& FogTokenType::get(TokenType tokenType)
{
 if (!FogTokenType_id_map)
  initialise();
 FogToken *aToken = FogTokenType_id_table[tokenType];
 if (aToken)
  return *aToken;
 ERRMSG("BUG - should not FogTokenType::find for " << tokenType);
 return FogToken::mutable_null();
}

#define FOGTOKENTYPE_GET_TYPED_TOKEN(T,N) \
T& FogTokenType::name2(get_,N)(TokenType tokenType) \
{ \
 FogToken& theToken = get(tokenType); \
 T *aToken = ptr_cast(T, &theToken); \
 if (aToken) \
  return *aToken; \
 ERRMSG("BUG - FogTokenType::" #N " failed to find token."); \
 return T::name2(invalid_,N)(); \
}

FOGTOKENTYPE_GET_TYPED_TOKEN(FogAccess, access)
FOGTOKENTYPE_GET_TYPED_TOKEN(FogCv, cv)
FOGTOKENTYPE_GET_TYPED_TOKEN(FogDeclSpecifier, decl_specifier)
FOGTOKENTYPE_GET_TYPED_TOKEN(FogInline, inline)
FOGTOKENTYPE_GET_TYPED_TOKEN(FogSegment, segment)
FOGTOKENTYPE_GET_TYPED_TOKEN(FogStatic, static)
FOGTOKENTYPE_GET_TYPED_TOKEN(FogTag, tag)
FOGTOKENTYPE_GET_TYPED_TOKEN(FogUtility, utility)
FOGTOKENTYPE_GET_TYPED_TOKEN(FogVirtual, virtual)

struct FogTokenType_IdName
{
 const char *_name;
 FogTokenType::TokenType _table_enum;  //   Index for storage in FogTokenType_id_table
 FogTokenType::TokenType _pp_enum;   //   Token type for preprocessor level parsing.
 FogTokenType::TokenType _enum;    //   Token type for normal level parsing.
};

void FogTokenType::initialise()
{
//  	Internal tokens must not have a valid source text identifier spelling.
//  
 static FogKeywordMapOfRef idMap(1024);
 FogTokenType_id_map = &idMap;

 static const FogTokenType_IdName punctuationNames[] =
 {
  { "# ", HASH_BLANK, HASH_BLANK, ERROR },        //   Extra space to avoid clash.
  { "#define", HASH_DEFINE, HASH_DEFINE, ERROR },
  { "#elif", HASH_ELIF, HASH_ELIF, ERROR },
  { "#else", HASH_ELSE, HASH_ELSE, ERROR },
  { "#endif", HASH_ENDIF, HASH_ENDIF, ERROR },
  { "#error", HASH_ERROR, HASH_ERROR, ERROR },
  { "#if", HASH_IF, HASH_IF, ERROR },
  { "#ifdef", HASH_IFDEF, HASH_IFDEF, ERROR },
  { "#ifndef", HASH_IFNDEF, HASH_IFNDEF, ERROR },
  { "#include", HASH_INCLUDE, HASH_INCLUDE, ERROR },
  { "#line", HASH_LINE, HASH_LINE, ERROR },
  { "#pragma", HASH_PRAGMA, HASH_PRAGMA, ERROR },
  { "#undef", HASH_UNDEF, HASH_UNDEF, ERROR },
        //  
  { "##", HASH_HASH, HASH_HASH, ERROR },
  { "%:%:", DI_HASH_HASH, HASH_HASH, ERROR },
  { "%:", DI_HASH, character('#'), ERROR },
  { "<:", DI_SQUARE, character('['), character('[') },
  { ":>", DI_ERAUQS, character(']'), character(']') },
  { "%>", DI_ECARB, character('}'), character('}') },
        //  
  { "-DEFINED-", DEFINED, DEFINED, ERROR },
  { "-DO_FUNCTION-", DO_FUNCTION, DO_FUNCTION, ERROR },
        //  
//  		{ "-BANG-", BANG, BANG, BANG },
  { "-ERROR-", ERROR, ERROR, ERROR },
  { "-NIL-", NIL, NIL, NIL },
//  		{ "-RAMMARG-", RAMMARG, ERROR, RAMMARG },
        //  
  { "...", ELLIPSIS, Punct, ELLIPSIS  },
  { "::", SCOPE, Punct, SCOPE },
  { "<<", SHL, SHL, SHL },
  { ">>", SHR, SHR, SHR },
  { "==", EQ, EQ, EQ },
  { "!=", NE, NE, NE },
  { "<=", LE, LE, LE },
  { ">=", GE, GE, GE },
  { "&&", LOG_AND, LOG_AND, LOG_AND },
  { "||", LOG_OR, LOG_OR, LOG_OR },
  { "++", INC, Punct, INC },
  { "--", DEC, Punct, DEC },
  { "->", ARROW, Punct, ARROW },
  { "->*", ARROW_STAR, Punct, ARROW_STAR },
  { ".*", DOT_STAR, Punct, DOT_STAR },
  { "+=", ASS_ADD, Punct, ASS_ADD },
  { "&=", ASS_AND, Punct, ASS_AND },
  { "/=", ASS_DIV, Punct, ASS_DIV },
  { "%=", ASS_MOD, Punct, ASS_MOD },
  { "*=", ASS_MUL, Punct, ASS_MUL },
  { "|=", ASS_OR, Punct, ASS_OR },
  { "<<=", ASS_SHL, Punct, ASS_SHL },
  { ">>=", ASS_SHR, Punct, ASS_SHR },
  { "-=", ASS_SUB, Punct, ASS_SUB },
  { "^=", ASS_XOR, Punct, ASS_XOR },
        //  
  { 0, TokenType(0), TokenType(0), TokenType(0) }
 };

 static const FogTokenType_IdName keywordNames[] =
 {
  { "asm", ASM, Identifier, ASM },
  { "break", BREAK, Identifier, BREAK },
  { "case", CASE, Identifier, CASE },
  { "catch", CATCH, Identifier, CATCH },
  { "const_cast", CONST_CAST, Identifier, CONST_CAST },
  { "continue", CONTINUE, Identifier, CONTINUE },
  { "default", DEFAULT, Identifier, DEFAULT },
  { "delete", DELETE, Identifier, DELETE },
  { "do", DO, Identifier, DO },
  { "dynamic_cast", DYNAMIC_CAST, Identifier, DYNAMIC_CAST },
  { "else", ELSE, Identifier, ELSE },
  { "false", FALSE, Identifier, FALSE },
  { "for", FOR, Identifier, FOR },
  { "goto", GOTO, Identifier, GOTO },
  { "if", IF, Identifier, IF },
  { "new", NEW, Identifier, NEW },
  { "operator", OPERATOR, Identifier, OPERATOR },
  { "reinterpret_cast", REINTERPRET_CAST, Identifier, REINTERPRET_CAST },
  { "return", RETURN, Identifier, RETURN },
  { "sizeof", SIZEOF, Identifier, SIZEOF },
  { "static_cast", STATIC_CAST, Identifier, STATIC_CAST },
  { "switch", SWITCH, Identifier, SWITCH },
  { "this", THIS, Identifier, THIS },
  { "throw", THROW, Identifier, THROW },
  { "true", TRUE, Identifier, TRUE },
  { "try", TRY, Identifier, TRY },
  { "typeid", TYPEID, Identifier, TYPEID },
  { "while", WHILE, Identifier, WHILE },
        //  
  { "and", LOG_AND, LOG_AND, LOG_AND },
  { "and_eq", ASS_AND, Punct, ASS_AND },
  { "not_eq", NE, NE, NE },
  { "or", LOG_OR, LOG_OR, LOG_OR },
  { "or_eq", ASS_OR, Punct, ASS_OR },
  { "xor_eq", ASS_XOR, Punct, ASS_XOR },
        //  
  { "bitand", character('&'), character('&'), character('&') },
  { "bitor", character('|'), character('|'), character('|') },
  { "compl", character('~'), character('~'), character('~') },
  { "not", character('!'), character('!'), character('!') },
  { "xor", character('^'), character('^'), character('^') },
        //  
  { 0, TokenType(0), TokenType(0), TokenType(0) }
 };

 static const FogTokenType_IdName nonReservedWordNames[] =
 {
  { "derived", DERIVED, TokenType(0), TokenType(0) },
  { "file", FILE, TokenType(0), TokenType(0) },
  { "guard", GUARD, TokenType(0), TokenType(0) },
  { "all", ALL, TokenType(0), TokenType(0) },
  { "implementation", IMPLEMENTATION, TokenType(0), TokenType(0) },
  { "include", INCLUDE, TokenType(0), TokenType(0) },
  { "interface", INTERFACE, TokenType(0), TokenType(0) },
  { "noguard", NOGUARD, TokenType(0), NOGUARD },
  { "noimplementation", NOIMPLEMENTATION, TokenType(0), TokenType(0) },
  { "path", PATH, TokenType(0), TokenType(0) },
  { "prefix", PREFIX, TokenType(0), TokenType(0) },
  { "pure", PURE, TokenType(0), TokenType(0) },
  { "suffix", SUFFIX, TokenType(0), TokenType(0) },
  { 0, TokenType(0), TokenType(0), TokenType(0) }
 };

 static const FogAccess_Initializer accessNames[] =
 {
  { "auto", AUTO, Identifier, AUTO, FogDeclSpecifier::AUTO_MASK, FogAccess::AUTO },
  { "private", PRIVATE, Identifier, PRIVATE, FogDeclSpecifier::PRIVATE_MASK, FogAccess::PRIVATE },
  { "protected", PROTECTED, Identifier, PROTECTED, FogDeclSpecifier::PROTECTED_MASK, FogAccess::PROTECTED },
  { "public", PUBLIC, Identifier, PUBLIC, FogDeclSpecifier::PUBLIC_MASK, FogAccess::PUBLIC },
  { 0, TokenType(0), TokenType(0), TokenType(0), FogDeclSpecifier::INVALID_MASK, FogAccess::INVALID }
 };

 static const FogCv_Initializer cvNames[] =
 {
  { "const", CONST, Identifier, CONST, FogDeclSpecifier::CONST_MASK, FogCv::CONST },
  { "volatile", VOLATILE, Identifier, VOLATILE, FogDeclSpecifier::VOLATILE_MASK, FogCv::VOLATILE },
  { 0, TokenType(0), TokenType(0), TokenType(0), FogDeclSpecifier::INVALID_MASK, FogCv::INVALID }
 };

 static const FogDeclSpecifier_Initializer declSpecifierNames[] =
 {
  { "explicit", EXPLICIT, Identifier, EXPLICIT, FogDeclSpecifier::EXPLICIT_MASK },
  { "export", EXPORT, Identifier, EXPORT, FogDeclSpecifier::EXPORT_MASK },
  { "extern", EXTERN, Identifier, EXTERN, FogDeclSpecifier::EXTERN_MASK },
  { "friend", FRIEND, Identifier, FRIEND, FogDeclSpecifier::FRIEND_MASK },
  { "mutable", MUTABLE, Identifier, MUTABLE, FogDeclSpecifier::MUTABLE_MASK },
  { "register", REGISTER, Identifier, REGISTER, FogDeclSpecifier::REGISTER_MASK },
  { "template", TEMPLATE, Identifier, TEMPLATE, FogDeclSpecifier::TEMPLATE_MASK },
  { "typedef", TYPEDEF, Identifier, TYPEDEF, FogDeclSpecifier::TYPEDEF_MASK },
  { "using", USING, Identifier, USING, FogDeclSpecifier::USING_MASK },
  { 0, TokenType(0), TokenType(0), TokenType(0), FogDeclSpecifier::INVALID_MASK }
 };

 static const FogInline_Initializer inlineNames[] =
 {
  { "inline", INLINE, Identifier, INLINE, FogDeclSpecifier::INLINE_IF_SHORT_MASK, FogInline::EXPLICIT_IF_SHORT },
  { 0, TokenType(0), TokenType(0), TokenType(0), FogDeclSpecifier::INVALID_MASK, FogInline::INVALID }
 };

 static const FogSegment_Initializer segmentNames[] =
 {
  { "entry", ENTRY, FogSegment::ENTRY },
  { "pre", PRE, FogSegment::PRE },
//  		{ "construct", CONSTRUCT, FogSegment::CONSTRUCT },
  { "body", BODY, FogSegment::BODY },
//  		{ "destruct", DESTRUCT, FogSegment::DESTRUCT },
  { "post", POST, FogSegment::POST },
  { "exit", EXIT, FogSegment::EXIT },
  { 0, TokenType(0), FogSegment::INVALID }
 };

 static const FogStatic_Initializer staticNames[] =
 {
  { "static", STATIC, Identifier, STATIC, FogDeclSpecifier::STATIC_MASK, FogStatic::STATIC },
  { 0, TokenType(0), TokenType(0), TokenType(0), FogDeclSpecifier::INVALID_MASK, FogStatic::INVALID }
 };

 static const FogTag_Initializer tagNames[] =
 {
  { "class", CLASS, Identifier, CLASS, &FogMetaType::class_type,
    &FogMetaType::class_specifier_type, FogTag::CLASS },
  { "enum", ENUM, Identifier, ENUM, &FogMetaType::enum_type,
    &FogMetaType::enum_specifier_type, FogTag::ENUM },
  { "namespace", NAMESPACE, Identifier, NAMESPACE, &FogMetaType::namespace_type,
    &FogMetaType::namespace_definition_type, FogTag::NAMESPACE },
  { "struct", STRUCT, Identifier, STRUCT, &FogMetaType::struct_type,
    &FogMetaType::class_specifier_type, FogTag::STRUCT },
  { "typename", TYPENAME, Identifier, TYPENAME, &FogMetaType::typename_type,
    &FogMetaType::type_specifier_type, FogTag::TYPENAME },
  { "union", UNION, Identifier, UNION, &FogMetaType::union_type,
    &FogMetaType::class_specifier_type, FogTag::UNION },
  { 0, TokenType(0), TokenType(0), TokenType(0), 0, 0, FogTag::INVALID }
 };

 static const FogUtility_Initializer utilityNames[] =
 {
  { "emit", EMIT, FogUtility::EMIT },
  { "frozen", FROZEN, FogUtility::FROZEN },
  { "pool", POOL, FogUtility::POOL },
  { "utility", UTILITY, FogUtility::UTILITY },
  { 0, TokenType(0), FogUtility::INVALID }
 };

 static const FogVirtual_Initializer virtualNames[] =
	{
		{ "virtual", VIRTUAL, Identifier, VIRTUAL, FogDeclSpecifier::VIRTUAL_MASK, FogVirtual::VIRTUAL },
		{ 0, TokenType(0), TokenType(0), TokenType(0), FogDeclSpecifier::INVALID_MASK, FogVirtual::INVALID }
	};

//
//	Install special tokens first, to occlude subsequent regular definitions.
//
//	FogTokenType_id_table[0] = &FogToken::mutable_null();
	FogTokenType_id_table['0'] = &FogNumber::make_zero();
	FogTokenType_id_table[' '] = &make_spacing(1);
	FogTokenType_id_table['{'] = &make_open_brace(0);
//
//	Fill up the gaps.
//
	for (size_t i = 0; i <= UCHAR_MAX; i++)
	{
		if (FogTokenType_id_table[i])						// Already a special case
			;
		else if (isalnum(i) || (i == '_'))					// Trivial identifier or number
			;
		else if ((i == '\'') || (i == '"'))					// Not valid pre-processor tokens
			;
		else if (i == '\\')									// An escape sequence, already a literal.
			;
		else if (isprint(i))								// Boring character such as =
		{
			char buf[2];
			buf[0] = i;
			buf[1] = 0;
			FogKeyword *keywordId = 0;
			if (isalpha(i)) 
				keywordId = new FogReservedId(buf, FogTokenType::character(i), FogTokenType::character(i));
			else
				keywordId = new FogPunctuation(buf, FogTokenType::character(i), FogTokenType::character(i));
			idMap.adopt(keywordId);
			FogTokenType_id_table[i] = keywordId;
		}
		else												// Unprintable character such as \n
		{
			char c = char(i);
			FogTokenType_id_table[i] = &FogCharacter::make(&c, 1, FogCharacter::LITERAL);
		}
	}

//	Install punctuation, dodging special preloaded values.
//
	for (const FogTokenType_IdName *pPunct = punctuationNames; pPunct->_name; ++pPunct)
	{
		FogKeyword *punctuationId = new FogPunctuation(pPunct->_name, pPunct->_pp_enum, pPunct->_enum);
		idMap.adopt(punctuationId);
		if (!FogTokenType_id_table[pPunct->_table_enum])
			FogTokenType_id_table[pPunct->_table_enum] = punctuationId;
	}

//	Install keywords, dodging special preloaded values.
//
	for (const FogTokenType_IdName *pReserved = keywordNames; pReserved->_name; ++pReserved)
	{
		FogKeyword *keywordId = new FogReservedId(pReserved->_name, pReserved->_pp_enum, pReserved->_enum);
		idMap.adopt(keywordId);
		if (!FogTokenType_id_table[pReserved->_table_enum])
			FogTokenType_id_table[pReserved->_table_enum] = keywordId;
	}

//	Install non-reserved words, dodging special preloaded values.
//
	for (const FogTokenType_IdName *pNonReserved = nonReservedWordNames; pNonReserved->_name; ++pNonReserved)
	{
		FogKeyword *keywordId = new FogNonReservedId(pNonReserved->_name, pNonReserved->_table_enum);
		idMap.adopt(keywordId);
		if (!FogTokenType_id_table[pNonReserved->_table_enum])
			FogTokenType_id_table[pNonReserved->_table_enum] = keywordId;
	}

//	Install access keywords.
//
	for (const FogAccess_Initializer *pAccess = accessNames; pAccess->_decl_specifier._name; ++pAccess)
	{
		FogKeyword *keywordId = new FogAccess(*pAccess);
		idMap.adopt(keywordId);
		if (!FogTokenType_id_table[pAccess->_decl_specifier._table_enum])
			FogTokenType_id_table[pAccess->_decl_specifier._table_enum] = keywordId;
	}

//	Install cv keywords.
//
	for (const FogCv_Initializer *pCv = cvNames; pCv->_decl_specifier._name; ++pCv)
	{
		FogKeyword *keywordId = new FogCv(*pCv);
		idMap.adopt(keywordId);
		if (!FogTokenType_id_table[pCv->_decl_specifier._table_enum])
			FogTokenType_id_table[pCv->_decl_specifier._table_enum] = keywordId;
	}

//	Install decl-specifier keywords.
//
	for (const FogDeclSpecifier_Initializer *pDeclSpecifier = declSpecifierNames; pDeclSpecifier->_name; ++pDeclSpecifier)
	{
		FogKeyword *keywordId = new FogDeclSpecifier(*pDeclSpecifier);
		idMap.adopt(keywordId);
		if (!FogTokenType_id_table[pDeclSpecifier->_table_enum])
			FogTokenType_id_table[pDeclSpecifier->_table_enum] = keywordId;
	}

//	Install inline keywords.
//
	for (const FogInline_Initializer *pInline = inlineNames; pInline->_decl_specifier._name; ++pInline)
	{
		FogKeyword *keywordId = new FogInline(*pInline);
		idMap.adopt(keywordId);
		if (!FogTokenType_id_table[pInline->_decl_specifier._table_enum])
			FogTokenType_id_table[pInline->_decl_specifier._table_enum] = keywordId;
	}

//	Install segment keywords.
//
	for (const FogSegment_Initializer *pSegment = segmentNames; pSegment->_name; ++pSegment)
	{
		FogKeyword *keywordId = new FogSegment(*pSegment);
		idMap.adopt(keywordId);
		if (!FogTokenType_id_table[pSegment->_table_enum])
			FogTokenType_id_table[pSegment->_table_enum] = keywordId;
	}

//	Install static keywords.
//
	for (const FogStatic_Initializer *pStatic = staticNames; pStatic->_decl_specifier._name; ++pStatic)
	{
		FogKeyword *keywordId = new FogStatic(*pStatic);
		idMap.adopt(keywordId);
		if (!FogTokenType_id_table[pStatic->_decl_specifier._table_enum])
			FogTokenType_id_table[pStatic->_decl_specifier._table_enum] = keywordId;
	}

//	Install tag keywords.
//
	for (const FogTag_Initializer *pTag = tagNames; pTag->_name; ++pTag)
	{
		FogKeyword *keywordId = new FogTag(*pTag);
		idMap.adopt(keywordId);
		if (!FogTokenType_id_table[pTag->_table_enum])
			FogTokenType_id_table[pTag->_table_enum] = keywordId;
	}

//	Install utility keywords.
//
	for (const FogUtility_Initializer *pUtility = utilityNames; pUtility->_name; ++pUtility)
	{
		FogKeyword *keywordId = new FogUtility(*pUtility);
		idMap.adopt(keywordId);
		if (!FogTokenType_id_table[pUtility->_table_enum])
			FogTokenType_id_table[pUtility->_table_enum] = keywordId;
	}

//	Install virtual keywords.
//
	for (const FogVirtual_Initializer *pVirtual = virtualNames; pVirtual->_decl_specifier._name; ++pVirtual)
	{
		FogKeyword *keywordId = new FogVirtual(*pVirtual);
		idMap.adopt(keywordId);
		if (!FogTokenType_id_table[pVirtual->_decl_specifier._table_enum])
			FogTokenType_id_table[pVirtual->_decl_specifier._table_enum] = keywordId;
	}

	FogBuiltInTypeId::install_singletons(idMap);			// double, void, etc
	FogMetaType::install_singletons(idMap);					// iterator, variable, etc
}

FogToken& FogTokenType::make_di_open_brace(size_t endColumn)
{
	return FogBrace::make_di_open_brace(endColumn);
}

FogKeyword& FogTokenType::make_identifier(const PrimId& anId)
{
	if (!FogTokenType_id_map)
		initialise();
	FogKeyword *aKeyword = FogTokenType_id_map->find(anId);
	if (aKeyword)
		return *aKeyword;
	else
		return FogIdentifier::make(anId);
}

FogKeyword& FogTokenType::make_identifier(const char *aBuffer)
{
	return make_identifier(*PrimIdHandle(aBuffer));
}

FogToken& FogTokenType::make_identifier(const char *aBuffer, size_t aLength)
{
	return make_identifier(*PrimIdHandle(aBuffer, aLength));
}

FogToken& FogTokenType::make_literal_character(const char *aBuffer, size_t aLength)
{
	return FogCharacter::make(aBuffer, aLength, FogCharacter::LITERAL);
}

FogToken& FogTokenType::make_literal_string(const char *aBuffer, size_t aLength)
{
	return FogString::make(aBuffer, aLength, FogString::LITERAL);
}

FogToken& FogTokenType::make_macro_identifier(const PrimId& anId)
{
	return FogMacroId::make(anId);
}

FogToken& FogTokenType::make_narrow_character(const char *aBuffer, size_t aLength)
{
	return FogCharacter::make(aBuffer, aLength, FogCharacter::NARROW);
}

FogToken& FogTokenType::make_narrow_string(const char *aBuffer, size_t aLength)
{
	return FogString::make(aBuffer, aLength, FogString::NARROW);
}

FogToken& FogTokenType::make_number(const char *aBuffer, size_t aLength)
{
	return FogNumber::make(aBuffer, aLength);
}

FogToken& FogTokenType::make_open_brace(size_t endColumn)
{
	return FogBrace::make_open_brace(endColumn);
}

FogToken& FogTokenType::make_spacing(int someSpaces)
{
	return FogSpacing::make_spacing(someSpaces);
}

FogToken& FogTokenType::make_wide_character(const char *aBuffer, size_t aLength)
{
	return FogCharacter::make(aBuffer, aLength, FogCharacter::WIDE);
}

FogToken& FogTokenType::make_wide_string(const char *aBuffer, size_t aLength)
{
	return FogString::make(aBuffer, aLength, FogString::WIDE);
}

FogToken& FogTokenType::nil()
{
	static FogToken& nilToken = get(FogTokenType::NIL);
	return nilToken;
}

FogToken& FogTokenType::null()
{
	static FogToken& nullToken = get(FogTokenType::init());
	return nullToken;
}

void FogTokenType::set_no_bool_type()
{
	if (!FogTokenType_id_map)
		initialise();
	FogTokenType_id_map->remove(*PrimIdHandle("bool"));
	FogTokenType_id_table[FALSE] = 0;
	FogTokenType_id_map->remove(*PrimIdHandle("false"));
	FogTokenType_id_table[TRUE] = 0;
	FogTokenType_id_map->remove(*PrimIdHandle("true"));
}

void FogTokenType::set_no_wchar_t_type()
{
	if (!FogTokenType_id_map)
		initialise();
	FogTokenType_id_map->remove(*PrimIdHandle("wchar_t"));
}

FogToken& FogTokenType::zero()
{
	static FogToken& zeroToken = FogNumber::make_zero();
	return zeroToken;
}
