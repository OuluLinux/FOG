
#ifndef FOGSTREAM_HXX
#define FOGSTREAM_HXX





class PrimTime;

class FogStream : public PrimOstrstream
{
 typedef PrimOstrstream Super;
 typedef FogStream This;
 friend class FogStaticEmitContext;     //   To set_depth()
 friend class FogStreamIndent;      //   To set_depth()
 friend class FogStreamEntityImplementation;   //   To adjust context.
 friend class FogStreamEntityInterface;    //   To adjust context.
 friend class FogStreamNestedEntity;     //   To adjust context.
public:
 enum BufferSize { BUFFER_SIZE = 4096 };
 enum Columns { COLUMNS };
 enum Indents { INDENTS };

private:
 const FogScope *_emit_scope;    //   Scope established by class{, extern{, namespace{ etc.
 FogEntity *_entity;       //   Current entity never 0, just null.
 const FogTargetFile *_file;     //   Target file, 0 if re-entrant.
 const FogSourceFile *_hash_file;   //   Most recent file in #line
 size_t _hash_line;       //   Current line w.r.t. #line
 FogAccessHandle _access;     //   Prevailing default access for emitted declarations.
 int _depth;         //   Current indentation depth, -ve strips leading spaces.
 size_t _blank_lines;      //   Number of trailing blank lines.
 bool _transparent;       //   True to suppress attribute evaluation.
 int _q;          //   Current write index in _buf.
 int _q_depth;        //   Spaces adjustment before advancing _q.
 PrimCountedArray<char> _buf;    //   Working line buffer.

private:
 FogStream(const This& aStream);
 This& operator=(const This& aStream);
 This& mutate() const { return *(This *)this; }

private:              //   std::ostream redirection
 void append(const char *b);
 void append(const char *b, int n);
 void copy(const char *aString);
 void copy(const char *aString, size_t aSize);
 int depth() const { return _depth; }
 void flush();
 FogStream& put(char x);
 FogStream& put(wchar_t x);
 FogStream& put(unsigned char x);
 FogStream& seekp(std::streampos p);
 //FogStream& seekp(std::streamoff o, std::ios_base::seek_dir d);
 void set_capacity(size_t aSize);
 size_t set_depth(size_t numColumns, Columns);
 size_t set_depth(size_t extraIndents, Indents);
 Super& super() { return *(Super *)this; }
 FogStream& write(const unsigned char *b, int n);
 FogStream& write(const wchar_t *b, int n);
 FogStream& operator<< (unsigned char x);
//  	FogStream& operator<< (wchar_t x);				-- suppress since tends to be a typedef at present
 FogStream& operator<< (const wchar_t* x);
 FogStream& operator<< (short x);
 FogStream& operator<< (unsigned short x);
 FogStream& operator<< (int x);
 FogStream& operator<< (unsigned int x);
 FogStream& operator<< (long x);
#ifndef _NO_LONGLONG
//  	FogStream& operator<< (long long x);
//  	FogStream& operator<< (unsigned long long x);
#endif
 FogStream& operator<< (float x);
 FogStream& operator<< (double x);
 FogStream& operator<< (long double x);
 FogStream& operator<< (void* x);
 FogStream& operator<< (const void* x);
 FogStream& operator<< (std::streambuf* x);
//  	FogStream& operator<< (std::ostream& (*x)(std::ostream&));
 FogStream& operator<< (std::ios& (*x)(std::ios&));

private:
 const FogAccess& access() const { return *_access; }
 void reset();
 void set_entity_interface(FogEntity& anEntity);
 void set_implementation(FogEntity& anEntity);
 void set_scope_interface(FogScope& aScope);

public:
 FogStream(FogEntity& anEntity, const FogTargetFile *aFile = 0);
 FogStream& change_to_access(const FogAccess& anAccess);
 FogStream& change_to_emit_scope(const FogScope& nameScope);
 FogStream& dummy_blank_line();
 bool emit_hash_line(const FogLine& aLine);
 const FogScope *emit_scope() const { return _emit_scope; }
 void emit_space(char nextChar);
 void emit_space_and_text(const char *aString);
 FogEntity& entity() { return *_entity; }
 const FogEntity& entity() const { return *_entity; }
 const FogTargetFile *file() { return _file; }
 const PrimId& id() { flush(); return Super::id(); }
 void indent(int extraDepth);
 FogScope& inner_scope();
 const FogScope& inner_scope() const { return mutate().inner_scope(); }
 bool is_transparent() const { return _transparent; }
 FogStream& next();
 FogScope& outer_scope();
 const FogScope& outer_scope() const { return mutate().outer_scope(); }
 size_t pcount() { flush(); return Super::pcount(); }
 virtual std::ostream& print_viz(std::ostream& s) const;
 void set_access(const FogAccess& anAccess) { _access = anAccess; }
 void set_transparent() { _transparent = true; }
 FogStream& start();
 const char *str() { flush(); return Super::str(); }
//  	size_t strlen() { flush(); return Super::strlen(); }
 FogStream& write(const char *b, int n) { if (n) append(b, n); return *this; }
//  	FogStream& write_number(const PrimNumber& aNumber, unsigned int numberBase);
//  	FogStream& write_through(const char *p) { super() << p; return *this; }
//  	FogStream& write_through(unsigned long x) { super() << x; return *this; }
//  	FogStream& write_through(const PrimCString& s); // { super() << s; return *this; }
 FogStream& operator<< (char x) { append(&x, 1); return *this; }
 FogStream& operator<< (const char *x) { append(x); return *this; }
 FogStream& operator<< (unsigned long x);
 FogStream& operator<< (FogStream& (*x)(FogStream&)) { (*x)(*this); return *this; }
 FogStream& operator<<(const PrimId& anId);
 FogStream& operator<<(const PrimIdHandle& anId);
 FogStream& operator<<(const PrimNumber& aNumber);
 FogStream& operator<<(const PrimString& aString);
 FogStream& operator<<(const PrimStringHandle& aString);
 FogStream& operator<<(const PrimTime& aTime);
//  	friend FogStream& dec(FogStream& s) { ::dec(s.super()); return s; }
//  	friend FogStream& endl(FogStream& s) { s.append("\n", 1); return s; }
//  	friend FogStream& ends(FogStream& s) { s.flush(); ::ends(s.super()); return s; }
//  	friend FogStream& flush(FogStream& s) { ::flush(s.super()); return s; }
//  	friend FogStream& hex(FogStream& s) { ::hex(s.super()); return s; }
//  	friend FogStream& oct(FogStream& s) { ::oct(s.super()); return s; }

public:
 static char space(std::ostream& s, char tailChar, char nextChar);
 static char space_and_emit(std::ostream& s, char tailChar, const char *aString);
 static char space_and_emit(std::ostream& s, char tailChar, const PrimId& anId);
 static char space_char(char tailChar, char nextChar);
public:
 friend FogStream& dummy_blank_line(FogStream& s) { return s.dummy_blank_line(); }
 friend FogStream& next(FogStream& s) { return s.next(); }
 friend FogStream& start(FogStream& s) { return s.start(); }
public:
 friend void annul(FogStream *aStream) { if (aStream) delete aStream; }
};

