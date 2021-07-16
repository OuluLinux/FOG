
#ifndef FOGLABELSTATEMENT_HXX
#define FOGLABELSTATEMENT_HXX




class FogLabelStatement : public FogStatement
{
 typedef FogStatement Super;
 typedef FogLabelStatement This;
 TYPEDECL_SINGLE(This, Super)
 FOGTOKEN_MEMBER_DECLS
 FOGTOKEN_LEAF_DECLS
private:
 FogNameRef _label;
 FogRawRef _statement;
private:
 This& mutate() const { return *(This *)this; }
protected:
 FogLabelStatement(const This& aStatement);
 virtual ~FogLabelStatement();
public:
 FogLabelStatement(FogName& aLabel, FogRaw& aStatement);
 virtual size_t executable_tokens() const;
 virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
 virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
    virtual void set_is_meta();
};
#endif
 
