
#ifndef FOGCATCHSTATEMENT_HXX
#define FOGCATCHSTATEMENT_HXX




class FogCatchStatement : public FogStatement {
	typedef FogStatement Super;
	typedef FogCatchStatement This;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	//      FOGTOKEN_DERIVED_DECLS
	FOGTOKEN_MEMBER_DECLS
	FOGTOKEN_LEAF_DECLS
private:
	FogExprRef _exception;
	FogRawRef _statement;
private:
	This& mutate() const { return *(This*)this; }
protected:
	FogCatchStatement();
	FogCatchStatement(const This& aStatement);
	virtual ~FogCatchStatement();
public:
	FogCatchStatement(FogExpr& anExpr, FogRaw& aStatement);
	virtual size_t executable_tokens() const;
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual void set_is_meta();
};
#endif

