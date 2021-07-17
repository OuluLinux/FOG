
#ifndef FOGMETAFUNCTIONSPECIFIER_HXX
#define FOGMETAFUNCTIONSPECIFIER_HXX





class FogMetaFunctionSpecifier : public FogMetaObjectSpecifier {
	typedef FogMetaObjectSpecifier Super;
	typedef FogMetaFunctionSpecifier This;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_UNIQUE_DECLS
private:
	bool _is_constructor;
	bool _is_destructor;
private:
	FogMetaParameterSpecifierListOfRefToConst _formals;     //   MetaFunction formal parameters in sequential order.
	FogTokenStatementsRefToConst _body;
	FogPositionHandle _position;                //   Positioning of body.
protected:
	virtual ~FogMetaFunctionSpecifier();
public:
	FogMetaFunctionSpecifier(FogMakeSpecifierContext& makeSpecifierContext, FogName& nameName);
	void add_parameter(FogMetaParameterSpecifier& metaParameter);
	virtual bool compile_specifier(FogMakeSpecifierContext& makeSpecifierContext);
	bool is_exclusive() const { return _position->is_exclusive(); }
	virtual FogMetaFunctionSpecifier* is_meta_function_specifier();
	virtual bool make_slot(FogMakerContext& makerContext) const;
	virtual const FogMetaType& meta_type() const;
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	void set_body(const FogTokenStatements& theBody);
	void set_position(const FogPosition& aPosition) { _position = aPosition; }
public:
	static FogName& constructor_id();
	static FogName& destructor_id();
};
#endif

