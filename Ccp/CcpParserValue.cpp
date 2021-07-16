// **************************** RACAL Confidential ****************************
// 
// 	  Title:			Parse Data class implementation
// 
// 	  File Name:		CcpParserValue.cxx
// 
// 	  Author:			E.D.Willink
// 
// 	  SCCS:				@(#)CcpParserValue.cxx	1.1 06/30/97
// 
// 	  Latest Modification:
//  EDW	1.1		Date:	 8-Oct-1996		Original
// END
// 

#include <Ccp/CcpIncludeAll.h>

// 
// 	Construct an empty parsed value.
// 
CcpParserValue::CcpParserValue()
{
 new (token()) CcpToken;
}

// 
// 	Destruct a parsed value, destructing any union contents.
// 
CcpParserValue::~CcpParserValue()
{
 CcpToken *aToken = &token();
 delete aToken;
}

// 
// 	Assign parserValue to this parsed value, reconfiguring the union to the required data type.
// 
CcpParserValue& CcpParserValue::operator=(const CcpParserValue& parserValue)
{
 if (&token() != &parserValue.token())
 {
  CcpToken *aToken = &token();
  delete aToken;
  parserValue.token().clone_to(token());
 }
 return (*this);
}
