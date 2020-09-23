#include <Fog/FogIncludeAll.h>












class FogStreamNestedEntity
{
private:
 FogStream& _stream;
 FogEntity *const _entity;
private:
 FogStreamNestedEntity(const FogStreamNestedEntity&);
 FogStreamNestedEntity& operator=(const FogStreamNestedEntity&);
public:
 FogStreamNestedEntity(FogStream& aStream, FogEntity& anEntity)
  : _stream(aStream), _entity(_stream._entity) { _stream._entity = &anEntity; }
 ~FogStreamNestedEntity() { _stream._entity = _entity; }
};

FogStream::FogStream(FogEntity& anEntity, const FogTargetFile *aFile)
:
 _emit_scope(&anEntity.name_emit_scope()),
 _entity(&anEntity),
 _file(aFile),       
//  	_line(0),
 _hash_file(0),
 _hash_line(0),
 _depth(0),
 _blank_lines(0),
 _transparent(false),
 _q(0),
 _q_depth(0),
 _buf(new char[BUFFER_SIZE], BUFFER_SIZE)
{}

//  
//  	Append aString[aSize].
//  
//  	Tabs are expanded to spaces.
//  	_depth spaces are output at the start of each line.
//  	$$ and $@ get unescaped to $ and @.
//  	@ sequences are expanded.
//  	Blank lines are counted.
//  
void FogStream::append(const char *aString)
{
 if (aString)
  append(aString, ::strlen(aString));
}
void FogStream::append(const char *aString, int aSize)
{
 if (!aString)
  return;
 const char *p = aString;
 const char *pStart = p;
 for (const char *pEnd = aString + aSize; p < pEnd; )
 {
  if (*p++ == '\n')
  {
   copy(pStart, p - pStart);
   flush();
   pStart = p;
  }
 }
 if (p > pStart)
  copy(pStart, p - pStart);
}

//  
//  	Output sufficient new lines to ensure that subsequent output starts after a blank line.
//  
FogStream& FogStream::change_to_access(const FogAccess& anAccess) {
	if (access().value() != anAccess.value()) {
		// Skip "public:" for namespaces
		if (_emit_scope->tag().is_namespace_tag() &&
			!_access->is_valid() && anAccess.is_public()) {
			next();
		}
		// Skip "private:" for class
		else if (_emit_scope->tag().is_class_tag() &&
			!_access->is_valid() && anAccess.is_private()) {
			next();
		}
		// Skip "public:" for struct
		else if (_emit_scope->tag().is_struct_tag() &&
			!_access->is_valid() && anAccess.is_public()) {
			next();
		}
		else if (anAccess) {
			next();
			if (_emit_scope && _emit_scope->has_access()) {
				int aDepth = _depth;
				int indentSize = Fog::get_indent_size();
				if (_depth >= indentSize)
					_depth -= indentSize;
				*this << anAccess.str() << ":\n";
				_depth = aDepth;
				dummy_blank_line();
			}
		}
		_access = anAccess;
	}
	return *this;
}

//  
//  	Adjust the braced scoping constructs to terminate as many existing contexts as required before
//  	starting as many new constructs necessary to end up in aScope.
//  
FogStream& FogStream::change_to_emit_scope(const FogScope& aScope)
{
 if (_emit_scope != &aScope)
 {
  while (_emit_scope)          //   While _emit_scope not a parent of aScope.
  {
   const FogScope *joinScope = &aScope;
   for ( ; joinScope && (_emit_scope != joinScope); joinScope = joinScope->outer_scope())
    ;
   if (joinScope)
    break;
   _emit_scope->emit_scope_tail(*this);
   _emit_scope = _emit_scope->outer_scope();
   _access = FogAccess::INVALID;
  }
  while (_emit_scope != &aScope)       //   Progress from join to aScope.
  {
   const FogScope *joinScope = &aScope;
   for (const FogScope *nextScope = joinScope->outer_scope(); 
     nextScope && (nextScope != _emit_scope);
      joinScope = nextScope, nextScope = nextScope->outer_scope())
    ;
//  			_emit_scope = joinScope;
//  			_emit_scope->emit_scope_head(*this);
   joinScope->emit_scope_head(*this);
   _emit_scope = joinScope;
   _access = FogAccess::INVALID;
  }
 }
 return *this;
}

//  
//  	Copy aString straight to the intermediate buffer without interpretation.
//  
void FogStream::copy(const char *aString)
{
 if (aString)
  copy(aString, ::strlen(aString));
}
void FogStream::copy(const char *aString, size_t aSize)
{
 set_capacity(aSize);
 if (!_q)
  _q_depth = _depth;
 memcpy(_buf.get() + _q, aString, aSize);
 _q += aSize;
 _buf.get()[_q] = 0;       //   Unnecessary, but makes debugger output cleaner.
}

