
#ifndef PRIMCOMTOKEN_HXX
#define PRIMCOMTOKEN_HXX






PRIM_WITH_LIST(PrimComToken)

//
//  	A PrimComToken defines the protocol and abstract implementation for a token within a
//  	Unix command line. Derived classes refine the behaviour to support parsing of boolean,
//  	integer, text, or list arguments.
//  .note tutorial
//  	@file(PrimComToken.tutorial)
//
class PrimComToken : public PrimNamed {
	friend class PrimComParse;       //   To initialise_parse etc
	TYPEDECL_SINGLE(PrimComToken, PrimNamed)
	PRIMSHARES_DECLS(PrimComToken)
	PRIMREF_DECLS(PrimComToken)
public:
	//
	//  	PrimComToken::TokenOptions enumerates the bits for the possible options associated with a
	//  	command line token.
	//
#undef OPTIONAL
	enum TokenOptions { DEFAULT = 0x0, REQUIRED = 0x1, OPTIONAL = 0x2, DYNAMIC = 0x4 };
private:
	const PrimStringHandle _usage;      //   Explanatory usage of this token.
	/*   cfront 3.0.1 cannot cope with constructing const bit fields */
	/*   const */ bool _is_optional : 1;     //   true if value may be omitted.
	bool _is_present : 1;        //   true if present on parsed command line.
	/*   const */ bool _is_required : 1;     //   true if must appear on command line.
	/*   const */ bool _is_dynamic : 1;     //   true if instance may delete.
	PrimComParse* _parser;        //   The parent parser.
	
private:
	static TokenOptions _default_options;    //   Set of token options for use by default.
	static int _default_number_base;     //   Default parsing number base for strtol().
	
private:
	//
	//  	Construction of a copy is forbidden.
	//
	PrimComToken(const PrimComToken& aToken);
	//
	//  	Assignment is forbidden.
	//
	void operator=(const PrimComToken& aToken);
	
protected:
	PrimComToken();
	PrimComToken(PrimComParse& aParser, const char* aName, const char* aUsage, const TokenOptions& someOptions);
	virtual ~PrimComToken();
	virtual bool check_parse(const PrimComParse& aParser);
	virtual void initialise_parse(PrimComParse& aParser);
	virtual const char* parse_text(PrimComParse& aParser, const char* someText);
	//
	//  	Set the flag to indicate that the command line token appears on the command line.
	//
	void set_is_present() { _is_present = true; }
	void set_parser(PrimComParse* aParser) { _parser = aParser; }
	
public:
	static int compare(const PrimComTokenRefToConst* v1, const PrimComTokenRefToConst* v2);
	//
	//  	Report the default number base variable.
	//
	static int& default_number_base() { return (_default_number_base); }
	//
	//  	Report the default parsing options variable.
	//
	static TokenOptions& default_options() { return (_default_options); }
	virtual bool is_file_token() const;
	virtual bool is_minus_token() const;
	virtual bool is_multiple_token() const;
	//
	//  	Report whether this command line token is permitted to have an option value.
	//
	bool is_optional() const { return (_is_optional ? true : false); }
	virtual bool is_plus_token() const;
	//
	//  	Report whether this command line token is present on the command line.
	//
	bool is_present() const { return (_is_present ? true : false); }
	//
	//  	Report whether this command line token is required to be present on the command line.
	//
	bool is_required() const { return (_is_required ? true : false); }
#ifndef NO_PRIM_STREAMS
	virtual std::ostream& print_this(std::ostream& s) const;
	virtual std::ostream& print_token_arg_text(std::ostream& s) const;
	virtual std::ostream& print_token_name(std::ostream& s) const;
	virtual std::ostream& print_usage(std::ostream& s) const;
#endif
};

/*   Inclusion of PrimComToken used to include everything, so if this include file
    is the first of the family to be included, the old policy is presumably assumed
    so include the lot. Just to make the tests harder PrimComToken_Enum was separate. */
/*
    //  #if !defined(PRIMCOMPARSE_HXX) && (!defined(PRIMCOMTOKENSTR_HXX) || defined(PRIMCOMTOKEN_ENUM_HXX)) \
    //  	&& !defined(PRIMCOMTOKENSTRS_HXX) && !defined(PRIMCOMTOKEN_BOOL_HXX) && !defined(PRIMCOMTOKEN_DOUBLE_HXX) \
    //  	&& !defined(PRIMCOMTOKEN_INT_HXX) && !defined(PRIMCOMTOKEN_LONG_HXX)
    //  #include <PrimComs.h>
*/
#endif

