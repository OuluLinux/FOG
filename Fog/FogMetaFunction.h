
#ifndef FOGMETAFUNCTION_HXX
#define FOGMETAFUNCTION_HXX





class FogMetaFunction : public FogMetaFunctionSlot {
	typedef FogMetaFunctionSlot Super;
	typedef FogMetaFunction This;
	TYPEDECL_SINGLE(FogMetaFunction, This)
private:
	FogTokenStatementsRefToConst _token;    //   Token defining the function body.
	bool _is_exclusive;                     //   True if EXCLUSIVE positioning.
protected:
	virtual ~FogMetaFunction();
public:
	FogMetaFunction(FogMakerContext& makerContext,
	                const FogStatic& aStatic, const FogMetaType& metaType, IsExposed isExposed,
	                const FogMetaParameterSpecifierListOfRefToConst& formalParameters, const FogTokenStatements& theToken);
	bool is_exclusive() const { return _is_exclusive; }
	virtual const FogMetaFunction* is_meta_function() const;
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual bool resolve_function(FogTokenRef& returnValue,  FogCallContext& callContext) const;
	//    const FogToken& token() const { return *_token; }
};
#endif