//  
//  	Pretend that an extra blank line has been output so that output predicated by next, does not generate
//  	a blank line.
//  
FogStream& FogStream::dummy_blank_line()
{
 if (_q)
 {
  ERRMSG("BUG - Should not generate a dummy_blank_line when not at start of line.");
  append("\n", 1);
 }
 _blank_lines++;
 return *this;
}

bool FogStream::emit_hash_line(const FogLine& aLine)
{
 start();
 static bool lineNumbers = !Fog::no_line_numbers();
 static bool commentLineNumbers = Fog::comment_line_numbers();
 if (lineNumbers && ((&aLine.file() != _hash_file) || (aLine.line_number() != (_hash_line))))
 {
  if (commentLineNumbers)
   super() << "/* ";
  super() << "#line ";
  super() << aLine.line_number();
  if (_hash_file != &aLine.file())
   super() << c_string(aLine.file().unique_id().str(), " \"");
  if (commentLineNumbers)
   super() << " */";
  super() << "\n";
 }
 _hash_file = &aLine.file();
 _hash_line = aLine.line_number();
 return true;
}

//  
//  	Generate any required spacing in a type string between tailChar already at the end of s, and
//  	nextChar that has yet to be written. This is the sole routine involved in pretty printing
//  	types strings.
//  
void FogStream::emit_space(char nextChar)
{
 if (!_q)
  return;
 char spaceChar = space_char(_buf.get()[_q-1], nextChar);
 if (spaceChar)
  *this << spaceChar;
}

//  
//  	Append aString to this stream, inserting any required spacing to pretty print, and removing
//  	any redundant spacing.
//  
void FogStream::emit_space_and_text(const char *aString)
{
 if (!aString || !*aString)         //   If no text
  ;
 else if (!_q)            //   If prior context unknown
  append(aString);
 else if (isspace(_buf.get()[_q-1]))       //   If trailing space already
 {
  if (isspace(*aString) && (*aString != '\n'))
   append(aString+1);
  else
   append(aString);
 }
 else              //   If no trailing space
 {
  if (!isspace(*aString))
  {
   char spaceChar = space_char(_buf.get()[_q-1], *aString);
   if (spaceChar)
    *this << spaceChar;
  }
  append(aString);
 }
}

//  
//  	Flush the current line buffer, performing any start of line spacing adjustment.
//  
void FogStream::flush()
{
 if (!_q)
  return;
 if (_q_depth > 0)
 {
  static const char spacesArray[] = "                                                                    ";
  static const int numSpaces = sizeof(spacesArray) - 1;
  while (_q_depth >= numSpaces)
  {
   Super::write(spacesArray, numSpaces);
   _q_depth -= numSpaces;
  }
  if (_q_depth > 0)
  {
   Super::write(spacesArray, _q_depth);
   _q_depth = 0;
  }
 }
 if (_q > -_q_depth)
 {
  if (_q_depth < 0)
  {
   const char *p = _buf.get(); 
   for (int i = -_q_depth; i > 0; i--, p++)
   {
    if (*p != ' ')
    {
     WRNMSG("Insufficient indentation:\n\t\t\"" << _buf.get() << "\"");
     _q_depth += i;
     break;
    }
   }
  }
  const char *pStart = _buf.get() - _q_depth;
  int pSize = _q + _q_depth;
  if ((pSize == 1) && (*pStart == '\n'))
   _blank_lines++;
  else
   _blank_lines = 0;
  Super::write(pStart, pSize);
 }
 _q = 0;
 _q_depth = _depth;
 _hash_line++;
}

//  
//  	Increase the indentation by extraDepth levels.
//  
void FogStream::indent(int extraDepth)
{
 int deltaDepth = extraDepth * Fog::get_indent_size();
//  	if ((deltaDepth < 0) && (-deltaDepth > _depth))
//  		_depth = 0;
//  	else
  _depth += deltaDepth;
}

FogScope& FogStream::inner_scope() { return entity().inner_scope(); }

//  
//  	Output sufficient new lines to ensure that subsequent output starts after a blank line.
//  
FogStream& FogStream::next()
{
 if (_q)
  append("\n\n", 2);
 else if (!_blank_lines)
  append("\n", 1);
 return *this;
}

std::ostream& FogStream::print_viz(std::ostream& s) const
{
 s << "\"stream: " << viz(*_entity) << '\"';
 return s;
}

