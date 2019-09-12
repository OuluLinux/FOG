
#ifndef FOGPOSITION_HXX
#define FOGPOSITION_HXX




struct FogPosition_Initializer;

class FogPositionEnums
{
public:
 enum Enum
 {
  REPLACE,
  PREPEND,
  APPEND,
  EXCLUSIVE,
  INVALID, DEFAULT = INVALID
 };
public:
 enum Size { ARRAY_SIZE = INVALID+1 };
};

struct FogPosition_Flyweights
{
 friend class FogPosition;
private:
 static FogPosition *_flyweights[FogPositionEnums::ARRAY_SIZE];
 static bool _initialised;
private:
 static bool initialise();
};

class FogPositionValue : public FogPositionEnums
{
private:
 Enum _position;
public:
 FogPositionValue(Enum anEnum) : _position(anEnum) {}
//  	bool operator!() const { return _position >= INVALID; }	
    bool is_append() const { return _position == APPEND; }
    bool is_exclusive() const { return _position == EXCLUSIVE; }
    bool is_prepend() const { return _position == PREPEND; }
    bool is_replace() const { return _position == REPLACE; }
    bool is_valid() const { return _position != INVALID; }
//      Enum value() const { return _position; }
public:
 friend bool operator==(const FogPositionValue& firstPosition, const FogPositionValue& secondPosition)
  { return firstPosition._position == secondPosition._position; }
 friend Enum operator|(const FogPositionValue& firstPosition, const FogPositionValue& secondPosition)
  { return Enum(firstPosition._position | secondPosition._position); }
};

class FogPosition : public FogIdentifier, public FogPositionEnums
{
 typedef FogPosition This;
 typedef FogIdentifier Super;
 TYPEDECL_SINGLE(This, Super)
 PRIMREF_DERIVED_DECLS(This)
private:
 FogPositionValue _position;

private:
 FogPosition(const FogPosition& aPosition);
 FogPosition& operator=(const FogPosition& aPosition);

protected:
 friend struct FogPosition_Flyweights;
 FogPosition(const char *aName = "", Enum anEnum = INVALID);

public:
    bool is_append() const { return _position.is_append(); }
    bool is_exclusive() const { return _position.is_exclusive(); }
    bool is_prepend() const { return _position.is_prepend(); }
    bool is_replace() const { return _position.is_replace(); }
//      bool is_valid() const { return _position != INVALID; }
//      Enum value() const { return _position; }

public:
 friend bool operator==(const FogPosition& firstPosition, const FogPosition& secondPosition)
  { return firstPosition._position == secondPosition._position; }

public:
 static FogPosition& append() { return *FogPosition_Flyweights::_flyweights[APPEND]; }
 static FogPosition& exclusive() { return *FogPosition_Flyweights::_flyweights[EXCLUSIVE]; }
 static FogPosition& flyweight(Enum anEnum = DEFAULT)
  { return *FogPosition_Flyweights::_flyweights[int(anEnum) < int(ARRAY_SIZE) ? anEnum : INVALID]; }
 static FogPosition& prepend() { return *FogPosition_Flyweights::_flyweights[PREPEND]; }
 static FogPosition& replace() { return *FogPosition_Flyweights::_flyweights[REPLACE]; }
};

typedef FogEnumHandle<FogPosition> FogPositionHandle;
#endif
 
