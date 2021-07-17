
#ifndef FOGRETURNSTATEMENT_HXX
#define FOGRETURNSTATEMENT_HXX




class FogReturnStatement : public FogStatement {
	typedef FogStatement Super;
	typedef FogReturnStatement This;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_MEMBER_DECLS
	FOGTOKEN_LEAF_DECLS
private:
	FogExprRefToConst _expr;
private:
	This& mutate() const { return *(This*)this; }
protected:
	FogReturnStatement(const This& aStatement);
	virtual ~FogReturnStatement();
public:
	FogReturnStatement(const FogExpr* anExpr);
	virtual size_t executable_tokens() const;
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual void set_is_meta();
};
#endif

