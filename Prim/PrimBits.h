
#ifndef PRIMBITS_HXX
#define PRIMBITS_HXX




//  
//  \		PrimBit_TN defines bit N in an unsigned T.
//  \		PrimSignedBits_MTN defines a M bit field with least significant bit N in a signed T.
//  \		PrimUnSignedBits_MTN defines a M bit field with least significant bit N in an unsigned T.
//  
//  		These bit classes all define the bit positions in a compiler and hardware independent
//  		fashion, and may be used as:
//  
//  \PrimUnsignedBitsdeclare(short,1,6);
//  \PrimSignedBitsdeclare(short,8,4);
//  \PrimSignedBitsdeclare(short,12,4);
//  \
//  \class Fred
//  \{
//  \		union
//  \		{
//  \			unsigned short bits;
//  \			PrimBit(short,0) bit_0;
//  \			PrimUnsignedBits(short,1,6) bit_1_6;
//  \			PrimBit(short,7) bit_7;
//  \			PrimBit(short,8) bit_8;
//  \			PrimSignedBits(short,8,4) bit_8_11;
//  \			PrimSignedBits(short,12,4) bit_12_15;
//  \		};
//  \		void doSomething()
//  \		{
//  \			if (bit_0)
//  \				bit_8_11 = bit_12_15;
//  \			else
//  \				bits = 0;
//  \		}
//  \};
//  
//  		The semantics of the use of a variable such as bit_12_15 are almost identical to the
//  		equivalent bit field. The only difference is that the union member cannot be mentioned
//  		as part of a ctor-initialiser (the constructor initialiser list).
//  
//  		There is no requirement for all bits to be used. Declarations may overlap to permit
//  		multiple names for the same location, or as in the example above to permit bulk access
//  		through "bits" as well as discreet access through the individual fields. Multiple sets
//  		of bit fields can be declared using multiple anonymous unions. Note that the size of each
//  		union is the size of the largest member, so if the members are of different sizes the
//  		byte positioning becomes compiler dependent - keep them all the same size.
//  
//  		All standard one bit fields are predeclared. Multiple bit fields must be explicitly declared.
//  
//  		When templates are available the () can be replaced by <>, and the declarations removed.
//  
#define PrimBit(T,aBit) name3(PrimBit_,T,aBit)
#define PrimBitdeclare(T,N)                  \
class PrimBit(T,N)                    \
{                        \
private:                      \
 unsigned T bits;                   \
 void operator&();                   \
public:                       \
 void operator=(int aBit) { aBit ? (bits |= (1 << N)) : (bits &= ~(1 << N)); }    \
 operator int() const { return ((bits >> N) & 1); }           \
}

#define PrimSignedBits(T,aBit,aRange) name4(PrimSignedBits_,aRange,T,aBit)
#define PrimSignedBitsdeclare(T,N,M)               \
class PrimSignedBits(T,N,M)                  \
{                        \
private:                      \
 unsigned T bits;                   \
 void operator&();                   \
public:                       \
 void operator=(T someBits)                 \
  { bits = (bits & ~(((1<<M)-1)<<N)) | (((unsigned T)someBits & ((1<<M)-1))<<N); }  \
 operator T() const { T temp = (bits >> N) & ((1 << M) - 1);         \
  return (temp < (1 << (M-1)) ? temp : (temp - (1<<M))); }        \
}

#define PrimUnsignedBits(T,aBit,aRange) name4(PrimUnsignedBits_,aRange,T,aBit)
#define PrimUnsignedBitsdeclare(T,N,M)               \
class PrimUnsignedBits(T,N,M)                 \
{                        \
private:                      \
 unsigned T bits;                   \
 void operator&();                   \
public:                       \
 void operator=(unsigned T someBits)               \
  { bits = (bits & ~(((1<<M)-1)<<N)) | (((someBits & ((1<<M)-1))<<N); }     \
 operator unsigned T() const { return ((bits >> N) & ((1 << M) - 1)); }      \
}

PrimBitdeclare(char,0);
PrimBitdeclare(char,1);
PrimBitdeclare(char,2);
PrimBitdeclare(char,3);
PrimBitdeclare(char,4);
PrimBitdeclare(char,5);
PrimBitdeclare(char,6);
PrimBitdeclare(char,7);

PrimBitdeclare(short,0);
PrimBitdeclare(short,1);
PrimBitdeclare(short,2);
PrimBitdeclare(short,3);
PrimBitdeclare(short,4);
PrimBitdeclare(short,5);
PrimBitdeclare(short,6);
PrimBitdeclare(short,7);
PrimBitdeclare(short,8);
PrimBitdeclare(short,9);
PrimBitdeclare(short,10);
PrimBitdeclare(short,11);
PrimBitdeclare(short,12);
PrimBitdeclare(short,13);
PrimBitdeclare(short,14);
PrimBitdeclare(short,15);

PrimBitdeclare(long,0);
PrimBitdeclare(long,1);
PrimBitdeclare(long,2);
PrimBitdeclare(long,3);
PrimBitdeclare(long,4);
PrimBitdeclare(long,5);
PrimBitdeclare(long,6);
PrimBitdeclare(long,7);
PrimBitdeclare(long,8);
PrimBitdeclare(long,9);
PrimBitdeclare(long,10);
PrimBitdeclare(long,11);
PrimBitdeclare(long,12);
PrimBitdeclare(long,13);
PrimBitdeclare(long,14);
PrimBitdeclare(long,15);
PrimBitdeclare(long,16);
PrimBitdeclare(long,17);
PrimBitdeclare(long,18);
PrimBitdeclare(long,19);
PrimBitdeclare(long,20);
PrimBitdeclare(long,21);
PrimBitdeclare(long,22);
PrimBitdeclare(long,23);
PrimBitdeclare(long,24);
PrimBitdeclare(long,25);
PrimBitdeclare(long,26);
PrimBitdeclare(long,27);
PrimBitdeclare(long,28);
PrimBitdeclare(long,29);
PrimBitdeclare(long,30);
PrimBitdeclare(long,31);

#endif

