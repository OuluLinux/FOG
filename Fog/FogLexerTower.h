
#ifndef FOGLEXERTOWER_HXX
#define FOGLEXERTOWER_HXX



class FogLexerTower : public FogLexerInput
{
 typedef FogLexerInput Super;
 typedef FogLexerTower This;
 TYPEDECL_SINGLE(This, Super)
private:
 FogReader& _reader;
public:
 FogLexerTower(FogReader& aReader);
 FogLexerTower(FogReader& aReader, const FogToken& aToken);
 virtual ~FogLexerTower();
 virtual void print_token(const char *aState, const FogToken *aValue) const;
 FogReader& reader() { return _reader; }
 const FogReader& reader() const { return _reader; }
};
#endif
 