class FogStreamIndent
{
private:
 FogStream& _stream;
 const int _depth;
private:
 FogStreamIndent(const FogStreamIndent&);
 FogStreamIndent& operator=(const FogStreamIndent&);
public:
 FogStreamIndent(FogStream& aStream, int extraIndents = 1)
  : _stream(aStream), _depth(aStream.depth()) { aStream.set_depth(extraIndents, FogStream::INDENTS); }
 FogStreamIndent(FogStream& aStream, int extraIndents, FogStream::Indents)
  : _stream(aStream), _depth(aStream.depth()) { aStream.set_depth(extraIndents, FogStream::INDENTS); }
 FogStreamIndent(FogStream& aStream, int extraColumns, FogStream::Columns)
  : _stream(aStream), _depth(aStream.depth())
  { aStream.set_depth(_depth + extraColumns, FogStream::COLUMNS); }
 ~FogStreamIndent() { _stream.set_depth(_depth, FogStream::COLUMNS); }
};

class FogStreamEntityImplementation
{
private:
 FogStream& _stream;
private:
 FogStreamEntityImplementation(const FogStreamEntityImplementation&);
 FogStreamEntityImplementation& operator=(const FogStreamEntityImplementation&);
public:
 FogStreamEntityImplementation(FogStream& aStream, FogEntity& anEntity)
  : _stream(aStream) { aStream.set_implementation(anEntity); }
 ~FogStreamEntityImplementation() { _stream.reset(); }
};

class FogStreamEntityInterface
{
private:
 FogStream& _stream;
private:
 FogStreamEntityInterface(const FogStreamEntityInterface&);
 FogStreamEntityInterface& operator=(const FogStreamEntityInterface&);
public:
 FogStreamEntityInterface(FogStream& aStream, FogEntity& anEntity)
  : _stream(aStream) { aStream.set_entity_interface(anEntity); }
 FogStreamEntityInterface(FogStream& aStream, FogScope& aScope)
  : _stream(aStream) { aStream.set_scope_interface(aScope); }
 ~FogStreamEntityInterface() { _stream.reset(); }
};
#endif
 
