
#ifndef FOGBUILTINTYPEID_HXX
#define FOGBUILTINTYPEID_HXX





typedef PrimSetElement<FogBuiltInTypeIdRef> FogBuiltInTypeIdSetOfRefElement;
typedef PrimMapOfRef<FogBuiltInTypeIdSetOfRefElement> FogBuiltInTypeIdSetOfRef;

class FogBuiltInTypeId : public FogKeyword
{
    typedef FogKeyword Super;
    typedef FogBuiltInTypeId This;
    TYPEDECL_SINGLE(This, Super)
    PRIMREF_DERIVED_DECLS(This)
public:
    enum IsType                             //   Bit mask of behaviours signalled to make().
    {                                       //   No specific order - IS_DEFAULT == 0 is more efficient.
        IS_DEFAULT = 0,                     //   Integer till we know better.
        IS_VOID = 1,
        IS_LOGICAL = 2,
        IS_NATURAL = 4,
        IS_INTEGER = 8,
        IS_REAL = 16,
        IS_COMPLEX = 32
    };
    enum TypeNumber                         //   Distinct value for each possible type behaviour.
    {                                       //   In precedence order - join of type1,type2 is at max(type1,type2)
        LOGICAL = 0,
        NATURAL = 1,
        INTEGER = 2,
        REAL = 3,
        VOID = 4,
        ARRAY_SIZE = 5                      //   Larger number used as array dimension of earlier values.
    };
private:
    mutable FogBuiltInTypeIdSetOfRef _children;     //   e.g "long double" indexed at "double" in "long"
    bool _is_simple;                                //   true if a single keyword.                
private:
    FogBuiltInTypeId(const This&);
    This& operator=(const This&);
private:
    virtual IsType is_type() const;
    This& mutate() const { return *(This *)this; }
protected:
    FogBuiltInTypeId(const PrimId& anId, bool isSimple);
    FogBuiltInTypeId();
public:
//    virtual const FogBuiltInTypeId *is_built_in() const;
    FogBuiltInTypeId *make(const FogBuiltInTypeId& anId) const;
    virtual void make_built_in(FogNameRef& aName, const FogBuiltInTypeId& anId);
    virtual const FogMetaType& meta_type() const;
    virtual FogMetaType& number_type() const;
    virtual FogTokenType::TokenType pp_token_type_enum() const;
    virtual std::ostream& print_viz(std::ostream& s) const;
    virtual bool resolve_semantics(FogSemanticsContext& theSemantics) const;
    virtual FogTokenType::TokenType token_type_enum() const;
    virtual TypeNumber type_number() const;
public:
    virtual const PrimNumber& add(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const;
    virtual const PrimNumber& and_(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const;
    virtual const PrimNumber& com(const PrimNumber& firstNumber) const;
    virtual const PrimNumber& div(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const;
    virtual bool eq(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const;
    virtual bool ge(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const;
    virtual bool gt(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const;
    virtual bool le(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const;
    virtual bool log_and_(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const;
    virtual bool log_neg(const PrimNumber& firstNumber) const;
    virtual bool log_or(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const;
    virtual bool lt(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const;
    virtual const PrimNumber& mod(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const;
    virtual const PrimNumber& mul(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const;
    virtual bool ne(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const;
    virtual const PrimNumber& neg(const PrimNumber& firstNumber) const;
    virtual const PrimNumber& or_(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const;
    virtual const PrimNumber& shl(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const;
    virtual const PrimNumber& shr(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const;
    virtual const PrimNumber& sub(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const;
    virtual const PrimNumber& xor_(const PrimNumber& firstNumber, const PrimNumber& secondNumber) const;
private:
    static FogBuiltInTypeIdMapOfRef& ids();
    static FogBuiltInTypeId& make(const char *anId, IsType isType, bool isSimple);
    static FogBuiltInTypeId& make(const PrimId& anId, IsType isType, bool isSimple);
public:
    static FogBuiltInTypeId& complex_type();
    static const FogBuiltInTypeId *find(const PrimId& anId);
    static FogBuiltInTypeId& get(const PrimId& anId);
    static void install_singletons(FogKeywordMapOfRefToConst& tokenTypes);
    static FogBuiltInTypeId& integer_type();
    static FogBuiltInTypeId& logical_type();
    static FogBuiltInTypeId& natural_type();
    static FogBuiltInTypeId& real_type();
    static FogBuiltInTypeId& void_type();
public:
    friend const FogBuiltInTypeId& max(const FogBuiltInTypeId& firstType, const FogBuiltInTypeId& secondType);
};
#endif
 
