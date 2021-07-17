
#ifndef FOGTYPEDNAME_HXX
#define FOGTYPEDNAME_HXX




class FogTypedName : public FogDecoratedName { //   name inherited
	typedef FogDecoratedName Super;
	typedef FogTypedName This;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_MEMBER_DECLS
	FOGTOKEN_LEAF_DECLS
	
private:
	//   Defining contents.
	FogNameRef _type;
	
private:
	This& mutate() const {
		return *(This*)this;
	}
	
	FogName& type() {
		return *_type;
	}
	
	const FogName& type() const {
		return *_type;
	}
	
protected:
	virtual ~FogTypedName();
	
public:
	FogTypedName(FogName& typeName, FogName& nameName);
	virtual const FogDeclSpecifierValue& decl_specifiers() const;
	virtual FogName* get_type();
	virtual FogTypedName* is_typed_name();
	virtual bool make_specifier(FogMakeSpecifierContext& makeSpecifierContext);
	virtual bool make_type_specifier(FogMakeSpecifierContext& makeSpecifierContext);
	virtual bool make_typed_expression(FogExprRef& theExpr, FogName& theType);
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
	virtual void set_decl_specifier_name(FogNameRef& aName, const FogDeclSpecifier& declSpecifiers);
	
public:
	static FogName* append_name(FogNameRef& frontName, FogName& backName);
};

#endif

