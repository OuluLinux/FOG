#include <Prim/PrimIncludeAll.h>






TYPEINFO_SINGLE(PrimComParse, PrimPrint);
PRIMREF_NULL_CLASS_AND_IMPL(PrimComParse)
PRIMSHARES_IMPL(PrimComParse)
PRIMREFS_IMPL(PrimComParse)


TMPL_HACK_FIX_DO(PrimComParse);

//
//  		Default command line parsing options.
//
const PrimComParse::ParseOptions PrimComParse::_default_options = PrimComParse::DEFAULT;


//
//  	Create a new parser.
//
PrimComParse::PrimComParse()
	:
	_current_arg(0),
	_options(_default_options),
	_is_sorted(false)
{}

//
//  	The destructor notifies all its tokens that it has been deleted.
//
PrimComParse::~PrimComParse() {
	for (PrimComTokenConstListOfRefIterator p(_tokens); p; p++)
		p->set_parser(0);
}

//
//  	Add a new command line token to the parser.
//
void PrimComParse::add(PrimComToken& aToken) {
	_is_sorted = false;
	_tokens.add(aToken);
}

//
//  	Return the static instance of a parser that is by convention used to parse the incoming
//  	application command line. The instance exists within function scope to avoid initialisation
//  	order problems that may result from multiple contributions to the command line.
//
PrimComParse& PrimComParse::main_parser() {
	static PrimComParseRef mainParser(new PrimComParse, PrimComParseRef::ADOPT);
	return *mainParser;
}

//
//  	Advance the parser to the next argument, returning 0 if no such argument.
//
const char* PrimComParse::next_arg() {
	if (_args.tally() <= _current_arg)
		return 0;
		
	return _args[++_current_arg].str();
}

//
//  	Parse the Unix command line identified by anArgc, anArgv.
//
//  	In the event that an argument is accepted by more than one token, each token gets the
//  	argument(s) and a warning message is produced. In the further event that the multiple
//  	usages absorb different numbers of arguments, the warning is replaced by an error and
//  	subsequent parsing proceeds after advancing by the largest number of arguments.
//
//  	Optional parsing options may suppress any error diagnostics resulting
//  	from illegal command line syntax and any warning diagnostics resulting
//  	from conflicting command line syntax.
//
bool PrimComParse::parse_args(int anArgc, const char* const anArgv[], const ParseOptions& someOptions) {
	const PrimText theArgs(anArgv, anArgc);
	return (parse_args(theArgs, someOptions));
}

//
//  	Parse the Unix command line identified by theArgs, which should correspond to a command
//  	line with theArgs[0] being the unparsed "program" name.
//
//  	In the event that an argument is accepted by more than one token, each token gets the
//  	argument(s) and a warning message is produced. In the further event that the multiple
//  	usages absorb different numbers of arguments, the warning is replaced by an error and
//  	subsequent parsing proceeds after advancing by the largest number of arguments.
//
//  	Optional parsing options may suppress any error diagnostics resulting
//  	from illegal command line syntax and any warning diagnostics resulting
//  	from conflicting command line syntax.
//
bool PrimComParse::parse_args(const PrimText& theArgs, const ParseOptions& someOptions) {
	_args = theArgs;
	_options = someOptions;
	
	if (!_is_sorted) {
		_tokens.sort(&PrimComToken::compare);
		_is_sorted = true;
	}
	
	for (PrimComTokenConstListOfRefIterator p1(_tokens); p1; p1++)
		p1->initialise_parse(*this);
		
	bool net_success = true;
	
	for (size_t lastArg = 0, firstArg; net_success && ((firstArg = ++lastArg) < _args.tally()); ) {
		const PrimComToken* theToken = 0;
		
		for (PrimComTokenConstListOfRefIterator p2(_tokens); p2; p2++)
			if (p2->parse_text(*this, _args[_current_arg = firstArg].str()) == 0)
				;
			else if (!theToken) {
				theToken = &*p2;
				lastArg = _current_arg;
			}
			else if (lastArg != _current_arg) {
				if (diagnose_errors())
					PRIMERR(BAD_PARSE, "Inconsistent command line parsing by " << *theToken
					        << " and " << *p2);
					        
				if (lastArg < _current_arg)
					lastArg = _current_arg;
			}
			else if (diagnose_warnings())
				PRIMWRN(BAD_PARSE, "Duplicate command line parsing by " << *theToken << " and " << *p2);
				
		if (!theToken) {
			if (diagnose_errors())
				PRIMERR(BAD_PARSE, *this << " failed to parse \"" << _args[firstArg] << "\"");
				
			net_success = false;
		}
	}
	
	if (net_success)
		for (PrimComTokenConstListOfRefIterator p3(_tokens); p3; p3++)
			if (!p3->check_parse(*this))
				net_success = false;
				
	return net_success;
}

#ifndef NO_PRIM_STREAMS
//
//  	Participate in a deep print-out of the command line parser. A full print-out of each
//  	of the command line tokens is appended to the inherited functionality.
//
std::ostream& PrimComParse::print_members(std::ostream& s, int indentLevel) const {
	PrimPrint::print_members(s, indentLevel);
	
	for (PrimComTokenConstListOfRefToConstIterator p(_tokens); p; p++)
		p->print_on(s, indentLevel);
		
	return s;
}

//
//  	Print out the usage of this command line token.
//
std::ostream& PrimComParse::print_usage(std::ostream& s) const {
	s << "Usage : " << _args[0];
	
	for (PrimComTokenConstListOfRefToConstIterator p(_tokens); p; p++)
		p->print_usage(s);
		
	s << std::endl;
	return s;
}
#endif

//
//  	Remove a specified token.
//
void PrimComParse::remove(PrimComToken& aToken) {
	_tokens.remove(aToken);
}


