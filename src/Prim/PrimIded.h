
#ifndef PRIMIDED_HXX
#define PRIMIDED_HXX




//  
//  	A PrimIded defines a class with standard printing facilities and a name that uses shared
//  	storage for similar names.
//  
class PrimIded : public PrimPrint
{
 TYPEDECL_SINGLE(PrimIded, PrimPrint)
private:
 const PrimIdHandle _id;     //   The ident.
public:
//  
//  	Create a new instance identifued by the null id.
//  
 PrimIded() {}
//  
//  	Create a new instance identified by anId.
//  
 PrimIded(const PrimId& anId) : _id(anId) {}
 virtual ~PrimIded();
//  
//  	Report whether this instance has anId as its identity.
//  
//  	bool operator==(const PrimId& anId) const { return (_id == anId); }
#ifndef NO_PRIM_STREAMS
 virtual std::ostream& print_this(std::ostream& s) const;
#endif
//  
//  	Report the identity.
//  
 const PrimId& id() const { return (*_id); }
};
#endif
 
