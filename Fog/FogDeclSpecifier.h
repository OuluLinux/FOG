
#ifndef FOGDECLSPECIFIER_HXX
#define FOGDECLSPECIFIER_HXX





struct FogDeclSpecifier_Initializer;

struct FogDeclSpecifier_Flyweights {      //   dbxtool goes infinite if T has static array of T.

	friend class FogDeclSpecifier;
	
private:
	static const FogDeclSpecifier_Initializer _invalid_init;
	static FogDeclSpecifier* _explicit;
	static FogDeclSpecifier* _ex_port;
	static FogDeclSpecifier* _extern;
	static FogDeclSpecifier* _friend;
	static FogDeclSpecifier* _invalid;
	static FogDeclSpecifier* _mutable;
	static FogDeclSpecifier* _register;
	static FogDeclSpecifier* _typedef;
	static FogDeclSpecifier* _template;
	static FogDeclSpecifier* _using;
	static FogDeclSpecifier& invalid();
};

class FogDeclSpecifier : public FogReservedId, public FogDeclSpecifierEnums {
	typedef FogDeclSpecifier This;
	typedef FogReservedId Super;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	
private:
	const FogDeclSpecifierValue _decl_specifier;
	
private:
	FogDeclSpecifier(const FogDeclSpecifier& aDeclSpecifier);
	FogDeclSpecifier& operator=(const FogDeclSpecifier& aDeclSpecifier);
	
protected:

	friend struct FogDeclSpecifier_Flyweights;
	friend void FogTokenType::initialise();
	FogDeclSpecifier(const FogDeclSpecifier_Initializer& declSpecifier = FogDeclSpecifier_Flyweights::_invalid_init);
	FogDeclSpecifier(const char* anId, Mask aMask);
	virtual ~FogDeclSpecifier() {}
	
public:
	const FogDeclSpecifierValue& decl_specifier_value() const {
		return _decl_specifier;
	}
	
	virtual const FogDeclSpecifierValue& decl_specifiers() const;
	virtual FogDeclSpecifier* is_decl_specifier();
	virtual const FogMetaType& meta_type() const;
	virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
	
public:
	static FogDeclSpecifier& flyweight() {
		return *FogDeclSpecifier_Flyweights::_invalid;
	}
	
	static FogDeclSpecifier& explicit_decl_specifier() {
		return *FogDeclSpecifier_Flyweights::_explicit;
	}
	
	static FogDeclSpecifier& export_decl_specifier() {
		return *FogDeclSpecifier_Flyweights::_ex_port;
	}
	
	static FogDeclSpecifier& extern_decl_specifier() {
		return *FogDeclSpecifier_Flyweights::_extern;
	}
	
	static FogDeclSpecifier& friend_decl_specifier() {
		return *FogDeclSpecifier_Flyweights::_friend;
	}
	
	static FogDeclSpecifier& invalid_decl_specifier() {
		return *FogDeclSpecifier_Flyweights::_invalid;
	}
	
	static FogDeclSpecifier& mutable_decl_specifier() {
		return *FogDeclSpecifier_Flyweights::_mutable;
	}
	
	static FogDeclSpecifier& register_decl_specifier() {
		return *FogDeclSpecifier_Flyweights::_register;
	}
	
	static FogDeclSpecifier& template_decl_specifier() {
		return *FogDeclSpecifier_Flyweights::_template;
	}
	
	static FogDeclSpecifier& typedef_decl_specifier() {
		return *FogDeclSpecifier_Flyweights::_typedef;
	}
	
	static FogDeclSpecifier& using_decl_specifier() {
		return *FogDeclSpecifier_Flyweights::_using;
	}
};

struct FogDeclSpecifier_Initializer {
	const char* _name;
	FogTokenType::TokenType _table_enum;    //   Index for storage in FogTokenType_id_table
	FogTokenType::TokenType _pp_enum;     //   Token type for preprocessor level parsing.
	FogTokenType::TokenType _enum;      //   Token type for normal level parsing.
	FogDeclSpecifier::Mask _decl_specifier_mask;
};

typedef FogEnumHandle<FogDeclSpecifier> FogDeclSpecifierHandle;
#endif

