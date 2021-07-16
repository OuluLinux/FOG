
#ifndef PRIMCOMPARSE_HXX
#define PRIMCOMPARSE_HXX







PRIM_WITH_REF(PrimComParse)

//  
//  	A PrimComParse implements a parser for a Unix command line. The elements of the command line
//  	are defined by derived classes of PrimComToken.
//  .note tutorial
//  	@file(PrimComParse.tutorial)
//  
class PrimComParse : public PrimPrint
{
 TYPEDECL_SINGLE(PrimComParse, PrimPrint)
 PRIMSHARES_DECLS(PrimComParse)
 PRIMREF_DECLS(PrimComParse)
public:
 //  
 //  	PrimComParse::ParseOptions enumerates the bits that define options for the parsing process.
 //  
 enum ParseOptions { DEFAULT = 0x0, NO_ERRORS = 0x1, NO_WARNINGS = 0x2 };
 static const ParseOptions _default_options;   //   Set of parsing options for use by default.
private:
 PrimText _args;          //   The set of command line arguments.
 size_t _current_arg;        //   Index to current argument in parsing context.
 ParseOptions _options;        //   Current parsing options.
 PrimListOfRef<PrimComToken> _tokens;    //   List of possible command line tokens.
 bool _is_sorted : 1;        //   True if tokens in alphabetical order.
private:
//  
//  	Construction of a copy is forbidden.
//  
 PrimComParse(const PrimComParse& aParser);
//  
//  	Assignment is forbidden.
//  
 void operator=(const PrimComParse& aParser);

protected:
 virtual ~PrimComParse();

public:
 PrimComParse();
 void add(PrimComToken& aToken);
//  
//  	Report whether errors should be diagnosed.
//  
 bool diagnose_errors() const { return (_options & NO_ERRORS) == 0; }
//  
//  	Report whether warnings should be diagnosed.
//  
 bool diagnose_warnings() const { return (_options & NO_WARNINGS) == 0; }
 static PrimComParse& main_parser();
 const char *next_arg();
 bool parse_args(int anArgc, const char *const anArgv[], const ParseOptions& = _default_options);
 bool parse_args(const PrimText& someArgs, const ParseOptions& = _default_options);
#ifndef NO_PRIM_STREAMS
 virtual std::ostream& print_members(std::ostream& s, int indentLevel = 0) const;
 virtual std::ostream& print_usage(std::ostream& s) const;
#endif
 void remove(PrimComToken& aToken);
};
#endif
 
