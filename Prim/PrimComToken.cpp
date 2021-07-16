#include <Prim/PrimIncludeAll.h>







TYPEINFO_SINGLE(PrimComToken, PrimNamed);
PRIMREF_NULL_CLASS_AND_IMPL(PrimComToken)
PRIMSHARES_IMPL(PrimComToken)
PRIMREFS_IMPL(PrimComToken)
PRIMLISTOFREFS_IMPL(PrimComToken)


TMPL_HACK_FIX_DO(PrimComToken);

//  
//  	Default command line token options.
//  
PrimComToken::TokenOptions PrimComToken::_default_options = PrimComToken::DEFAULT;

//  
//  	Default parsing number base.
//  
int PrimComToken::_default_number_base = 0;

//  
//  	Construct a null token.
//  
PrimComToken::PrimComToken()
:
 _is_optional(false),
 _is_present(false),
 _is_required(false),
 _is_dynamic(false),
 _parser(0)
{}

//  
//  	Create a new token named aName and it with someOptions and aUsage for use with aParser.
//  
PrimComToken::PrimComToken(PrimComParse& aParser, const char *aName, const char *aUsage,
  const TokenOptions& someOptions)
:
 PrimNamed(aName),
 _usage(aUsage),
 _is_optional((someOptions & OPTIONAL) != 0),
 _is_present(false),
 _is_required((someOptions & REQUIRED) != 0),
 _is_dynamic((someOptions & DYNAMIC) != 0),
 _parser(&aParser)
{
 aParser.add(*this);
}

//  
//  	The destructor removes this token from the parser, avoiding a destruction order share
//  	count race with respect to a static instance.
//  
PrimComToken::~PrimComToken()
{
 if (_parser)
  _parser->remove(*this);
}

//  
//  	Check whether parsing was successful. The default implementation returns true if ok,
//  	but false optionally generating an error message if a required token is absent.
//  
bool PrimComToken::check_parse(const PrimComParse& aParser)
{
 if (!is_required() || is_present())
  return true;
 if (aParser.diagnose_errors())
  PRIMERR(MISSING_TOKEN, *this << " not present on command line");
 return false;
}

//  
//  	Return +ve, 0, -ve according to the lexical ordering of token names.
//  
int PrimComToken::compare(const PrimComTokenRefToConst *v1, const PrimComTokenRefToConst *v2)
{
 if (!v1)
  return -1;
 if (!v2)
  return 1;
 return (*v1)->name().compare((*v2)->name());
}

//  
//  	Initialise for a new parsing pass.
//  
void PrimComToken::initialise_parse(PrimComParse& aParser) { _is_present = false; }

//  
//  	Report that this token does not parse file names.
//  
bool PrimComToken::is_file_token() const { return false; }

//  
//  	Report that this token does not use a minus prefix.
//  
bool PrimComToken::is_minus_token() const { return false; }

//  
//  	Report that this token does not support multiple values.
//  
bool PrimComToken::is_multiple_token() const { return false; }

//  
//  	Report that this token does not use a plus prefix.
//  
bool PrimComToken::is_plus_token() const { return false; }

//  
//  	Compare the incoming command line text to see if it matches this token name, returning a pointer
//  	past the parsed text if it matches, or 0 if it does not. The implementation here invokes
//  	is_minus_token() and is_plus_token() to determine whether any leading minus or plus character
//  	should be included in the comparison of the token name. This method therefore forms a useful
//  	prefix to any processing required by derived classes.
//  
const char *PrimComToken::parse_text(PrimComParse& aParser, const char *someText)
{
 const char *p = someText;
 if (!p)
  return 0;
 if ((is_minus_token() && (*p++ != '-')) || (*p == '-'))
  return 0;
 if ((is_plus_token() && (*p++ != '+')) || (*p == '+'))
  return 0;
 if (is_file_token())     //   File tokens have no prefix
  return p;
 const size_t nameLen = strlen(name().str());
 if ((strlen(p) < nameLen) || (strncmp(p, name().str(), nameLen) != 0))
  return 0;
 return p + nameLen;
}

#ifndef NO_PRIM_STREAMS
//  
//  	Add an indication of command line status to an inline print-out.
//  
std::ostream& PrimComToken::print_this(std::ostream& s) const
{
 if (name().is_null())
  s << "<file>";
 else
  s << name();
 s << ", " << (is_present() ? "PRESENT" : "ABSENT");
 if (is_required())
  s << ", REQUIRED";
 if (is_optional())
  s << ", OPTIONAL_VALUE";
 return s;
}

//  
//  	Print out a text string to describe a token argument. The default implementation here is nothing.
//  
std::ostream& PrimComToken::print_token_arg_text(std::ostream& s) const
{
 return s;
}

//  
//  	Print out the name of this command line token which in this implementation is the name with
//  	appropriate plus or minus prefixes.
//  
std::ostream& PrimComToken::print_token_name(std::ostream& s) const
{
 if (is_minus_token())
  s << "-";
 if (is_plus_token())
  s << "+";
 s << name();
 return s;
}

//  
//  	Print out the usage of this command line token.
//  
std::ostream& PrimComToken::print_usage(std::ostream& s) const
{
 s << "\n\t";
 if (!is_required())
  s << "[";
 print_token_name(s);
 if (is_optional())
  s << "[";
 print_token_arg_text(s);
 if (is_optional())
  s << "]";
 if (!is_required())
  s << "]";
 if (is_multiple_token())
  s << "*";
 s << " " << _usage;
 return s;
}
#endif


