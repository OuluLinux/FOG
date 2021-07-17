
#ifndef FOGRAWDECLARATIONS_HXX
#define FOGRAWDECLARATIONS_HXX



class FogRawDeclarations : public FogRaws {
	typedef FogRawDeclarations This;
	typedef FogRaws Super;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_LEAF_DECLS
protected:
	virtual ~FogRawDeclarations();
public:
	FogRawDeclarations();
	virtual void create_usage(FogUsageContext& usageContext) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
};
#endif

