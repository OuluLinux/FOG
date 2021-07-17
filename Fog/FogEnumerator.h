
#ifndef FOGENUMERATOR_HXX
#define FOGENUMERATOR_HXX



class FogEnumerator : public FogEntity {
	typedef FogEntity Super;
	TYPEDECL_SINGLE(FogEnumerator, Super)
	PRIMREF_DERIVED_DECLS(FogEnumerator)
private:                                //   Defining contents.
	const FogExprRefToConst _init;          //   Enumerator value.
	const FogEnumerator* _previous;         //   Previous enumerator in list, 0 if first.
	bool _has_previous : 1;                 //   Set true by set_previous
	mutable bool _has_value : 1;            //   Set true by resolve_value
	mutable FogTokenRef _number;            //   Cached representation of value.
private:
	FogEnumerator& operator=(const FogEnumerator& anEnumerator);
	FogEnumerator(const FogEnumerator& anEnumerator);
	FogEnumerator& mutate() const { return *(FogEnumerator*)this; }
	void resolve_value() const;
	void resolve_value(FogTokenRef& returnValue, FogScopeContext& inScope) const;
protected:
	FogEnumerator();
	virtual ~FogEnumerator();
public:
	FogEnumerator(FogScope& aScope, const PrimId& anId, const FogExpr& initExpr);
	void emit_interface(FogStream& s);
	virtual bool get_number_token(FogTokenRef& returnValue, FogScopeContext& inScope) const;
	virtual bool is_active() const;
	virtual FogEnumerator* is_enumerator();
	bool matches(FogEnumerator& anEnumerator);
	virtual const FogMetaType& meta_type() const;
	virtual std::ostream& print_this(std::ostream& s) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	FogScope& scope() { return non_virtual_scope(); }
	const FogScope& scope() const { return non_virtual_scope(); }
	void set_previous(const FogEnumerator* anEnumerator);
	friend bool operator==(const FogEnumerator& firstEnumerator, const FogEnumerator& secondEnumerator) {
		return (firstEnumerator.short_id() == secondEnumerator.short_id())
		       && (firstEnumerator._init == secondEnumerator._init);
	}
};
#endif

