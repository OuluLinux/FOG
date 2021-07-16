
#ifndef FOGELEMENTTYPE_HXX
#define FOGELEMENTTYPE_HXX




struct FogElementType_Flyweights      //   dbxtool goes infinite if T has static array of T.
{
 static const FogElementType _flyWeights[];
};

class FogElementType
{
public:
 //  
 //  	Enum enumerates the formatting style to be used.
 //  
 enum Enum
 {
  DEFAULT,
  CHARACTER,
  WIDE_CHARACTER,
  IDENTIFIER,
  NUMBER,
  STRING,
  WIDE_STRING
 };
private:
 Enum _element_type;

private:
 FogElementType(const FogElementType&);
 FogElementType& operator=(const FogElementType&);

private:
 friend struct FogElementType_Flyweights;
 FogElementType(Enum elementType) : _element_type(elementType) {}
public:            //   egcs ignores friendship on static destructor
 ~FogElementType() {}

public:
    bool is_character() const { return (_element_type == CHARACTER) || (_element_type == WIDE_CHARACTER); }
    bool is_default() const { return _element_type == DEFAULT; }
    bool is_identifier() const { return _element_type == IDENTIFIER; }
    bool is_number() const { return _element_type == NUMBER; }
    bool is_string() const { return (_element_type == STRING) || (_element_type == WIDE_STRING); }
    bool is_wide() const { return (_element_type == WIDE_CHARACTER) || (_element_type == WIDE_STRING); }
 const FogElementType& merge(const FogElementType& elementType) const;
 const FogMetaType& meta_type() const;
  const FogMerge& needs_merge(const FogElementType& elementType) const;
 FogTokenType::TokenType token_type() const;
 Enum value() const { return _element_type; }

public:
 friend bool operator==(const FogElementType& firstType, const FogElementType& secondType)
  { return firstType._element_type == secondType._element_type; }
 friend std::ostream& operator<<(std::ostream& s, const FogElementType& elementType);

public:
 static const FogElementType& flyweight(Enum anEnum)
  { return FogElementType_Flyweights::_flyWeights[anEnum <= WIDE_STRING ? anEnum : DEFAULT]; }
 static const FogElementType& character() { return FogElementType_Flyweights::_flyWeights[CHARACTER]; }
 static const FogElementType& character(bool isWide)
  { return FogElementType_Flyweights::_flyWeights[isWide ? WIDE_CHARACTER : CHARACTER]; }
 static const FogElementType& identifier() { return FogElementType_Flyweights::_flyWeights[IDENTIFIER]; }
 static const FogElementType& number() { return FogElementType_Flyweights::_flyWeights[NUMBER]; }
 static const FogElementType& string() { return FogElementType_Flyweights::_flyWeights[STRING]; }
 static const FogElementType& string(bool isWide)
  { return FogElementType_Flyweights::_flyWeights[isWide ? WIDE_STRING : STRING]; }
 static const PrimEnum& values();
 static const FogElementType& wide_character()
  { return FogElementType_Flyweights::_flyWeights[WIDE_CHARACTER]; }
 static const FogElementType& wide_string() { return FogElementType_Flyweights::_flyWeights[WIDE_STRING]; }
};

typedef FogEnumHandle<FogElementType> FogElementTypeHandle;
#endif
 
