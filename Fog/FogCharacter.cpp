#include <Fog/FogIncludeAll.h>
















TYPEINFO_SINGLE(FogCharacter, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogCharacter)
PRIMREFS_IMPL(FogCharacter)
PRIMMAPOFREFS_IMPL(FogCharacter)
FOGTOKEN_SHAREDLEAF_IMPL(FogCharacter)
TMPL_HACK_FIX_DO(FogCharacter)

FogCharacter::FogCharacter()
	:
	_character_type(NARROW),
	_value(0) {}

FogCharacter::FogCharacter(const PrimId& rawId, const PrimId& binId, unsigned long aValue, CharacterType characterType)
	:
	_raw_id(rawId),
	_character_type(characterType),
	_value(aValue),
	_bin_id(binId) {}

FogCharacter::~FogCharacter() {}

const PrimId& FogCharacter::bin_id() const {
	if (_bin_id || !_raw_id)
		;
	else if (_character_type == LITERAL)
		mutate()._bin_id = _raw_id;
	else {
		char buf[Fog::MAX_MULTI_BYTE_COUNT];
		size_t byteCount = _character_type == WIDE ? Fog::get_multi_byte_count() : 1;
		char* q = buf + sizeof(buf);
		unsigned long charValue = _value;
		
		for (size_t i = byteCount; i-- > 0; charValue >>= 8)
			* --q = charValue & 0xFF;
			
		mutate()._bin_id.adopt(PrimId::create(q, byteCount));
	}
	
	return *_bin_id;
}

bool FogCharacter::emit(FogEmitContext& emitContext) const {
	switch (_character_type) {
	case NARROW:
		emitContext.emit_character(put_id(), false);
		break;
		
	case WIDE:
		emitContext.emit_character(put_id(), true);
		break;
		
	case LITERAL:
		emitContext.emit_text("'");
		emitContext.emit_text(raw_id().str());
		emitContext.emit_text("'");
		break;
	}
	
	return true;
}

//
//  	Return the value of the next character from the string at p terminated at pEnd, updating p
//  	to point past the analysed character. Sets p to 0 on error.
//
unsigned long FogCharacter::get(const unsigned char*& p, const unsigned char* pEnd) {
	if (p >= pEnd) {
		p = 0;
		return 0;
	}
	
	unsigned long charCode = *p++;
	
	if (charCode != '\\')
		return charCode;
		
	if (p >= pEnd)
		return 0;
		
	charCode = 0;
	
	switch (*p++) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7': {
		charCode = *(p - 1) - '0';
		
		for (int i = 2; (p < pEnd) && ('0' <= *p) && (*p <= '7') && (i > 0); p++, i--)
			charCode = (charCode << 3) | (*p - '0');
			
		break;
	}
	
	case 'x':
		if (!isxdigit(*p)) {
			p = 0;
			break;
		}
		
		for (; (p < pEnd) && isxdigit(*p); p++)
			charCode = (charCode << 4) | (isdigit(*p) ? (*p - '0') : (tolower(*p) - 'a' + 10));
			
		break;
		
	case 'u': {
		int i = 4;
		
		for (; (p < pEnd); p++, i--)
			charCode = (charCode << 4) | (isdigit(*p) ? (*p - '0') : (tolower(*p) - 'a' + 10));
			
		if (i != 0)
			p = 0;
			
		break;
	}
	
	case 'U': {
		int i = 8;
		
		for (; (p < pEnd); p++, i--)
			charCode = (charCode << 4) | (isdigit(*p) ? (*p - '0') : (tolower(*p) - 'a' + 10));
			
		if (i != 0)
			p = 0;
			
		break;
	}
	
	case '\'':
		return '\'';
		
	case '\"':
		return '\"';
		
	case '?':
		return '?';
		
	case '\\':
		return '\\';
		
	case 'a':
		return '\a';
		
	case 'b':
		return '\b';
		
	case 'f':
		return '\f';
		
	case 'n':
		return '\n';
		
	case 'r':
		return '\r';
		
	case 't':
		return '\t';
		
	case 'v':
		return '\v';
		
	default:
		p = 0;
		break;
	}
	
	return charCode;
}

bool FogCharacter::get_character(PrimIdHandle& returnId, FogScopeContext& inScope) {
	returnId = bin_id();
	return true;
}

