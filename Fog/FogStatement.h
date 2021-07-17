
#ifndef FOGSTATEMENT_HXX
#define FOGSTATEMENT_HXX



class FogStatement : public FogRaw {
	typedef FogRaw Super;
	typedef FogStatement This;
	TYPEDECL_SINGLE(This, Super)
private:
	//      FogLineRef _line;                         // Inherited from FogRaw.
	bool _is_meta;
private:
	This& operator=(const This&);
protected:
	FogStatement();
	FogStatement(const This& aStatement);
	virtual ~FogStatement();
public:
	virtual bool compile_parsed(FogParseContext& parseContext);
	bool is_meta() const { return _is_meta; }
	virtual FogStatement* is_statement();
	//      virtual FogLine& line() const;
	virtual const FogMetaType& meta_type() const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
	virtual void set_is_meta();
	//      virtual void set_line(FogLine& aLine);
};
#endif

