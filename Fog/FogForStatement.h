
#ifndef FOGFORSTATEMENT_HXX
#define FOGFORSTATEMENT_HXX




class FogForStatement : public FogStatement {
	typedef FogStatement Super;
	typedef FogForStatement This;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_MEMBER_DECLS
	FOGTOKEN_LEAF_DECLS
private:
	FogExprRef _init;
	FogExprRefToConst _test;
	FogExprRef _step;
	FogRawRef _body;
private:
	This& mutate() const { return *(This*)this; }
protected:
	FogForStatement(const This& aStatement);
	virtual ~FogForStatement();
public:
	FogForStatement(FogExpr* initExpr, const FogExpr* testExpr, FogExpr* stepExpr, FogRaw& bodyStatement);
	virtual bool compile_parsed(FogParseContext& parseContext);
	virtual size_t executable_tokens() const;
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual void set_is_meta();
};
#endif

