
#ifndef FOGMEMBEREXPR_HXX
#define FOGMEMBEREXPR_HXX



 
class FogMemberExpr : public FogDecoratedExpr
{
    typedef FogDecoratedExpr Super;
    typedef FogMemberExpr This;
    TYPEDECL_SINGLE(This, Super)
    FOGTOKEN_MEMBER_DECLS
private:
    FogNameRefToConst _member;
private:
    This& mutate() const { return *(This *)this; }
protected:
    FogMemberExpr(const This& anExpr);
    FogMemberExpr(FogExpr& anExpr, FogName& aMember);
    virtual ~FogMemberExpr();
    const FogName& member() const { return *_member; }
public:
    virtual size_t executable_tokens() const;
    virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
    virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
};

#define FOGMEMBEREXPR_INTERFAC(T) \
class T : public FogMemberExpr \
{ \
    typedef FogMemberExpr Super; \
    typedef T This; \
    TYPEDECL_SINGLE(This, Super) \
    FOGTOKEN_LEAF_DECLS \
private: \
    T(const This& anExpr) : Super(anExpr) {} \
public: \
    T(FogExpr& anExpr, FogName& aMember) : Super(anExpr, aMember) {}

#define FOGMEMBEREXPR_INTERFACE(T) \
FOGMEMBEREXPR_INTERFAC(T) \
};

FOGMEMBEREXPR_INTERFAC(FogArrowExpr)
    virtual bool emit(FogEmitContext& emitContext) const;
    virtual bool get_object(FogTokenRef& tokenValue, FogScopeContext& inScope) const;
    virtual char print_named(std::ostream& s, const PrimId *scopeId, char tailChar) const;
};
FOGMEMBEREXPR_INTERFAC(FogDotExpr)
    virtual bool emit(FogEmitContext& emitContext) const;
    virtual bool get_object(FogTokenRef& tokenValue, FogScopeContext& inScope) const;
    virtual char print_named(std::ostream& s, const PrimId *scopeId, char tailChar) const;
};
#endif
 
