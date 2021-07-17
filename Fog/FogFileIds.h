
#ifndef FOGFILEIDS_HXX
#define FOGFILEIDS_HXX




class FogFileIds : public FogToken {
	typedef FogToken Super;
	typedef FogFileIds This;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	FOGTOKEN_DERIVED_DECLS
	FOGTOKEN_ACTUAL_DECLS
private:
	FogFileIdListOfRefToConst _ids;
private:
	FogFileIds(const This&);
	FogFileIds& operator=(const This&);
protected:
	virtual ~FogFileIds();
public:
	FogFileIds();
	void add(const FogFileId& fileId) { _ids.add(fileId); }
	const FogFileIdListOfRefToConst& ids() const { return _ids; }
	virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_this(std::ostream& s) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	size_t tally() const { return _ids.tally(); }
};
#endif

