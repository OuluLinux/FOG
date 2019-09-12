
#ifndef PRIMBUFREP_HXX
#define PRIMBUFREP_HXX


//  
//   A PrimBufRep holds the shared representation of a buffer, keeping track of the number of shared copies
//   owned by PrimBuf.
//  
//  	Implement a reference counted buffer of characters.
//  	The buffer is not 'C' null terminated not stores the length of the buffer.
//  	Its usage is very similar to that of PrimString. See also PrimBufStatic.
//  	PrimBufRep holds the shared repressentation of a buffer,
//  	keeping track of the number of shared copies owned by PrimBuf.
//  
//  .note cscrelations
//   PrimBufRep is a base class used by @x(PrimBuf).
//  .note csusum
//   Methods are provided to construct and destruct arrays of characters maintaining a share count.
//  
class PrimBufRep
{
 friend class PrimBuf;
 friend class PrimBufStatic;

private:
 const char *const _bytes;  //   Pointer to the buffer, may be 0.
 const size_t _size;    //   Size of buffer
 unsigned short _share_count; //   Number of references to this representation. Zero means indestructible.
 static const char *const _null_buf; //   The null text to avoid text == 0
 static const PrimBufRep _null_rep; //   The null representation to avoid rep == 0.

private:
//  
//  	Create a new representation for the provided text array, with a given initial size and share count.
//  
 PrimBufRep(const char *aBuf, size_t aBufSize, unsigned int aShareCount)
  : _bytes(aBuf != 0 ? aBuf : _null_buf), _size(aBufSize), _share_count(aShareCount) {}
 PrimBufRep(const PrimBufRep& aPrimBufRep); 
 PrimBufRep& operator=(const PrimBufRep& aPrimBufRep);
//  
//  	Identify the representation bytes.
//  
 const char *bytes() const { return (_bytes); }
 static const PrimBufRep& construct(const char *aCString);
 static const PrimBufRep& construct(const char *aBuffer, size_t aLength);
 const PrimBufRep& share() const;
//  
//  	Identify the representation size.
//  
 size_t size() const { return (_size); }
 const PrimBufRep& unshare() const;

public:
//  
//  	Create a default string representation which is the null buffer representation.
//  
 PrimBufRep() : _bytes(_null_buf), _share_count(0), _size(0) {}
 ~PrimBufRep(); 
};
#endif