bool FogCharacter::get_identifier(PrimIdHandle& returnId, FogScopeContext& inScope) {
	returnId = bin_id();
	return true;
}

const PrimNumber& FogCharacter::get_number(FogScopeContext& inScope) const {
	return PrimNumber::make_nat(_value);
}

bool FogCharacter::get_number_token(FogTokenRef& returnValue, FogScopeContext& inScope) const {
	returnValue.assign(FogNumber::make_natural(_value));
	return true;
}

bool FogCharacter::get_string(PrimIdHandle& returnId, FogScopeContext& inScope) {
	returnId = bin_id();
	return true;
}

const FogCharacter* FogCharacter::is_character() const {
	return this;
}

bool FogCharacter::is_wide() const {
	return _character_type == WIDE;
}

FogCharacter& FogCharacter::make(const PrimId& rawId, const PrimId& binId, CharacterType characterType) {
	static FogCharacterMapOfRef charMap[SIZEOF_CHARACTERTYPE];
	FogCharacter* aCharacter = charMap[characterType].find(rawId);
	
	if (!aCharacter) {
		const unsigned char* pStart = (const unsigned char*)rawId.str();
		const unsigned char* pEnd = pStart + rawId.length();
		unsigned long charValue = 0;
		
		while (pStart && (pStart < pEnd))
			charValue = (charValue << 8) | get(pStart, pEnd);
			
		if (pStart != pEnd)
			ERRMSG("Failed to extract a valid character from " << c_string(rawId.str()));
			
		aCharacter = new FogCharacter(rawId, binId, charValue, characterType);
		
		if (aCharacter)
			charMap[characterType].adopt(aCharacter);
	}
	
	return aCharacter ? *aCharacter : FogCharacter::mutable_null();
}

FogCharacter& FogCharacter::make(const PrimId& anId, CharacterType characterType) {
	return make(anId, PrimId::null(), characterType);
}

FogCharacter& FogCharacter::make(const char* aBuffer, size_t aLength, CharacterType characterType) {
	return make(*PrimIdHandle(aBuffer, aLength), PrimId::null(), characterType);
}

const FogMetaType& FogCharacter::meta_type() const {
	return FogMetaType::character_type();
}

FogTokenType::TokenType FogCharacter::pp_token_type_enum() const {
	return FogTokenType::CharacterLiteral;
}

std::ostream& FogCharacter::print_diagnostic(std::ostream& s, int aDepth) const {
	return s << "Character " << *this;
}

char FogCharacter::print_named(std::ostream& s, const PrimId* fullId, char tailChar) const {
	switch (_character_type) {
	case NARROW:
		FogStream::space_and_emit(s, tailChar, "'");
		break;
		
	case WIDE:
		FogStream::space_and_emit(s, tailChar, "L'");
		break;
		
	case LITERAL:
		FogStream::space_and_emit(s, tailChar, "'");
		break;
	}
	
	const PrimId& anId = put_id();
	s << c_string(anId.str(), 0, "'",
	              PrimCString::PrimCStringFlags(PrimCString::ECHO_BACKSLASH |
	                      PrimCString::ECHO_WHITESPACE | PrimCString::ECHO_SINGLE_QUOTE));
	return '\"';
}

std::ostream& FogCharacter::print_source(std::ostream& s, int aDepth) const {
	switch (_character_type) {
	case NARROW:
		s << c_string(put_id().str(), "'", "'");
		break;
		
	case WIDE:
		s << c_string(put_id().str(), "L'", "'");
		break;
		
	case LITERAL:
		s << c_string(raw_id().str(), "", "",
		              PrimCString::PrimCStringFlags(PrimCString::ECHO_BACKSLASH | PrimCString::ECHO_WHITESPACE |
		                      PrimCString::ECHO_SINGLE_QUOTE));
		break;
	}
	
	return s;
}

