
#ifndef FOGACCESSIBILITYSPECIFIER_HXX
#define FOGACCESSIBILITYSPECIFIER_HXX




class FogAccessibilitySpecifier : public FogRaw {
	typedef FogRaw Super;
	typedef FogAccessibilitySpecifier This;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_LEAF_DECLS
	
private:
	const FogAccessHandle _access;
	
public:
	FogAccessibilitySpecifier(const FogAccess& anAccess);
	virtual bool compile_parsed(FogParseContext& parseContext);
	virtual char print_named(std::ostream& s, const PrimId* scopeId, char tailChar) const;
	virtual std::ostream& print_this(std::ostream& s) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	virtual void set_line(FogLine& aLine);
};

#endif

