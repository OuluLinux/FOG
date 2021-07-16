
#ifndef FOGUTILITY_HXX
#define FOGUTILITY_HXX




struct FogUtility_Initializer;

struct FogUtility_Flyweights       //   dbxtool goes infinite if T has static array of T.
{
 friend class FogUtility;
private:
 static const FogUtility_Initializer _invalid_init;
 static FogUtility *_emit;
 static FogUtility *_frozen;
 static FogUtility *_invalid;
 static FogUtility *_pool;
 static FogUtility *_utility;
 static FogUtility& invalid();
};

class FogUtility : public FogIdentifier
{
 typedef FogUtility This;
 typedef FogIdentifier Super;
 TYPEDECL_SINGLE(This, Super)
 PRIMREF_DERIVED_DECLS(This)
private:
 enum IsValid {};         //   Useless type for pointer return.
public:
 enum Enum
 {
  POOL,      //   new declarations are only emitted if scope is emitted
  EMIT,      //   new declarations must be emitted
  UTILITY,     //   new declarations are not permitted
  FROZEN,      //   new declarations are from input files (NB HASH is a #define in FogLexer).
  INVALID,
  HASH_FILE = FROZEN,
  DEFAULT = POOL
 };
private:
 const Enum _utility;
 const FogTokenType::TokenType _token_type;

private:
 FogUtility(const FogUtility& aUtility);
 FogUtility& operator=(const FogUtility& aUtility);

protected:
 friend struct FogUtility_Flyweights;
 friend void FogTokenType::initialise();
 FogUtility(const FogUtility_Initializer& utilityInit = FogUtility_Flyweights::_invalid_init);
 virtual ~FogUtility() {}

public:
    bool is_emit() const { return _utility == EMIT; }
    bool is_hash() const { return _utility == HASH_FILE; }
 bool is_not_emitted() const  { return (_utility == UTILITY) || (_utility == HASH_FILE); }
 bool is_pool() const { return _utility == POOL; }
 bool is_utility() const { return _utility >= UTILITY; }
 bool is_valid() const { return _utility != INVALID; }
 const FogUtility& merge_utility(const FogUtility& aUtility) const;
 This& mutate() const { return *(This *)this; }
 const FogMerge& needs_merge_utility(const FogUtility& aUtility) const;
 virtual std::ostream& print_viz(std::ostream& s) const;
 virtual FogTokenType::TokenType token_type_enum() const;

public:
 friend const FogUtility& max(const FogUtility& firstUtility, const FogUtility& secondUtility)
  { return firstUtility._utility > secondUtility._utility ? firstUtility : secondUtility; }
 friend const FogUtility& min(const FogUtility& firstUtility, const FogUtility& secondUtility)
  { return firstUtility._utility < secondUtility._utility ? firstUtility : secondUtility; }
 friend bool operator==(const FogUtility& firstUtility, const FogUtility& secondUtility)
  { return firstUtility._utility == secondUtility._utility; }
 friend bool operator!=(const FogUtility& firstUtility, const FogUtility& secondUtility)
  { return firstUtility._utility != secondUtility._utility; }
 friend bool operator<(const FogUtility& firstUtility, const FogUtility& secondUtility)
  { return firstUtility._utility < secondUtility._utility; }
 friend bool operator<=(const FogUtility& firstUtility, const FogUtility& secondUtility)
  { return firstUtility._utility <= secondUtility._utility; }
 friend bool operator>=(const FogUtility& firstUtility, const FogUtility& secondUtility)
  { return firstUtility._utility >= secondUtility._utility; }
 friend bool operator>(const FogUtility& firstUtility, const FogUtility& secondUtility)
  { return firstUtility._utility > secondUtility._utility; }

public:
    static FogUtility& emit_utility() { return *FogUtility_Flyweights::_emit; }
 static FogUtility& flyweight() { return *FogUtility_Flyweights::_invalid; }
    static FogUtility& frozen_utility() { return *FogUtility_Flyweights::_frozen; }
    static FogUtility& invalid_utility() { return *FogUtility_Flyweights::_invalid; }
    static FogUtility& pool_utility() { return *FogUtility_Flyweights::_pool; }
    static FogUtility& utility_utility() { return *FogUtility_Flyweights::_utility; }
};

struct FogUtility_Initializer
{
 const char *_name;
 FogTokenType::TokenType _table_enum;  //   Index for storage in FogTokenType_id_table
 FogUtility::Enum _utility_enum;    //   Index into tags.
};

typedef FogEnumHandle<FogUtility> FogUtilityHandle;
#endif
 
