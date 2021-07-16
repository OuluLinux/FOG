#include <Prim/PrimIncludeAll.h>




#ifndef NO_DATA
TYPEINFO_SINGLE(PrimComTokenStr, PrimComToken);
#endif

#ifndef NO_CODE
//  
//  	The default destructor does nothing.
//  
PrimComTokenStr::~PrimComTokenStr() {}

//  
//  	Initialise for a new parsing pass.
//  
void PrimComTokenStr::initialise_parse(PrimComParse& aParser)
{
 _value = _default_value;
 Super::initialise_parse(aParser);
}

//  
//  	Attempt to parse someText, returning non-null if accepted.
//  
const char *PrimComTokenStr::parse_text(PrimComParse& aParser, const char *someText)
{
 const char *p = Super::parse_text(aParser, someText);
 if (!p)            //   If bad prefix
  return 0;          //  	parsing fails
 if (*p != EOS)          //   If more
  ;            //  	must parse it
 else if (is_optional())        //   If no more and optional
 {             //  	all done
  set_is_present();
  return (p);
 }
 else if ((p = aParser.next_arg()) == 0)    //   If no additional argument
  return 0;          //  	parsing fails
 _value = p;
 set_is_present();
 return p + strlen(p);
}

#ifndef NO_PRIM_STREAMS
//  
//  	Add an indication of token value to an in-line print-out.
//  
std::ostream& PrimComTokenStr::print_this(std::ostream& s) const
{
 Super::print_this(s);
 s << ", \"" << _value << "\"";
 return (s);
}
#endif
#endif
