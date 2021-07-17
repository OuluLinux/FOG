
#ifndef FOGACTUALDECLARATION_HXX
#define FOGACTUALDECLARATION_HXX




class FogActualDeclaration : public PrimPrint {
	typedef PrimPrint Super;
	typedef FogActualDeclaration This;
	TYPEDECL_SINGLE(This, Super)
	
private:
	enum IsValid {};         //   Useless type for pointer return.
	
private:
	FogSpecifierRefToConst _specifier;
	FogSpecifierRef _mutable;      //   Locally owned actual declaration, if needed.
	
private:
	FogActualDeclaration(const This&);
	This& operator=(const This&);
	
public:
	FogActualDeclaration(const FogSpecifier* aSpecifier = 0);
	virtual ~FogActualDeclaration();
	operator const IsValid* () const {
		return (const IsValid*)(_specifier ? this : 0);
	}
	
	bool operator!() const {
		return !_specifier;
	}
	
	void destroy();
	void instantiate();
	void merge(FogMergeContext& mergeContext, const FogSpecifier& aSpecifier);
	FogSpecifier& mutable_specifier();
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_this(std::ostream& s) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	const FogSpecifier& specifier() const {
		return *_specifier;
	}
};

#endif