std::ostream& FogCharacter::print_stringized(std::ostream& s) const {
	switch (_character_type) {
	case NARROW:
		s << c_string(raw_id().str(), "'", "'",
		              PrimCString::PrimCStringFlags(PrimCString::ECHO_WHITESPACE | PrimCString::ECHO_SINGLE_QUOTE));
		break;
		
	case WIDE:
		s << c_string(raw_id().str(), "L'", "'",
		              PrimCString::PrimCStringFlags(PrimCString::ECHO_WHITESPACE | PrimCString::ECHO_SINGLE_QUOTE));
		break;
		
	case LITERAL:
		s << c_string(raw_id().str(), "", "",
		              PrimCString::PrimCStringFlags(PrimCString::ECHO_BACKSLASH | PrimCString::ECHO_WHITESPACE |
		                      PrimCString::ECHO_SINGLE_QUOTE));
		break;
	}
	
	return s;
}

std::ostream& FogCharacter::print_this(std::ostream& s) const {
	switch (_character_type) {
	case NARROW:
		return s << '\'' << *_raw_id << '\'';
		
	case WIDE:
		return s << "L\'" << *_raw_id << '\'';
		
	case LITERAL:
	default:
		return s << *_raw_id ;
	}
}

std::ostream& FogCharacter::print_viz(std::ostream& s) const {
	switch (_character_type) {
	case NARROW:
		return s << "\"character: " << *_raw_id << '"';
		
	case WIDE:
		return s << "\"wide-character: " << *_raw_id << '"';
		
	case LITERAL:
	default:
		return s << "\"literal-character: " << *_raw_id << '"';
	}
}

//
//	Emit the 8 bit character sequence that represents aValue, returning the number of characters generated.
//
size_t FogCharacter::put(std::ostream& s, unsigned long aValue, AsCharacter asCharacter) {
	if (aValue <= 0xFF) {
		switch (aValue) {
		case '\'':
			if (asCharacter) {
				s << "\\\'";
				return 2;
			}
			else {
				s << "'";
				return 1;
			}
			
		case '"':
			if (asCharacter) {
				s << "\"";
				return 1;
			}
			else {
				s << "\\\"";
				return 2;
			}
			
		case '?':
			s << "\\\?";
			return 2;
			
		case '\\':
			s << "\\\\";
			return 2;
			
		case '\a':
			s << "\\a";
			return 2;
			
		case '\b':
			s << "\\b";
			return 2;
			
		case '\f':
			s << "\\f";
			return 2;
			
		case '\n':
			s << "\\n";
			return 2;
			
		case '\r':
			s << "\\r";
			return 2;
			
		case '\t':
			s << "\\t";
			return 2;
			
		case '\v':
			s << "\\v";
			return 2;
			
		default:
			if (isprint(aValue)) {
				s << char(aValue);
				return 1;
			}
			else {
				s << "\\" << std::setw(3) << std::setfill('0') << std::oct << aValue << std::dec;
				return 4;
			}
		}
	}
	else if (aValue <= 0xFFFF) {
		s << "\\u" << std::setw(4) << std::setfill('0') << std::hex << aValue << std::dec;
		return 6;
	}
	else {
		s << "\\U" << std::setw(8) << std::setfill('0') << std::hex << aValue << std::dec;
		return 10;
	}
}

const PrimId& FogCharacter::put_id() const {
	if (_put_id || !bin_id())
		;
	else if (!is_wide()) {
		PrimOstrstream s;
		const unsigned char* p = (const unsigned char*)bin_id().str();
		unsigned long charValue = 0;
		size_t putCount = put(s, *p, AS_CHARACTER);
		mutate()._put_id.adopt(PrimId::create(s.str(), putCount));
	}
	else {
		PrimOstrstream s;
		size_t byteCount = Fog::get_multi_byte_count();
		const unsigned char* p = (const unsigned char*)bin_id().str();
		unsigned long charValue = 0;
		
		for (size_t j = byteCount; j--;)
			charValue = (charValue << 8) | *p++;
			
		size_t putCount = put(s, charValue, AS_CHARACTER);
		mutate()._put_id.adopt(PrimId::create(s.str(), putCount));
	}
	
	return *_put_id;
}

bool FogCharacter::resolve_semantics(FogSemanticsContext& theSemantics) const {
	theSemantics.assign(FogSemantics::IS_CHARACTER);
	return true;
}

size_t FogCharacter::source_columns() const {
	return _raw_id->length();
}

FogTokenType::TokenType FogCharacter::token_type_enum() const {
	return FogTokenType::CharacterLiteral;
}