FogScope& FogStream::outer_scope()
{
 FogScope *outerScope = entity().outer_scope();
 return outerScope ? *outerScope : entity().inner_scope();
}

void FogStream::reset()
{
 _entity = &_entity->global_scope();
}

void FogStream::set_capacity(size_t aSize)
{
 if (_q + aSize >= _buf.capacity())
 {
  size_t newCapacity = 2 * _buf.capacity();
  if (_q + aSize > newCapacity)
   newCapacity = _buf.capacity() + _q + aSize;
  PrimCountedArray<char> newBuf(new char[newCapacity], newCapacity);
  memcpy(newBuf.get(), _buf.get(), _q);
  _buf = newBuf;
 }
}

size_t FogStream::set_depth(size_t numColumns, Columns)
{
 size_t oldDepth = _depth;
 _depth = numColumns;
 if (_depth >= (BUFFER_SIZE/4))
 {
  static bool doneMessage = false;
  if (!doneMessage)
  {
   doneMessage = true;
   WRNMSG("Excessive actual indention trimmed to " << (BUFFER_SIZE/4) << " spaces.");
   _depth = BUFFER_SIZE/4;
  }
 }
 return oldDepth;
}

size_t FogStream::set_depth(size_t extraIndents, Indents)
{
 return set_depth(_depth + Fog::get_indent_size() * extraIndents, COLUMNS);
}

void FogStream::set_entity_interface(FogEntity& anEntity)
{
 _entity = &anEntity;
 const FogScope& emitScope = anEntity.name_emit_scope();
 if (_emit_scope != &emitScope)
  change_to_emit_scope(emitScope);
 const FogAccess& anAccess = anEntity.access();
 if (!emitScope.has_access())
  ;
 else if (!anAccess)
  ERRMSG("Unique accessibility definition not available for " << viz(anEntity));
 else if (access().value() != anAccess.value())
  change_to_access(anAccess);
}

void FogStream::set_implementation(FogEntity& anEntity)
{
 _entity = &anEntity;
 const FogScope& emitScope = anEntity.name_emit_transient_scope();
 if (_emit_scope != &emitScope)
  change_to_emit_scope(emitScope);
}

void FogStream::set_scope_interface(FogScope& aScope)
{
 _entity = &aScope;
 const FogScope *emitScope = aScope.outer_scope();
 if (!emitScope)
  emitScope = &aScope;
 if (_emit_scope != emitScope)
  change_to_emit_scope(*emitScope);
 const FogAccess& anAccess = aScope.access();
 if (!emitScope->has_access())
  ;
 else if (!anAccess)
  ERRMSG("Unique accessibility definition not available for " << viz(aScope));
 else if (access().value() != anAccess.value())
  change_to_access(anAccess);
}

//  
//  	Emit any required space character in a type string between tailChar, and nextChar that
//  	has yet to be written. Returns a potentially changed tail character.
//  
char FogStream::space(std::ostream& s, char tailChar, char nextChar)
{
 char spaceChar = space_char(tailChar, nextChar);
 if (!spaceChar)
  return tailChar;
 s << spaceChar;
 return spaceChar;
}

char FogStream::space_and_emit(std::ostream& s, char tailChar, const char *aString)
{
 if (!aString || !*aString)         //   If no text
  return tailChar;
 if (isspace(tailChar))          //   If trailing space already
 {
  if (isspace(*aString) && (*aString != '\n'))
   s << (aString+1);
  else
   s << aString;
 }
 else              //   If no trailing space
 {
  if (!isspace(*aString))
  {
   char spaceChar = space_char(tailChar, *aString);
   if (spaceChar)
    s << spaceChar;
  }
  s << aString;
 }
 return aString[::strlen(aString)-1];
}

char FogStream::space_and_emit(std::ostream& s, char tailChar, const PrimId& anId)
{
 size_t aLength = anId.length();
 if (!aLength)            //   If no text
  return tailChar;
 const char *aString = anId.str();
 if (isspace(tailChar))          //   If trailing space already
 {
  if (isspace(*aString) && (*aString != '\n'))
   s.write(aString+1, aLength-1);
  else
   s.write(aString, aLength);
 }
 else              //   If no trailing space
 {
  if (!isspace(*aString))
  {
   char spaceChar = space_char(tailChar, *aString);
   if (spaceChar)
    s << spaceChar;
  }
  s.write(aString, aLength);
 }
 return aString[aLength-1];
}

