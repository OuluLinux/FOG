#include <Fog/FogIncludeAll.h>
















TYPEINFO_SINGLE(FogString, Super)
PRIMREF_DERIVED_NULL_CLASS_AND_IMPL(FogString)
PRIMREFS_IMPL(FogString)
PRIMMAPOFREFS_IMPL(FogString)
FOGTOKEN_SHAREDLEAF_IMPL(FogString)
TMPL_HACK_FIX_DO(FogString)

FogString::FogString()
:
 _string_type(NARROW)
{}

FogString::FogString(const PrimId& rawId, const PrimId& binId, StringType stringType)
:
 _raw_id(rawId),
 _string_type(stringType),
 _bin_id(binId)
{}

FogString::~FogString() {}

const PrimId& FogString::bin_id() const
{
 if (_bin_id || !_raw_id)
  ;
 else if (_string_type == LITERAL)
  mutate()._bin_id = _raw_id;
 else
 {
  size_t byteCount = is_wide() ? Fog::get_multi_byte_count() : 1;
  PrimOstrstream s;
  const unsigned char *p = (const unsigned char *)_raw_id->str();
  const unsigned char *pEnd = p + _raw_id->length();
  size_t sCount = 0;
  while (p < pEnd)
  {
   unsigned long charValue = FogCharacter::get(p, pEnd);
   if (!p)
    break;
   unsigned char buf[Fog::MAX_MULTI_BYTE_COUNT];
   unsigned char *q = buf + sizeof(buf);
   for (size_t i = byteCount; i-- > 0; charValue >>= 8)
    *--q = charValue & 0xFF;
   s.write((char *)q, byteCount);
   sCount += byteCount;
  }
  mutate()._bin_id.adopt(PrimId::create(s.str(), sCount));
 }
 return *_bin_id;
}

bool FogString::emit(FogEmitContext& emitContext) const
{
 switch (_string_type)
 {
  case NARROW:
   emitContext.emit_string(put_id(), false);
   break;
  case WIDE:
   emitContext.emit_string(put_id(), true);
   break;
  case LITERAL:
   emitContext.emit_text("\"");
   emitContext.emit_text(raw_id().str());
   emitContext.emit_text("\"");
   break;
 }
 return true;
}

//  
//  	Extract and return the next string segment from a potential concatenation of strings.
//  	The pointer to the text is updated to point past the extracted segment and any whitespace which
//  	is ignored before and after segments.
//  
const FogString& FogString::get(const char *& p)
{
 StringType stringType = NARROW;
 while (isspace(*p))
  p++;
 if (!*p)
  return FogString::immutable_null();
 if (*p == '\"')
  p++;
 else if ((*p == 'L') && (*(p+1) == '\"'))
 {
  stringType = WIDE;
  p += 2;
 }
 else
 {
  ERRMSG("No leading \" found for FogString::get.");
  return FogString::immutable_null();
 }
 const char *pStart = p;
 for ( ; *p && (*p != '\"'); p++)
 {
  if (*p == '\\')
  {
   if (!(*p+1))
   {
    ERRMSG("Unterminated \\ found for FogString::get.");
    break;
   }
   p++;
  }
 }
 if (*p != '\"')
  ERRMSG("No trailing \" found for FogString::get.");
 const char *pEnd = p;
 if (*p)
  for (p++; isspace(*p); p++)
   ;
 return FogString::make(pStart, pEnd - pStart, stringType);
}

bool FogString::get_character(PrimIdHandle& returnId, FogScopeContext& inScope)
 { returnId = bin_id(); return true; }
bool FogString::get_identifier(PrimIdHandle& returnId, FogScopeContext& inScope)
 { returnId = bin_id(); return true; }

bool FogString::get_number_token(FogTokenRef& returnValue, FogScopeContext& inScope) const
{
 returnValue.assign(FogNumber::make(bin_id()));
 return true;
}

bool FogString::get_string(PrimIdHandle& returnId, FogScopeContext& inScope)
 { returnId = bin_id(); return true; }
const FogString *FogString::is_string() const { return this; }
bool FogString::is_wide() const { return _string_type == WIDE; }

FogString& FogString::make(const PrimId& rawId, const PrimId& binId, StringType stringType)
{
 static FogStringMapOfRef stringMap[SIZEOF_STRINGTYPE];
 FogString *aString = stringMap[stringType].find(rawId);
 if (!aString)
 {
  aString = new FogString(rawId, binId, stringType);
  if (aString)
   stringMap[stringType].adopt(aString);
 }
 return aString ? *aString : FogString::mutable_null();
}

FogString& FogString::make(const PrimId& anId, StringType stringType)
{
 return make(anId, PrimId::null(), stringType);
}

FogString& FogString::make(const char *aBuffer, size_t aLength, StringType stringType)
{
 PrimIdHandle theId(aBuffer, aLength);
 return make(*theId, PrimId::null(), stringType);
}

