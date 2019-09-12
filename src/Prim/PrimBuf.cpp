#include <Prim/PrimIncludeAll.h>









const char *const PrimBufRep::_null_buf = "";
const PrimBufRep PrimBufRep::_null_rep;

//  
//  	Construct a buffer representation for some null terminated text. In the event of
//  	an allocation failure the null representation is returned.
//  
const PrimBufRep& PrimBufRep::construct(const char *aBuf)
{
 if (aBuf == 0)
  return (_null_rep);
 char *myBuf = new char[strlen(aBuf)];
 if (myBuf == 0)
  return (_null_rep);
 unsigned int length = strlen(aBuf);
 memcpy(myBuf, aBuf, length);
 const PrimBufRep *aRep = new PrimBufRep(myBuf, length, 1);
 if (aRep != 0)
  return *aRep;
 delete[] myBuf;
 return (_null_rep);
}

//  
//  	Construct a string representation for a character array of length aLength. In the event of
//  	an allocation failure the null representation is returned.
//  
const PrimBufRep& PrimBufRep::construct(const char *aBuffer, size_t aLength)
{
 if (aBuffer == 0)
  return (_null_rep);
 char *myBuf = new char[aLength];
 if (myBuf == 0)
  return (_null_rep);
 memcpy(myBuf, aBuffer, aLength);
 const PrimBufRep *aRep = new PrimBufRep(myBuf, aLength, 1);
 if (aRep != 0)
  return *aRep;
 delete[] myBuf;
 return (_null_rep);
}

//  
//  	Add an additional shared usage of the representation, returning the representation.
//  
const PrimBufRep& PrimBufRep::share() const
{
 if (_share_count != 0)
  ((PrimBufRep *)this)->_share_count++;
 return *this;
}

//  
//  	Unshare the buffer representation, and return a pointer to the null representation.
//  
const PrimBufRep& PrimBufRep::unshare() const
{
 PrimBufRep *mutableThis = (PrimBufRep *)this;
 if ((_share_count > 0) && (--mutableThis->_share_count == 0))
  delete mutableThis;
 return (_null_rep);
}

//  
//  	Construct a new buffer by sharing the representation of another buffer.
//  
PrimBuf::PrimBuf(const PrimBuf& aPrimBuf) : _rep(&aPrimBuf._rep->share()) {}

//  
//  	Construct a new buffer using the supplied representation. This constructor is only
//  	intended for the use of PrimBufStatic. It assumes that aPrimBufRep describes indestructible text.
//  
PrimBuf::PrimBuf(const PrimBufRep& aPrimBufRep) : _rep(&aPrimBufRep) {}

//  
//  	Assign the contents of another PrimBuf to this one.
//  
PrimBuf& PrimBuf::operator=(const PrimBuf& aPrimBuf)
{
 _rep->unshare();
 _rep = &aPrimBuf._rep->share();
 return (*this);
}

//  
//  	Compare the representation of this buffer to aPrimBuf. Returns true if the buffers exactly match
//  	or are both 0. A 0 buffer does not equal an empty buffer.
//  
int PrimBuf::operator==(const PrimBuf& aPrimBuf) const
{
 if (_rep == aPrimBuf._rep)     //   Catch the trivial case and the both null case
  return true;
 if (is_null())
  return false;
 if (size() == aPrimBuf.size())
 {
  unsigned int i = 0;
  const char *p1 = bytes();
  const char *p2 = aPrimBuf.bytes();
  while (i < size() && *p1++ == *p2++)
   i++;
  return (i == size());
 }
 return false;
}

//  
//  	Compare the representation of this buffer to aBuf. aBuf may be 0 in which
//  	case it compares equal to the representation of the 0 buffer, but not the empty buffer.
//  
int PrimBuf::operator==(const char *aBuf) const
{
 if (is_null())
  return (aBuf == 0);
 if (aBuf == 0)
  return false;
 unsigned int i = 0;
 const char *p1 = bytes();
 const char *p2 = aBuf;
 while (i < size() && *p2 != '\0' && *p1++ == *p2++)
  i++;
 return (i == size());
}

#ifndef NO_PRIM_STREAMS
//  
//  	Append the buffer to the supplied output stream. Returns the updated buffer.
//  
std::ostream& operator<<(std::ostream& s, const PrimBuf& aBuf)
{
 s.write(aBuf.bytes(), aBuf.size());
 return (s);
}

//  
//  	Read a word (1024 characters maximum) from the input stream and store it in aBuf.
//  	Returns the updated stream.
//  
std::istream& operator>>(std::istream& s, PrimBuf& aBuf)
{
 char aWord[1025];
 s >> aWord;
 aBuf = (s ? aWord : 0);
 return (s);
}
#endif

//  
//  	Compare this buffer with aPrimBuf for sort comparison purposes, returns -ve
//  	if "this" < aPrimBuf, 0 if == aPrimBuf, +ve if "this" > aPrimBuf. 0 text sorts after non-0 text.
//  
int PrimBuf::compare(const PrimBuf& aPrimBuf) const
{
 if (_rep == aPrimBuf._rep) //   Catch the trivial case and the both null case
  return (0);
 else if (is_null())
  return (1);
 else if (aPrimBuf.is_null())
  return (-1);
 else if (size() > aPrimBuf.size())
  return (1);
 else if (size() < aPrimBuf.size())
  return (-1);
 else
  return (memcmp(bytes(), aPrimBuf.bytes(), size()));
}

#ifndef NO_PRIM_STREAMS
//  
//  	Append the class name to the supplied output stream.
//  	Returns the updated stream.
//  
std::ostream& PrimBuf::print_class_name(std::ostream& s) const
{
 return (s << class_name());
}

//  
//  	Append the buffer to the supplied output stream applying a class name prefix as for PrimPrint.
//  	Returns the updated stream.
//  
std::ostream& PrimBuf::print_instance(std::ostream& s) const
{
 s << class_name();
 PrimPrint::print_address_of(s, this);
 s << "(\"";
 s.write(bytes(), size());
 s << "\")";
 return (s);
}
#endif

//  
//  	Convert the string representation into a hash code suitable for use in a dictionary.
//  
unsigned int PrimBuf::hash() const
{
 unsigned int hashCode = 0;
 const char *p = bytes();
 for (unsigned int i = 0; i < size(); i++)
  hashCode = (hashCode << 5) ^ hashCode ^ p[i];
 return (hashCode);
}
