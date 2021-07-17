
#ifndef FOGTREELITERAL_HXX
#define FOGTREELITERAL_HXX



class FogTreeLiteral : public FogWordName {
	typedef FogWordName Super;
	typedef FogTreeLiteral This;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_MEMBER_DECLS
	FOGTOKEN_LEAF_DECLS
private:
	//   Defining contents
	FogExprRef _expr;                       //   The tree naming expression.
	const size_t _dollar_count;             //   Number of dollars in invocation, 0 for an @.
private:
	//   Derived contents
	FogTokenRef _actual;                    //   The actual resolved entity.
private:
	This& mutate() const { return *(This*)this; }
protected:
	FogTreeLiteral(const This& thatId);
	virtual ~FogTreeLiteral();
	FogTokenRefToConst& actual() { return _actual.to_const(); }
	virtual void make_actual_token(FogTokenRefToConst& actualToken, FogMakeActualContext& makeActualContext) const;
public:
	FogTreeLiteral(FogExpr& anExpr, size_t dollarCount);
	virtual bool compile_parsed(FogParseContext& parseContext);
	virtual size_t executable_tokens() const;
	virtual void find_entities_in(FogScopeContext& inScope, FogEntityFinding& theFinding) const;
	virtual const FogMetaArgument* find_formal_in(FogScopeContext& scopeContext) const;
	virtual bool find_meta_entity_in(FogTokenRef& returnValue, FogScopeContext& inScope) const;
	virtual bool get_object(FogTokenRef& tokenValue, FogScopeContext& inScope) const;
	virtual FogName* get_type();
	virtual bool has_at() const;
	virtual bool has_dollar() const;
	virtual bool is_compound() const;
	virtual const PrimId* is_resolved() const;
	virtual const FogTemplateParameterSpecifier* is_unnormalised_template_parameter
	(FogScopeContext& scopeContext) const;
	virtual bool make_specifier(FogMakeSpecifierContext& makeSpecifierContext);
	virtual bool morph_to(FogTokenRef& returnValue, const FogMetaType& metaType, IsExposed isExposed,
	                      FogScopeContext& inScope) const;
	virtual FogTokenType::TokenType pp_token_type_enum() const;
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	virtual bool resolve_id(PrimIdHandle& returnId, FogScopeContext& inScope) const;
	virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
	virtual FogTokenType::TokenType token_type_enum() const;
};
#endif

