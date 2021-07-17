
#ifndef FOGDECLSPECIFIERVALUE_HXX
#define FOGDECLSPECIFIERVALUE_HXX



class FogDeclSpecifierValue : public FogDeclSpecifierEnums {
private:
	Mask _mask;
	
private:
	bool is_pure() const { return (_mask & PURE_MASK) == PURE_MASK; }
public:
	FogDeclSpecifierValue(Mask aMask = Mask(0)) : _mask(aMask) {}
	FogDeclSpecifierValue& operator=(Mask aMask) { _mask = aMask; return *this; }
	FogDeclSpecifierValue& operator|=(const FogDeclSpecifierValue& aValue) { _mask = Mask(_mask | aValue._mask); return *this; }
	FogDeclSpecifierValue& operator|=(Mask aMask) { _mask = Mask(_mask | aMask); return *this; }
	FogDeclSpecifierValue& operator&=(Mask aMask) { _mask = Mask(_mask & aMask); return *this; }
	bool deduce_template_parameter_type(const FogDeclSpecifierValue& actualDeclSpecifiers,
	                                    FogNameRef& deducedType) const;
	bool emit(FogEmitContext& emitContext) const;
	bool is_auto() const { return (_mask & AUTO_MASK) != 0; }
	bool is_const() const { return (_mask & CONST_MASK) != 0; }
	bool is_explicit() const { return (_mask & EXPLICIT_MASK) != 0; }
	bool is_explicit_access() const { return (_mask & (PRIVATE_MASK | PROTECTED_MASK | PUBLIC_MASK)) != 0; }
	bool is_explicit_inline_if_short() const { return (_mask & INLINE_IF_SHORT_MASK) != 0; }
	bool is_export() const { return (_mask & EXPORT_MASK) != 0; }
	bool is_extern() const { return (_mask & EXTERN_MASK) != 0; }
	bool is_friend() const { return (_mask & FRIEND_MASK) != 0; }
	bool is_implicit_inline_if_short() const { return (_mask & INLINE_MASK) == 0; }
	bool is_inline_in_implementation() const { return (_mask & INLINE_IN_IMPLEMENTATION_MASK) != 0; }
	bool is_inline_in_interface() const { return (_mask & INLINE_IN_INTERFACE_MASK) != 0; }
	bool is_just_auto() const { return _mask == AUTO_MASK; }
	bool is_just_template() const { return _mask == TEMPLATE_MASK; }
	bool is_mutable() const { return (_mask & MUTABLE_MASK) != 0; }
	bool is_not_const() const { return (_mask & NOT_CONST_MASK) != 0; }
	bool is_not_inline() const { return (_mask & NOT_INLINE_MASK) != 0; }
	bool is_not_static() const { return (_mask & NOT_STATIC_MASK) != 0; }
	bool is_not_virtual() const { return (_mask & NOT_VIRTUAL_MASK) != 0; }
	bool is_not_volatile() const { return (_mask & NOT_VOLATILE_MASK) != 0; }
	bool is_private() const { return (_mask & PRIVATE_MASK) != 0; }
	bool is_protected() const { return (_mask & PROTECTED_MASK) != 0; }
	bool is_public() const { return (_mask & PUBLIC_MASK) != 0; }
	bool is_pure_virtual() const { return (_mask & PURE_VIRTUAL_MASK) == PURE_VIRTUAL_MASK; }
	bool is_register() const { return (_mask & REGISTER_MASK) != 0; }
	bool is_static() const { return (_mask & STATIC_MASK) != 0; }
	bool is_template() const { return (_mask & TEMPLATE_MASK) != 0; }
	bool is_typedef() const { return (_mask & TYPEDEF_MASK) != 0; }
	bool is_using() const { return (_mask & USING_MASK) != 0; }
	bool is_virtual() const { return (_mask & VIRTUAL_MASK) != 0; }
	bool is_volatile() const { return (_mask & VOLATILE_MASK) != 0; }
	bool is_with_friend() const { return (_mask & INLINE_AS_FRIEND_MASK) != 0; }
	void merge_into(FogMergeContext& mergeContext, FogDeclSpecifierValue& declSpecifiers) const;
	const FogMerge& needs_merge(FogMergeContext& mergeContext, const FogDeclSpecifierValue& declSpecifiers) const;
	char print_named(std::ostream& s, char tailChar) const;
	bool resolve_semantics(FogSemantics& theSemantics) const;
	FogDeclSpecifierValue& reset(Mask aMask) { _mask = Mask(_mask & ~aMask); return *this; }
	FogDeclSpecifierValue& set(Mask aMask) { _mask = Mask(_mask | aMask); return *this; }
public:
	friend std::ostream& operator<<(std::ostream& s, const FogDeclSpecifierValue& declSpecifier);
	friend bool operator==(const FogDeclSpecifierValue& firstValue, const FogDeclSpecifierValue& secondValue)
	{ return firstValue._mask == secondValue._mask; }
	friend FogDeclSpecifierValue operator&(const FogDeclSpecifierValue& firstValue, const FogDeclSpecifierValue& secondValue)
	{ return FogDeclSpecifierValue(Mask(firstValue._mask & secondValue._mask)); }
	friend FogDeclSpecifierValue operator|(const FogDeclSpecifierValue& firstValue, const FogDeclSpecifierValue& secondValue)
	{ return FogDeclSpecifierValue(Mask(firstValue._mask | secondValue._mask)); }
};
#endif

