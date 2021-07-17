
#ifndef FOGTRYSTATEMENT_HXX
#define FOGTRYSTATEMENT_HXX




class FogTryStatement : public FogStatement {
	typedef FogStatement Super;
	typedef FogTryStatement This;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_MEMBER_DECLS
	FOGTOKEN_LEAF_DECLS
private:
	FogRawRef _statement;
	FogTokensRef _handlers;
private:
	This& mutate() const { return *(This*)this; }
protected:
	FogTryStatement(const This& aStatement);
	virtual ~FogTryStatement();
public:
	FogTryStatement(FogRaw& aStatement, FogTokens& exceptionHandlers);
	virtual size_t executable_tokens() const;
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual void set_is_meta();
};
#endif

