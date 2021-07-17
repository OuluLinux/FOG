
#ifndef FOGRAW_HXX
#define FOGRAW_HXX



class FogRaw : public FogToken {
	typedef FogToken Super;
	typedef FogRaw This;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	FOGTOKEN_DERIVED_DECLS
	
private:
	FogLineRef _line;    //  .bugbug This variable is inelegant and almost unnecessary
	//   Elimination requires introduction of decorators in
	//   FogParserMaker::make_linked_declaration/statement/token
	//    for those tokens that do'nt have lines inherently.
	//   Specifiers and Statements do, TreeLiteral could.
	//    Others are probably rare and so decorable.
	
private:
	This& operator=(const This& aDeclaration);
	
private:
	This& mutate() const {
		return *(This*)this;
	}
	
protected:
	FogRaw();
	FogRaw(const This& aRaw);
	virtual ~FogRaw();
	
public:
	virtual bool compile_declaration(FogParseContext& parseContext) const;
	virtual const FogLazyBool& derive_at(FogScopeContext& inScope) const;
	virtual FogDerivedClause* is_derived_clause();
	const FogDerivedClause* is_derived_clause() const {
		return mutate().is_derived_clause();
	}
	
	virtual FogRaw* is_raw();
	virtual FogStatement* is_statement();
	virtual FogLine& line() const;
	virtual const FogMetaType& meta_type() const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	virtual void set_decl_specifier_raw(FogRawRef& aRaw, FogDeclSpecifier& declSpecifiers);
	virtual void set_explicit_instantiation();
	virtual void set_line(FogLine& aLine);
	virtual size_t source_lines() const;
	
public:
	static FogTypeSpecifier* new_filespace(FogModifier& aDecl);
	static FogTypeSpecifier* new_namespace();
	static FogTypeSpecifier* new_namespace(FogModifier& aDecl);
	static FogRaw* new_set_all(const FogFileId& fileId);
	//  	static FogRaw *new_set_implementation(const FogList& someCode);
	static FogRaw* new_set_implementation(const FogFileId& fileId);
	//  	static FogRaw *new_set_interface(const FogList& someCode);
	static FogRaw* new_set_interface(const FogFileId& fileId);
	static FogRaw* new_set_no_implementation();
	static FogRaw* new_set_no_interface();
	static FogRaw* new_set_utility(const FogUtility& aUtility);
	static FogRaw* new_use_implementation(const FogFileId& fileId);
	static FogRaw* new_use_interface(const FogFileId& fileId);
};

#endif

