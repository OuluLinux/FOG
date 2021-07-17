
#ifndef FOGTAG_HXX
#define FOGTAG_HXX




struct FogTag_Initializer;

class FogTagEnums {
public:
	enum Enum {
		BUILT_IN,
		CLASS,
		ENUM,
		LINKAGE,
		FILESPACE,
		META,
		NAMESPACE,
		STRUCT,
		TYPENAME,
		UNION,
		VIEW,
		SCOPE,                              //   unresolved CLASS, or STRUCT, or UNION or NAMESPACE
		INVALID
	};
public:
	enum Size { ARRAY_SIZE = INVALID + 1 };
};

struct FogTag_Flyweights {
	friend class FogTag;
private:
	static FogTag* _flyweights[FogTagEnums::ARRAY_SIZE];
	static bool _initialised;
private:
	static bool initialise();
};

class FogTagMask : public FogTagEnums {
private:
	Enum _tag;
public:
	FogTagMask(Enum anEnum) : _tag(anEnum) {}
	bool is_built_in_tag() const { return _tag == BUILT_IN; }
	bool is_class_tag() const { return _tag == CLASS; }
	bool is_determined() const { return _tag < SCOPE; }
	bool is_filespace_tag() const { return _tag == FILESPACE; }
	bool is_linkage_tag() const { return _tag == LINKAGE; }
	bool is_meta_tag() const { return _tag == META; }
	bool is_namespace_tag() const { return _tag == NAMESPACE; }
	bool is_struct_tag() const { return _tag == STRUCT; }
	bool is_type_specifier() const { return is_class_tag() || is_struct_tag() || is_union_tag(); }
	bool is_typename_tag() const { return _tag == TYPENAME; }
	bool is_union_tag() const { return _tag == UNION; }
	bool is_valid() const { return _tag != INVALID; }
	Enum value() const { return _tag; }
public:
	friend bool operator==(const FogTagMask& firstTag, const FogTagMask& secondTag)
	{ return firstTag._tag == secondTag._tag; }
	friend Enum operator|(const FogTagMask& firstTag, const FogTagMask& secondTag)
	{ return Enum(firstTag._tag | secondTag._tag); }
};

class FogTag : public FogReservedId, public FogTagEnums {
	typedef FogTag This;
	typedef FogReservedId Super;
	TYPEDECL_SINGLE(This, Super)
	PRIMREF_DERIVED_DECLS(This)
private:
	enum IsValid {};                                    //   Useless type for pointer return.
private:
	const FogTagMask _tag;
	const FogMetaType& _entity_meta_type;
	const FogMetaType& _specifier_meta_type;
	
private:
	FogTag(const FogTag& aTag);
	FogTag& operator=(const FogTag& aTag);
	
private:
	friend struct FogTag_Flyweights;
	friend void FogTokenType::initialise();
	FogTag(const FogTag_Initializer& inIt);
protected:
	FogTag();
	virtual ~FogTag() {}
	
public:
	operator const IsValid* () const { return (const IsValid*)(_tag.is_valid() ? this : 0); }
	bool operator!() const { return !_tag.is_valid(); }
	const FogMetaType& entity_meta_type() const { return _entity_meta_type; }
	bool is_built_in_tag() const { return _tag.is_built_in_tag(); }
	bool is_class_tag() const { return _tag.is_class_tag(); }
	bool is_determined() const { return _tag.is_determined(); }
	bool is_filespace_tag() const { return _tag.is_filespace_tag(); }
	bool is_linkage_tag() const { return _tag.is_linkage_tag(); }
	bool is_meta_tag() const { return _tag.is_meta_tag(); }
	bool is_namespace_tag() const { return _tag.is_namespace_tag(); }
	bool is_struct_tag() const { return _tag.is_struct_tag(); }
	bool is_type_specifier() const { return _tag.is_type_specifier(); }
	bool is_typename_tag() const { return _tag.is_typename_tag(); }
	bool is_union_tag() const { return _tag.is_union_tag(); }
	bool is_valid() const { return _tag.is_valid(); }
	const FogTag& merge_tag(const FogTag& aTag) const;
	virtual const FogMetaType& meta_type() const;
	FogMetaType* meta_type_name() const;
	This& mutate() const { return *(This*)this; }
	const FogMerge& needs_merge_tag(const FogTag& aTag) const;
	virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
	const FogMetaType& specifier_meta_type() const { return _specifier_meta_type; }
	Enum value() const { return _tag.value(); }
	
public:
	friend bool operator==(const FogTag& firstTag, const FogTag& secondTag)
	{ return firstTag._tag == secondTag._tag; }
	
public:
	static FogTag& flyweight() { return *FogTag_Flyweights::_flyweights[INVALID]; }
	static FogTag& built_in_tag() { return *FogTag_Flyweights::_flyweights[BUILT_IN]; }
	static FogTag& class_tag() { return *FogTag_Flyweights::_flyweights[CLASS]; }
	static FogTag& enum_tag() { return *FogTag_Flyweights::_flyweights[ENUM]; }
	static FogTag& filespace_tag() { return *FogTag_Flyweights::_flyweights[FILESPACE]; }
	static FogTag& invalid_tag() { return *FogTag_Flyweights::_flyweights[INVALID]; }
	static FogTag& linkage_tag() { return *FogTag_Flyweights::_flyweights[LINKAGE]; }
	static FogTag& meta_tag() { return *FogTag_Flyweights::_flyweights[META]; }
	static FogTag& namespace_tag() { return *FogTag_Flyweights::_flyweights[NAMESPACE]; }
	static FogTag& scope_tag() { return *FogTag_Flyweights::_flyweights[SCOPE]; }
	static FogTag& struct_tag() { return *FogTag_Flyweights::_flyweights[STRUCT]; }
	static FogTag& typename_tag() { return *FogTag_Flyweights::_flyweights[TYPENAME]; }
	static FogTag& union_tag() { return *FogTag_Flyweights::_flyweights[UNION]; }
};

struct FogTag_Initializer {
	const char* _name;
	FogTokenType::TokenType _table_enum;                //   Index for storage in FogTokenType_id_table
	FogTokenType::TokenType _pp_enum;                   //   Token type for preprocessor level parsing.
	FogTokenType::TokenType _enum;                      //   Token type for normal level parsing.
	FogMetaType& (*_entity_meta_type)();                //   Routine to return the meta-entity-type.
	FogMetaType& (*_specifier_meta_type)();             //   Routine to return the meta-specifier-type.
	FogTag::Enum _tag_enum;                             //   Index into tags.
};

typedef FogEnumHandle<FogTag> FogTagHandle;
#endif

