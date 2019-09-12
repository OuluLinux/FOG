
#ifndef FOGMETATYPE_HXX
#define FOGMETATYPE_HXX






#ifdef NEEDS_TYPENAME
#undef typename
#endif
#ifdef NEEDS_USING
#undef using
#endif

#define FOGMETATYPE_FLYWEIGHTS_LIST(M) \
        M(array_modifier) \
        M(asm_definition) \
        M(base) \
        M(base_specifier) \
        M(bool) \
        M(built_in) \
        M(built_in_type_specifier) \
        M(character) \
        M(class) \
        M(class_key) \
        M(class_specifier) \
        M(cv_qualifier) \
        M(declaration) \
        M(decl_specifier) \
        M(double) \
        M(elaborated_type_specifier) \
        M(entity) \
        M(enum) \
        M(enum_specifier) \
        M(enumerator) \
        M(enumerator_definition) \
        M(exception) \
        M(exception_specification) \
        M(expression) \
        M(expression_statement) \
        M(file_dependency_specifier) \
        M(file_placement_specifier) \
        M(filespace) \
        M(filespace_specifier) \
        M(function) \
        M(function_body) \
        M(function_modifier) \
        M(function_specifier) \
        M(handler) \
        M(identifier) \
        M(initializer_clause) \
        M(iterator) \
        M(keyword) \
        M(linkage) \
        M(linkage_specification) \
        M(meta_class) \
        M(meta_class_specifier) \
        M(meta_function) \
        M(meta_function_specifier) \
        M(meta_parameter) \
        M(meta_parameter_specifier) \
        M(meta_type) \
        M(meta_variable) \
        M(meta_variable_specifier) \
        M(modifier) \
        M(name) \
        M(namespace) \
        M(namespace_alias) \
        M(namespace_alias_definition) \
        M(namespace_definition) \
        M(number) \
        M(object) \
        M(object_specifier) \
        M(object_statement) \
        M(parameter) \
        M(parameter_specifier) \
        M(pointer_modifier) \
        M(punctuation) \
        M(reference_modifier) \
        M(reserved) \
        M(scope) \
        M(scope_specifier) \
        M(scoped_modifier) \
        M(signed) \
        M(specifier) \
        M(statement) \
        M(string) \
        M(struct) \
        M(template_argument) \
        M(template_parameter_specifier) \
        M(templated_parameter_specifier) \
        M(token) \
        M(tree_literal) \
        M(type) \
        M(type_parameter_specifier) \
        M(type_specifier) \
        M(typedef) \
        M(typedef_specifier) \
        M(typename) \
        M(union) \
        M(unsigned) \
        M(using) \
        M(using_declaration) \
        M(using_directive) \
        M(value_parameter_specifier) \
        M(variable) \
        M(variable_specifier) \
        M(void)

class FogMetaType : public FogIdentifier
{
    typedef FogIdentifier Super;
    typedef FogMetaType This;
    typedef bool (FogSemantics::*SemanticsChecker)() const;
    typedef const FogSemantics& (*SemanticsValue)();
    TYPEDECL_SINGLE(This, Super)
    PRIMREF_DERIVED_DECLS(This)
private:
    enum { BIT_WORDS = 4 };
private:
    const FogTokenType::TokenType _is_token;        //   Parser token describing the actual type. e.g. Token
    const unsigned long _my_bit3;                   //   Single bit set mask identifying this meta-type.
    const unsigned long _my_bit2;                   //   Single bit set mask identifying this meta-type.
    const unsigned long _my_bit1;                   //   Single bit set mask identifying this meta-type.
    const unsigned long _my_bit0;                   //   Single bit set mask identifying this meta-type.
    unsigned long _is_bits3;                        //   Bit mask of all meta-types that this meta-type also is.
    unsigned long _is_bits2;                        //   Bit mask of all meta-types that this meta-type also is.
    unsigned long _is_bits1;                        //   Bit mask of all meta-types that this meta-type also is.
    unsigned long _is_bits0;                        //   Bit mask of all meta-types that this meta-type also is.
    SemanticsChecker _semantics_checker;            //   FogSemantics::is_xxx().
    SemanticsValue _semantics_value;                //   FogSemantics::xxx_value().
    FogMetaSlotMapOfRefToConst _slots;              //   Miscellaneous slots.
    FogMetaEntityMapOfRef _meta_entities;           //   Associated meta-entities.

private:
    This& operator=(const This& aMetaType);
    FogMetaType(const This& aMetaType);

private:
#define FOGMETATYPE_FLYWEIGHTS_DECL(lower) \
    void name3(add_,lower,_slots)();
    FOGMETATYPE_FLYWEIGHTS_LIST(FOGMETATYPE_FLYWEIGHTS_DECL)
#undef FOGMETATYPE_FLYWEIGHTS_DECL
    static void set_bases_of(FogMetaType *thisType, const FogMetaType *firstBaseType,
                                                    const FogMetaType *secondBaseType = 0);
    static void set_synonym(FogMetaType *thisType, FogMetaType *thatType);

protected:
    FogMetaType();
    virtual ~FogMetaType();

public:
    FogMetaType(const PrimId& anId, FogTokenType::TokenType isTerminal, SemanticsChecker semanticsChecker,
        SemanticsValue semanticsValue);
    virtual FogMetaEntity *adopt_meta_entity(FogMetaEntity *metaEntity);
    virtual void destroy();
    virtual FogMetaEntity *find_meta_entity(const PrimId& anId);
    const FogMetaSlot *find_meta_slot(const FogToken& aToken, const PrimId& anId) const;
    bool is(const FogMetaType& aMetaType) const
        { return ((_is_bits3 & aMetaType._my_bit3) != 0)
              || ((_is_bits2 & aMetaType._my_bit2) != 0)
              || ((_is_bits1 & aMetaType._my_bit1) != 0)
              || ((_is_bits0 & aMetaType._my_bit0) != 0); }
    virtual FogMetaType *is_meta_type();
    const FogSemantics& is_semantics() const { return _semantics_value(); }
    FogTokenType::TokenType is_token() const { return _is_token; }
    virtual const FogMetaType& meta_type() const;
    virtual std::ostream& print_depth(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_members(std::ostream& s, int aDepth) const;
    virtual std::ostream& print_viz(std::ostream& s) const;
    virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
    bool satisfied_by(const FogToken& aToken, FogScopeContext& inScope) const;
    virtual FogTokenType::TokenType token_type_enum() const;
    const FogToken& want_token() const { return FogTokenType::get(_is_token); }

private:
    static void initialise();
    static FogMetaType *make(FogMetaTypeMapOfRefToConst& metaTypes, const PrimId& anId,
        FogTokenType::TokenType isTerminal, SemanticsChecker semanticsChecker, SemanticsValue semanticsValue);
    static unsigned long next_bit(size_t wordNumber, bool postIncrement = false);               

public:
    static FogMetaType& get(const PrimId& anId);
    static void install_singletons(FogKeywordMapOfRefToConst& tokenTypes);
#define FOGMETATYPE_FLYWEIGHTS_DECL(lower) \
    static const PrimId& name2(lower,_id)(); \
    static FogMetaType& name2(lower,_type)();
    FOGMETATYPE_FLYWEIGHTS_LIST(FOGMETATYPE_FLYWEIGHTS_DECL)
#undef FOGMETATYPE_FLYWEIGHTS_DECL
};

#ifdef NEEDS_TYPENAME
#define typename
#endif
#ifdef NEEDS_USING
#define using
#endif
#endif
 
