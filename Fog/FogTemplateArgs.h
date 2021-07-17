
#ifndef FOGTEMPLATEARGS_HXX
#define FOGTEMPLATEARGS_HXX





class FogTemplateArgs : public FogToken {
	typedef FogToken Super;
	typedef FogTemplateArgs This;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	FOGTOKEN_DERIVED_DECLS
	FOGTOKEN_MEMBER_DECLS
	FOGTOKEN_LEAF_DECLS
private:
	enum IsValid {};                                    //   Useless type for pointer return.
private:
	FogTemplateArgListOfRefToConst _arguments;          //   List of actual arguments.
	mutable PrimIdHandle _id;                           //   Encoded id such as "< X , 4, char >"
private:
	FogTemplateArgs(const FogTemplateArgs& templateArgs);
	FogTemplateArgs& operator=(const FogTemplateArgs&);
private:
	const FogTemplateArg* adopt(const FogTemplateArg* anArgument);
	FogTemplateArgs& mutate() const { return *(FogTemplateArgs*)this; }
protected:
	virtual ~FogTemplateArgs();
public:
	FogTemplateArgs();
	operator const IsValid* () const { return (const IsValid*)(!is_null() ? this : 0); }
	bool operator!() const { return is_null(); }
	void add_argument(const FogTemplateParameterSpecifier& parameterSpecifier, FogExpr& anExpr);
	const FogTemplateArgListOfRefToConst& arguments() const { return _arguments; }
	virtual const PrimId& id() const;
	bool matches(FogMergeContext& mergeContext, const FogTemplateParameterSpecifiers& templateParameters) const;
	virtual std::ostream& print_this(std::ostream& s) const;
virtual std::ostream& print_viz(std::ostream& s) const; public:
};
#endif

