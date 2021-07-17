
#ifndef FOGTYPENAME_HXX
#define FOGTYPENAME_HXX



class FogTypeName : public FogEntity {
	typedef FogEntity Super;
	TYPEDECL_SINGLE(FogTypeName, Super)
	PRIMREF_DERIVED_DECLS(FogTypeName)
private:
	bool _built_in;        //   True if a built-in type.
private:
	virtual FogUsage* make_name_usage();
	FogTypeName& mutate() const { return *(FogTypeName*)this; }
protected:
	FogTypeName();
public:
	FogTypeName(FogMakerContext& makerContext, bool builtIn = false);
	virtual ~FogTypeName();
	virtual void add_use_by(FogEntity& anEntity, const FogOfUseBy& ofUseBy);
	virtual void emit_forward_reference(FogStream& s);
	virtual FogUsage& interface_usage_finish();
	virtual bool is_type() const;
	virtual const FogMetaType& meta_type() const;
	virtual FogTargetFile* name_file();
	virtual std::ostream& print_this(std::ostream& s) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
public:
	static int compare(const FogTypeNameRefToConst* p1, const FogTypeNameRefToConst* p2);
};
#endif

