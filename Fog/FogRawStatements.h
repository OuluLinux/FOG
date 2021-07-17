
#ifndef FOGRAWSTATEMENTS_HXX
#define FOGRAWSTATEMENTS_HXX



class FogRawStatements : public FogRaws {
	typedef FogRawStatements This;
	typedef FogRaws Super;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_LEAF_DECLS
private:
	IsExposed _is_exposed;
protected:
	virtual ~FogRawStatements();
public:
	FogRawStatements();
	virtual void create_usage(FogUsageContext& usageContext) const;
	virtual IsExposed is_exposed() const;
	virtual void make_actual_from(FogMakeActualContext& makeActualContext);
	virtual char print_named(std::ostream& s, const PrimId* scopeId, char tailChar) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	virtual void set_exposed();
};
#endif

