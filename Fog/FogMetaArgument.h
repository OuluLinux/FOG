
#ifndef FOGMETAARGUMENT_HXX
#define FOGMETAARGUMENT_HXX



class FogMetaArgument : public FogMetaVariableSlot {
	typedef FogMetaVariableSlot Super;
	typedef FogMetaArgument This;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
public:
	enum Index {
		DYNAMIC = -3,                       //   Meaningless index denoting the $Dynamic context.
		STATIC = -2,                        //   Meaningless index denoting the $Static context.
		INVALID = -1,                       //   Meaningless index for an invlaid argument
		FIRST = 0                           //   First of unlimited number of positional arguments.
	};
private:
	const Index _index;                     //   Positional index.
private:
	FogMetaArgument(const This&);
	This& operator=(const This&);
	This& mutate() const { return *(This*)this; }
protected:
	FogMetaArgument() : _index(INVALID) {}
	virtual ~FogMetaArgument();
	virtual FogToken* clone() const;
public:
	FogMetaArgument(size_t anIndex, const FogMetaType& metaType, IsExposed isExposed, const PrimId& anId,
	                const FogToken* aToken)
		: Super(FogToken::mutable_null(), metaType, isExposed, anId, aToken), _index(Index(anIndex)) {}
	virtual bool get_object(FogTokenRef& returnValue, FogScopeContext& inScope) const;
	Index index() const { return _index; }
	virtual bool is_actual(const FogScopeContext& scopeContext) const;
	virtual const FogMetaArgument* is_meta_argument() const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	virtual bool resolve_object(FogTokenRef& returnValue, FogScopeContext& inScope) const;
private:
	static bool matches(const FogMetaArgument& firstArgument, const FogMetaArgument& secondArgument);
public:
	static bool matches(const FogMetaArgumentListOfRefToConst& firstArgument,
	                    const FogMetaArgumentListOfRefToConst& secondArgument);
};
#endif