//  
//  	Generate any required space character in a type string between tailChar, and
//  	nextChar that has yet to be written. This is the sole routine involved in pretty printing
//  	types strings.
//  
//  	In order to facilitate a forced space gap, if nextChar isspace but tailChar is not
//  	nextChar is returned indicating that spacing is required.
//  
char FogStream::space_char(char tailChar, char nextChar)
{
 if (isspace(tailChar))
  return 0;
//  	if (isspace(nextChar))
//  		return 0;
 if (isspace(nextChar))
  return nextChar;
 if (tailChar && (isalnum(tailChar) || (tailChar == '_')) && (isalnum(nextChar) || (nextChar == '_')))
  return ' ';
 switch (tailChar)
 {
  case ',':
  case '=':
  case '|':
//  		case '{':
  case '<':
//  		case '>':
  case '%':
  case '+':
  case '-':
  case '/':
  case '^':
   return ' ';
  case '>':
   if (nextChar != ':') return ' ';
   break;
  case '*':
//  			if (nextChar == '(') return ' ';
   break;
  case '&':
//  			if (nextChar == ',') break;
//  			else if (nextChar == ')') break;
//  			else if (nextChar == '(') break;
//  			else if (nextChar == '*') break;
//  			else return ' ';
  case '\0':
  case '@':
  case '$':
  case '{':
  case '}':
  case '(':
//  		case ')':
  case '[':
  case '~':
  case '!':
  case '.':
  case ':':    //   No space to suit ::, space after isolated : must be forced.
   return 0;
  case ')':
   if (isalpha(nextChar)) return ' ';   //   Need a space for ") const"
   return 0;
//  		case ')':
//  			if (nextChar == ')') ;
//  			else if (nextChar == '(') ;
//  			else if (nextChar == ';') ;
//  			else if (nextChar == ',') ;
//  			else if (nextChar == '&') ;
//  			else if (isalnum(nextChar)) ;
//  			else return ' ';
//  			break;
  default:
   switch (nextChar)
   {
    case '@':
    case '$':
    case '{':
    case '}':
    case '&':
    case '.':
    case ':':
    case ')':
    case '(':
    case ',':
    case '[':
    case ']':
    case ';':
     return 0;
    default:
     return ' ';
   }
 }
 return 0;
}

//  
//  	Output sufficient a neww-line if not at start of line.
//  
FogStream& FogStream::start()
{
 if (_q)
  append("\n", 1);
 return *this;
}

#if 0
FogStream& FogStream::write_number(const PrimNumber& aNumber, unsigned int numberBase)
{
 set_capacity(20);
 char *p = _buf.get() + _q;
 std::ostrstream s(p, 20);
 if (aNumber.is_nat())
 {
  unsigned long natValue = aNumber.as_nat();
  if (numberBase == 8)
   s << oct;
  else if (numberBase == 16)
   s << hex;
  s << natValue;
 }
 else if (aNumber.is_int())
 {
  long intValue = aNumber.as_int();
  if (numberBase == 8)
   s << oct << (unsigned long)intValue;
  else if (numberBase == 16)
   s << hex << (unsigned long)intValue;
  else
   s << intValue;
 }
 else
  aNumber.print_decimal(s);
 s << std::ends;
 _q += ::strlen(p);
 return *this;
}
#endif

//  FogStream& FogStream::write_through(const PrimCString& s) { super() << s; return *this; }

FogStream& FogStream::operator<< (unsigned long x)
{
 set_capacity(20);
 char *p = _buf.get() + _q;
 std::ostrstream s(p, 20);
 s << x << std::ends;
 _q += ::strlen(p);
 return *this;
}

FogStream& FogStream::operator<<(const PrimId& anId) { append(anId.str(), anId.length()); return *this; }
FogStream& FogStream::operator<<(const PrimIdHandle& anId) { append(anId->str(), anId->length()); return *this; }

FogStream& FogStream::operator<<(const PrimNumber& aNumber)
{
 set_capacity(80);
 char *p = _buf.get() + _q;
 std::ostrstream s(p, 80);
 aNumber.print_decimal(s);
 s << std::ends;
 _q += ::strlen(p);
 return *this;
}

FogStream& FogStream::operator<<(const PrimString& aString) { copy(aString.str()); return *this; }
FogStream& FogStream::operator<<(const PrimStringHandle& aString) { copy(aString->str()); return *this; }

FogStream& FogStream::operator<<(const PrimTime& aTime)
{
 set_capacity(40);
 char *p = _buf.get() + _q;
 std::ostrstream s(p, 40);
 s << aTime << std::ends;
 _q += ::strlen(p);
 return *this;
}
