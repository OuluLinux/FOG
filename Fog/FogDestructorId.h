
#ifndef FOGDESTRUCTORID_HXX
#define FOGDESTRUCTORID_HXX



class FogDestructorId : public FogEntityName
{
 NEEDS_A_FRIEND_FOR_GNUC
 typedef FogEntityName Super;
 typedef FogDestructorId This;
 TYPEDECL_SINGLE(This, Super)
 FOGTOKEN_MEMBER_DECLS
 FOGTOKEN_LEAF_DECLS
private:
 FogNameRef _name;        //   The untwidded name
 mutable PrimIdHandle _resolved_id;
private:
 FogDestructorId(const This& anId);
 virtual ~FogDestructorId();
 This& mutate() const { return *(This *)this; }
public:
 FogDestructorId(FogName& aName);
 virtual size_t executable_tokens() const;
 virtual FogDestructorId *is_destructor_id();
 virtual const PrimId *is_resolved() const;
 const FogName& name() const { return *_name; }
 virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
 virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
 virtual std::ostream& print_viz(std::ostream& s) const;
 virtual bool resolve_id(PrimIdHandle& returnId, FogScopeContext& inScope) const;
 virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
 virtual void set_destructor_name(FogNameRef& aName);
 virtual NameSort sort_class() const;
};
#endif
 
