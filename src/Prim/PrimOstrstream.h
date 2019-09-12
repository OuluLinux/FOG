
#ifndef PRIMOSTRSTREAM_HXX
#define PRIMOSTRSTREAM_HXX





//  
//  	A PrimOstrstream refines the standard ostrstream class to support:
//  
//  		auto-destruction of any auto-allocated buffer.
//  		auto-application of trailing 0 before returning any string.
//  		caching of a PrimString implementation if used.
//  		caching of a PrimId implementation if used.
//  
//  	The return from str() is const char *, preventing change, and detecting the delete[] s.str() when existing
//  	code is adapted to replace an ostrstream by a PrimOstrstream.
//  
//  .note
//  		use of str() adds a terminating null if needed and consequently changes the return from strlen().
//  		use of id() invokes str() and constructs from a null terminated string rather than the put count.
//  		If a PrimId containing nulls is needed, then invoke strlen() to get the length, then invoke str()
//  		to get the data and pass the results to PrimIdHandle.
//  
//  .limits
//  		public inheritance from ostrstream is required to ensure that conventional << overloading
//  		of user types operates as expected, but means that this class cannot guarantee to intercept
//  		str() or ends. As a result multiple null teminations may be applied. This is benign unless
//  		pcount() is used to count generated characters.
//  
class PrimOstrstream : public std::ostrstream
{
 typedef std::ostrstream Super;
private:
 PrimStringHandle _string;
 PrimIdHandle _id;
 bool _fixed_buffer : 1;         //   True if constructed with fixed buffer.
 bool _ends : 1;           //   True once buffer terminated by ends.
private:
 std::ostrstream& super() { return *(Super *)this; }

public:              //   ios redirection
//  	static long	bitalloc() { return Super::bitalloc(); }
//  	static void	sync_with_stdio() { Super::sync_with_stdio(); }
//  	static int xalloc() { return Super::xalloc(); }

public:              //   ios redirection
//  	using Super::bad;
//  	using Super::clear;
//  	using Super::eof;
//  	using Super::fail;
//  	char fill() { return Super::fill(); }
//  	char fill(char c) { return Super::fill(c); }
//  	long flags() { return Super::flags(); }
//  	long flags(long l) { return Super::flags(l); }
//  	using Super::good;
//  	using Super::iword;
//  	int	precision(int i) { return Super::precision(i); }
//  	int	precision() { return Super::precision(); }
//  	using Super::pword;
//  	using Super::rdstate;
//  	long setf(long _setbits, long _field) { return Super::setf(_setbits, _field); }
//  	long setf(long l) { return Super::setf(l); }
//  	ostream* tie(ostream* s) { return Super::tie(s); }
//  	ostream* tie() { return Super::tie(); }
//  	using Super::unsetf;
//  	int	width() { return Super::width(); }
//  	int	width(int i) { return Super::width(i); }
//  	using Super::operator!;

protected:             //   ostream redirection
//  	using Super::do_opfx;
//  	using Super::do_osfx;
public:              //   ostream redirection
//  	PrimOstrstream& flush() { Super::flush(); return *this; }
//  	using Super::opfx;
//  	using Super::osfx;
//  	PrimOstrstream& put(char x) { Super::put(x); return *this; }
//  	PrimOstrstream& put(wchar_t x) { Super::put(x); return *this; }
//  	PrimOstrstream& put(unsigned char x) { Super::put(x); return *this; }
//  	PrimOstrstream& seekp(streampos p) { Super::seekp(p); return *this; }
//  	PrimOstrstream& seekp(streamoff o, unsafe_ios::seek_dir d) { Super::seekp(o, d); return *this; }
//   	using Super::tellp;
//  	PrimOstrstream& write(const char*b, int n) { Super::write(b, n); return *this; }
//  	PrimOstrstream& write(const unsigned char* b, int n) { Super::write(b, n); return *this; }
//  	PrimOstrstream& write(const wchar_t *b, int n) { Super::write(b, n); return *this; }
//  	PrimOstrstream& operator<< (char x) { Super::operator<<(x); return *this; }
//  	PrimOstrstream& operator<< (unsigned char x) { Super::operator<<(x); return *this; }
//  	PrimOstrstream& operator<< (wchar_t x) { Super::operator<<(x); return *this; }
//  	PrimOstrstream& operator<< (const wchar_t* x) { Super::operator<<(x); return *this; }
//  	PrimOstrstream& operator<< (short x) { Super::operator<<(x); return *this; }
//  	PrimOstrstream& operator<< (unsigned short x) { Super::operator<<(x); return *this; }
//  	PrimOstrstream& operator<< (int x) { Super::operator<<(x); return *this; }
//  	PrimOstrstream& operator<< (unsigned int x) { Super::operator<<(x); return *this; }
//  	PrimOstrstream& operator<< (long x) { Super::operator<<(x); return *this; }
//  	PrimOstrstream& operator<< (unsigned long x) { Super::operator<<(x); return *this; }
#ifndef _NO_LONGLONG
//    PrimOstrstream& operator<< (long long x) { Super::operator<<(x); return *this; }
//    PrimOstrstream& operator<< (unsigned long long x) { Super::operator<<(x); return *this; }
#endif
//  	PrimOstrstream& operator<< (float x) { Super::operator<<(x); return *this; }
//  	PrimOstrstream& operator<< (double x) { Super::operator<<(x); return *this; }
//    PrimOstrstream& operator<< (long double x) { Super::operator<<(x); return *this; }
//  	PrimOstrstream& operator<< (const char* x) { Super::operator<<(x); return *this; }
//  	PrimOstrstream& operator<< (void* x) { Super::operator<<(x); return *this; }
//  	PrimOstrstream& operator<< (const void* x) { Super::operator<<(x); return *this; }
//  	PrimOstrstream& operator<< (streambuf* x) { Super::operator<<(x); return *this; }
//  	PrimOstrstream& operator<< (ostream& (*x)(ostream&)) { Super::operator<<(x); return *this; }
//  	PrimOstrstream& operator<< (ios& (*x)(ios&)) { Super::operator<<(x); return *this; }

//  	friend PrimOstrstream& dec(PrimOstrstream& s) { ::dec(s.super()); return s; }
//  	friend PrimOstrstream& endl(PrimOstrstream& s) { ::endl(s.super()); return s; }
//  	friend PrimOstrstream& ends(PrimOstrstream& s) { return s.ends(); }
//  	friend PrimOstrstream& flush(PrimOstrstream& s) { ::flush(s.super()); return s; }
//  	friend PrimOstrstream& hex(PrimOstrstream& s) { ::hex(s.super()); return s; }
//  	friend PrimOstrstream& oct(PrimOstrstream& s) { ::oct(s.super()); return s; }

public:              //   strstreambase redirection
 using Super::rdbuf;

public:              //   ostrstream redirection
 using Super::pcount;

public:
 PrimOstrstream();
 PrimOstrstream(char *fixedBuffer, int fixedSize, ios_base::openmode aMode = std::ios::out);
 virtual ~PrimOstrstream();
 PrimOstrstream& ends();
 const PrimId& id();
 size_t strlen();
 const char *str();
 const char *str() const;
 const PrimString& string();

public:
 friend void annul(PrimOstrstream *aStream) { if (aStream) delete aStream; }
 friend std::ostream& operator<<(std::ostream& s, PrimOstrstream& anObject);
 friend std::ostream& operator<<(std::ostream& s, const PrimOstrstream& anObject);
};
#endif
 
