// **************************** RACAL Confidential ****************************
// 
// 	  Title:			Lexer and Parser token class interfaces
// 
// 	  File Name:		CcpToken.hxx
// 
// 	  Author:			E.D.Willink
// 
// 	  SCCS:				%W% %G%
// 
// 	  Latest Modification:
//  EDW	1.1		Date:	 8-Oct-1996		Original.
//  EDW	1.2		Date:	28-Apr-1998		Adjust to PrimStringHandle
// 				Date:	16-Mar-1999		Add placement delete
// END
// 

#ifndef CCPTOKEN_HXX
#define CCPTOKEN_HXX

#define CCPTOKEN_READ_CHECK


// 
// 	Declare the list of macro invocations to permit an appropriate definition of CCPTOKEN_MACRO_xxx to
// 	generate code for each possible token.
// 
#define CCPTOKEN_LIST_REF
// 	CCPTOKEN_MACRO_REF(CcpTokenName,const,CcpName,RefToConst,name) \
// 	CCPTOKEN_MACRO_REF(CcpTokenNames,,CcpNames,Ref,names) \
// 	CCPTOKEN_MACRO_REF(CcpTokenTerm,const,FolTerm,RefToConst,term) \
// 	CCPTOKEN_MACRO_REF(CcpTokenTerms,,FolTerms,Ref,terms)
#define CCPTOKEN_LIST_VAL \
 CCPTOKEN_MACRO_VAL(CcpTokenString,PrimStringHandle,string) \
 CCPTOKEN_MACRO_VAL(CcpTokenNumber,int,number)

// 
// 	CcpToken provides the abstract behaviour for a parsed token. Derived classes implement a specific
// 	parsed data type. The parsed values are used polymorphically in a private memory buffer forming part
// 	of CcpParserValue that implements yylval. Construction is therefore exclusively by placement in
// 	a provided memory buffer.
// 
// .limits
// 	This is a private class of CcpParserValue. Instances can only be created by and within CcpParserValue.
// 
class CcpToken
{
 friend class CcpParserValue;
protected:
 void *operator new(size_t aSize) { return (0); }
 void *operator new(size_t aSize, CcpToken& p) { return (&p); }
 void operator delete(void *p, size_t aSize);
#ifndef IGNORES_OVERLOADED_DELETE
 void operator delete(void *p, CcpToken& q) {}
#endif
 virtual ~CcpToken() {}
private:
 virtual void clone_to(CcpToken& p) const;
 virtual const char *data_type() const;
// 
// 	Declare the member access functions for the parsed data value class T whose data type is D and which
// 	is accessed by methods named N.
// 
#ifdef CCPTOKEN_READ_CHECK
#define CCPTOKEN_READ_KEYWORD virtual
#else
#define CCPTOKEN_READ_KEYWORD inline
#endif
#define CCPTOKEN_MACRO_REF(T,C,D,R,N) \
 CCPTOKEN_READ_KEYWORD C D& N() const; \
 virtual name2(D,R)& N();
#define CCPTOKEN_MACRO_VAL(T,D,N) \
 CCPTOKEN_READ_KEYWORD const D& N() const; \
 virtual D& N();
CCPTOKEN_LIST_REF
CCPTOKEN_LIST_VAL
#undef CCPTOKEN_MACRO_REF
#undef CCPTOKEN_MACRO_VAL
#undef CCPTOKEN_READ_KEYWORD
};

// 
// 	Declare a derived parser value class T whose data type is defined by D and which
// 	is accessed by methods named N.
// 
#ifdef CCPTOKEN_READ_CHECK
#define CCPTOKEN_MACRO_REF(T,C,D,R,N) \
class T : public CcpToken \
{ \
 friend class CcpToken; \
 name2(D,R) name2(_,N); \
private: \
 virtual ~T(); \
 virtual void clone_to(CcpToken& p) const; \
 virtual const char *data_type() const; \
 virtual C D& N() const; \
 virtual name2(D,R)& N(); \
};
#define CCPTOKEN_MACRO_VAL(T,D,N) \
class T : public CcpToken \
{ \
 friend class CcpToken; \
 D name2(_,N); \
private: \
 virtual ~T(); \
 virtual void clone_to(CcpToken& p) const; \
 virtual const char *data_type() const; \
 virtual const D& N() const; \
 virtual D& N(); \
};
#else
#define CCPTOKEN_MACRO_REF(T,C,D,R,N) \
class T : public CcpToken \
{ \
 friend class CcpToken; \
 name2(D,R) name2(_,N); \
private: \
 virtual ~T(); \
 virtual void clone_to(CcpToken& p) const; \
 virtual const char *data_type() const; \
 virtual name2(D,R)& N(); \
}; \
inline C D& CcpToken::N() const { return *((const T *)this)->name2(_,N); }
#define CCPTOKEN_MACRO_VAL(T,D,N) \
class T : public CcpToken \
{ \
 friend class CcpToken; \
 D name2(_,N); \
private: \
 virtual ~T(); \
 virtual void clone_to(CcpToken& p) const; \
 virtual const char *data_type() const; \
 virtual D& N(); \
}; \
inline const D& CcpToken::N() const { return ((const T *)this)->name2(_,N); }
#endif
CCPTOKEN_LIST_REF
CCPTOKEN_LIST_VAL
#undef CCPTOKEN_MACRO_REF
#undef CCPTOKEN_MACRO_VAL

#endif
