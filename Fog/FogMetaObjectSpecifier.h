
#ifndef FOGMETAOBJECTSPECIFIER_HXX
#define FOGMETAOBJECTSPECIFIER_HXX



class FogMetaObjectSpecifier : public FogObjectSpecifier {
	typedef FogObjectSpecifier Super;
	typedef FogMetaObjectSpecifier This;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_UNIQUE_DECLS
protected:
	FogMetaObjectSpecifier();
	FogMetaObjectSpecifier(FogMakeSpecifierContext& makeSpecifierContext, FogName& nameName);
	virtual ~FogMetaObjectSpecifier();
	const FogMetaType& resolve_type_meta_type(FogScopeContext& scopeContext) const;
public:
	virtual bool compile_specifier(FogMakeSpecifierContext& makeSpecifierContext);
	const FogStatic& get_static() const;
	virtual IsExposed is_exposed() const;
	virtual FogMetaObjectSpecifier* is_meta_object_specifier();
	virtual FogEntity* make_name_entity(FogMakeEntityContext& makeEntityContext) const;
	virtual bool make_slot(FogMakerContext& makerContext) const = 0;
	virtual FogEntity* make_type_entity(FogMakeEntityContext& makeEntityContext) const;
};
#endif

