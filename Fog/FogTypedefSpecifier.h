
#ifndef FOGTYPEDEFSPECIFIER_HXX
#define FOGTYPEDEFSPECIFIER_HXX




class FogTypedefSpecifier : public FogObjectSpecifier {
	typedef FogObjectSpecifier Super;
	typedef FogTypedefSpecifier This;
	TYPEDECL_SINGLE(This, Super)
	FOGTOKEN_LEAF_DECLS
private:
	//    FogTypedefSpecifier(const This& aDecl);     -- default ok
protected:
	virtual ~FogTypedefSpecifier();
public:
	FogTypedefSpecifier(FogMakeSpecifierContext& makeSpecifierContext, FogName& nameName);
	virtual bool compile_specifier(FogMakeSpecifierContext& makeSpecifierContext);
	virtual bool emit(FogEmitContext& emitContext) const;
	virtual FogTypedefSpecifier* is_typedef_specifier();
	virtual FogEntity* make_name_entity(FogMakeEntityContext& makeEntityContext) const;
	virtual const FogMetaType& meta_type() const;
	virtual std::ostream& print_viz(std::ostream& s) const;
};
#endif

