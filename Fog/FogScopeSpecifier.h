
#ifndef FOGSCOPESPECIFIER_HXX
#define FOGSCOPESPECIFIER_HXX




class FogScopeSpecifier : public FogSpecifier {
	typedef FogSpecifier Super;
	typedef FogScopeSpecifier This;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	FOGTOKEN_DERIVED_DECLS
private:
	const FogTagHandle _tag;
private:
	This& mutate() const { return *(This*)this; }
protected:
	FogScopeSpecifier();
	FogScopeSpecifier(const This& thatSpecifier);
	FogScopeSpecifier(const FogTag& aTag, FogName& aName);
	virtual ~FogScopeSpecifier() {}
public:
	virtual bool compile_specifier(FogMakeSpecifierContext& makeSpecifierContext);
	virtual bool emit(FogEmitContext& emitContext) const;
	virtual FogScopeSpecifier* is_scope_specifier();
	virtual FogEntity* make_name_entity(FogMakeEntityContext& makeEntityContext) const;
	virtual bool make_type_specifier(FogMakeSpecifierContext& makeSpecifierContext);
	virtual const FogMetaType& meta_type() const;
	virtual char print_named(std::ostream& s, const PrimId* scopeId, char tailChar) const;
	virtual const FogTag& tag() const;
};
#endif

