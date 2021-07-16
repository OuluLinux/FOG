// **************************** RACAL Confidential ****************************
// 
// 	  Title:			Defined macro class interface
// 
// 	  File Name:		CcpDefine.hxx
// 
// 	  Author:			E.D.Willink
// 
// 	  SCCS:				%W% %G%
// 
// 	  Latest Modification:
//  EDW	1.1		Date:	 8-Oct-1996		Original.
//  EDW	1.2		Date:	 2-Sep-1998		Use std streams
// END
// 

#ifndef CCPDEFINE_HXX
#define CCPDEFINE_HXX


// 
// 	A CcpDefine holds the definition of a defined macro. 
// 
class CcpDefine : public PrimPrint
{
 TYPEDECL_SINGLE(CcpDefine, PrimPrint)
 PRIMSHARES_DECLS(CcpDefine)
 PRIMREF_DECLS(CcpDefine)
private:
 const PrimIdHandle _id;     //  Identity of the define.
 PrimText _text;       //  Definition of the define.

protected:
// 
// 	Construct a null definition.
// 
 CcpDefine() {}

public:
// 
// 	Construct a definition called anId.
// 
 CcpDefine(const char *anId) : _id(anId) {}
// 
// 	Add aLine to the definition.
// 
 void add(const PrimString& aLine) { _text.append(aLine); }
// 
// 	Return the identity for map access.
// 
 const PrimId& id() const { return *_id; }
 virtual std::ostream& print_depth(std::ostream& s, int aDepth = 0) const;
 virtual std::ostream& print_this(std::ostream& s) const;
// 
// 	Return the text of the definition.
// 
 const PrimText& text() const { return _text; }
};

#endif
