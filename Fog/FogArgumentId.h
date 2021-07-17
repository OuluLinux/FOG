
#ifndef FOGARGUMENTID_HXX
#define FOGARGUMENTID_HXX



class FogArgumentId : public FogWordName {
	typedef FogWordName Super;
	typedef FogArgumentId This;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_MEMBER_DECLS
	FOGTOKEN_LEAF_DECLS
	
private:
	//   Defining contents
	FogExprRef _expr;       //   The tree naming expression.
	const size_t _dollar_count;    //   Number of dollars in invocation.
	const FogMetaArgument& _argument;   //   Identity of a formal argument
	
private:
	//   Derived contents
	FogTokenRef _actual;      //   The actual resolved entity.
	
private:
	This& mutate() const {
		return *(This*)this;
	}
	
protected:
	FogArgumentId(const This& thatId);
	virtual ~FogArgumentId();
	FogTokenRefToConst& actual() {
		return _actual.to_const();
	}
	
public:
	FogArgumentId(FogExpr& anExpr, size_t dollarCount, const FogMetaArgument& aSlot);
	virtual bool compile_parsed(FogParseContext& parseContext);
	virtual size_t executable_tokens() const;
	virtual void find_entities_in(FogScopeContext& inScope, FogEntityFinding& theFinding) const;
	virtual bool find_meta_entity_in(FogTokenRef& returnValue, FogScopeContext& inScope) const;
	virtual bool get_object(FogTokenRef& tokenValue, FogScopeContext& inScope) const;
	virtual FogName* get_type();
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

