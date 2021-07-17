
#ifndef FOGMETAITERATOR_HXX
#define FOGMETAITERATOR_HXX



class FogMetaIterator : public FogMetaVariableSlot {
	NEEDS_A_FRIEND_FOR_GNUC
	typedef FogMetaVariableSlot Super;
	typedef FogMetaIterator This;
	TYPEDECL_SINGLE(This, Super)
private:
	virtual ~FogMetaIterator();
	virtual FogMetaEntity* new_meta_entity(FogToken& inToken) const;
public:
	FogMetaIterator(FogMakerContext& makerContext,
	                const FogStatic& aStatic, const FogMetaType& metaType, IsExposed isExposed, const FogToken& aToken);
	virtual std::ostream& print_viz(std::ostream& s) const;
};
#endif

