
#ifndef FOGBREAKSTATEMENT_HXX
#define FOGBREAKSTATEMENT_HXX




class FogBreakStatement : public FogStatement {
	typedef FogStatement Super;
	typedef FogBreakStatement This;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_MEMBER_DECLS
	FOGTOKEN_LEAF_DECLS
	
private:
	enum Enum { IS_BREAK, IS_CONTINUE };
	Enum _is_continue;
	
public:
	enum BreakEnum { BREAK };
	enum ContinueEnum { CONTINUE };
	
protected:
	FogBreakStatement(const This& aStatement);
	virtual ~FogBreakStatement();
	
public:
	FogBreakStatement(BreakEnum);
	FogBreakStatement(ContinueEnum);
	virtual bool compile_parsed(FogParseContext& parseContext);
	virtual size_t executable_tokens() const;
};

#endif

