
#ifndef FOGMETAFUNCTIONENTITY_HXX
#define FOGMETAFUNCTIONENTITY_HXX



class FogMetaFunctionEntity : public FogMetaEntity {
	typedef FogMetaEntity Super;
	typedef FogMetaFunctionEntity This;
	TYPEDECL_SINGLE(This, Super)
private:
	FogMetaFunctionEntity(const This&);
	This& operator=(const This&);
	This& mutate() const { return *(This*)this; }
protected:
	virtual ~FogMetaFunctionEntity();
public:
	FogMetaFunctionEntity(FogToken& inToken, const FogMetaFunctionSlot& metaFunction);
	const FogMetaFunctionSlot& function() const { return (const FogMetaFunctionSlot&)slot(); }
	virtual bool get_dyadic(FogTokenRef& returnValue, FogScopeContext& inScope,
	                        FogTokenType::TokenType dyadicOp, const FogToken& rightValue) const;
	virtual FogMetaFunctionEntity* is_meta_function_entity();
	virtual const FogMetaType& meta_type() const;
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	virtual bool set_value(const FogToken& tokenValue, FogScopeContext& inScope);
};
#endif

