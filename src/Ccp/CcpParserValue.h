// **************************** RACAL Confidential ****************************
// 
// 	  Title:			Parse Data class interface
// 
// 	  File Name:		CcpParserValue.hxx
// 
// 	  Author:			E.D.Willink
// 
// 	  SCCS:				@(#)CcpParserValue.hxx	1.1 06/30/97
// 
// 	  Latest Modification:
//  EDW	1.1		Date:	 8-Oct-1996		Original.
// END
// 

#ifndef CCPPARSERVALUE_HXX
#define CCPPARSERVALUE_HXX



// 
// 	CcpParserValue is the data type used as yylval, it is therefore a union of all data types detected
// 	by the lexer and subsequently used by the parser, and also of data types used exclusively by the parser.
// 	Each of these data types is a derived class of CcpToken.
// 	Some of the data types are fully fledged classes to support share counting etc. It is not possible
// 	to incorporate such types in a union. The internal union is therefore implemented as a memory
// 	buffer in which an appropriate derived instance of CcpToken is constructed to represent the
// 	current content. 
// 
class CcpParserValue
{
private:
 union
 {
  char _null[sizeof(CcpToken)];
// 
// 	Declare the memory reservations for the parsed data value class T whose data type is D and which
// 	is accessed by methods named N.
// 
#define CCPTOKEN_MACRO_REF(T,C,D,R,N) char name2(_,N)[sizeof(T)];
#define CCPTOKEN_MACRO_VAL(T,D,N) char name2(_,N)[sizeof(T)];
 CCPTOKEN_LIST_REF
 CCPTOKEN_LIST_VAL
#undef CCPTOKEN_MACRO_REF
#undef CCPTOKEN_MACRO_VAL
 };

private:
// 
// 	Return the parse object.
// 
 const CcpToken& token() const { return *(const CcpToken *)&_null[0]; }
 CcpToken& token() { return *(CcpToken *)&_null[0]; }

public:
 CcpParserValue();
// 
// 	Construct a copy of parserValue.
// 
 CcpParserValue(const CcpParserValue& parserValue) { parserValue.token().clone_to(token()); }
 CcpParserValue& operator=(const CcpParserValue& parserValue);
 ~CcpParserValue();

// 
// 	Declare the member access functions for the parsed data value class T whose data type is D and which
// 	is accessed by methods named N.
// 
#define CCPTOKEN_MACRO_REF(T,C,D,R,N) \
 C D& N() const { return token().N(); } \
 name2(D,R)& N() { return token().N(); }
#define CCPTOKEN_MACRO_VAL(T,D,N) \
 const D& N() const { return token().N(); } \
 D& N() { return token().N(); }
 CCPTOKEN_LIST_REF
 CCPTOKEN_LIST_VAL
#undef CCPTOKEN_MACRO_REF
#undef CCPTOKEN_MACRO_VAL
};

#endif
