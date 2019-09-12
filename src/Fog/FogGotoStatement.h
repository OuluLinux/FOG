
#ifndef FOGGOTOSTATEMENT_HXX
#define FOGGOTOSTATEMENT_HXX




class FogGotoStatement : public FogStatement
{
    typedef FogStatement Super;
    typedef FogGotoStatement This;
    TYPEDECL_SINGLE(This, Super)
    FOGTOKEN_MEMBER_DECLS
    FOGTOKEN_LEAF_DECLS
private:
    FogNameRef _label;
private:
    This& mutate() const { return *(This *)this; }
protected:
    FogGotoStatement(const This& aStatement);
    virtual ~FogGotoStatement();
public:
    FogGotoStatement(FogName& aLabel);
    virtual size_t executable_tokens() const;
    virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
    virtual void set_is_meta();
};
#endif
 