FogString& FogString::make(const FogString& firstString, const FogString& secondString)
{
 const PrimId& firstId = firstString.bin_id();
 const PrimId& secondId = secondString.bin_id();
 size_t netLength = firstId.length() + secondId.length();
 StringType stringType = NARROW;
 if ((firstString._string_type == LITERAL) || (secondString._string_type == LITERAL))
  stringType = LITERAL;
 else if ((firstString._string_type == WIDE) && (secondString._string_type == WIDE))
  stringType = WIDE;
 PrimOstrstream s;
 s.write(firstId.str(), firstId.length());
 s.write(secondId.str(), secondId.length());
 PrimIdHandle theId(s.str(), netLength);
 return make(*theId, *theId, stringType);
}

const FogMetaType& FogString::meta_type() const { return FogMetaType::string_type(); }
FogTokenType::TokenType FogString::pp_token_type_enum() const { return FogTokenType::StringLiteral; }

std::ostream& FogString::print_diagnostic(std::ostream& s, int aDepth) const
{
 return s << "String " << *this;
}

char FogString::print_named(std::ostream& s, const PrimId *fullId, char tailChar) const
{
 FogStream::space_and_emit(s, tailChar, is_wide() ? "L\"" : "\"");
 const PrimId& anId = put_id();
 s << c_string(anId.str(), 0, "\"",
   PrimCString::PrimCStringFlags(PrimCString::ECHO_BACKSLASH |
        PrimCString::ECHO_WHITESPACE | PrimCString::ECHO_SINGLE_QUOTE));
 return '\"';
}

std::ostream& FogString::print_source(std::ostream& s, int aDepth) const
{
 switch (_string_type)
 {
  case NARROW:
   s << c_string(put_id().str(), "\"", "\"");
   break;
  case WIDE:
   s << c_string(put_id().str(), "L\"", "\"");
   break;
  case LITERAL:
   s << c_string(raw_id().str(), "", "",
    PrimCString::PrimCStringFlags(PrimCString::ECHO_BACKSLASH | PrimCString::ECHO_WHITESPACE |
     PrimCString::ECHO_SINGLE_QUOTE));
   break;
 }
 return s;
}

std::ostream& FogString::print_stringized(std::ostream& s) const
{
 switch (_string_type)
 {
  case NARROW:
   s << c_string(raw_id().str(), "\\\"", "\\\"",
    PrimCString::PrimCStringFlags(PrimCString::ECHO_WHITESPACE |PrimCString::ECHO_SINGLE_QUOTE));
   break;
  case WIDE:
   s << c_string(raw_id().str(), "L\\\"", "\\\"",
    PrimCString::PrimCStringFlags(PrimCString::ECHO_WHITESPACE |PrimCString::ECHO_SINGLE_QUOTE));
   break;
  case LITERAL:
   s << c_string(raw_id().str(), "", "",
    PrimCString::PrimCStringFlags(PrimCString::ECHO_BACKSLASH | PrimCString::ECHO_WHITESPACE |
     PrimCString::ECHO_SINGLE_QUOTE));
   break;
 }
 return s;
}

std::ostream& FogString::print_this(std::ostream& s) const
{
 switch (_string_type)
 {
  case NARROW:
   return s << "\"" << *_raw_id << '\"';
  case WIDE:
   return s << "L\"" << *_raw_id << '\"';
  case LITERAL:
  default:
   return s << *_raw_id ;
 }
}

std::ostream& FogString::print_viz(std::ostream& s) const
{
 switch (_string_type)
 {
  case NARROW:
   return s << "\"string: " << *_raw_id << '\"';
  case WIDE:
   return s << "\"wide-string: " << *_raw_id << '\"';
  case LITERAL:
  default:
   return s << "\"literal-string: " << *_raw_id << '\"';
 }
}

const PrimId& FogString::put_id() const
{
 if (_put_id || !bin_id())
  ;
 else if (!is_wide())
 {
  PrimOstrstream s;
  const unsigned char *p = (const unsigned char *)bin_id().str(); 
  size_t putCount = 0;
  for (size_t i = bin_id().length(); i ; --i)
   putCount += FogCharacter::put(s, *p++, FogCharacter::AS_STRING);
  mutate()._put_id.adopt(PrimId::create(s.str(), putCount));
 }
 else
 {
  PrimOstrstream s;
  size_t byteCount = Fog::get_multi_byte_count();
  const unsigned char *p = (const unsigned char *)bin_id().str(); 
  size_t putCount = 0;
  for (size_t i = bin_id().length(); i ; i -= byteCount)
  {
   unsigned long charValue = 0;
   for (size_t j = byteCount; j--; )
    charValue = (charValue << 8) | *p++;
   putCount += FogCharacter::put(s, charValue, FogCharacter::AS_STRING);
  }
  mutate()._put_id.adopt(PrimId::create(s.str(), putCount));
 }
 return *_put_id;
}

bool FogString::resolve_semantics(FogSemanticsContext& theSemantics) const
{
 theSemantics.assign(FogSemantics::IS_STRING);
 return true;
}

size_t FogString::source_columns() const { return _raw_id->length(); }
FogTokenType::TokenType FogString::token_type_enum() const { return FogTokenType::StringLiteral; }
