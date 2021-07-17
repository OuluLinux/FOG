
#ifndef FOGASMSPECIFIER_HXX
#define FOGASMSPECIFIER_HXX



class FogAsmSpecifier : public FogSpecifier {
	typedef FogSpecifier Super;
	typedef FogAsmSpecifier This;
	TYPEDECL_SINGLE(This, Super)
	//      PRIMREF_DERIVED_DECLS(This)
	FOGTOKEN_LEAF_DECLS
private:
	const FogStringRef _asm_text;
protected:
	FogAsmSpecifier();
	virtual ~FogAsmSpecifier();
public:
	FogAsmSpecifier(FogString& asmText);
	virtual bool compile_parsed(FogParseContext& parseContext);
	virtual bool emit(FogEmitContext& emitContext) const;
	virtual FogEntity* make_name_entity(FogMakeEntityContext& makeEntityContext) const;
	virtual FogEntity* make_type_entity(FogMakeEntityContext& makeEntityContext) const;
	virtual const FogMetaType& meta_type() const;
	virtual char print_named(std::ostream& s, const PrimId* scopeId, char tailChar) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
};
#endif

