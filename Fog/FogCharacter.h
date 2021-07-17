
#ifndef FOGCHARACTER_HXX
#define FOGCHARACTER_HXX





class FogCharacter : public FogExpr {
	typedef FogExpr Super;
	typedef FogCharacter This;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
	FOGTOKEN_SHAREDLEAF_DECLS
	
public:
	enum CharacterType {
		NARROW,         //   e.g. \011 from '\011'
		WIDE,         //   e.g. \011 from L'\011'
		LITERAL         //   e.g. \011 from \011 (only occurs at preprocessor level)
	};
	enum AsCharacter {
		AS_STRING,        //   " needs escaping (within "...")
		AS_CHARACTER       //   ' needs escaping (within '...')
	};
	
private:
	enum SizeofCharacterType { SIZEOF_CHARACTERTYPE = LITERAL + 1 };
	
private:
	const PrimIdHandle _raw_id;     //   The source text.
	const CharacterType _character_type;
	const unsigned long _value;     //   The resolved value.
	mutable PrimIdHandle _bin_id;    //   The binary formatted byte stream.
	mutable PrimIdHandle _put_id;    //   The ASCII formatted (multi-)byte stream.
	
private:
	FogCharacter(const This&);
	This& operator=(const This&);
	
private:
	const PrimId& id() const {
		return *_raw_id;
	}
	
	friend struct PrimMapOfRefToConstFunctions<FogCharacter>;
	
private:
	FogCharacter(const PrimId& rawId, const PrimId& binId, unsigned long aValue, CharacterType characterType);
	This& mutate() const {
		return *(This*)this;
	}
	
	const PrimId& put_id() const;
	const PrimId& raw_id() const {
		return *_raw_id;
	}
	
protected:
	FogCharacter();
	virtual ~FogCharacter();
	
public:
	const PrimId& bin_id() const;
	virtual bool emit(FogEmitContext& emitContext) const;
	virtual bool get_character(PrimIdHandle& returnId, FogScopeContext& inScope);
	virtual bool get_identifier(PrimIdHandle& returnId, FogScopeContext& inScope);
	virtual const PrimNumber& get_number(FogScopeContext& inScope) const;
	virtual bool get_number_token(FogTokenRef& returnValue, FogScopeContext& inScope) const;
	virtual bool get_string(PrimIdHandle& returnId, FogScopeContext& inScope);
	virtual const FogCharacter* is_character() const;
	virtual bool is_wide() const;
	virtual const FogMetaType& meta_type() const;
	virtual FogTokenType::TokenType pp_token_type_enum() const;
	virtual std::ostream& print_diagnostic(std::ostream& s, int aDepth) const;
	virtual char print_named(std::ostream& s, const PrimId* fullId, char tailChar) const;
	virtual std::ostream& print_source(std::ostream& s, int aDepth) const;
	virtual std::ostream& print_stringized(std::ostream& s) const;
	virtual std::ostream& print_this(std::ostream& s) const;
	virtual std::ostream& print_viz(std::ostream& s) const;
	virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
	virtual size_t source_columns() const;
	virtual FogTokenType::TokenType token_type_enum() const;
	unsigned long value() const {
		return _value;
	}
	
public:
	static unsigned long get(const unsigned char*& p, const unsigned char* pEnd);
	static FogCharacter& make(const PrimId& rawId, const PrimId& binId, CharacterType characterType);
	static FogCharacter& make(const PrimId& anId, CharacterType characterType);
	static FogCharacter& make(const char* aBuffer, size_t aLength, CharacterType characterType);
	static size_t put(std::ostream& s, unsigned long aValue, AsCharacter asCharacter);
	friend std::ostream& operator<<(std::ostream& s, const FogCharacter& aCharacter) {
		return aCharacter.print_this(s);
	}
};

#endif

