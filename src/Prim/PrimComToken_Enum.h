
#ifndef PRIMCOMTOKEN_ENUM_HXX
#define PRIMCOMTOKEN_ENUM_HXX



//  
//  	A PrimComToken_Enum implements a parser for a string Unix command line token.
//  
class PrimComToken_Enum : public PrimComToken_String
{
 TYPEDECL_SINGLE(PrimComToken_Enum, PrimComToken_String)
 typedef PrimComToken_String Super;
private:
 const PrimEnum& _enum;    //   The enum manager.
 PrimStringHandle _buffer;   //   Buffer for the textual value.
 int& _value;      //   The enum value.
 int _default_value;     //   The default enum value.

protected:
 virtual void initialise_parse(PrimComParse& aParser);
 virtual const char *parse_text(PrimComParse& aParser, const char *someText);

public:
 PrimComToken_Enum(PrimComParse& aParser, const PrimEnum& anEnum, int& theValue, const char *aName,
  const char *aUsage, const TokenOptions& someOptions = default_options());
 PrimComToken_Enum(PrimComParse& aParser, const PrimEnum& anEnum, int& theValue, const char *aName,
  const char *aUsage, int defaultValue, const TokenOptions& someOptions = default_options());
 virtual ~PrimComToken_Enum();
};
#endif
 
