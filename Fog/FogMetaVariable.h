
#ifndef FOGMETAVARIABLE_HXX
#define FOGMETAVARIABLE_HXX



class FogMetaVariable : public FogMetaVariableSlot {
	NEEDS_A_FRIEND_FOR_GNUC
	typedef FogMetaVariableSlot Super;
	typedef FogMetaVariable This;
	TYPEDECL_SINGLE(This, Super)
private:
	FogMetaVariable(const This&);
	This& operator=(const This&);
private:
	virtual ~FogMetaVariable();
	virtual FogMetaEntity* new_meta_entity(FogToken& inToken) const;
public:
	FogMetaVariable(FogMakerContext& makerContext,
	                const FogStatic& aStatic, const FogMetaType& metaType, IsExposed isExposed, const FogToken& aToken);
	virtual std::ostream& print_viz(std::ostream& s) const;
};
#endif

